
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>



/**
 * Type of the elements of the circular buffer
 * If you want to use a different element type, then change this typedef
 */
typedef int cb_element_t;

/**
 * Struct representing a circular buffer
 */
typedef struct {
    cb_element_t* buffer;
    int first;  // Index des ersten Elements
    int count;  // Anzahl der Elemente im Buffer
    int size;   // GrÃ¶ÃŸe des Buffers
} circularbuffer_t;

/**
 * Initializes the circular buffer.
 *
 * \param buffer pointer to struct representing a circular buffer
 * \param size size of the circular buffer
 * \return a negative value in case of an error otherwise 0
 */
int cb_init(circularbuffer_t* buffer, int size) {
    if (size <= 0) {
        return -1;
    }
    buffer->buffer = (cb_element_t*)malloc(sizeof(cb_element_t)*size);
    if (buffer->buffer == NULL) {
        return -2;
    }
    buffer->first = 0;
    buffer->count = 0;
    buffer->size = size;
    return 0;
}

/**
 * Destroys the circular buffer and frees all ressources.
 *
 * \param buffer pointer to struct representing the circular buffer
 * \return a negative value in case of an error otherwise 0
 */
int cb_destroy(circularbuffer_t* buffer) {
    if (buffer->buffer) {
        free(buffer->buffer);
        buffer->buffer = NULL;
        return 0;
    }
    return -1;
}

/**
 * Adds an Element to the circular buffer
 *
 * \param buffer pointer to struct representing a circular buffer
 * \param elem value to be added to the circular buffer
 * \return -1 when the buffer is full otherwise 0
 */
int cb_add(circularbuffer_t* buffer, cb_element_t elem) {
    if (buffer->count >= buffer->size) {
        return -1;
    }
    int elem_index = (buffer->first + buffer->count) % buffer->size;
    buffer->buffer[elem_index] = elem;
    buffer->count++;
    return 0;
}

/**
 * Removes an Element from the circular buffer. The removed element is returned via call-by-reference.
 *
 * \param buffer pointer to struct representing a circular buffer
 * \param elem pointer to where the removed value should be stored (call-by-reference)
 * \return -1 when the buffer is empty otherwise 0
 */
int cb_remove(circularbuffer_t* buffer, cb_element_t* elem) {
    if (buffer->count <= 0) {
        return -1;
    }
    if (elem != NULL) {
        *elem = buffer->buffer[buffer->first];
    }
    buffer->first = (buffer->first + 1) % buffer->size;
    buffer->count--;
    return 0;
}

/**
 * Returns the number of elements in the circular buffer
 *
 * \param buffer pointer to struct representing a circular buffer
 * \return number of elements in the circular buffer
 */
int cb_count(circularbuffer_t* buffer) {
    return buffer->count;
}

/**
 * Returns whether the circular buffer is full
 *
 * \param buffer pointer to struct representing a circular buffer
 * \return boolean value indicating that the buffer is full
 */
int cb_isFull(circularbuffer_t* buffer) {
    return buffer->count == buffer->size;
}

/**
 * Returns whether the circular buffer is empty
 *
 * \param buffer pointer to struct representing a circular buffer
 * \return boolean value indicating whether the buffer is empty
 */
int cb_isEmpty(circularbuffer_t* buffer) {
    return buffer->count == 0;
}

pthread_mutex_t mutex;
pthread_cond_t producerCondition;
pthread_cond_t consumerCondition;
circularbuffer_t myCB;


int counter = 1;


