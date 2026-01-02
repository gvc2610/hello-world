#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdatomic.h>

/*
 * EMBEDDED SYSTEM PATTERN: Lock-Free Event Queue (ISR Safe)
 * 
 * CRITICAL FIX:
 * You cannot use Mutexes in an ISR! They block, which crashes systems.
 * 
 * SOLUTION:
 * 1. Data Path: Lock-Free Ring Buffer (Single Producer / Single Consumer).
 *    - The ISR (Producer) modifies 'tail'.
 *    - The SM (Consumer) modifies 'head'.
 *    - Since they modify different indices, we only need memory barriers (atomics), not locks.
 * 
 * 2. Signaling Path: Semaphore / Task Notification.
 *    - We simulate 'xSemaphoreGiveFromISR' using a Condition Variable helper.
 */

// ---------------------------------------------------------------------------
// 1. Definitions
// ---------------------------------------------------------------------------

typedef enum { 
    EVT_START, 
    EVT_DATA_RECEIVED, 
    EVT_TIMEOUT, 
    EVT_STOP 
} event_t;

typedef enum { 
    STATE_IDLE, 
    STATE_PROCESSING, 
    STATE_ERROR, 
    STATE_END 
} state_t;

const char* event_names[] = { "EVT_START", "EVT_DATA_RECEIVED", "EVT_TIMEOUT", "EVT_STOP" };
const char* state_names[] = { "IDLE", "PROCESSING", "ERROR", "END" };

// ---------------------------------------------------------------------------
// 2. Lock-Free Ring Buffer (The "Bridge")
// ---------------------------------------------------------------------------

#define QUEUE_SIZE 16 // Power of 2 is often faster for wrapping, but modulo is fine here.

typedef struct {
    event_t events[QUEUE_SIZE];
    atomic_int head; // Modified by Consumer (SM)
    atomic_int tail; // Modified by Producer (ISR)
    
    // Simulation of RTOS Semaphore/Signal
    pthread_mutex_t signal_lock;
    pthread_cond_t  signal_cond;
    int signal_pending; 
} event_queue_t;

void queue_init(event_queue_t *q) {
    atomic_init(&q->head, 0);
    atomic_init(&q->tail, 0);
    
    // Init simulation primitives
    pthread_mutex_init(&q->signal_lock, NULL);
    pthread_cond_init(&q->signal_cond, NULL);
    q->signal_pending = 0;
}

// Helper to simulate RTOS "Give Semaphore"
void rtos_signal_task_from_isr(event_queue_t *q) {
    // In a real RTOS (FreeRTOS), this would be:
    // xTaskNotifyFromISR(sm_task_handle, ...);
    // or xSemaphoreGiveFromISR(sem_handle, ...);
    
    pthread_mutex_lock(&q->signal_lock);
    q->signal_pending = 1;
    pthread_cond_signal(&q->signal_cond);
    pthread_mutex_unlock(&q->signal_lock);
}

// Helper to simulate RTOS "Take Semaphore"
void rtos_wait_for_signal(event_queue_t *q) {
    // In a real RTOS:
    // xTaskNotifyWait(...);
    
    pthread_mutex_lock(&q->signal_lock);
    while (q->signal_pending == 0) {
        pthread_cond_wait(&q->signal_cond, &q->signal_lock);
    }
    q->signal_pending = 0; // Consume signal
    pthread_mutex_unlock(&q->signal_lock);
}

// ---------------------------------------------------------------------------
// ISR / Hardware Thread (Producer) - LOCK FREE DATA PATH
// ---------------------------------------------------------------------------
void send_event_isr(event_queue_t *q, event_t evt) {
    // 1. Capture current indices atomically
    int current_tail = atomic_load(&q->tail);
    int next_tail = (current_tail + 1) % QUEUE_SIZE;
    int current_head = atomic_load(&q->head);

    // 2. Check for Full (Lock-Free)
    if (next_tail != current_head) {
        // 3. Write Data
        q->events[current_tail] = evt;
        
        // 4. Commit Write (Update Tail) - Acts as memory barrier
        atomic_store(&q->tail, next_tail);
        
        printf("[ISR] -> Pushed %s (Lock-Free)\n", event_names[evt]);
        
        // 5. Wake up the SM Task
        rtos_signal_task_from_isr(q);
    } else {
        printf("[ISR] -> Queue Full! Dropped %s\n", event_names[evt]);
    }
}

