// Benoetigt fuer fork() und waitpid()
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
// Benoetigt fuer perror()
#include <errno.h>
// Benoetigt fuer printf()
#include <stdio.h>

int main() {

	// Hier erzeugen wir einen neuen Prozess
	pid_t cpid = fork();

	if (cpid < 0) {
		perror("Ein Fehler ist aufgetreten");

	} else if (cpid == 0) {
		printf("Dies ist der Kindprozess\n");

	} else if (cpid > 0) {
		printf("Dies ist der Elternprozess\n");
		printf("Die PID des Kindprozesses ist %d\n", cpid);
		// Hier warten wir auf die Beendigung des Kindprozesses
		waitpid(cpid, 0, 0);
		printf("Der Kindprozess hat sich nun beendet\n");
	}

	return 0;
}
