#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <stdint.h>
#include <stdbool.h>


enum ret_codes { ok, fail, repeat};

int entry_state(void) {
    printf("%s\n","In entry_state" );
    return fail;
}
int foo_state(void) {
    printf("%s\n","In foo_state" );
    return ok;
}
int bar_state(void) {
    printf("%s\n", "In bar_state");
    return ok;
}
int exit_state(void) {
    printf("%s\n","In exit_state" );
    return 0;
}

/* array and enum below must be in sync! */
int (* state[])(void) = { entry_state, foo_state, bar_state, exit_state};
enum state_codes { entry, foo, bar, end};

struct transition {
    enum state_codes src_state;
    enum ret_codes   ret_code;
    enum state_codes dst_state;
};
/* transitions from end state aren't needed */
struct transition state_transitions[] = {
    {entry, ok,     foo},
    {entry, fail,   end},
    {foo,   ok,     bar},
    {foo,   fail,   end},
    {foo,   repeat, foo},
    {bar,   ok,     end},
    {bar,   fail,   end},
    {bar,   repeat, foo}};

#define EXIT_STATE end
#define ENTRY_STATE entry

enum state_codes lookup_transitions(enum state_codes state, enum ret_codes ret) {
    
    for(int i=0; i<sizeof(state_transitions)/sizeof(state_transitions[0]);i++) {
        if(state_transitions[i].src_state == state) {
            if(state_transitions[i].ret_code == ret) return state_transitions[i].dst_state;
        }
    }
   
   printf("%s\n","Invalid state transition" );
   return 0;
}

int main(int argc, char *argv[]) {
    enum state_codes cur_state = ENTRY_STATE;
    enum ret_codes rc;
    int (* state_fun)(void);

    for (;;) {
        state_fun = state[cur_state];
        rc = state_fun();
        if (EXIT_STATE == cur_state)
            break;
        cur_state = lookup_transitions(cur_state, rc);
    }

    return EXIT_SUCCESS;
}