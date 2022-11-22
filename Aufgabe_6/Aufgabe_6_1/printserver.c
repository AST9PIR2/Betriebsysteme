#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>


#define SHM_NAME	"/shm_testsegment"
#define SHM_SIZE	sizeof(shm_data_t)


// Struct representing the memory layout of the shared-memory segment
typedef struct {
    char buffer[32];
} shm_data_t;

typedef struct {
    char *data[4];
} shm_content_t;

//Probability generator. Stollen from: https://stackoverflow.com/questions/58326619/how-can-i-make-something-happen-x-percentage
//Input in % z.B probability(15) for 15%



int main() {
    shm_content_t message;
    memset(message.data, 0, 32);
    srand((unsigned) time(0));


    printf("My PID is %d\n", getpid());
    printf("shm_size = %lu\n", SHM_SIZE);

    // Open the shared memory object
    // No need to configure the size of an existing shared-memory segment
    int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("Could not create shared memory segment");
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

    // Read and print data from shared-memory segment
    printf("shm-data: \"%s\"\n", shm_data->buffer);

    char *command_ptr;
    int command_counter = 0;

    //Separating input stream in commands by | symbol
    command_ptr = strtok(shm_data->buffer, "|");
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

    printf("Druckauftrag %s wird mit prio: %s beartbeitet...\n", message.data[0], message.data[1]);
    sleep(rand() % 10);
    if(counter >= 10){
        printf("Fehler bei der Bearbeitung des Druckauftrags %s!\n", message.data[0]);
    }else{
        printf("Druckauftrag %s mit prio: %s abgeschlossen\n", message.data[0], message.data[1]);
        printf("Druckauftrag message: %s\n", message.data[2]);
    }



    // Unmap the shared memory segment from memory
    int status = munmap(shm_ptr, SHM_SIZE);
    if (status == -1) {
        perror("Could not unmap the shared memory segment from memory");
        exit(1);
    }

    // Delete the shared memory object
    status = shm_unlink(SHM_NAME);
    if (status == -1) {
        perror("Could not delete the shared memory segment");
        exit(1);
    }

}