// ---------------------------------------------------------------------------
// State Machine Loop (Consumer)
// ---------------------------------------------------------------------------
event_t wait_for_event(event_queue_t *q) {
    int current_head, current_tail;

    while (1) {
        // 1. Check if queue has data (Lock-Free)
        current_head = atomic_load(&q->head);
        current_tail = atomic_load(&q->tail);

        if (current_head != current_tail) {
            // Data available!
            event_t evt = q->events[current_head];
            
            // Update Head
            atomic_store(&q->head, (current_head + 1) % QUEUE_SIZE);
            return evt;
        }

        // 2. Queue empty? Sleep until signaled.
        // This avoids "busy waiting" (spinning 100% CPU).
        rtos_wait_for_signal(q);
    }
}

// ---------------------------------------------------------------------------
// 3. State Machine Logic (Unchanged)
// ---------------------------------------------------------------------------

typedef struct {
    int data_counter;
} sm_context_t;

void action_start_processing(sm_context_t *ctx) {
    printf("   [Action] Resetting counters. Starting hardware...\n");
    ctx->data_counter = 0;
}

void action_process_data(sm_context_t *ctx) {
    ctx->data_counter++;
    printf("   [Action] Processed chunk #%d\n", ctx->data_counter);
}

void action_handle_error(sm_context_t *ctx) {
    printf("   [Action] Logging error to flash...\n");
}

void action_shutdown(sm_context_t *ctx) {
    printf("   [Action] Powering down peripherals...\n");
}

struct transition {
    state_t src_state;
    event_t trigger_event;
    state_t dst_state;
    void (*action)(sm_context_t *ctx);
};

const struct transition transition_table[] = {
    { STATE_IDLE,       EVT_START,          STATE_PROCESSING,   action_start_processing },
    { STATE_IDLE,       EVT_STOP,           STATE_END,          action_shutdown },
    { STATE_PROCESSING, EVT_DATA_RECEIVED,  STATE_PROCESSING,   action_process_data },
    { STATE_PROCESSING, EVT_TIMEOUT,        STATE_ERROR,        action_handle_error },
    { STATE_PROCESSING, EVT_STOP,           STATE_END,          action_shutdown },
    { STATE_ERROR,      EVT_START,          STATE_PROCESSING,   action_start_processing },
    { STATE_ERROR,      EVT_STOP,           STATE_END,          action_shutdown }
};

#define TABLE_SIZE (sizeof(transition_table)/sizeof(transition_table[0]))

// ---------------------------------------------------------------------------
// 4. Execution Engine
// ---------------------------------------------------------------------------

void* sm_thread_func(void* arg) {
    event_queue_t *q = (event_queue_t*)arg;
    sm_context_t ctx = {0};
    state_t current_state = STATE_IDLE;

    printf("[SM] Started. Waiting for events...\n");

    while (current_state != STATE_END) {
        event_t evt = wait_for_event(q);
        
        printf("[SM] State: %s | Event: %s\n", state_names[current_state], event_names[evt]);

        bool transition_found = false;
        for (int i = 0; i < TABLE_SIZE; i++) {
            if (transition_table[i].src_state == current_state && 
                transition_table[i].trigger_event == evt) {
                
                if (transition_table[i].action) {
                    transition_table[i].action(&ctx);
                }
                current_state = transition_table[i].dst_state;
                transition_found = true;
                break;
            }
        }

        if (!transition_found) {
            printf("   [Warning] Ignored event %s in state %s\n", event_names[evt], state_names[current_state]);
        }
    }

    printf("[SM] Reached END state. Thread exiting.\n");
    return NULL;
}

// ---------------------------------------------------------------------------
// 5. Simulation (Main)
// ---------------------------------------------------------------------------

int main() {
    event_queue_t queue;
    queue_init(&queue);

    pthread_t sm_thread;
    pthread_create(&sm_thread, NULL, sm_thread_func, &queue);

    // Simulate ISRs
    usleep(500000);
    send_event_isr(&queue, EVT_START);
    
    usleep(200000);
    send_event_isr(&queue, EVT_DATA_RECEIVED);
    
    usleep(200000);
    send_event_isr(&queue, EVT_DATA_RECEIVED);
    
    usleep(200000);
    send_event_isr(&queue, EVT_TIMEOUT);
    
    usleep(200000);
    send_event_isr(&queue, EVT_START);
    
    usleep(200000);
    send_event_isr(&queue, EVT_STOP);

    pthread_join(sm_thread, NULL);
    return 0;
}
