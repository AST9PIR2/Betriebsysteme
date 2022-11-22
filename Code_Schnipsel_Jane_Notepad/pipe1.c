#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>


int main() {
	//Wir erzeugen eine Pipe
	int pipefd[2];
	if (pipe(pipefd) != 0) {
		perror("Fehler beim Erzeugen der Pipe");
		exit(1);
	}

	// Wir Erzeugen einen Kindprozess
	pid_t cpid = fork();
	if (cpid == -1) { // Fehler
		perror("Fehler beim Erzeugen des Kindprozesses");
		exit(1);

	} else if (cpid == 0) { // Kindprozess
		// Schliesse das Schreib-Ende der Pipe
		close(pipefd[1]);

		// Lese Daten aus der Pipe
		char buffer[128];
		read(pipefd[0], buffer, 128);

		// Gebe die Daten aus
		printf("Kind: %s\n", buffer);

	} else { // Elternprozess
		// Schliesse das Lese-Ende der Pipe
		close(pipefd[0]);

		// Schreibe Daten in die Pipe
		write(pipefd[1], "Hallo Welt!", 12);

		// Warte auf den Kindprozess
		waitpid(cpid, NULL, 0);
	}

}