#include "usart.h"
#include "stm32f103xx.h"

static USART1_IRQHandler_callback_t USART1_IRQHandler_callback;

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
}

void usart1_write_char(uint8_t c) {
  while ((USART1_SR & USART1_SR_TXE) == 0) {}
  USART1_DR = c;
}

void usart1_set_USART1_IRQHandler_callback(USART1_IRQHandler_callback_t cb) {
  USART1_IRQHandler_callback = cb;
}

void USART1_IRQHandler(void) {
  if ((USART_CR1 & USART1_CR1_RXNEIE) > 0U) {
    if ((USART1_SR & USART1_SR_RXNE) > 0U) {
      uint8_t byte = USART1_DR;
      USART1_IRQHandler_callback(byte);
    }
  }
}