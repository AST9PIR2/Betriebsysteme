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
    int error_flag = 0;

    while (strcmp(buffer, "exit\n") != 0) {

        // Hier erzeugen wir einen neuen Prozess

        printf(" > ");

        fgets(buffer, 51, stdin);
        fflush(stdin);
        //Overflow protection, ignore any line after 20 lines read
        if (strlen(buffer) > 50) {
            printf("**************************************************************************************\n\n");
            printf("Attention, myShell can only handle inputs with max. 50 characters!\n");
            printf("Your input has more than 50 lines\n\n");
            //printf("The programm will be terminated\n\n");
            printf("**************************************************************************************\n");
        }

        if (strcmp(buffer, "exit\n") == 0) {
            return 0;
        }

        //printf("Buffer output: %s\n", buffer);

        char *command_ptr;
        //char string[20];
        char *command[4];
        int command_counter = 0;
        command_ptr = strtok(buffer, "|");

        while (command_ptr != NULL) {
            //printf("Abschnitt gefunden: %s\n", command_ptr);
            command[command_counter] = command_ptr;
            command_ptr = strtok(NULL, "|\n");
            command_counter++;
            if (command_counter >= 5) {
                perror("To many commands! Max commands this shell can handle is 4!\n");
                error_flag = 1;
                break;
            }
        }

        if (error_flag == 1) {
            continue;
        }

        int prozess_aray[4];
        int pipefd[2];
        ///usr/bin/ls -lha | /usr/bin/sort | /usr/bin/wc -lpipe(pipefd);
        //char buffer_r[5000];

        // /usr/bin/ls -lha|/bin/ps aux|/bin/echo Hallo Welt|/usr/bin/which ls
        close(pipefd[0]);
        close(pipefd[1]);

        for (int i = 0; i < command_counter; i++) {
            printf("Command gefunden: %s\n", command[i]);

            pid_t cpid = fork();
            prozess_aray[i] = cpid;


            if (cpid < 0) {
                perror("Ein Fehler ist aufgetreten");

            } else if (cpid == 0) {
                printf("Dies ist der Kindprozessssssss %i\n",i);
                printf("test channel %i\n", i);
/*
            if(i == 0){
                close(pipefd[0]);
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[1]);
                printf("i == 0");
            }else if(i < (command_counter - 1)){
                close(pipefd[1]);
                dup2(pipefd[0], STDIN_FILENO);
                close(pipefd[0]);
                printf("i < (command_counter -1");
            }else{
                close(pipefd[0]);
                dup2( STDOUT_FILENO,pipefd[1]);
                close(STDOUT_FILENO);

                printf("Working!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
            }*/

                char *ptr;
                char *args[5000];
                int counter = 0;
                //printf("Buffer output: %s\n", buffer);
                ptr = strtok(command[i], " ");


                while (ptr != NULL) {
                    //printf("Abschnitt gefunden: %s\n", ptr);
                    args[counter] = ptr;
                    counter++;
                    //naechsten Abschnitt erstellen
                    ptr = strtok(NULL, " \n");
                }

                // Dann rufen wir exec() auf
                int error = execv(args[0], args);
                if (error < 0) {
                    perror("Ein Fehler ist aufgetreten");
                }
                //command[i] = NULL;
                return 0;


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
    return 0;
}