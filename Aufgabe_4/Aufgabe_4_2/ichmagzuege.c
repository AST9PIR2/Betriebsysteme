#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

sem_t semaphore;

typedef struct{
    char * zugname;
    int dauer;
}Zug;

Zug G[10];

int befahreneStecke(int arg){
    printf("%s fährt auf Abschnitt ein. Thread: %i\n", G[arg].zugname,arg);
    fflush(stdout);
    return 0;
}


void* thread_func(void* arg) {
    int* mydata = (int*)arg;
    sem_wait(&semaphore);
    befahreneStecke(*mydata);
    sleep(1);
    sem_post(&semaphore);
}

int main() {
    // Reserviere Speicher fÃ¼r die Thread-Handles
    // Initialisiere den Semaphor
    // 1. Argument: Zeiger auf die Semaphore Variable
    // 2. Argument: 0 ... wird zwischen Threads geteilt, 1 ...  wird zwischen Prozessen geteilt
    // 3. Argument: Anfangswert des Semaphors
    sem_init(&semaphore, 0, 1);

    pthread_t myThreads[10];

    char namepool[20][10] = {"Intercity","TrainIt","ÖBB","DB","Rittner", "Hypertrain", "Locomo", "Speddio", "Dampfi", "Schwebi"};


    for(int i = 0; i < 10; i++){
        G[i].zugname = namepool[rand() % 10];
        G[i].dauer = rand() % 10;
    }

    for(int i = 0; i < 10; i++) {
        printf("Neuer Zuege angelegt auf und geparkt auf Gleis %i.\tName: %20s.\t\tStreckenabschnittsdauer: %i.\n", i, G[i].zugname, G[i].dauer);
    }
    int counter = 0;

    int array[10];

    // Erzeuge 10 Threads
    for (int i = 0; i < 10; i++) {
        printf("Erzeuge Thread %d\n", i);
        fflush(stdout);
        // Erzeuge den Thread

        array[i] = i;

        pthread_create(&myThreads[i], NULL, &thread_func, &array[i]);


    }

    // Warte auf die Beendigung aller Threads
    for (int i = 0; i < 10; i++) {
        pthread_join(myThreads[i], NULL);
        printf("Thread %d wurde beendet\n", i);
    }

    // Gebe den Semaphor wieder frei
    sem_destroy(&semaphore);
    printf("Semaphore wurde zerstört\n");

    return 0;
}