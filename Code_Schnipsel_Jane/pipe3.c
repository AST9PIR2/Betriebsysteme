#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>


// Ruft "ls -lha | sort" auf


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
		// Schliesse das Lese-Ende der Pipe
		close(pipefd[0]);
		// Tausche stdout durch die Pipe aus
		dup2(pipefd[1], STDOUT_FILENO);
		// Schliesse das Schreib-Ende der Pipe
		close(pipefd[1]);

		// Rufe "ls -lha" auf
		execlp("ls", "ls", "-lha", NULL);

	} else { // Elternprozess

		// Wir Erzeugen einen zweiten Kindprozess
		pid_t cpid2 = fork();
		if (cpid2 == -1) { // Fehler
			perror("Fehler beim Erzeugen des zweiten Kindprozesses");
			exit(1);

		} else if (cpid2 == 0) { // Kindprozess
			// Schliesse das Schreib-Ende der Pipe
			close(pipefd[1]);
			// Tausche stdin durch die Pipe aus
			dup2(pipefd[0], STDIN_FILENO);
			// Schliesse das Lese-Ende der Pipe
			close(pipefd[0]);

			// Rufe "sort" auf
			execlp("sort", "sort", NULL);

		} else { // Elternprozess

			// Schliesse alle Pipe-Enden
			// Wenn die Enden nicht geschlossen werden, dann wird sich das Programm nie beenden!
			close(pipefd[0]);
			close(pipefd[1]);

			// Warte auf den ersten Kindprozess
			waitpid(cpid, NULL, 0);

			// Warte auf den zweiten Kindprozess
			waitpid(cpid2, NULL, 0);
		}
	}

}