#ifndef USART_H
#define USART_H

#include <stdint.h>

typedef enum {
  USART1_STATUS_OK,
  USART1_STATUS_NO_DATA
} usart1_status_t;

void usart1_init(void);
void usart1_write_char(uint8_t byte);
usart1_status_t usart1_read_char(uint8_t *byte);
void usart1_enable_rx(void);
void usart1_disable_rx(void);

#endif