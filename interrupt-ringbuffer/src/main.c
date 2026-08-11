#include <stdint.h>
#include "io.h"
#include "usart.h"
#include "ringbuffer.h"

#define BUFFER_MAX_LENGTH                                 512

uint8_t buffer[BUFFER_MAX_LENGTH];
uint16_t buffer_index;

void main(void) {
  usart1_init();
  print_string((const uint8_t *)"> Input string : ");

  while (1) {
    uint8_t byte;
    
    while (ringbuffer_get(&byte) != RINGBUFFER_STATUS_OK);

    if (byte == '\r') {
      buffer[buffer_index] = '\0';
      
      if (buffer_index == (BUFFER_MAX_LENGTH - 1)) {
        print_string((const uint8_t *)"\n");
      }

      print_string((const uint8_t *)"\n");
      print_string((const uint8_t *)"> Output string : ");
      print_string((const uint8_t *)&buffer);
      print_string((const uint8_t *)"\n\n");
      buffer_index = 0;
      print_string((const uint8_t *)"> Input string : ");
      continue;
    }

    if (buffer_index < (BUFFER_MAX_LENGTH - 1)) {
      usart1_write_char(byte);
      buffer[buffer_index++] = byte;
    } else {
      print_string((const uint8_t *)"\nBuffer overflow");
    }
  }
}