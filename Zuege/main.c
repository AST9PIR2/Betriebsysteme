#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>
#include <signal.h>


#define ZUG_COUNT 4
#define STRECKE_COUNT 3


// Variable representing semaphore
sem_t semaphore;


// Struct representing a train
typedef struct {
    char zugname[10];
    int dauer;
} Zug;


// Counts the trains allowed to pass the rail section
int train_count = 0;

// Flag used for exiting the threads
int thread_shutdown_flag = 0;

// Function for manaing the trains that want to enter the rail section
void befahreStrecke(Zug zug) {
    // Fahre in Streckenabschnitt
    // We use sem_trywait to be able to print a message when we are not able to sucessfully lock the semaphore
    if(sem_trywait(&semaphore) != 0) {
        printf("Fahrterlaubnis für %s verweigert und muss auf ein freies Gleis warten!\n", zug.zugname);
        sem_wait(&semaphore);
    }
    // Shortcut for exiting the thread
    if (thread_shutdown_flag != 0) {
        sem_post(&semaphore);
        return;
    }
    printf("Fahrterlaubnis für %s wurde erteilt!\n", zug.zugname);
    train_count++;

    // After 10 trains we exit the program
    if (train_count >= 10) {
        thread_shutdown_flag = 1;
    }

    // Verweile in Streckenabschnitt
    printf("Zug %s befindet sich %d Sekunden in Abschnitt\n", zug.zugname, zug.dauer);
    sleep(zug.dauer);

    // Verlasse Streckenabschnitt
    sem_post(&semaphore);
    printf("Zug %s verlässt Abschnitt!\n", zug.zugname);
}


// Thread function representing a train
void* zugThreadFunc(void* args) {
    Zug* zug = (Zug*)args;
    while(thread_shutdown_flag == 0) {
        zug->dauer = 1 + rand() % 5;
        befahreStrecke(*zug);
        if (thread_shutdown_flag == 0) {
            sleep(1);
        }
    }
    return NULL;
}


// Signal handler for graceful shutdown
void graceful_shutdown_handler(int signum) {
    thread_shutdown_flag = 1;
}


int main() {
    // Erzeuge die sigaction Struktur
    struct sigaction my_signal;

    // Struktur mit 0 initialisieren
    memset(&my_signal, 0, sizeof(struct sigaction));

    // Befülle die sigaction Struktur
    my_signal.sa_handler = graceful_shutdown_handler;
    // Wenn der Signal-Handler gerade aktiv ist, sollten alle anderen Signale,
    // die auch den Prozess beenden, ignoriert werden
    sigemptyset (&my_signal.sa_mask);
    sigaddset(&my_signal.sa_mask, SIGTERM);
    sigaddset(&my_signal.sa_mask, SIGINT);
    sigaddset(&my_signal.sa_mask, SIGQUIT);
    // Wenn ein Systemaufruf von einem Signal unterbrochen wurde, dann soll er neu gestartet werden
    my_signal.sa_flags = 0;

    // Registriere den Signal-Handler für die Signale SIGTERM, SIGINT und SIGQUIT
    if (sigaction(SIGTERM, &my_signal, NULL) != 0) {
        perror("Fehler beim Registrieren des Signal-Handlers");
        exit(1);
    }
    if (sigaction(SIGINT, &my_signal, NULL) != 0) {
        perror("Fehler beim Registrieren des Signal-Handlers");
        exit(1);
    }
    if (sigaction(SIGQUIT, &my_signal, NULL) != 0) {
        perror("Fehler beim Registrieren des Signal-Handlers");
        exit(1);
    }

    // Init Semaphore
    sem_init(&semaphore, 0, STRECKE_COUNT);

    // Create Threads
    pthread_t threads[ZUG_COUNT];
    Zug zuege[ZUG_COUNT];
    for (int i = 0; i < ZUG_COUNT; i++) {
        snprintf(zuege[i].zugname, 10, "Zug #%d", i);
        pthread_create(&threads[i], NULL, zugThreadFunc, &zuege[i]);
    }

    // Join Threads
    for (int i = 0; i < ZUG_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }

    // Und am Ende die Semaphore wieder zerstören
    sem_destroy(&semaphore);

    return 0;
}
