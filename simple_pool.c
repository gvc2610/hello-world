//#include "threadpool.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void task1() {
	 while(1) {
	char ch;
	printf("Read1 Enter\n" );
 	printf("Enter char1:");
    scanf("%c\n",&ch);
    printf("Entered char2:%c\n",ch);
    printf("Read1 Exit\n" );
 }
}


void task2() {
	 while(1) {
	char ch;
	printf("Read2 Enter\n" );
 	printf("Enter char2:");
    scanf("%c\n",&ch);
    printf("Entered char2:%c\n",ch);
    printf("Read2 Exit\n" );
 }
}

int main(int argc, char **argv)
{
        struct threadpool *pool;
        int ret;
        /* Create a threadpool of 10 thread workers. */
        if ((pool = threadpool_init(2)) == NULL) {
                printf("Error! Failed to create a thread pool struct.\n");
                exit(EXIT_FAILURE);
        }
                        ret = threadpool_add_task(pool,task1,NULL,0);

                        ret = threadpool_add_task(pool,task2,NULL,0);

        /* Stop the pool. */
        threadpool_free(pool,1);
        return 0;
}
