#include <stdint.h>
#include <stdbool.h>
#include "io.h"
#include "usart.h"


uint8_t buffer[512];
uint16_t buffer_index;
bool buffer_full;

void main(void) {
  usart1_init();

  while (1) {
    uint8_t c;

    buffer_index = 0;
    buffer_full = false;

    print_string((const uint8_t *)"> Input string : ");

    while ((c = usart1_read_char()) != '\r') {
      usart1_write_char(c);
      buffer[buffer_index++] = c;
      if (buffer_index >= 511) {
        buffer_full = true;
        break;
      } 
    }
    
    buffer[buffer_index] = '\0';
    print_string((const uint8_t *)"\n");

    if (buffer_full == true) {
      while ((c = usart1_read_char()) != '\r') {
        print_string((const uint8_t *)"Buffer overflow\n");
      }
      print_string((const uint8_t *)"\n"); 
    }

    print_string((const uint8_t *)"> Output string : ");
    print_string((const uint8_t *)&buffer);
    print_string((const uint8_t *)"\n\n");
  }
}