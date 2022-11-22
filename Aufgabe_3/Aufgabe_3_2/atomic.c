#include <pthread.h>
#include <stdio.h>
#include <threads.h>
#include <stdatomic.h>

atomic_int acnt;
int cnt;

void* thread_func(void* arg) {
    int* mydata = (int*)arg;
    // Wir addieren zur Ã¼bergebenen Zahl 10000-mal die 1
    for (int i = 0; i < 10000; i++) {
        *mydata += 1;
    }

    for(int n = 0; n < 10000; ++n) {
        atomic_fetch_add_explicit(& acnt, 1, memory_order_relaxed); // atomic
        ++cnt; // undefined behavior, in practice some updates missed
    }
    return NULL;
}

int main() {
    // Reserviere Speicher fÃ¼r die Thread-Handles
    pthread_t myThreads[10];
    // Reserviere Speicher fÃ¼r die benutzerdefinierten Daten
    // Zu dieser Zahl werden 10 Threads 10000-mal die 1 dazuaddieren
    int mydata = 0;

    // Erzeuge 10 Threads
    for (int i = 0; i < 10; i++) {
        printf("Erzeuge Thread %d\n", i);
        // Erzeuge den Thread
        pthread_create(&myThreads[i], NULL, &thread_func, &mydata);
    }

    // Warte auf die Beendigung aller Threads
    for (int i = 0; i < 10; i++) {
        pthread_join(myThreads[i], NULL);
        printf("Thread %d wurde beendet\n", i);
    }

    // Gebe das Endergebnis aus
    // Wenn 10 Threads 10000-mal die 1 dazuaddieren, mÃ¼sste doch 100000 rauskommen
    // Tut es das?
    // Wenn es das tut, ist das immer der Fall?
    printf("mydata atomic counter = %d\n", acnt);
    printf("mydata thread counter = %d\n", mydata);

    return 0;
}