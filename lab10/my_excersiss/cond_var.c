#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int buffer = 0;  // 0 = empty, 1 = full

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void* producer(void* arg) {
    for (int i = 0; i < 5; i++) {
        pthread_mutex_lock(&mutex);
        
        while (buffer == 1) {
            // wait for consumer to consume
            pthread_cond_wait(&cond, &mutex);
        }

        buffer = 1;
        printf("Creator: Produced (iteration %d)\n", i);

        pthread_cond_signal(&cond);  // wake up the consumer
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    return NULL;
}

void* consumer(void* arg) {
    for (int i = 0; i < 5; i++) {
        pthread_mutex_lock(&mutex);
        
        while (buffer == 0) {
            // wait for creator to provide
            pthread_cond_wait(&cond, &mutex);
        }

        buffer = 0;
        printf("Consumer: consumed (iteration %d)\n", i);

        pthread_cond_signal(&cond);  // wake up creator
        pthread_mutex_unlock(&mutex);
        sleep(2);
    }
    return NULL;
}

int main() {
    pthread_t prod, cons;

    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}
