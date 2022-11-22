#include <stdio.h>
// Benötigt für fork(), exec() und waitpid()
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
// Benötigt für perror()
#include <errno.h>
// Benötigt für printf()
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

void printMatrix(int * array, int zeile, int spalte){
    int counter = 0;
    int smalestValue = array[0];
    int biggestValue = array[0];
    int sum = array[0];

    for( int i = 0; i < spalte * zeile; i++ ){
        printf("%5i ", array[i]);
        counter ++;
        if(counter == spalte){
            printf("\n");
            counter = 0;
        }
    }

    for( int i = 0; i < spalte * zeile; i++ ){

        sum += array[i];

        if(smalestValue > array[i]){
            smalestValue = array[i];
        }

        if(biggestValue < array[i]){
            biggestValue = array[i];
        }
    }

    printf("Minimaler Wert: %i; Maximaler Wert: %i; sum aller Werte: %i\n\n", smalestValue, biggestValue, sum);
}

struct thread_test{
    int ZeileA;
    int SpalteA;
    int ZeileB;
    int SpalteB;
    int * ArrayA;
    int * ArrayB;
    int * ArrayC;
    long temp;
    long array_counter;
    int counterC;
}dataHandler;

void* test(void* arg) {

    int* mydata = (int*)arg;

    for(int j = 0; j < dataHandler.SpalteB; j++){
        for(int x = 0; x < dataHandler.SpalteA; x++){
            dataHandler.temp = dataHandler.temp + (dataHandler.ArrayA[*mydata+x] * dataHandler.ArrayB[j+(dataHandler.SpalteB*x)]);
        }
        dataHandler.ArrayC[dataHandler.array_counter] = dataHandler.temp;
        dataHandler.temp = 0;
        dataHandler.array_counter++;
    }

    return 0;
}

int main() {
    printf("*** Matrizenmultiplikation ***\n");


    dataHandler.ZeileA = 10;
    dataHandler.SpalteA = 20;
    dataHandler.ZeileB = 20;
    dataHandler.SpalteB = 12;

    int a;
    int b;
    int size;

    if(dataHandler.ZeileA < dataHandler.SpalteA){
        a = dataHandler.SpalteA;
    }else{
        a = dataHandler.ZeileA;
    }

    if(dataHandler.ZeileB < dataHandler.SpalteB){
        b = dataHandler.SpalteB;
    }else{
        b = dataHandler.ZeileB;
    }

    size = a * b;

    dataHandler.ArrayA = (int *) malloc(size * sizeof(int));
    for( int i = 0; i < dataHandler.SpalteA * dataHandler.ZeileA; i++ ){
        dataHandler.ArrayA[i] = rand() % 10;
    }
    printf("*** Array A ***\n");
    printMatrix(dataHandler.ArrayA, dataHandler.ZeileA,dataHandler.SpalteA);

    dataHandler.ArrayB = (int *) malloc(size * sizeof(int));
    for( int i = 0; i < dataHandler.SpalteB * dataHandler.ZeileB; i++ ){
        dataHandler.ArrayB[i] = rand() % 10;
    }
    printf("*** Array B ***\n");
    printMatrix(dataHandler.ArrayB, dataHandler.ZeileB,dataHandler.SpalteB);

    // Reserviere Speicher fÃ¼r die Thread-Handles
    pthread_t myThreads[(dataHandler.SpalteA * dataHandler.ZeileB)];

    printf("*** Array C ***\n");
    dataHandler.ArrayC = (int *) malloc(size * sizeof(int));
    dataHandler.counterC = 0;

    dataHandler.array_counter = 0;
    dataHandler.temp = 0;

    for( int i  = 0; i < (size); i=i+dataHandler.SpalteA ){
        pthread_create(&myThreads[i], NULL, &test, &i);
        pthread_join(myThreads[i], NULL);
    }



    printMatrix(dataHandler.ArrayC, dataHandler.ZeileA,dataHandler.SpalteB);

    free(dataHandler.ArrayA);
    free(dataHandler.ArrayB);
    free(dataHandler.ArrayC);

    return 0;

}