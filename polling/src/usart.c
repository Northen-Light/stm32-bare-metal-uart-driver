#include "usart.h"
#include "stm32f103xx.h"

void usart1_init(void) { 
  RCC_APB2ENR |= RCC_APB2ENR_IOPAEN;
  RCC_APB2ENR |= RCC_APB2ENR_USART1EN;

  GPIOPA_CRH &= ~GPIOPA_CRH_9_MASK;        
  GPIOPA_CRH |= GPIOPA_CRH_9;

  GPIOPA_CRH &= ~GPIOPA_CRH_10_MASK;
  GPIOPA_CRH |= GPIOPA_CRH_10;

  USART_CR1 &= ~USART_CR1_UE;
  USART_CR1 |= USART_CR1_RE;
  USART_CR1 |= USART_CR1_TE;

  /*
    Select Baud rate (BR) = 115200
    USARTDIV = PCLK / (16 * BR)
             = 4.3402
    Mantissa = 4
    Fraction = round(0.3 * 16)
             = 5 
  */
  USART1_BRR = USART1_BRR_115200_BR;

  USART_CR1 |= USART_CR1_UE;
}

void usart1_write_char(uint8_t c) {
  while ((USART1_SR & USART1_SR_TXE) == 0) {}
  USART1_DR = c;
}

uint8_t usart1_read_char(void) {
  while ((USART1_SR & USART1_SR_RXNE) == 0) {}
  return USART1_DR;
}