void* thread_func1(void* arg) {
    int* run = (int*)arg;
    while(counter == 1){
        pthread_mutex_lock(&mutex);
        if(cb_isFull(&myCB)) {
            printf("Temperatursensor <%i> wurde schlafengelegt\n",*run);
            //pthread_cond_signal(&consumerCondition);
            pthread_cond_wait(&producerCondition, &mutex);
            //pthread_cond_signal(&consumerCondition);
        }else{
            cb_add(&myCB, (rand() % 55));
            //pthread_cond_signal(&consumerCondition);
        }
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}
float temp = 0;

void* thread_func2(void* arg) {
    int* mydata = (int*)arg;

    // Wir addieren zur Ã¼bergebenen Zahl 10000-mal die 1
    for (int i = 0; i < 1000; i++) {

            // Sperre den Mutex
            pthread_mutex_lock(&mutex);

            // Begin der kritischen Region

            cb_element_t element = myCB.buffer[counter];

            cb_remove(&myCB, &element);
            *mydata += 1;
            temp += element;

            if(pthread_cond_signal(&producerCondition)==0){
                //printf("Temperatursensor <x> wurde aufgeweckt\n");
            }

            pthread_mutex_unlock(&mutex);
        }

    counter = 0;

    printf("Temperatur Durchschnitt: %0.2f\n", (temp/1000));

    return NULL;
}

int main() {

    // Creates a circular buffer with 7 elements
    //circularbuffer_t myCB;
    cb_init(&myCB, 7);

    // Checks whether the buffer is empty
    printf("Circular Buffer is Empty: %d\n", cb_isEmpty(&myCB));
    printf("Circular Buffer is Full: %d\n", cb_isFull(&myCB));

    // Reserviere Speicher fÃ¼r die Thread-Handles
    pthread_t myThreads[3];
    // Reserviere Speicher fÃ¼r die benutzerdefinierten Daten
    // Zu dieser Zahl werden 10 Threads 10000-mal die 1 dazuaddieren
    //int mydata1 = 0;
    int mydata2 = 0;

    // Initialisiere den Mutex
    // 1. Argument: Zeiger auf die Mutex Variable
    // 2. Argument: Zeiger auf die Attribut-Variable (NULL bedeutet Default Attribute)
    pthread_mutex_init(&mutex, NULL);

    // Initialisiere die Condition Variable
    // 1. Argument: Zeiger auf die Condition Variable
    // 2. Argument: Zeiger auf die Attribut-Variable (NULL bedeutet Default Attribute)
    pthread_cond_init(&producerCondition, NULL);

    int id[2];

    // Erzeuge 2 Threads
    for (int i = 0; i < 2; i++) {
        id[i] = i;
        printf("Erzeuge Thread für Temperaturesensor ID: %d\n", id[i]);
        pthread_create(&myThreads[i], NULL, &thread_func1, &id[i]);
    }

    printf("Erzeuge Thread für Logging ID: %d\n", 2);
    pthread_create(&myThreads[2], NULL, &thread_func2, &mydata2);
/*
    if(pthread_join(myThreads[2], NULL)){
        return 0;
    }*/

    // Warte auf die Beendigung aller Threads
    for (int i = 0; i < 3; i++) {
        pthread_join(myThreads[i], NULL);
        printf("Thread %d wurde beendet\n", i);
    }
/*
    // Gebe das Endergebnis aus
    printf("mydata1 = %d\n", mydata1);
    printf("mydata2 = %d\n", mydata2);
    */
    // Gebe den Mutex wieder frei
    pthread_mutex_destroy(&mutex);

    // Checks whether the buffer is full
    printf("Circular Buffer is Empty: %d\n", cb_isEmpty(&myCB));
    printf("Circular Buffer is Full: %d\n", cb_isFull(&myCB));

    printf("cb_add() now returns %d\n", cb_add(&myCB, 8));

    // Removes Elements from the buffer
    printf("Elements in the circular buffer: ");
    cb_element_t element;
    while (cb_remove(&myCB, &element) == 0) {
        printf("%d ", element);
    }
    printf("\n");

    // Checks whether the buffer is empty
    printf("Circular Buffer is Empty: %d\n", cb_isEmpty(&myCB));
    printf("Circular Buffer is Full: %d\n", cb_isFull(&myCB));

    printf("cb_remove() now returns %d\n", cb_remove(&myCB, &element));

    // Destroys the buffer und frees all ressources
    cb_destroy(&myCB);

    return 0;
}






