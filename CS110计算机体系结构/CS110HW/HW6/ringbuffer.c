#include "ringbuffer.h"

ringbuffer_t *ringbuffer_create(size_t size) {
    ringbuffer_t *ringbuffer = malloc(sizeof(ringbuffer_t));
    if (ringbuffer == NULL) {
        return NULL;
    }
    ringbuffer->size = size;
    ringbuffer->head = 0;
    ringbuffer->tail = 0;
    ringbuffer->count = 0;
    ringbuffer->buffer = malloc(size * sizeof(threadpool_task_t));
    if (ringbuffer->buffer == NULL) {
        free(ringbuffer);
        return NULL;
    }
    return ringbuffer;
}

void ringbuffer_destroy(ringbuffer_t *ringbuffer) {
    free(ringbuffer->buffer);
    free(ringbuffer);
}

bool ringbuffer_is_empty(ringbuffer_t *ringbuffer) {
    return ringbuffer->count == 0;
}

bool ringbuffer_is_full(ringbuffer_t *ringbuffer) {
    return ringbuffer->count == ringbuffer->size;
}

bool ringbuffer_push(ringbuffer_t *ringbuffer, threadpool_task_t value) {
    // Push the value value into the ringbuffer ringbuffer.
    // Return true if the push is successful, otherwise return false.
    if (ringbuffer_is_full(ringbuffer)) {
        return false;
    }
    ringbuffer->buffer[ringbuffer->tail] = value;
    ringbuffer->tail = (ringbuffer->tail + 1) % ringbuffer->size;
    ringbuffer->count++;
    return true;     
}

bool ringbuffer_pop(ringbuffer_t *ringbuffer, threadpool_task_t *value) {
    // Retrieve the value from the ringbuffer ringbuffer and store it in value.
    // Return true if the operation is successful, otherwise return false.
    if (ringbuffer_is_empty(ringbuffer)) {
        return false;
    }
    *value = ringbuffer->buffer[ringbuffer->head];
    ringbuffer->head = (ringbuffer->head + 1) % ringbuffer->size;
    ringbuffer->count--;
    return true;
}
