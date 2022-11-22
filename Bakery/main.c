
#include <pthread.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

#define STORAGE_MAX_COUNT 30
#define STORAGE_SUPPLIER_COUNT 20

typedef struct {
    int count;
    pthread_mutex_t mutex;
} storage_t;

pthread_cond_t supplier_cond;
pthread_mutex_t supplier_mutex;

pthread_cond_t baking_cond;
pthread_mutex_t baking_mutex;

storage_t storage;

int shutdownFlag = 0;
int supplierInformed = 0;

void* bakingBread(void* arg) {
    while(!shutdownFlag) {
        // Brot backen

        if(pthread_mutex_lock(&storage.mutex)){
            perror("Could not lock bakingBread_storage.mutex");
            exit(1);
        }

        if (storage.count < STORAGE_MAX_COUNT) {
            printf("Lagerstand wird erhöht\n");
            storage.count += 1;
            if (storage.count >= STORAGE_SUPPLIER_COUNT) {
                // Lieferant benachrichtigen
                if (supplierInformed == 0) {
                    printf("Lieferant wird informiert\n");
                    if(pthread_mutex_lock(&supplier_mutex)){
                        perror("Could not lock bakingBread_supplier_mutex");
                        exit(1);
                    }
                    if(pthread_cond_signal(&supplier_cond)){
                        perror("Could not lock bakingBread_supplier_cond");
                        exit(1);
                    }
                    if(pthread_mutex_unlock(&supplier_mutex)){
                        perror("Could not unlock bakingBread_supplier_mutex");
                        exit(1);
                    }
                    supplierInformed = 1;
                }
            }
            if(pthread_mutex_unlock(&storage.mutex)){
                perror("Could not unlock bakingBread_storage.mutex");
                exit(1);
            }
            sleep(1);
        } else {

            if(pthread_mutex_unlock(&storage.mutex)){
                perror("Could not unlock storage.mutex");
                exit(1);
            }
            // Warten bis Platz im Lager
            printf("Warte bis Platz im Lager\n");
            if(pthread_mutex_lock(&baking_mutex)){
                perror("Could not lock bakingBread_baking_mutex");
                exit(1);
            }
            if(pthread_cond_wait(&baking_cond, &baking_mutex)){
                perror("Could not unlock bakingBread_cond_wait");
                exit(1);
            }
            if(pthread_mutex_unlock(&baking_mutex)){
                perror("Could not unlock bakingBread_baking_mutex");
                exit(1);
            }
            printf("Warte bis Platz im Lager: Aufgeweckt\n");
        }
    }
    printf("Backstation beendet sich\n");
    return 0;
}


void* deliverBread(void* arg) {
    while(!shutdownFlag) {

        // Schlafen und warten auf signal
        printf("Lieferant wartet auf Anruf\n");
        if(pthread_mutex_lock(&supplier_mutex)){
            perror("Could not lock supplier_mutex");
            exit(1);
        }
        if(pthread_cond_wait(&supplier_cond, &supplier_mutex)){
            perror("Could not cond_wait deliverBread_supplier_mutex");
            exit(1);
        }
        if(pthread_mutex_unlock(&supplier_mutex)){
            perror("Could not unlock supplier_mutex");
            exit(1);
        }
        printf("Lieferant hat Anruf bekommen\n");

        if (!shutdownFlag) {

            // Zeitlang warten (Fahrtzeit zum Lager)
            sleep(4);

            // lagerstand auf 0 setzen
            printf("Lieferant hat lager geleert\n");
            if(pthread_mutex_lock(&storage.mutex)){
                perror("Could not lock storage.mutex");
                exit(1);
            }
            storage.count = 0;
            if(pthread_mutex_unlock(&storage.mutex)){
                perror("Could not unlock storage.mutex");
                exit(1);
            }
            supplierInformed = 0;

            // Backstationen aufwecken
            if(pthread_mutex_lock(&baking_mutex)){
                perror("Could not lock deliverBread_baking_mutex");
                exit(1);
            }
            if(pthread_cond_broadcast(&baking_cond)){
                perror("Could not cond_broadcast deliverBread_baking_cond");
                exit(1);
            }
            if(pthread_mutex_unlock(&baking_mutex)){
                perror("Could not unlock deliverBread_baking_mutex");
                exit(1);
            }
        }
    }
    printf("Lieferant beendet sich\n");
    return 0;
}


void qualityCheck(int signum) {
    printf("qualityCheck()\n");
    int count = 4;

    if(pthread_mutex_lock(&storage.mutex)){
        perror("Could not lock storage.mutex");
        exit(1);
    }
    if (storage.count < count) {
        count = storage.count;
    }
    storage.count -= count;
    if(pthread_mutex_unlock(&storage.mutex)){
        perror("Could not unlock storage.mutex");
        exit(1);
    }

    // Backstationen aufwecken

    if(pthread_mutex_lock(&baking_mutex)){
        perror("Could not lock qualityCheck_baking_mutex");
        exit(1);
    }
    if(pthread_cond_broadcast(&baking_cond)){
        perror("Could not cond_broadcast qualityCheck_baking_cond");
        exit(1);
    }
    if(pthread_mutex_unlock(&baking_mutex)){
        perror("Could not unlock qualityCheck_baking_mutex");
        exit(1);
    }
    alarm(5);
}


