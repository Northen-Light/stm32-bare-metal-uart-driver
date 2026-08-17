#include <stdbool.h>
#include "ringbuffer.h"

#define RINGBUFFER_SIZE                                 64

static uint8_t ringbuffer[RINGBUFFER_SIZE];
static volatile uint16_t head;
static volatile uint16_t tail;

static bool ringbuffer_full(void);
static bool ringbuffer_empty(void);

ringbuffer_status_t ringbuffer_put(uint8_t c) {
  if (ringbuffer_full()) {
    return RINGBUFFER_STATUS_FULL;
  }

  ringbuffer[head] = c;
  head = (head + 1) & (RINGBUFFER_SIZE - 1);  
  return RINGBUFFER_STATUS_OK;
}

ringbuffer_status_t ringbuffer_get(uint8_t *byte) {
  if (ringbuffer_empty()) {
    return RINGBUFFER_STATUS_EMPTY;
  }
  
  *byte = ringbuffer[tail];
  tail = (tail + 1) & (RINGBUFFER_SIZE - 1);
  return RINGBUFFER_STATUS_OK;
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
