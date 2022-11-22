#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

/**
 * Variable, welche die Barrier repraesentiert.
 *
 * Von dieser Variable sollte nie eine Kopie angefertigt werden!
 */
pthread_barrier_t barrier;


// Wird zur Datenuebergabe an die Threads verwendet
typedef struct {
	int theadId;
	int sleepTime;
} thread_data_t;


// Funktion, welche von den Threads ausgefuehrt wird
void* thread_func(void* arg) {
	thread_data_t* data = (thread_data_t*)arg;
	printf("Thread %d wartet %d Sekunden\n", data->theadId, data->sleepTime);
	sleep(data->sleepTime); // Schlafe eine Zeitlang

	printf("Thread %d erreicht Barrier\n", data->theadId);
	// Warte bis alle Threads die Barrier erreichen
	pthread_barrier_wait(&barrier);

	printf("Thread %d hat Barrier ueberwunden\n", data->theadId);
	return NULL;
}


int main() {
	// Reserviere Speicher fuer die Thread-Handles
	pthread_t myThreads[4];
	// Reserviere Speicher fuer die benutzerdefinierten Daten
	thread_data_t threadData[4];

	// Initialisiere die Barrier
	// 1. Argument: Zeiger auf die Barrier Variable
	// 2. Argument: Zeiger auf die Attribut-Variable (NULL bedeutet Default Attribute)
	// 3. Argument: Anzahl der Threads
	pthread_barrier_init(&barrier, NULL, 4);

	// Erzeuge 4 Threads
	for (int i = 0; i < 4; i++) {
		threadData[i].theadId = i;
		threadData[i].sleepTime = 1 + i*2; // Jeder Thread soll eine aufsteigende Anzahl von Sekunden schlafen
		printf("Erzeuge Thread %d\n", i);
		// Erzeuge den Thread
		pthread_create(&myThreads[i], NULL, &thread_func, &threadData[i]);
	}

	// Warte auf die Beendigung aller Threads
	for (int i = 0; i < 10; i++) {
		pthread_join(myThreads[i], NULL);
		printf("Thread %d wurde beendet\n", i);
	}

	// Gebe die Barrier wieder frei
	pthread_barrier_destroy(&barrier);
}