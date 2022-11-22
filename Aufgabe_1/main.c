// Benötigt für fork(), exec() und waitpid()
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
// Benötigt für perror()
#include <errno.h>
// Benötigt für printf()
#include <stdio.h>
#include <string.h>

int main() {
    printf("*** myShell initialized ***\n");
    char buffer[51] = "";

    while (1) {

        // Hier erzeugen wir einen neuen Prozess

        printf(" > ");

            fgets( buffer, 51 , stdin);
            fflush(stdin);
            //Overflow protection, ignore any line after 20 lines read
            if (strlen(buffer) > 50) {
                printf("**************************************************************************************\n\n");
                printf("Attention, myShell can only handle inputs with max. 50 characters!\n");
                printf("Your input has more than 50 lines\n\n");
                //printf("The programm will be terminated\n\n");
                printf("**************************************************************************************\n");
            }

        if (strcmp(buffer, "exit\n") == 0){
            return 0;
        }

        pid_t cpid = fork();

        if (cpid < 0) {
            perror("Ein Fehler ist aufgetreten");

        } else if (cpid == 0) {
            printf("Dies ist der Kindprozess\n");

            char * ptr;
            char* args[50];
            int counter = 0;
            ptr = strtok(buffer, " ");

            while(ptr != NULL) {
                //printf("Abschnitt gefunden: %s\n", ptr);
                args[counter] = ptr;
                counter ++;
                //naechsten Abschnitt erstellen
                ptr = strtok(NULL, " \n");
            }

            // Dann rufen wir exec() auf
            int error = execv(args[0], args);
            if (error < 0) {
                perror("Ein Fehler ist aufgetreten");
            }


        } else if (cpid > 0) {
            //printf("myShell initialized\n");
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
    }
}