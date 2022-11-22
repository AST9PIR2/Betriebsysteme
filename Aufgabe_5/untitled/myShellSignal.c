// Benötigt für fork(), exec() und waitpid()
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
// Benötigt für perror()
#include <errno.h>
// Benötigt für printf()
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>



void graceful_shutdown_handler(int signum) {
    printf("\nExit myShell? (y/n): ");
    char buffer[128];
    fgets(buffer, 128, stdin);
    if (strncmp(buffer, "y", 1) == 0) {
        printf("Ok, Ich beende mich.\n");
        exit(0);
    } else {
        printf("Ok, Ich beende mich nicht.\n");
        fflush(stdin);
        fflush(stdout);
        printf(" > ");
        fflush(stdout);
    }
}



int main() {
    printf("*** myShell initialized ***\n");
    char buffer[255] = "";
    int cpid_array[3];

    // Erzeuge die sigaction Struktur
    struct sigaction my_signal;

    // BefÃ¼lle die sigaction Struktur
    my_signal.sa_handler = graceful_shutdown_handler;
    // Wenn der Signal-Handler gerade aktiv ist, sollten alle anderen Signale,
    // die auch den Prozess beenden, ignoriert werden
    sigemptyset (&my_signal.sa_mask);
    sigaddset(&my_signal.sa_mask, SIGTERM);
    sigaddset(&my_signal.sa_mask, SIGINT);
    sigaddset(&my_signal.sa_mask, SIGQUIT);
    // Wenn ein Systemaufruf von einem Signal unterbrochen wurde, dann soll er neu gestartet werden
    my_signal.sa_flags = SA_RESTART;

    // Registriere den Signal-Handler fÃ¼r die Signale SIGTERM, SIGINT und SIGQUIT
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

    // Wir geben die PID aus, damit der Benutzer weiÃŸ, welche PID er fÃ¼r den "kill" Befehl verwenden muss
    printf("Meine PID ist %d\n", getpid());

    //Run shell until user sends exit command
    while (1) {

        int error_flag = 0;

        //Init Pipe 1 for communication between command 1 and command 2
        int pipefd_1[2];
        if (pipe(pipefd_1) != 0) {
            perror("Fehler beim Erzeugen der Pipe1");
            return 1;
        }

        //Init Pipe 2 for communication between command 2 and command 3
        int pipefd_2[2];
        if (pipe(pipefd_2) != 0) {
            perror("Fehler beim Erzeugen der Pipe2");
            return 1;
        }

        //Reset cpid array to 0
        for (int i = 0; i < 3; i++){
            cpid_array[i] = 0;
        }

        printf(" > ");

        fgets(buffer, 256, stdin);
        fflush(stdin);
        //Overflow protection
        if (strlen(buffer) > 255) {
            printf("**************************************************************************************\n\n");
            printf("Attention, myShell can only handle inputs with max. 255 characters!\n");
            printf("Your input has more than 255 lines\n\n");
            printf("**************************************************************************************\n");
        }

        //Exit handler: Exit programm immediately without any further output
        if (strcmp(buffer, "exit\n") == 0) {
                graceful_shutdown_handler(2);
        }


        char *command_ptr;
        char *command[4];
        int command_counter = 0;

        //Separating input stream in commands by | symbol
        command_ptr = strtok(buffer, "|");
        while (command_ptr != NULL) {
            //Overflow protection
            if (command_counter > 2) {
                perror("ERROR: To many commands! This shell can only handle up to 3 commands! Status error handler ");
                error_flag = 1;
                break;
            }
            command[command_counter] = command_ptr;
            command_ptr = strtok(NULL, "|\n");
            command_counter++;
        }

        //Overflow handler: clear stdin and start from beginning

        if (error_flag == 1) {
            fflush(stdin);
            continue;
        }

        char *ptr;
        char *args[50];

        //First fork
        pid_t cpid0 = fork();
        cpid_array[0] = cpid0;

        if (cpid0 < 0) {
            perror("Ein Fehler ist aufgetreten");

        } else if (cpid0 == 0) {
            int counter = 0;

            //Separating commands in arguments by " " symbol
            ptr = strtok(command[0], " ");
            while (ptr != NULL) {
                args[counter] = ptr;
                counter++;
                ptr = strtok(NULL, " \n");
            }

            //Redirect stdout to pipe 1
            if (command_counter > 1) {
                // Schliesse das Lese-Ende der Pipe
                close(pipefd_1[0]);
                // Tausche stdout durch die Pipe aus
                dup2(pipefd_1[1], STDOUT_FILENO);
                // Schliesse das Schreib-Ende der Pipe
                close(pipefd_1[1]);
            }


            // Dann rufen wir exec() auf
            int error = execv(args[0], args);
            if (error < 0) {
                perror("Ein Fehler ist aufgetreten");
            }

        }else{
            //Run this section if more than one command to execute
            if (command_counter > 1) {
                pid_t cpid1 = fork();
                cpid_array[1] = cpid1;

                if (cpid1 == -1) { // Fehler
                    perror("Fehler beim Erzeugen des zweiten Kindprozesses");
                    return 1;
                } else if (cpid1 == 0) {

                    //Redirect pipe 1 to stdin
                    //Schliesse das Schreib-Ende der Pipe
                    close(pipefd_1[1]);
                    //Tausche stdin durch die Pipe aus
                    dup2(pipefd_1[0], STDIN_FILENO);
                    //Schliesse das Lese-Ende der Pipe
                    close(pipefd_1[0]);

                    //Redirect stdout to pipe 2 if more than two commands to execute
                    if (command_counter > 2) {
                        //Schliesse das Lese-Ende der Pipe
                        close(pipefd_2[0]);
                        //Tausche stdout durch die Pipe aus
                        dup2(pipefd_2[1], STDOUT_FILENO);
                        //Schliesse das Schreib-Ende der Pipe
                        close(pipefd_2[1]);
                    }

                    int counter = 0;

                    //Separating commands in arguments by " " symbol
                    ptr = strtok(command[1], " ");
                    while (ptr != NULL) {
                        args[counter] = ptr;
                        counter++;
                        ptr = strtok(NULL, " \n");
                    }

                    int error = execv(args[0], args);
                    if (error < 0) {
                        perror("Ein Fehler ist aufgetreten");
                    }
                }else {
                    //Run this section if more than two commands to execute
                    if (command_counter > 2) {
                        pid_t cpid2 = fork();
                        cpid_array[2] = cpid2;

                        if (cpid2 == -1) { // Fehler
                            perror("Fehler beim Erzeugen des zweiten Kindprozesses");
                            return 1;
                        } else if (cpid2 == 0) {

                            //Close unused pipes
                            close(pipefd_1[1]);
                            close(pipefd_1[0]);

                            //Redirect pipe 2 to stdin
                            close(pipefd_2[1]);
                            // Tausche stdin durch die Pipe aus
                            dup2(pipefd_2[0], STDIN_FILENO);
                            // SchlieÃŸe das Lese-Ende der Pipe
                            close(pipefd_2[0]);

                            int counter = 0;

                            ///usr/bin/ls -lha | /usr/bin/sort | /usr/bin/grep Aufgabe

                            //Separating commands in arguments by " " symbol
                            ptr = strtok(command[2], " ");
                            while (ptr != NULL) {
                                args[counter] = ptr;
                                counter++;
                                ptr = strtok(NULL, " \n");
                            }

                            int error = execv(args[0], args);
                            if (error < 0) {
                                perror("Ein Fehler ist aufgetreten");
                            }
                        }
                    }
                }
            }

            // /usr/bin/ls -lha | /usr/bin/grep Aufgabe
            // /usr/bin/ls -lha | /usr/bin/grep Aufgabe | /usr/bin/sort
            close(pipefd_1[0]);
            close(pipefd_1[1]);
            close(pipefd_2[0]);
            close(pipefd_2[1]);


            // Hier warten wir auf die Beendigung des Kindprozesses
            int exitstatus0;
            waitpid(cpid0, &exitstatus0, 0);

            if (WIFEXITED(exitstatus0)) {
                int code = WEXITSTATUS(exitstatus0);
                printf("Der Kindprozess1 wurde normal mit Exit-Code %d beendet\n", code);
            } else if (WIFSIGNALED(exitstatus0)) {
                int code = WTERMSIG(exitstatus0);
                printf("Der Kindprozess1 wurde durch das Signal %d beendet\n", code);
            } else {
                printf("Der Kindprozess1 wurde auf sonstige Art beendet\n");
            }

            if(command_counter > 1){
                int exitstatus1;
                waitpid(cpid_array[1], &exitstatus1, 0);

                if (WIFEXITED(exitstatus1)) {
                    int code = WEXITSTATUS(exitstatus1);
                    printf("Der Kindprozess2 wurde normal mit Exit-Code %d beendet\n", code);
                } else if (WIFSIGNALED(exitstatus1)) {
                    int code = WTERMSIG(exitstatus1);
                    printf("Der Kindprozess2 wurde durch das Signal %d beendet\n", code);
                } else {
                    printf("Der Kindprozess2 wurde auf sonstige Art beendet\n");
                }
            }

            if(command_counter > 2){
                int exitstatus2;
                waitpid(cpid_array[2], &exitstatus2, 0);

                if (WIFEXITED(exitstatus2)) {
                    int code = WEXITSTATUS(exitstatus2);
                    printf("Der Kindprozess3 wurde normal mit Exit-Code %d beendet\n", code);
                } else if (WIFSIGNALED(exitstatus2)) {
                    int code = WTERMSIG(exitstatus2);
                    printf("Der Kindprozess3 wurde durch das Signal %d beendet\n", code);
                } else {
                    printf("Der Kindprozess3 wurde auf sonstige Art beendet\n");
                }
            }
        }
    }
}
