// @siliconx
// 2017-05-10 09:56:00

// The producer-consumer problem base on pthread

#include <stdio.h>
#include <pthread.h>
#define N 4  // size of buffer
#define MAX_PRODUCT 16  // max count of new product

int wait(int*);
int signal(int*);

void *producer();
void *consumer();


int buffer[N];  // buffer
int mutex=1, empty=N, full=0;  // semaphore in intger
int in=0, out=0;
int new_product=0;  // how many product create
int consumed_product=0;  // how many product consume

int main(void) {
    pthread_t producer_thread, consumer_thread;
    long t = 0;
    // thread for producer
    pthread_create(&producer_thread, NULL, producer, (void *)t);

    // thread for consumer
    pthread_create(&consumer_thread, NULL, consumer, (void *)t);

    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);
    return 0;
}

int wait(int *s) {
    while (*s <= 0) {
        // waiting...
    }
    (*s)--;
    return 0;
}

int signal(int *s) {
    (*s)++;
    return 0;
}

void *producer() {
    do {
        new_product++;  // new product
        wait(&empty);
        wait(&mutex);

        buffer[in] = new_product;
        printf("\x1B[32mcreate new product: %d\n", new_product);

        in = (in + 1) % N;
        signal(&mutex);
        signal(&full);
        if (new_product >= MAX_PRODUCT) {
            break;
        }
    } while(1);
}

void *consumer() {
    do {
        wait(&full);
        wait(&mutex);

        int consume_product = buffer[out];
        printf("\x1B[31mconsume the product: %d\n", consume_product);
        consumed_product++;

        out = (out + 1) % N;
        signal(&mutex);
        signal(&empty);
        if (consumed_product >= MAX_PRODUCT) {
            break;
        }
    } while(1);
}
