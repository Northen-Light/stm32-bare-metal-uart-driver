#include <stdint.h>
#include "io.h"
#include "usart.h"
#include "gpio.h"

#define BUFFER_MAX_LENGTH                                   512

uint8_t buffer[BUFFER_MAX_LENGTH];
uint16_t buffer_index;

void USART1_IRQHandler_callback(uint8_t byte);

void main(void) {
  gpio_portC_init();
  usart1_init();
  usart1_set_USART1_IRQHandler_callback(USART1_IRQHandler_callback);
  print_string((const uint8_t *)"> Input string : ");

  while (1) {}
}

void USART1_IRQHandler_callback(uint8_t byte) { 
  if (byte == '\r') {
    buffer[buffer_index] = '\0';
    
    if (buffer_index == (BUFFER_MAX_LENGTH - 1)) {
      print_string((const uint8_t *)"\n");
    }

    print_string((const uint8_t *)"\r\n");
    print_string((const uint8_t *)"> Output string : ");
    print_string((const uint8_t *)&buffer);
    print_string((const uint8_t *)"\r\n\n");
    buffer_index = 0;
    print_string((const uint8_t *)"> Input string : ");
    return;
  }

  if (buffer_index < (BUFFER_MAX_LENGTH - 1)) {
    usart1_write_char(byte);
    buffer[buffer_index++] = byte;
  } else {
    print_string((const uint8_t *)"\r\nBuffer overflow");
  }
}