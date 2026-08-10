#ifndef USART_H
#define USART_H

#include <stdint.h>

typedef void (*USART1_IRQHandler_callback_t)(uint8_t byte);

void usart1_init(void);
void usart1_write_char(uint8_t c);
void usart1_set_USART1_IRQHandler_callback(USART1_IRQHandler_callback_t cb);

#endif