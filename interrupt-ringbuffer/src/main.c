#include <stdint.h>
#include "io.h"
#include "usart.h"
#include "ringbuffer.h"

/*
  * 1. Blocking code of this application blocks CPU for atleast 1.5(ms).
  * 2. TX, RX speed of frame (1 start + 8 data + 1 stop = 10 bits) is 87(μs). 
  * 3. Keeping a ring buffer of 32 bytes, allows CPU gets blocked for 32 * 87(μs) = 2.8(ms), 
  *    bytes are buffered in ring buffer, without being overridden or lost. 
  * 4. With polling and interrupt with long ISR processing, we will miss incoming bytes. 
*/


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
        print_string((const uint8_t *)"\r\n");
      }

      print_string((const uint8_t *)"\r\n");
      print_string((const uint8_t *)"> Output string : ");
      print_string((const uint8_t *)&buffer);
      print_string((const uint8_t *)"\r\n\n");
      buffer_index = 0;
      print_string((const uint8_t *)"> Input string : ");
      continue;
    }

    if (buffer_index < (BUFFER_MAX_LENGTH - 1)) {
      usart1_write_char(byte);
      buffer[buffer_index++] = byte;
    } else {
      /*
      * Blocking code, it blocks CPU for atleast 1.5(ms) 
      */
      print_string((const uint8_t *)"\r\nBuffer overflow");
    }
  }
}
