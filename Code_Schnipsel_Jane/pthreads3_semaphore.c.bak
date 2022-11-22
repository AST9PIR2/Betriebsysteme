#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>

/**
 * Variable, welche den Semaphor repraesentiert.
 *
 * Von dieser Variable sollte nie eine Kopie angefertigt werden!
 */
sem_t semaphore;


void* thread_func(void* arg) {
	int* mydata = (int*)arg;
	// Wir addieren zur uebergebenen Zahl 10000-mal die 1
	for (int i = 0; i < 10000; i++) {
		// Sperre den Semaphor
		sem_wait(&semaphore);

		// Begin der kritischen Region
		*mydata += 1;
		// Ende der kritischen Region

		// Entsperre den Semaphor
		sem_post(&semaphore);
	}
	return NULL;
}

int main() {
	// Reserviere Speicher fuer die Thread-Handles
	pthread_t myThreads[10];
	// Reserviere Speicher fuer die benutzerdefinierten Daten
	// Zu dieser Zahl werden 10 Threads 10000-mal die 1 dazuaddieren
	int mydata = 0;

	// Initialisiere den Semaphor
	// 1. Argument: Zeiger auf die Semaphore Variable
	// 2. Argument: 0 ... wird zwischen Threads geteilt, 1 ...  wird zwischen Prozessen geteilt
	// 3. Argument: Anfangswert des Semaphors
	sem_init(&semaphore, 0, 1);

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
	printf("mydata = %d\n", mydata);

	// Gebe den Semaphor wieder frei
	sem_destroy(&semaphore);

	return 0;
}
