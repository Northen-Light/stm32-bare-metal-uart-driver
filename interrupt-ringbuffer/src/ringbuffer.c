#include <stdbool.h>
#include "ringbuffer.h"

#define RINGBUFFER_SIZE                          (uint16_t)512

static uint8_t ringbuffer[RINGBUFFER_SIZE];
static uint16_t head;
static uint16_t tail;

static bool ringbuffer_full(void);
static bool ringbuffer_empty(void);

ringbuffer_status_t ringbuffer_put(uint8_t c) {
  if (!ringbuffer_full()) {
    ringbuffer[head] = c;
    head = (uint16_t)((head + (uint16_t)1) % RINGBUFFER_SIZE);  
    return RINGBUFFER_STATUS_OK;
  }
  
  return RINGBUFFER_STATUS_ERROR;
}

ringbuffer_status_t ringbuffer_get(uint8_t *byte) {
  if (!ringbuffer_empty()) {
    *byte = ringbuffer[tail];
    tail = (uint16_t)((tail + (uint16_t)1) % RINGBUFFER_SIZE);
    return RINGBUFFER_STATUS_OK;
  }
  
  return RINGBUFFER_STATUS_ERROR;
}

void ringbuffer_init(void) {
  head = (uint16_t)0;
  tail = (uint16_t)0;
}

static bool ringbuffer_full(void) {
  return (((head + (uint16_t)1) % RINGBUFFER_SIZE) == tail);
}

static bool ringbuffer_empty(void) {
  return (head == tail);
}
