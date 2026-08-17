#ifndef USART_H
#define USART_H

#include <stdint.h>

typedef enum {
  USART1_STATUS_READ_DONE,
  USART1_STATUS_READ_DATA_NOT_PRESENT
} usart1_status_t;

void usart1_init(void);
void usart1_write_char(uint8_t c);
usart1_status_t usart1_read_char(uint8_t *byte);
void usart1_enable_rx(void);
void usart1_disable_rx(void);

#endif