#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


const char* fifoPath = "/tmp/myfifo";

int main() {
	//Wir erzeugen eine FIFO
	if (mkfifo(fifoPath, 0666) != 0) {
		if (errno == EEXIST) {
			printf("Es existiert schon eine Datei namens %s.\n", fifoPath);
			printf("Wurde eine alte FIFO nicht geloescht?\n");
			printf("Ausfuehrung wird fortgesetzt in der Hoffung, dass es sich tatsaechlich um eine FIFO handelt.\n");
		} else {
			perror("Fehler beim Erzeugen der FIFO");
			exit(1);
		}
	}

	// Ã–ffne FIFO nur zum Schreiben
	int fd = open(fifoPath, O_WRONLY);
	if (fd == -1) {
		perror("Fehler beim Ã–ffnen der FIFO");
		exit(1);
	}

	// Schreibe Daten in die FIFO
	write(fd, "Hallo Welt!", 12);

	// Schliesse FIFO wieder
	close(fd);

	// Loesche FIFO
	unlink(fifoPath);
}