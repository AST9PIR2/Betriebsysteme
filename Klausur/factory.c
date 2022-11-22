
//coded by Pirmin Aster
//Matrikelnummer 52108897 |2110836038
#include <pthread.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define STORAGE_MAX_COUNT 30

typedef struct {
    int kerzen;
    int wachs;
    int docht;
    pthread_mutex_t mutex;
} storage_t;

pthread_cond_t supplier_cond;
pthread_mutex_t supplier_mutex;

pthread_cond_t lager_cond;
pthread_mutex_t lager_mutex;

storage_t storage;

int shutdownFlag = 0;
int supplierInformed = 0;
int wachs;
int docht;
int ablaufcounter = 0;


void* createCandle(void* arg) {
    while(!shutdownFlag) {

        if(pthread_mutex_lock(&storage.mutex)){
            perror("Could not lock createCandle_storage.mutex");
            exit(1);
        }

        if(shutdownFlag){
            if(pthread_mutex_unlock(&storage.mutex)){
                perror("Could not unlock createCandle_storage.mutex");
                exit(1);
            }
            break;
        }

        if (storage.kerzen < STORAGE_MAX_COUNT && storage.wachs >= 3 && storage.docht >= 1) {

            printf("Eine Kerze wird aus 3 x Wachs und 1 x Docht hergestellt\n");
            storage.wachs -= 3;
            storage.docht -= 1;
            sleep(1);
            storage.kerzen += 1;
            if(pthread_mutex_unlock(&storage.mutex)){
                perror("Could not unlock createCandle_storage.mutex");
                exit(1);
            }
        } else {

            if(pthread_mutex_unlock(&storage.mutex)){
                perror("Could not unlock createCandle_storage.mutex");
                exit(1);
            }
            // Warten bis Platz im Lager
            printf("Warte bis Platz im Lager. Nickerchen!\n");
            if(pthread_mutex_lock(&lager_mutex)){
                perror("Could not lock createCandle_lager_mutex");
                exit(1);
            }
            if(pthread_cond_wait(&lager_cond, &lager_mutex)){
                perror("Could not unlock createCandle_cond_wait");
                exit(1);
            }
            if(pthread_mutex_unlock(&lager_mutex)){
                perror("Could not unlock createCandle_lager_mutex");
                exit(1);
            }
            printf("Warte bis Platz im Lager: Aufgeweckt\n");
        }


    }
    printf("Kerzenfabrik beendet sich\n");
    return 0;
}


void* manageStorage(void* arg) {

    while(!shutdownFlag) {


        // Schlafen und warten auf signal
        printf("Lieferant wartet auf Anruf\n");
        if(pthread_mutex_lock(&supplier_mutex)){
            perror("Could not lock manageStorage_supplier_mutex");
            exit(1);
        }
        if(pthread_cond_wait(&supplier_cond, &supplier_mutex)){
            perror("Could not cond_wait manageStorage_supplier_mutex");
            exit(1);
        }
        if(pthread_mutex_unlock(&supplier_mutex)){
            perror("Could not unlock manageStorage_supplier_mutex");
            exit(1);
        }

        printf("Lieferant hat Anruf bekommen\n");

        if (!shutdownFlag) {

            // lagerstand auf 0 setzen
            printf("Kerzen werden ausgeliefert!\n");
            if(pthread_mutex_lock(&storage.mutex)){
                perror("Could not lock manageStorage_storage.mutex");
                exit(1);
            }
            storage.kerzen = 0;
            sleep(1);
            if(pthread_mutex_unlock(&storage.mutex)){
                perror("Could not unlock manageStorage_storage.mutex");
                exit(1);
            }
            supplierInformed = 0;

            // Kerzenfabrik aufwecken
            if(pthread_mutex_lock(&lager_mutex)){
                perror("Could not lock manageStorage_lager_mutex");
                exit(1);
            }
            if(pthread_cond_broadcast(&lager_cond)){
                perror("Could not cond_broadcast manageStorage_lager_cond");
                exit(1);
            }
            if(pthread_mutex_unlock(&lager_mutex)){
                perror("Could not unlock manageStorage_lager_mutex");
                exit(1);
            }
        }
    }
    printf("Lieferant beendet sich\n");
    return 0;
}


