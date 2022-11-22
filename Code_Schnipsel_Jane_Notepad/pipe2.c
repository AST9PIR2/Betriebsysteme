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
		// Tausche stdin durch die Pipe aus
		dup2(pipefd[0], STDIN_FILENO);
		// Schliesse das Lese-Ende der Pipe
		close(pipefd[0]);

		// Lese Daten von stdin
		char buffer[128];
		fgets(buffer, 128, stdin);

		// Gebe die Daten aus
		printf("Kind: %s", buffer);

	} else { // Elternprozess
		// Schliesse das Lese-Ende der Pipe
		close(pipefd[0]);
		// Tausche stdout durch die Pipe aus
		dup2(pipefd[1], STDOUT_FILENO);
		// Schliesse das Schreib-Ende der Pipe
		close(pipefd[1]);

		// Schreibe nach stdout
		printf("Hallo Welt!\n");
		fflush(stdout);

		// Warte auf den Kindprozess
		waitpid(cpid, NULL, 0);
	}

}
