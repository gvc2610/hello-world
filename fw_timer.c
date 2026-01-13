#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

/*
 * MODERN ARM CORTEX-M SOFT TIMER LIBRARY
 * 
 * Improvements over original:
 * 1. Uses Standard CMSIS SysTick (Hardware Abstraction).
 * 2. No 'malloc' (Static Allocation) - Critical for embedded reliability.
 * 3. Function Pointer Callbacks - No more "bitmask" limits (16 timers).
 * 4. Deferred Execution - ISR marks "expired", Main Loop runs callback (Thread Safety).
 * 5. Support for One-Shot and Periodic timers.
 */

// ---------------------------------------------------------------------------
// 1. Configuration & Types
// ---------------------------------------------------------------------------

#define MAX_TIMERS          16      // Maximum number of software timers
#define SYSTICK_FREQ_HZ     1000    // 1ms tick

typedef void (*timer_callback_t)(void);

typedef enum {
    TIMER_MODE_ONE_SHOT,
    TIMER_MODE_PERIODIC
} timer_mode_t;

typedef struct {
    bool             active;        // Is the timer running?
    bool             expired;       // Has it expired? (Flag for Main Loop)
    timer_mode_t     mode;          // One-shot or Periodic
    uint32_t         period_ticks;  // Reload value
    uint32_t         remaining_ticks; // Countdown value
    timer_callback_t callback;      // Function to call
} soft_timer_t;

// Static Pool - No malloc!
static soft_timer_t timer_pool[MAX_TIMERS];
static volatile uint32_t system_ticks = 0;

// ---------------------------------------------------------------------------
// 2. Public API
// ---------------------------------------------------------------------------

// Initialize the Hardware Timer (SysTick)
void Timer_Init(void) {
    // Reset all timers
    for (int i = 0; i < MAX_TIMERS; i++) {
        timer_pool[i].active = false;
        timer_pool[i].expired = false;
    }

    // In a real ARM Cortex-M, this CMSIS function sets up the hardware:
    // SysTick_Config(SystemCoreClock / SYSTICK_FREQ_HZ);
    
    printf("[System] Timer System Initialized. Tick Freq: %dHz\n", SYSTICK_FREQ_HZ);
}

// Create and Start a new timer. Returns handle (index) or -1 on failure.
int Timer_Start(uint32_t period_ms, timer_mode_t mode, timer_callback_t cb) {
    for (int i = 0; i < MAX_TIMERS; i++) {
        if (!timer_pool[i].active) {
            timer_pool[i].active = true;
            timer_pool[i].expired = false;
            timer_pool[i].mode = mode;
            timer_pool[i].period_ticks = period_ms; // Assuming 1 tick = 1 ms
            timer_pool[i].remaining_ticks = period_ms;
            timer_pool[i].callback = cb;
            
            printf("[System] Timer %d Started (%d ms)\n", i, period_ms);
            return i; // Return handle
        }
    }
    printf("[Error] No free timers available!\n");
    return -1;
}

// Stop a timer manually
void Timer_Stop(int handle) {
    if (handle >= 0 && handle < MAX_TIMERS) {
        timer_pool[handle].active = false;
        printf("[System] Timer %d Stopped\n", handle);
    }
}

// ---------------------------------------------------------------------------
// 3. Interrupt Service Routine (The "Heartbeat")
// ---------------------------------------------------------------------------

// This is the standard ARM Cortex-M Exception Handler name
void SysTick_Handler(void) {
    system_ticks++;

    // Iterate through pool (Fast! No linked list chasing)
    for (int i = 0; i < MAX_TIMERS; i++) {
        if (timer_pool[i].active) {
            if (timer_pool[i].remaining_ticks > 0) {
                timer_pool[i].remaining_ticks--;
            }

            if (timer_pool[i].remaining_ticks == 0) {
                // MARK as expired. DO NOT run callback here (keep ISR short).
                timer_pool[i].expired = true;

                // Handle Auto-Reload for Periodic
                if (timer_pool[i].mode == TIMER_MODE_PERIODIC) {
                    timer_pool[i].remaining_ticks = timer_pool[i].period_ticks;
                } else {
                    timer_pool[i].active = false; // One-shot done
                }
            }
        }
    }
}

// ---------------------------------------------------------------------------
// 4. Main Loop Task (The "Worker")
// ---------------------------------------------------------------------------

// Call this frequently in your main while(1) loop
void Timer_Process(void) {
    for (int i = 0; i < MAX_TIMERS; i++) {
        // Check atomically-ish (simple bool read is safe enough here)
        if (timer_pool[i].expired) {
            // Clear flag FIRST to allow re-entry if needed (though we are single threaded here)
            timer_pool[i].expired = false;

            // Execute Callback safely in Thread Mode
            if (timer_pool[i].callback) {
                timer_pool[i].callback();
            }
        }
    }
}

// ---------------------------------------------------------------------------
// 5. User Application (Example)
// ---------------------------------------------------------------------------

void LED_Blink_Callback(void) {
    static bool state = false;
    state = !state;
    printf("   [Callback] LED Blink! State: %s\n", state ? "ON" : "OFF");
}

void Timeout_Callback(void) {
    printf("   [Callback] Operation Timed Out! Stopping...\n");
}

int main(void) {
    Timer_Init();

    // 1. Start a Periodic Timer (e.g., LED Blink every 200ms)
    Timer_Start(200, TIMER_MODE_PERIODIC, LED_Blink_Callback);

    // 2. Start a One-Shot Timer (e.g., Timeout after 1000ms)
    Timer_Start(1000, TIMER_MODE_ONE_SHOT, Timeout_Callback);

    printf("[Main] Entering Loop...\n");

    // Simulation Loop
    // In real hardware, SysTick_Handler fires automatically.
    // Here, we simulate 1500ms of time passing.
    for (int t = 0; t < 1500; t++) {
        SysTick_Handler(); // Simulate Hardware Interrupt
        
        // Run the "OS" scheduler/process
        Timer_Process();
    }

    return 0;
}
