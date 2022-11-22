#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

// Das ist der Signal-Handler
 void alarm_handler(int signum) {
	 printf("Der Alarm wurde ausgeloest\n");
 }


int main() {
	// Erzeuge die sigaction Struktur
	struct sigaction my_signal;

	// BefÃ¼lle die sigaction Struktur
	my_signal.sa_handler = alarm_handler;
	sigemptyset (&my_signal.sa_mask);
	my_signal.sa_flags = 0;

	// Registriere den Signal-Handler fÃ¼r das Signal SIGALRM
	if (sigaction(SIGALRM, &my_signal, NULL) != 0) {
		perror("Fehler beim Registrieren des Signal-Handlers");
		exit(1);
	}

	// Wir geben die PID aus, damit der Benutzer weiss, welche PID er fÃ¼r den "kill" Befehl verwenden muss
	printf("Meine PID ist %d\n", getpid());

	while (1) {
		// Stelle Alarm ein
		printf("Stelle den Alarm auf 3 Sekunden ein\n");
		alarm(3);
		// Schlafe eine lange Zeit lang
		// Da wir das SA_RESTART Flag nicht gesetzt haben, wird der Schlaf durch das Signal unterbrochen.
		// Wenn sleep durch ein Signal unterbrochen wurde, gibt es die Anzahl der restlichen Sekunden zurÃ¼ck.
		if (sleep(999) > 0) {
			printf("Schlaf wurde unterbrochen\n");
		}
	}

	return 0;
}
