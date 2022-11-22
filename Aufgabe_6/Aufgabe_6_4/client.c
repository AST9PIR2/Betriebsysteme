#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <mqueue.h>

typedef struct {
    char buffer[32];
} shm_content_t;

//MessageQueue struct
typedef struct {
    long mq_flags;
    long mq_maxmsg;
    long mq_msgsize;
    long mq_curmsgs;
}mq_attr;


int main(int argc, char *argv[]) {

    shm_content_t message;
    memset(message.buffer, 0, 32);
    mqd_t mqd_t_server, mqd_t_client;
    mq_attr attr;

    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 32;
    attr.mq_curmsgs = 0;

    if(argc != 3){
        perror("Please enter 2 arguments, 1. Name, 2. Prio !");
        return 1;
    }

    srand((unsigned) time(0));

    //Message generator
    char * collection[10] = {"Grüße aus Tirol", "Grüße vom MCI","Grüße aus Südtirol", "Grüße aus Amerika",
                             "Grüße aus Polen","Grüße aus Spanien","Grüße aus Italien","Grüße aus Griechenland",
                             "Grüße aus Afrika","Grüße aus Vietnam"};

    strcat(message.buffer,argv[1]);
    strcat(message.buffer,"|");
    strcat(message.buffer,collection[rand() % 10]);

    mqd_t_client = mq_open ("/shm_testsegment_client",O_RDONLY | O_CREAT, 0666, &attr );
    if (mqd_t_client == -1) {
        perror("Client: Could not create message queue");
        exit(1);
    }

    mqd_t_server = mq_open ("/shm_testsegment_server", O_WRONLY);
    if (mqd_t_server == -1) {
        perror("Client: Could not send message queue to server");
        exit(1);
    }

    char in_buffer[256];

    int prio = atoi(argv[2]);

    printf("Client: Start übertragung Druckauftrag: %s mit prio %i....\n",argv[1], prio);
    if (mq_send(mqd_t_server, message.buffer, 32, prio) == -1) {
        perror("Client: Could not send message to server!");
        exit(1);
    }else{
        printf("Client: Druckauftrag übermittelt\n");
    }

    if (mq_receive (mqd_t_client, in_buffer, 32, NULL) == -1) {
        perror ("Client: Could not open communication to server");
        exit (1);
    }

    if(strcmp(in_buffer,"0")==0){
        printf("Server: Auftrag erledigt!\n" );
    }else{
        perror("Server: Auftrag konnte nicht gedruckt werden. Abbruch" );
    }

    //Closing Queues
    if (mq_close (mqd_t_client) == -1) {
        perror ("Client: mq_close");
        exit (1);
    }

    if (mq_unlink ("/shm_testsegment_client") == -1) {
        perror ("Client: mq_unlink");
        exit (1);
    }
    return 0;
}