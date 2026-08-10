#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <stdint.h>

typedef enum {
  RINGBUFFER_STATUS_OK,
  RINGBUFFER_STATUS_ERROR
} ringbuffer_status_t;

ringbuffer_status_t ringbuffer_put(uint8_t byte);
ringbuffer_status_t ringbuffer_get(uint8_t *byte);
void ringbuffer_init(void);

#endif