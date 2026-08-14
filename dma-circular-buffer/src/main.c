#include <stdint.h>
#include <stdbool.h>
#include "io.h"
#include "usart.h"

#define BUFFER_MAX_LENGTH                                 512

uint8_t buffer[BUFFER_MAX_LENGTH];
uint16_t buffer_index = 0U;
bool buffer_full = false;

void main(void) {
  usart1_init();
  print_string((const uint8_t *)"> Input string : ");

  while (1) {
    uint8_t byte;
    
    while (usart1_read_char(&byte) != USART1_STATUS_READ_OK);

    if (byte == '\r') {
      buffer[buffer_index] = '\0';
      
      if (buffer_index == (BUFFER_MAX_LENGTH - 1)) {
        print_string((const uint8_t *)"\r\n");
      }

      print_string((const uint8_t *)"\r\n");
      print_string((const uint8_t *)"> Output string : ");
      print_string((const uint8_t *)&buffer);
      print_string((const uint8_t *)"\r\n\n");
      buffer_index = 0U;
      print_string((const uint8_t *)"> Input string : ");
      continue;
    }

    if (buffer_index < (BUFFER_MAX_LENGTH - 1)) {
      usart1_write_char(byte);
      buffer[buffer_index++] = byte;
      buffer_full = true;
    } else {
      if (buffer_full) {
        buffer_full = false;
        print_string((const uint8_t *)"\r\n\n> Buffer Overflow !!! Enter return to continue.");
      }
    }
  }
}
