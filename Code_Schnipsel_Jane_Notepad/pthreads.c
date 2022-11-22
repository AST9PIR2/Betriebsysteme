#include <pthread.h>
#include <stdio.h>

void* thread_func(void* arg) {
	int* mydata = (int*)arg;
	printf("Dies ist Thread %d\n", *mydata);
	return NULL;
}

int main() {
	// Reserviere Speicher fuer die Thread-Handles
	pthread_t myThreads[10];
	// Reserviere Speicher fuer die benutzerdefinierten Daten
	int mydata[10];

	// Erzeuge 10 Threads
	for (int i = 0; i < 10; i++) {
		printf("Erzeuge Thread %d\n", i);
		// Initialisieren die benutzerdefinierten Daten fuer den Thread
		mydata[i] = i;
		// Erzeuge den Thread
		pthread_create(&myThreads[i], NULL, &thread_func, &mydata[i]);
	}

	// Warte auf die Beendigung aller Threads
	for (int i = 0; i < 10; i++) {
		pthread_join(myThreads[i], NULL);
		printf("Thread %d wurde beendet\n", i);
	}

	return 0;
}