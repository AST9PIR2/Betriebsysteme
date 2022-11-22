// Benötigt für fork(), exec() und waitpid()
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
// Benötigt für perror()
#include <errno.h>
// Benötigt für printf()
#include <stdio.h>

int main() {

    // Hier erzeugen wir einen neuen Prozess
    pid_t cpid = fork();

    if (cpid < 0) {
        perror("Ein Fehler ist aufgetreten");

    } else if (cpid == 0) {
        printf("Dies ist der Kindprozess\n");

        // Im folgenden wollen nur nun "ls -lha /home" aufrufen

        // Zuerst bauen wir uns das Argument-Array zusammen
        char* args[] = {
                "/bin/ls", // Das erste Argument ist immer der Name des Programms!
                "-lha",
                "/home",
                NULL // das Argument-Array muss immer mit NULL abgeschlossen werden!
        };

        // Dann rufen wir exec() auf
        int error = execv("/bin/ls", args);
        if (error < 0) {
            perror("Ein Fehler ist aufgetreten");
        }

    } else if (cpid > 0) {
        printf("Dies ist der Elternprozess\n");
        printf("Die PID des Kindprozesses ist %d\n", cpid);

        // Hier warten wir auf die Beendigung des Kindprozesses
        int exitstatus;
        waitpid(cpid, &exitstatus, 0);

        if (WIFEXITED(exitstatus)) {
            int code = WEXITSTATUS(exitstatus);
            printf("Der Kindprozess wurde normal mit Exit-Code %d beendet\n", code);
        } else if (WIFSIGNALED(exitstatus)) {
            int code = WTERMSIG(exitstatus);
            printf("Der Kindprozess wurde durch das Signal %d beendet\n", code);
        } else {
            printf("Der Kindprozess wurde auf sonstige Art beendet\n");
        }
    }

    return 0;
}