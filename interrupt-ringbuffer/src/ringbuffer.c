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
    head = (uint16_t)((head + 1U) & (RINGBUFFER_SIZE - 1U));  
    return RINGBUFFER_STATUS_OK;
  }
  
  return RINGBUFFER_STATUS_ERROR;
}

ringbuffer_status_t ringbuffer_get(uint8_t *byte) {
  if (!ringbuffer_empty()) {
    *byte = ringbuffer[tail];
    tail = (uint16_t)((tail + 1U) & (RINGBUFFER_SIZE - 1U));
    return RINGBUFFER_STATUS_OK;
  }
  
  return RINGBUFFER_STATUS_ERROR;
}

void ringbuffer_init(void) {
  head = 0U;
  tail = 0U;
}

static bool ringbuffer_full(void) {
  return (((head + 1U) & (RINGBUFFER_SIZE - 1U)) == tail);
}

static bool ringbuffer_empty(void) {
  return (head == tail);
}
