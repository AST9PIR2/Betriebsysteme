#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

int threadRunFlag = 1;

void* thread_func(void* arg) {
	while (threadRunFlag) {
		printf("Thread: Hallo Welt\n");
		sleep(2);
	}
	printf("Thread: Ich beende mich jetzt\n");
	return NULL;
}

int main() {
	// Reserviere Speicher fuer das Thread-Handles
	pthread_t myThread;

	// Erzeuge den Thread
	printf("main: Thread wird erzeugt\n");
	pthread_create(&myThread, NULL, &thread_func, NULL);

	// Warte 10 Sekunden
	sleep(5);

	// Teile dem Thread mit, dass er sich beenden soll
	printf("main: Thread soll sich beenden\n");
	threadRunFlag = 0;

	// Warte auf die Beendigung des Threads
	pthread_join(myThread, NULL);
	printf("main: Thread ist beendet\n");

	return 0;
}

