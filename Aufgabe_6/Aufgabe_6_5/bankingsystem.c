#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>
#include <unistd.h>


#define SHM_NAME	"/shm_counter"
#define SHM_SIZE	sizeof(shm_data_t)


// Struct containing the counter and a mutex
// Represents the memory layout of the shared-memory segment
typedef struct {
    unsigned counter;
    pthread_mutex_t mutex;

    struct kontodata{
        int saldo;
        int kredit;
    } data[10];

} shm_data_t;

void clientAccessAccount(shm_data_t* shm_data, int kontoId, int action){
    shm_data->data[kontoId].saldo += action;
}

int alarm_flag = 0;

// Das ist der Signal-Handler
void alarm_handler(int signum) {
    printf("Der Alarm wurde ausgelöst\n");
    alarm_flag = 1;
}

int shutdownflag = 0;

void graceful_shutdown_handler(int signum) {

    //STRG + C gives me 20 seconds to act, otherwise go on with routine
    alarm(20);
    printf("\nExit programm? (y/n): ");
    char buffer[128];
    fgets(buffer, 128, stdin);
    if (strncmp(buffer, "y", 1) == 0) {
        printf("Ok, Ich beende mich!\n");
        shutdownflag = 1;
    } else {
        printf("Ok, Ich beende mich nicht!.\n");
        printf("...\n");
    }
}

int serverReadAccounts(shm_data_t *shm_data){

    for(int i = 0; i < 10; i++){
        printf("Konto: %i hat folgenden Kontostand %i und %i Schulden\n", i,shm_data->data[i].saldo,shm_data->data[i].kredit);
        if (shm_data->data[i].saldo < 0){
            shm_data->data[i].kredit += 1000;
            clientAccessAccount(shm_data,i, 1000);
            //printf("Kredit wird von 1000 wird gewährt");
        }
    }
    return 0;
}


int main() {
    printf("My PID is %d\n", getpid());
    printf("shm_size = %lu\n", SHM_SIZE);

    // Erzeuge die sigaction Struktur
    struct sigaction my_signal;

    //Initialize my_signal with 0
    memset(&my_signal, 0,sizeof(struct sigaction));

    // Befülle die sigaction Struktur
    my_signal.sa_handler = alarm_handler;
    if(sigemptyset (&my_signal.sa_mask)){
        perror("Error on initialisation of sigemptyset");
        exit(1);
    }
    my_signal.sa_flags = 0;

    // Registriere den Signal-Handler fÃ¼r das Signal SIGALRM
    if (sigaction(SIGALRM, &my_signal, NULL) != 0) {
        perror("Error on initialisation of sigaction");
        exit(1);
    }

    // Befülle die sigaction Struktur
    my_signal.sa_handler = graceful_shutdown_handler;

    // Wenn der Signal-Handler gerade aktiv ist, sollten alle anderen Signale,
    if(sigemptyset (&my_signal.sa_mask)){
        perror("Error on initialisation of sigemptyset");
        exit(1);
    }
    if(sigaddset(&my_signal.sa_mask, SIGTERM)){
        perror("Error on adding sigaddset: SIGTERM");
        exit(1);
    }
    if(sigaddset(&my_signal.sa_mask, SIGINT)){
        perror("Error on adding sigaddset: SIGINT");
        exit(1);
    }
    if(sigaddset(&my_signal.sa_mask, SIGQUIT)){
        perror("Error on adding sigaddset: SIGQUIT");
        exit(1);
    }

    // Wenn ein Systemaufruf von einem Signal unterbrochen wurde, dann soll er neu gestartet werden
    my_signal.sa_flags = SA_RESTART;

    // Registriere den Signal-Handler für die Signale SIGTERM, SIGINT und SIGQUIT
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

    // Create the shared memory object
    // We create the shared memory segment once in the parent process
    // All child processes then inherit the shared memory segment
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("Could not create shared memory segment");
        exit(1);
    }

    // Configure the size of the shared memory segment
    int status = ftruncate(shm_fd, SHM_SIZE);
    if (status == -1) {
        perror("Could not configure the size of the shared memory segment");
        exit(1);
    }

    // Map the shared memory segment into memory
    void* shm_ptr = mmap(0, SHM_SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_ptr == MAP_FAILED) {
        perror("Could not map the shared memory segment into memory");
        exit(1);
    }

    // Cast shared memory pointer to whatever data type we need it to be
    // The programmer needs to manually manage the shared-memory segment
    // Best practice is to use a struct that represents the memory layout
    shm_data_t* shm_data = (shm_data_t*) shm_ptr;

    // Initialize the counter
    shm_data->counter = 0;

    //Initialize Konten und schütze den process
    pthread_mutex_lock(&shm_data->mutex);
    for(int i = 0; i < 10 ; i++){
        shm_data->data[i].saldo = 0;
        shm_data->data[i].kredit = 0;
    }
    serverReadAccounts(shm_ptr);
    printf("counter = %u\n", shm_data->counter);
    pthread_mutex_unlock(&shm_data->mutex);

    // Initialize mutex attributes
    // The process-shared attribute needs to be set when the mutex is used among several processes
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);

    // Initialize mutex
    // The mutex needs to reside in the shared-memory segment so that all processes can access it
    pthread_mutex_init(&shm_data->mutex, &attr);

    while(shutdownflag == 0){
        // Create 10 child processes which add one to the counter 10000-times each
        pid_t children[4];
        for (int i = 0; i < 4; i++) {
            children[i] = fork();
            if (children[i] == -1) {
                perror("Error while forking");
                exit(1);

                // child-process
            } else if (children[i] == 0) {

                //Seed for rand()
                srand((unsigned) time(0));

                pthread_mutex_lock(&shm_data->mutex);
                // critical region start
                shm_data->counter += 1;
                clientAccessAccount(shm_ptr,rand()%10, (rand()%200)-100);
                // critical region end
                pthread_mutex_unlock(&shm_data->mutex);
                exit(0);
            }
        }

        // Wait for children
        for (int i = 0; i < 4; i++) {
            waitpid(children[i], NULL, 0);
        }
        alarm(3);
        //sleep(10);
        pause();

        if(alarm_flag == 1){
            pthread_mutex_lock(&shm_data->mutex);
            serverReadAccounts(shm_ptr);
            pthread_mutex_unlock(&shm_data->mutex);
            // Print counter value
            printf("counter = %u\n", shm_data->counter);
            alarm_flag = 0;
        }
    }

    // Delete mutex

    pthread_mutex_destroy(&shm_data->mutex);
    printf("Clearing data: thread_mutex_destroy...\n");

    // Unmap the shared memory segment from memory
    status = munmap(shm_ptr, SHM_SIZE);
    if (status == -1) {
        perror("Could not unmap the shared memory segment from memory");
        exit(1);
    }
    printf("Clearing data: munmap...\n");

    // Delete the shared memory object
    status = shm_unlink(SHM_NAME);
    if (status == -1) {
        perror("Could not delete the shared memory segment");
        exit(1);
    }
    printf("Clearing data: shm_unlink...\n");

    return 0;
}
