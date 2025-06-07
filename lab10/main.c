#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>

#define MAX_PATIENTS_IN_HOSPITAL 3
#define MAX_MEDICINE 6
#define MINIMAL_OK_AMOUNT_OF_MEDICINE 3
#define MAX_QUEUE_SIZE 100

// Synchronization primitives
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t doctor_wakeup = PTHREAD_COND_INITIALIZER;
pthread_cond_t medicine_space = PTHREAD_COND_INITIALIZER;

// Simulation state
int waiting_patients = 0;
int medicine = MAX_MEDICINE;
bool delivery_in_progress = false;

// Patient ID queue
int patient_queue[MAX_QUEUE_SIZE];
int queue_front = 0, queue_rear = 0;

void enqueue_patient(int id) {
    patient_queue[queue_rear++] = id;
    if (queue_rear == MAX_QUEUE_SIZE) queue_rear = 0;
}

int dequeue_patient() {
    int id = patient_queue[queue_front++];
    if (queue_front == MAX_QUEUE_SIZE) queue_front = 0;
    return id;
}

// Timestamped logger
void print_time() {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    printf("[%02d:%02d:%02d] ", t->tm_hour, t->tm_min, t->tm_sec);
}

// Patient thread
void *patient_thread(void *arg) {
    int id = *(int *)arg;
    free(arg);

    while (1) {
        int sleep_time = rand() % 4 + 2;
        printf("Patient(%d): Wybieram się , będe za : %d\n", id, sleep_time);

        sleep(sleep_time);

        pthread_mutex_lock(&mutex);

        if (waiting_patients >= MAX_PATIENTS_IN_HOSPITAL) {
            pthread_mutex_unlock(&mutex);
            print_time();
            int sleep_time = rand() % 4 + 2;
            printf("Patient(%d): Too many patients, retrying later in %d seconds\n", id, sleep_time);
            sleep(sleep_time);
            continue;
        }

        waiting_patients++;
        enqueue_patient(id);
        print_time();
        printf("Patient(%d): Entered hospital, waiting patients: %d\n", id, waiting_patients);

        if (waiting_patients == 3) {
            print_time();
            printf("Patient(%d): Notifying doctor\n", id);
            pthread_cond_signal(&doctor_wakeup);
        }

        pthread_mutex_unlock(&mutex);
        // Wait before next hospital visit
        sleep(rand() % 6 + 5);
    }
    return NULL;
}

// Pharmacist thread
void *pharmacist_thread(void *arg) {
    int id = *(int *)arg;
    free(arg);

    while (1) {
        int sleep_time = rand() % 11 + 5;
        printf("Pharmacist(%d): Ide do szpitala bede za  %d\n", id, sleep_time);
        sleep(sleep_time); // Travel time
        
        pthread_mutex_lock(&mutex);

        while (medicine == MAX_MEDICINE) {
            print_time();
            printf("Pharmacist(%d): Waiting, cabinet full\n", id);
            pthread_cond_wait(&medicine_space, &mutex);
        }

        if (medicine < MINIMAL_OK_AMOUNT_OF_MEDICINE) {
            delivery_in_progress = true;
            print_time();
            printf("Pharmacist(%d): Requesting doctor for delivery\n", id);
            pthread_cond_signal(&doctor_wakeup);
        }

        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

// Doctor thread
void *doctor_thread(void *arg) {
    pthread_mutex_lock(&mutex);
    while (1) {
        while (!((waiting_patients >= 3 && medicine >= 3) ||
                 (delivery_in_progress && medicine < MINIMAL_OK_AMOUNT_OF_MEDICINE))) {
            pthread_cond_wait(&doctor_wakeup, &mutex);
        }

        print_time();
        printf("Doctor: Woken up\n");

        if (waiting_patients >= 3 && medicine >= 3) {
            print_time();
            printf("Doctor: Consulting 3 patients...\n");
            medicine -= 3;
            waiting_patients -= 3;
            for (int i = 0; i < 3; i++) {
                int pid = dequeue_patient();
                sleep(1);  // Simulate consult
                print_time();
                printf("Doctor: Consultation done for Patient(%d)\n", pid);
            }
            pthread_cond_signal(&medicine_space); // May allow pharmacist in
        }

        else if (delivery_in_progress && medicine < MINIMAL_OK_AMOUNT_OF_MEDICINE) {
            int refill = MAX_MEDICINE - medicine;
            sleep(1);  // Simulate acceptance
            medicine = MAX_MEDICINE;
            delivery_in_progress = false;

            print_time();
            printf("Doctor: Accepted delivery, refilled %d units\n", refill);
            pthread_cond_broadcast(&medicine_space);
        }

        print_time();
        printf("Doctor: Going back to sleep\n");
    }
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void handle_sigint(int sig) {
    printf("\nCaught signal %d (Ctrl+C). Exiting...\n", sig);
    exit(0);
}

int main(int argc, char *argv[]) {

    signal(SIGINT, handle_sigint);

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <num_patients> <num_pharmacists>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int total_patients = atoi(argv[1]);
    int total_pharmacists = atoi(argv[2]);

    srand(time(NULL));

    pthread_t doctor;
    pthread_t patient_threads[total_patients];
    pthread_t pharmacist_threads[total_pharmacists];

    pthread_create(&doctor, NULL, doctor_thread, NULL);

    for (int i = 0; i < total_patients; i++) {
        int *id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&patient_threads[i], NULL, patient_thread, id);
    }

    for (int i = 0; i < total_pharmacists; i++) {
        int *id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&pharmacist_threads[i], NULL, pharmacist_thread, id);
    }

    pthread_join(doctor, NULL); // Keep main alive (though doctor never exits)
    return EXIT_SUCCESS;
}
