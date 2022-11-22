#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <mqueue.h>


// Struct representing the memory layout of the shared-memory segment

typedef struct {
    char *data[4];
} shm_content_t;

typedef struct {
    long mq_flags;
    long mq_maxmsg;
    long mq_msgsize;
    long mq_curmsgs;
}mq_attr;


int main() {
    shm_content_t message;
    //memset(message.data, 0, 256);
    srand((unsigned) time(0));

    mqd_t mqd_t_server, mqd_t_client;

    mq_attr attr;

    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 32;
    attr.mq_curmsgs = 0;

    mqd_t_server = mq_open("/shm_testsegment_server",O_RDONLY | O_CREAT, 0666, &attr);
    if (mqd_t_server == -1) {
        perror("Could not send message queue to server");
        exit(1);
    }

    char in_buffer[256];
    char out_buffer[256];

    printf("Server initialized!\n");

    int schleifencounter = 0;

    while (schleifencounter < 20) {
        //Von Queue nach FIFO mit prio betrachtung einlesen
        ssize_t mqd_t_reciever = mq_receive (mqd_t_server, in_buffer, 32, NULL);
        if (mqd_t_reciever == -1) {
            perror ("Server: mq_receive");
            //exit (1);
            break;
        }

        printf ("Server: message received.\n");

        if ((mqd_t_client = mq_open ("/shm_testsegment_client", O_WRONLY)) == 1) {
            perror ("Server: Not able to open client queue");
            continue;
        }

        char *command_ptr;
        int command_counter = 0;

        //Separating input stream in commands by | symbol
        command_ptr = strtok(in_buffer, "|");
        while (command_ptr != NULL) {
            message.data[command_counter] = command_ptr;
            command_ptr = strtok(NULL, "|\n");
            command_counter++;
        }

        //int flag = (100 - rand()%2 * 100 / 10);
        int counter = 0;

        for(int i =0;i < 100;i++){
            if(rand()%10 ==5){
                counter++;
            }
        }
        int error_flag = 0;

        printf("Server: Druckauftrag %s wird mit Nachricht: %s wird beartbeitet...\n", message.data[0],message.data[1]);
        sleep(rand() % 10);
        if(counter >= 10){
            printf("Server: Fehler bei der Bearbeitung des Druckauftrags %s!\n", message.data[0]);
            error_flag = 1;
        }else{
            printf("Server: Druckauftrag %s abgeschlossen\n", message.data[0]);
        }

        //Sending back feedback to client
        sprintf (out_buffer, "%i",error_flag);

        if (mq_send (mqd_t_client, out_buffer, strlen (out_buffer) + 1, 0) == -1) {
            perror ("Server: Not able to send message to client");
            continue;
        }

        printf ("Server: response sent to client.\n");
        schleifencounter++;
    }

    //Closing Queues
    if (mq_close (mqd_t_server) == -1) {
        perror ("Client: mq_close");
        exit (1);
    }

    if (mq_unlink ("/shm_testsegment_server") == -1) {
        perror ("Client: mq_unlink");
        exit (1);
    }

    //printf("After 20 Messages a manual restart oft the server is required. Shutting down server....");
    return 0;
}