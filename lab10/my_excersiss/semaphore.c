#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define THREAD_COUNT 10

sem_t semaphore;

void* task(void* arg) {
    int id = *(int*)arg;

    printf("Thread %d: waiting for semaphor...\n", id);
    sem_wait(&semaphore);  // decrements counter
    printf("Thread %d: entered critical section.\n", id);

    printf("Thread %d is performing critical task\n", id);
    sleep(1);  // work simulations

    printf("Thread %d: leaves.\n", id);
    sem_post(&semaphore);  // increaments counter

    return NULL;
}

int main() {
    pthread_t threads[THREAD_COUNT];
    int ids[THREAD_COUNT];

    sem_init(&semaphore, 0, 3); 

    for (int i = 0; i < THREAD_COUNT; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, task, &ids[i]);
    }

    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&semaphore);
    return 0;
}
