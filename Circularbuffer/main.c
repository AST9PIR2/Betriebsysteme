#include "circularbuffer.h"

int main() {

    // Creates a circular buffer with 7 elements
    circularbuffer_t myCB;
    cb_init(&myCB, 7);

    // Checks whether the buffer is empty
    printf("Circular Buffer is Empty: %d\n", cb_isEmpty(&myCB));
    printf("Circular Buffer is Full: %d\n", cb_isFull(&myCB));

    // Add values to the circular buffer
    cb_add(&myCB, 1);
    printf("Number of Elements: %d\n", cb_count(&myCB));
    cb_add(&myCB, 2);
    printf("Number of Elements: %d\n", cb_count(&myCB));
    cb_add(&myCB, 3);
    printf("Number of Elements: %d\n", cb_count(&myCB));
    cb_add(&myCB, 4);
    printf("Number of Elements: %d\n", cb_count(&myCB));
    cb_add(&myCB, 5);
    printf("Number of Elements: %d\n", cb_count(&myCB));
    cb_add(&myCB, 6);
    printf("Number of Elements: %d\n", cb_count(&myCB));
    cb_add(&myCB, 7);
    printf("Number of Elements: %d\n", cb_count(&myCB));

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
}