#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
 
sem_t even_lock, odd_lock;
pthread_mutex_t mutex ;
 
void * even_thread(void *args) {
   int i = 0;
   while (i < 10) {
      sem_wait(&even_lock);
      //sleep(random()%3);
      printf("even:%d\n", i);
      i+=2;
      sem_post(&odd_lock);
    }
    pthread_exit(0);
}
 
void * odd_thread(void *args) {
    int i = 1;
    while (i < 10) {
        sem_wait(&odd_lock);
        //sleep(random()%4);
        printf("odd:%d\n", i);
        i+=2;
        sem_post(&even_lock);
    }
    pthread_exit(0);
}


 int g_j;
void * even_thread4(void *args) {
   //int i = 0;
   while (g_j < 10) {
      pthread_mutex_lock(&mutex);
      //sleep(random()%3);
    //  printf("Even\n");
      //sleep(1); 
      if(g_j%2 == 0) {
      printf("even:%d\n", g_j);
      g_j+=1;
    }
      pthread_mutex_unlock(&mutex);
    }
    pthread_exit(0);
}
 
void * odd_thread4(void *args) {
    //int i = 1;
    while (g_j < 10) {
        pthread_mutex_lock(&mutex);
        // sleep(random()%4);
       // printf("Odd\n");
        //sleep(1);
        if(g_j%2 !=0) {
        printf("odd:%d\n", g_j);
        g_j+=1;
      }
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(0);
}


int main() {
     pthread_t thread[2];
     sem_init(&even_lock, 0, 1);
     sem_init(&odd_lock, 0, 0);
 
     printf("Solution semaphores:\n");
     pthread_create(&thread[0], NULL, even_thread, NULL);
     pthread_create(&thread[1], NULL, odd_thread, NULL);
     pthread_join(thread[0], NULL);
     pthread_join(thread[1], NULL);
 
 
     pthread_mutex_init(&mutex, NULL);
     //pthread_mutex_init(&odd_mutex, NULL);

/* wrong mu should be acquired and released by same thread 
     printf("Solution mutexes:\n");
     pthread_create(&thread[0], NULL, even_thread2, NULL);
     pthread_create(&thread[1], NULL, odd_thread2, NULL);
     pthread_join(thread[0], NULL);
     pthread_join(thread[1], NULL);
 
     printf("Solution mutexes Resource:\n");
     pthread_create(&thread[0], NULL, even_thread3, NULL);
     pthread_create(&thread[1], NULL, odd_thread3, NULL);
     pthread_join(thread[0], NULL);
     pthread_join(thread[1], NULL);

 */

     printf("Solution mutexes :\n");
     pthread_create(&thread[0], NULL, even_thread4, NULL);
     pthread_create(&thread[1], NULL, odd_thread4, NULL);
     pthread_join(thread[0], NULL);
     pthread_join(thread[1], NULL);

     sem_destroy(&even_lock);
     sem_destroy(&odd_lock);
     pthread_mutex_destroy(&mutex);
     //pthread_mutex_destroy(&odd_mutex);
     return 0;
}