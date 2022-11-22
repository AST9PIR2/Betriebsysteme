#include "circularbuffer.h"

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


int cb_destroy(circularbuffer_t* buffer) {
    if (buffer->buffer) {
        free(buffer->buffer);
        buffer->buffer = NULL;
        return 0;
    }
    return -1;
}

int cb_add(circularbuffer_t* buffer, cb_element_t elem) {
    if (buffer->count >= buffer->size) {
        return -1;
    }
    int elem_index = (buffer->first + buffer->count) % buffer->size;
    buffer->buffer[elem_index] = elem;
    buffer->count++;
    return 0;
}

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

int cb_count(circularbuffer_t* buffer) {
    return buffer->count;
}

int cb_isFull(circularbuffer_t* buffer) {
    return buffer->count == buffer->size;
}

int cb_isEmpty(circularbuffer_t* buffer) {
    return buffer->count == 0;
}