#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Variable, welche den Mutex repraesentiert.
 *
 * Von dieser Variable sollte nie eine Kopie angefertigt werden!
 */
pthread_mutex_t mutex;
pthread_mutex_t mutex2;

/**
 * Variable, welche die Condition repraesentiert.
 */
pthread_cond_t condition;
pthread_cond_t condition2;

#define MAX_SIZE_LAGER 200

typedef struct {
    int lagercounter;
} lager_t;

lager_t backlager;
int triggerflag = 0;
int counter = 0;
int lagerfreiflag = 0;

int beakingbread(){
    backlager.lagercounter++;
    return 0;
}

void* thread_func(void* arg) {
    int* mydata = (int*)arg;
    // Wir addieren zur uebergebenen Zahl 10000-mal die 1
    while(1) {

        if(lagerfreiflag == 0){
            // Sperre den Mutex
            pthread_mutex_lock(&mutex);
            if(backlager.lagercounter < 200){
                sleep(1);
                beakingbread();
            }else{
                triggerflag = 1;
                lagerfreiflag = 1;
            }


            if(backlager.lagercounter > 133){
                triggerflag = 1;
                printf("%i triggerflag %i\n",backlager.lagercounter,133);
            }

        }




        // Ende der kritischen Region

        // Entsperre den Mutex
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

/**
 * Repraesentiert den Thread, welcher auf das Eintreten eines Ereignisses wartet
 */
void* deliverBread(void* arg) {

    while(1){
        printf("Waiting Thread: Warte auf Ereignis\n");


        // Bevor man Operationen auf Conditions ausfuehrt, muss der dazugehoerige Mutex gesperrt werden
        pthread_mutex_lock(&mutex2);
        // Warte auf das Eintreten des Ereignisses
        // Der uebergebene Mutex wird waehrend des Wartens automatisch entsperrt
        pthread_cond_wait(&condition, &mutex2);
        sleep(rand() %2);
        printf("%i Broetchen abgeholt!",backlager.lagercounter);
        backlager.lagercounter = 0;
        lagerfreiflag = 0;
        // Nach Beendigung der Condition-Operation kann der Mutex wieder entsperrt werden
        pthread_mutex_unlock(&mutex2);

        printf("Waiting Thread: Ereignis ist eingetreten\n");
        return NULL;
    }

}


/**
 * Repraesentiert den Thread, welcher das Eintreten des Ereignisses dem anderen Thread mitteilt
 */
void* signaling_thread(void* arg) {
    sleep(2);
    while(1){
        sleep(1);
        printf("Signaling Thread: Warte 4 Sekunden\n");
        if(triggerflag == 1){
            printf("Signaling Thread: Teile das Eintreten der Bedingung mit\n");

            // Bevor man Operationen auf Conditions ausfuehrt, muss der dazugehoerige Mutex gesperrt werden
            pthread_mutex_lock(&mutex2);
            // Teile dem anderen Thread das Eintreten des Ereignisses mit
            pthread_cond_signal(&condition);

            // Nach Beendigung der Condition-Operation kann der Mutex wieder entsperrt werden
            triggerflag = 0;
            printf("Brotlager: %i\n",backlager.lagercounter);
            pthread_mutex_unlock(&mutex2);

            printf("Signaling Thread: Das Eintreten der Bedingung wurde mitgeteilt\n");
        }
    }

    return NULL;
}

int main() {

    pthread_t myThreads[6];
    pthread_t myCondition1[2];

    srand(time(NULL));


    // Initialisiere den Mutex
    // 1. Argument: Zeiger auf die Mutex Variable
    // 2. Argument: Zeiger auf die Attribut-Variable (NULL bedeutet Default Attribute)
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&mutex2, NULL);

    // Initialisiere die Condition Variable
    // 1. Argument: Zeiger auf die Condition Variable
    // 2. Argument: Zeiger auf die Attribut-Variable (NULL bedeutet Default Attribute)
    pthread_cond_init(&condition, NULL);

    // Erzeuge den "Waiting"-Thread
    pthread_create(&myCondition1[0], NULL, &deliverBread, NULL);


    // Erzeuge den "Signaling"-Thread
    pthread_create(&myCondition1[1], NULL, &signaling_thread, NULL);


    // Erzeuge 10 Threads
    for (int i = 0; i < 6; i++) {
        printf("Erzeuge Thread %d\n", i);
        // Erzeuge den Thread
        pthread_create(&myThreads[i], NULL, &thread_func, &backlager.lagercounter);
    }

    if(counter == 10){
        return 0;
    }


    // Warte auf die Beendigung aller Threads
    for (int i = 0; i < 6; i++) {
        pthread_join(myThreads[i], NULL);
        printf("Thread %d wurde beendet\n", i);
    }

    // Gebe das Endergebnis aus
    printf("mydata = %d\n", backlager.lagercounter);

    // Gebe den Mutex wieder frei
    pthread_mutex_destroy(&mutex);

    // Gebe die Condition wieder frei
    pthread_cond_destroy(&condition);

    return 0;
}
