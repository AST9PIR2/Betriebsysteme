#include <stdlib.h>
#include <stdio.h>



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
    int size;   // Groesse des Buffers
} circularbuffer_t;

/**
 * Initializes the circular buffer.
 *
 * \param buffer pointer to struct representing a circular buffer
 * \param size size of the circular buffer
 * \return a negative value in case of an error otherwise 0
 */
int cb_init(circularbuffer_t* buffer, int size);

/**
 * Destroys the circular buffer and frees all ressources.
 *
 * \param buffer pointer to struct representing the circular buffer
 * \return a negative value in case of an error otherwise 0
 */
int cb_destroy(circularbuffer_t* buffer);

/**
 * Adds an Element to the circular buffer
 *
 * \param buffer pointer to struct representing a circular buffer
 * \param elem value to be added to the circular buffer
 * \return -1 when the buffer is full otherwise 0
 */
int cb_add(circularbuffer_t* buffer, cb_element_t elem);

/**
 * Removes an Element from the circular buffer. The removed element is returned via call-by-reference.
 *
 * \param buffer pointer to struct representing a circular buffer
 * \param elem pointer to where the removed value should be stored (call-by-reference)
 * \return -1 when the buffer is empty otherwise 0
 */
int cb_remove(circularbuffer_t* buffer, cb_element_t* elem);

/**
 * Returns the number of elements in the circular buffer
 *
 * \param buffer pointer to struct representing a circular buffer
 * \return number of elements in the circular buffer
 */
int cb_count(circularbuffer_t* buffer);

/**
 * Returns whether the circular buffer is full
 *
 * \param buffer pointer to struct representing a circular buffer
 * \return boolean value indicating that the buffer is full
 */
int cb_isFull(circularbuffer_t* buffer);

/**
 * Returns whether the circular buffer is empty
 *
 * \param buffer pointer to struct representing a circular buffer
 * \return boolean value indicating whether the buffer is empty
 */
int cb_isEmpty(circularbuffer_t* buffer);