void graceful_shutdown(int signum) {
    printf("Graceful shutdown\n");
    shutdownFlag = 1;
    // Backstationen aufwecken
    if(pthread_mutex_lock(&baking_mutex)){
        perror("Could not lock graceful_shutdown_baking_mutex");
        exit(1);
    }
    if(pthread_cond_broadcast(&baking_cond)){
        perror("Could not cond_broadcast graceful_shutdown_baking_cond");
        exit(1);
    }
    if(pthread_mutex_unlock(&baking_mutex)){
        perror("Could not unlock graceful_shutdown_baking_mutex");
        exit(1);
    }

    // Lieferant benachrichtigen

    if(pthread_mutex_lock(&supplier_mutex)){
        perror("Could not lock graceful_shutdown_baking_mutex");
        exit(1);
    }
    if(pthread_cond_broadcast(&supplier_cond)){
        perror("Could not cond_broadcast graceful_shutdown_supplier_cond");
        exit(1);
    }
    if(pthread_mutex_unlock(&supplier_mutex)){
        perror("Could not unlock graceful_shutdown_supplier_mutex");
        exit(1);
    }
}


int main() {

    storage.count = 0;

    //****************************************************************************************************************

    //Initializeing mutex and check if return 0
    int mutex_initializing_error_array[5];
    mutex_initializing_error_array[0] = pthread_mutex_init(&storage.mutex, NULL);
    mutex_initializing_error_array[1] = pthread_mutex_init(&supplier_mutex, NULL);
    mutex_initializing_error_array[2] = pthread_mutex_init(&baking_mutex, NULL);
    mutex_initializing_error_array[3] = pthread_cond_init(&supplier_cond, NULL);
    mutex_initializing_error_array[4] = pthread_cond_init(&baking_cond, NULL);

    for(int i = 0; i < sizeof(mutex_initializing_error_array) / sizeof(mutex_initializing_error_array[0]) ; i++){
        if (mutex_initializing_error_array[i]){
            fprintf(stderr,"Error while initializing mutex/cond index: %i \n",i);
            perror("Error");
            exit(1);
        }
    }

    //****************************************************************************************************************

    // Registriere Signal Handler für ALARM Signal
    struct sigaction signal;
    signal.sa_handler = qualityCheck;
    int signal_error_array[8];

    signal_error_array[0] = sigemptyset(&signal.sa_mask);
    signal_error_array[1] = sigaction(SIGALRM, &signal, NULL);
    signal.sa_handler = graceful_shutdown;
    signal_error_array[2] = sigaddset(&signal.sa_mask, SIGTERM);
    signal_error_array[3] = sigaddset(&signal.sa_mask, SIGINT);
    signal_error_array[4] = sigaddset(&signal.sa_mask, SIGQUIT);
    signal.sa_flags = SA_RESTART;
    signal_error_array[5] = sigaction(SIGTERM, &signal, NULL);
    signal_error_array[6] = sigaction(SIGINT, &signal, NULL);
    signal_error_array[7] = sigaction(SIGQUIT, &signal, NULL);

    for(int i = 0; i < sizeof(signal_error_array) / sizeof(signal_error_array[0]) ; i++){
        if (signal_error_array[i]){
            fprintf(stderr,"Error while working on main_signal_handlers, index: %i \n",i);
            perror("Error");
            exit(1);
        }
    }

    //****************************************************************************************************************

    pthread_t bakingThread[6];
    for (int i = 0; i < 6; i++) {
        pthread_create(&bakingThread[i], NULL, bakingBread, NULL);
    }

    pthread_t supplierThread;
    pthread_create(&supplierThread, NULL, deliverBread, NULL);

    alarm(5);

    for (int i = 0; i < 6; i++) {
        pthread_join(bakingThread[i], NULL);
    }
    pthread_join(supplierThread, NULL);

    //****************************************************************************************************************
    //Destroying all mutexes
    int mutex_destroying_error_array[5];

    mutex_destroying_error_array[0] = pthread_mutex_destroy(&storage.mutex);
    mutex_destroying_error_array[1] = pthread_mutex_destroy(&supplier_mutex);
    mutex_destroying_error_array[2] = pthread_mutex_destroy(&baking_mutex);
    mutex_destroying_error_array[3] = pthread_cond_destroy(&supplier_cond);
    mutex_destroying_error_array[4] = pthread_cond_destroy(&baking_cond);

    for(int i = 0; i < sizeof(mutex_destroying_error_array) / sizeof(mutex_destroying_error_array[0]) ; i++){
        if (mutex_destroying_error_array[i]){
            fprintf(stderr,"Error while destroying main_mutex/cond index: %i \n",i);
            perror("Error");
            exit(1);
        }
    }
    return 0;
}
