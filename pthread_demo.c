#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define N 999999

int count = 0;

void *print_message_function(void *ptr);
void *add();
void *different();

void main() {
    pthread_t thread1, thread2;
    char *message1 = "Thread 1";
    char *message2 = "Thread 2";
    int iret1, iret2;

    /* Create independent threads each of which will execute function */

    iret1 =
      pthread_create(&thread1, NULL, add, (void *)message1);
    iret2 =
      pthread_create(&thread2, NULL, different, (void *)message2);

    /* Wait till threads are complete before main continues. Unless we  */
    /* wait we run the risk of executing an exit which will terminate   */
    /* the process and all threads before the threads have completed.   */

    pthread_join(thread2, NULL);
    pthread_join(thread1, NULL);


    printf("Thread 1 returns: %d\n", iret1);
    printf("Thread 2 returns: %d\n", iret2);
}

void *print_message_function(void *ptr) {
    char *message;
    message = (char *) ptr;
    for (int i=0;i<N;i++) {
        if (i % 9999 == 0) {
            printf("%s: %d\n", message, i);
        }
    }
}

void *add() {
    while (1) {
        while (count > 0) {
            // pass
        }
        printf("in add, count=%d\n", count);
        count += 2;
    }
}

void *different() {
    while (1) {
        while (count < 0) {
        }
        printf("in different, count=%d\n", count);
        count--;
    }
}