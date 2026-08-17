#include <stdint.h>
#include <stdbool.h>
#include "io.h"
#include "usart.h"
#include "ringbuffer.h"
#include "gpio.h"

#define BUFFER_MAX_LENGTH                                 512

static uint8_t buffer[BUFFER_MAX_LENGTH];
static uint16_t buffer_index = 0;
static bool buffer_full = false;
static bool should_print_input_prompt = true;

static void process_usart1_received_data_task(void);

void main(void) {
  gpio_portC_init();
  usart1_init();

  while (1) {
    process_usart1_received_data_task();
  }
}

/*
 * Reception is disabled while printing the completed line.
 * The terminal application assumes that the user waits for the
 * next input prompt before entering another line.
 */

static void process_usart1_received_data_task(void) {
  gpioPC13_set();
  uint8_t byte;

  if (should_print_input_prompt) {
    print_string((const uint8_t *)"> Input string : ");
    should_print_input_prompt = false;
  }

  while (ringbuffer_get(&byte) == RINGBUFFER_STATUS_OK) {
    if (byte == '\r') {
      usart1_disable_rx();
      buffer[buffer_index] = '\0';
      
      if (buffer_index == (BUFFER_MAX_LENGTH - 1)) {
        print_string((const uint8_t *)"\r\n");
      }

      print_string((const uint8_t *)"\r\n");
      print_string((const uint8_t *)"> Output string : ");
      print_string(buffer);
      print_string((const uint8_t *)"\r\n\n");
      buffer_index = 0;
      should_print_input_prompt = true;
      usart1_enable_rx();
      continue;
    }

    if (buffer_index < (BUFFER_MAX_LENGTH - 1)) {
      usart1_write_char(byte);
      buffer[buffer_index++] = byte;
      
      if (buffer_index == (BUFFER_MAX_LENGTH - 1)) {
        buffer_full = true;
      }
    } else {
      if (buffer_full) {
        usart1_disable_rx();
        buffer_full = false;
        print_string((const uint8_t *)"\r\n\n> Buffer Overflow !!! Enter return to continue.");
        usart1_enable_rx();
      }
    }
  }

  gpioPC13_reset();
}