void purchase(int signum) {
    srand(time(NULL));

    if(pthread_mutex_lock(&storage.mutex)){
        perror("Could not lock storage.mutex");
        exit(1);
    }

    //Ablauf 10, 15, 20
    if(ablaufcounter == 0){
        ablaufcounter++;
    }else if(ablaufcounter == 1){
        wachs = rand() % 60;
        storage.wachs += wachs;
        printf("Bestände werden : +%i x Wachs\n", wachs );
        ablaufcounter++;
    }else if(ablaufcounter == 2){
        docht = rand() % 20;
        storage.docht += docht;
        printf("Bestände werden : +%i x Docht\n", docht );
        ablaufcounter++;
    }else{
        // Lieferant benachrichtigen
        if (supplierInformed == 0) {
            printf("Lieferant wird informiert\n");
            if(pthread_mutex_lock(&supplier_mutex)){
                perror("Could not lock purchase_supplier_mutex");
                exit(1);
            }
            if(pthread_cond_signal(&supplier_cond)){
                perror("Could not lock purchase_supplier_cond");
                exit(1);
            }
            if(pthread_mutex_unlock(&supplier_mutex)){
                perror("Could not unlock purchase_supplier_mutex");
                exit(1);
            }
            supplierInformed = 1;
        }
        ablaufcounter = 0;
    }

    if(pthread_mutex_unlock(&storage.mutex)){
        perror("Could not unlock purchase_storage.mutex");
        exit(1);
    }

    // Kerzenfabrik aufwecken
    if(pthread_mutex_lock(&lager_mutex)){
        perror("Could not lock purchase_lager_mutex");
        exit(1);
    }
    if(pthread_cond_broadcast(&lager_cond)){
        perror("Could not cond_broadcast purchase_lager_cond");
        exit(1);
    }
    if(pthread_mutex_unlock(&lager_mutex)){
        perror("Could not unlock purchase_lager_mutex");
        exit(1);
    }
    alarm(5);
}


void graceful_shutdown(int signum) {
    printf("Graceful shutdown\n");
    shutdownFlag = 1;

    // Kerzenfabrik benachrichtigen
    if(pthread_mutex_lock(&lager_mutex)){
        perror("Could not lock graceful_shutdown_lager_mutex");
        exit(1);
    }
    if(pthread_cond_broadcast(&lager_cond)){
        perror("Could not cond_broadcast graceful_shutdown_lager_cond");
        exit(1);
    }
    if(pthread_mutex_unlock(&lager_mutex)){
        perror("Could not unlock graceful_shutdown_lager_mutex");
        exit(1);
    }

    // Lieferant benachrichtigen
    if(pthread_mutex_lock(&supplier_mutex)){
        perror("Could not lock graceful_shutdown_lager_mutex");
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

    storage.kerzen = 0;
    storage.wachs = 18;
    storage.docht = 6;

    //****************************************************************************************************************

    //Initializing mutex and check if return 0
    int mutex_initializing_error_array[5];
    mutex_initializing_error_array[0] = pthread_mutex_init(&storage.mutex, NULL);
    mutex_initializing_error_array[1] = pthread_mutex_init(&supplier_mutex, NULL);
    mutex_initializing_error_array[2] = pthread_mutex_init(&lager_mutex, NULL);
    mutex_initializing_error_array[3] = pthread_cond_init(&supplier_cond, NULL);
    mutex_initializing_error_array[4] = pthread_cond_init(&lager_cond, NULL);

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
    memset(&signal, 0, sizeof(signal));
    signal.sa_handler = purchase;
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
        pthread_create(&bakingThread[i], NULL, createCandle, NULL);
    }

    pthread_t supplierThread;
    pthread_create(&supplierThread, NULL, manageStorage, NULL);

    alarm(6);

    for (int i = 0; i < 6; i++) {
        pthread_join(bakingThread[i], NULL);
    }
    pthread_join(supplierThread, NULL);
    printf("All threads finished and closed!\n");

    //****************************************************************************************************************
    //Destroying all mutexes
    int mutex_destroying_error_array[5];

    mutex_destroying_error_array[0] = pthread_mutex_destroy(&storage.mutex);
    mutex_destroying_error_array[1] = pthread_mutex_destroy(&supplier_mutex);
    mutex_destroying_error_array[2] = pthread_mutex_destroy(&lager_mutex);
    mutex_destroying_error_array[3] = pthread_cond_destroy(&supplier_cond);
    mutex_destroying_error_array[4] = pthread_cond_destroy(&lager_cond);

    for(int i = 0; i < sizeof(mutex_destroying_error_array) / sizeof(mutex_destroying_error_array[0]) ; i++){
        if (mutex_destroying_error_array[i]){
            fprintf(stderr,"Error while destroying main_mutex/cond index: %i \n",i);
            perror("Error");
            exit(1);
        }
    }
    printf("All mutex cleared!\nMemory freed!\n");
    return 0;
}
