#include "usart.h"
#include "stm32f103xx.h"
#include "ringbuffer.h"

void usart1_init(void) { 
  RCC_APB2ENR |= RCC_APB2ENR_IOPAEN;
  RCC_APB2ENR |= RCC_APB2ENR_USART1EN;

  GPIOPA_CRH &= ~GPIOPA_CRH_9_MASK;        
  GPIOPA_CRH |= GPIOPA_CRH_9;

  GPIOPA_CRH &= ~GPIOPA_CRH_10_MASK;
  GPIOPA_CRH |= GPIOPA_CRH_10;

  USART_CR1 &= ~USART1_CR1_UE;
  USART_CR1 |= USART1_CR1_RE;
  USART_CR1 |= USART1_CR1_TE;
  USART_CR1 |= USART1_CR1_RXNEIE;

  /*
    Select Baud rate (BR) = 115200
    USARTDIV = PCLK / (16 * BR)
             = 4.3402
    Mantissa = 4
    Fraction = round(0.3 * 16)
             = 5 
  */
  USART1_BRR = USART1_BRR_115200_BR;

  NVIC_ISER1 = NVIC_ISER1_USART1;
  USART_CR1 |= USART1_CR1_UE;
  
  ringbuffer_init();
}

void usart1_write_char(uint8_t c) {
  while ((USART1_SR & USART1_SR_TXE) == 0U) {}
  USART1_DR = c;
}

void USART1_IRQHandler(void) {
  if ((USART_CR1 & USART1_CR1_RXNEIE) > 0U) { 
    // Previously faced frame error probably due to the above condition
    if ((USART1_SR & USART1_SR_RXNE) > 0U) {
      uint8_t byte = USART1_DR;
      ringbuffer_put(byte);
    }
  }
}