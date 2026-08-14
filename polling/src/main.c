#include <stdint.h>
#include <stdbool.h>
#include "io.h"
#include "usart.h"
#include "gpio.h"

#define BUFFER_MAX_LENGTH                                 512

uint8_t buffer[BUFFER_MAX_LENGTH];
uint16_t buffer_index = 0U;
bool buffer_full = false;

void main(void) {
  gpio_portC_init();
  usart1_init();

  while (1) {
    uint8_t c;

    buffer_index = 0;
    buffer_full = false;

    print_string((const uint8_t *)"> Input string : ");

    while ((c = usart1_read_char()) != '\r') {
      usart1_write_char(c);
      buffer[buffer_index++] = c;
      if (buffer_index >= (BUFFER_MAX_LENGTH - 1)) {
        buffer_full = true;
        break;
      } 
    }
    
    buffer[buffer_index] = '\0';

    if (buffer_full == true) {
      gpioPC13_set();
      print_string((const uint8_t *)"\r\n\n> Buffer Overflow !!! Enter return to continue.");
      gpioPC13_reset();
      while ((c = usart1_read_char()) != '\r') {}
      print_string((const uint8_t *)"\r\n"); 
    }

    print_string((const uint8_t *)"\r\n");
    print_string((const uint8_t *)"> Output string : ");
    print_string((const uint8_t *)&buffer);
    print_string((const uint8_t *)"\r\n\n");
  }
}