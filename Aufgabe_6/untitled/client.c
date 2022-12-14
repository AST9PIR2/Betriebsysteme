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
    char buffer[32];
} shm_content_t;

int main(int argc, char *argv[]) {

    shm_content_t message;
    memset(message.buffer, 0, 32);

    if(argc != 3){
        printf("Error: Please enter 2 arguments, 1. Name, 2. Prio !");
        return 1;
    }

    printf("My PID is %d\n", getpid());
    printf("shm_size = %lu\n", SHM_SIZE);
    srand((unsigned) time(0));

    //Message generator
    char * collection[10] = {"Grüße aus Tirol", "Grüße vom MCI","Grüße aus Südtirol", "Grüße aus Amerika",
                             "Grüße aus Polen","Grüße aus Spanien","Grüße aus Italien","Grüße aus Griechenland",
                             "Grüße aus Afrika","Grüße aus Vietnam"};

    strcat(message.buffer,argv[1]);
    strcat(message.buffer,"|");
    strcat(message.buffer,argv[2]);
    strcat(message.buffer,"|");
    strcat(message.buffer,collection[rand() % 10]);


    // Create the shared memory object
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

    // Save data into buffer in shared-memory segment
    strncpy(shm_data->buffer, message.buffer, 32);
    printf("Übertrage: %s\n", message.buffer );

    // Unmap the shared memory segment from memory
    status = munmap(shm_ptr, SHM_SIZE);
    if (status == -1) {
        perror("Could not unmap the shared memory segment from memory");
        exit(1);
    }

}