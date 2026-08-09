#include "io.h"
#include "usart.h"

void print_string(const uint8_t *buffer) {
  while (*buffer) {
    usart1_write_char(*buffer++);
  }
}