#include <stdbool.h>
#include "ringbuffer.h"

#define RINGBUFFER_SIZE                               512

static uint8_t ringbuffer[RINGBUFFER_SIZE];
static uint16_t head;
static uint16_t tail;

static bool ringbuffer_full(void);
static bool ringbuffer_empty(void);

ringbuffer_status_t ringbuffer_put(uint8_t c) {
  if (!ringbuffer_full()) {
    ringbuffer[head] = c;
    head = (head + 1) & (RINGBUFFER_SIZE - 1);  
    return RINGBUFFER_STATUS_OK;
  }
  
  return RINGBUFFER_STATUS_ERROR;
}

ringbuffer_status_t ringbuffer_get(uint8_t *byte) {
  if (!ringbuffer_empty()) {
    *byte = ringbuffer[tail];
    tail = (tail + 1) & (RINGBUFFER_SIZE - 1);
    return RINGBUFFER_STATUS_OK;
  }
  
  return RINGBUFFER_STATUS_ERROR;
}

void ringbuffer_init(void) {
  head = 0;
  tail = 0;
}

static bool ringbuffer_full(void) {
  return ((head + 1) & (RINGBUFFER_SIZE - 1)) == tail;
}

static bool ringbuffer_empty(void) {
  return (head == tail);
}
