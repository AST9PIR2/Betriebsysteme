#include <pthread.h>
#include <stdio.h>

/**
 * Variable, welche den Mutex repraesentiert.
 *
 * Von dieser Variable sollte nie eine Kopie angefertigt werden!
 */
pthread_mutex_t mutex;


void* thread_func(void* arg) {
	int* mydata = (int*)arg;
	// Wir addieren zur uebergebenen Zahl 10000-mal die 1
	for (int i = 0; i < 10000; i++) {
		// Sperre den Mutex
		pthread_mutex_lock(&mutex);

		// Begin der kritischen Region
		*mydata += 1;
		// Ende der kritischen Region

		// Entsperre den Mutex
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}

int main() {
	// Reserviere Speicher fuer die Thread-Handles
	pthread_t myThreads[10];
	// Reserviere Speicher fuer die benutzerdefinierten Daten
	// Zu dieser Zahl werden 10 Threads 10000-mal die 1 dazuaddieren
	int mydata = 0;

	// Initialisiere den Mutex
	// 1. Argument: Zeiger auf die Mutex Variable
	// 2. Argument: Zeiger auf die Attribut-Variable (NULL bedeutet Default Attribute)
	pthread_mutex_init(&mutex, NULL);

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

	// Gebe den Mutex wieder frei
	pthread_mutex_destroy(&mutex);

	return 0;
}
