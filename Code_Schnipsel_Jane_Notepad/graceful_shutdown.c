#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

// Das ist der Signal-Handler, der fuer den graceful shutdown zustaendig ist
 void graceful_shutdown_handler(int signum) {
	 printf("\nWollen Sie wirklich das Programm beenden? [yes|no]: ");
	 char buffer[128];
	 fgets(buffer, 128, stdin);
	 if (strncmp(buffer, "yes", 3) == 0) {
		 printf("Ok, Ich beende mich.\n");
		 exit(0);
	 } else {
		 printf("Ok, Ich beende mich nicht.\n");
	 }
 }


int main() {
	// Erzeuge die sigaction Struktur
	struct sigaction my_signal;

	// Befuelle die sigaction Struktur
	my_signal.sa_handler = graceful_shutdown_handler;
	// Wenn der Signal-Handler gerade aktiv ist, sollten alle anderen Signale,
	// die auch den Prozess beenden, ignoriert werden
	sigemptyset (&my_signal.sa_mask);
	sigaddset(&my_signal.sa_mask, SIGTERM);
	sigaddset(&my_signal.sa_mask, SIGINT);
	sigaddset(&my_signal.sa_mask, SIGQUIT);
	// Wenn ein Systemaufruf von einem Signal unterbrochen wurde, dann soll er neu gestartet werden
	my_signal.sa_flags = SA_RESTART;

	// Registriere den Signal-Handler fuer die Signale SIGTERM, SIGINT und SIGQUIT
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

	// Wir geben die PID aus, damit der Benutzer weiss, welche PID er fuer den "kill" Befehl verwenden muss
	printf("Meine PID ist %d\n", getpid());

	// Wir lassen den Prozess die ganze Zeit schlafen, damit er sich nicht beendet.
	// Uns interessiert nur das Signal-Handling.
	while (1) {
		sleep(1);
	}

	return 0;
}