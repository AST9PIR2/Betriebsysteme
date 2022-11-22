#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


const char* fifoPath = "/tmp/myfifo";

int main() {

	// Ã–ffne FIFO nur zum Schreiben
	int fd = open(fifoPath, O_RDONLY);
	if (fd == -1) {
		if (errno == ENOENT) {
			printf("Konnte die FIFO %s nicht finden.\n", fifoPath);
			printf("Stellen Sie sicher, dass fifo_writer vor fifo_reader gestartet wird!\n");
			exit(1);
		} else {
			perror("Fehler beim Ã–ffnen der FIFO");
			exit(1);
		}
	}

	// Lese Daten aus der FIFO
	char buffer[128];
	read(fd, buffer, 128);

	// Gebe Daten aus
	printf("FIFO: %s\n", buffer);

	// Schliesse FIFO wieder
	close(fd);
}
