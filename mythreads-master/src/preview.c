/// @brief Represents supported scheduling policis, that can be executed by scheduler.
typedef enum
{
    POLICY_ROUND_ROBIN
} schedulingpolicy_t;

typedef struct
{
    bool started;
    schedulingpolicy_t scheduling_policy;
    struct sigaction signal_action;
    unsigned int threads_num;
    unsigned int next_id;
    threadnode_t *current_thread_node;
    threadnode_t *dead_thread_node;
    ucontext_t context;
    char stack[THREAD_STACK_DEPTH];
} scheduler_t;

/// @brief Represents current state of thread in terms of scheduling.
typedef enum
{
    STATE_NEW,
    STATE_PENDING,
    STATE_READY,
    STATE_RUNNING,
    STATE_DEAD
} taskstate_t;

// @brief Represents thread handle and all its resources.
typedef struct
{
    int id;
    char name[THREAD_NAME_MAX_LENGTH];
    char stack[THREAD_STACK_DEPTH];
    int priority;
    bool was_preempted;
    taskstate_t state;
    ucontext_t context;
    mymutex_t *pending_mutex;
} mythread_t;

/// @brief Represents node in POSIX doubly-linked list.
typedef struct node
{
    struct node *next;
    struct node *prev;
    mythread_t *thread;
} threadnode_t;

/// @brief Represents mutex to synchronize threads.
typedef struct
{
    bool locked;
    struct node *owner;
    int owner_original_priority;
} mymutex_t;


TimerISR();
TImer_set(int val);
TImer_en_dis(bool en);



save_context();
restore_context();
make_context();

disable_interrupts();
enable_interrupts();

queue <node*> pending,ready;



int main() {



    
}
