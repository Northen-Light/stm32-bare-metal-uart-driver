#include "usart.h"
#include "stm32f103xx.h"

#define DMA_BUFFER_SIZE                     32

static volatile uint8_t dma_buffer[DMA_BUFFER_SIZE];
uint16_t dma_buffer_index = 0;
volatile uint32_t rounds = 0;

void usart1_init(void) { 
  RCC_APB2ENR |= RCC_APB2ENR_IOPAEN;
  RCC_APB2ENR |= RCC_APB2ENR_USART1EN;
  RCC_AHBENR |= RCC_AHBENR_DMA1EN;

  GPIOPA_CRH &= ~GPIOPA_CRH_9_MASK;        
  GPIOPA_CRH |= GPIOPA_CRH_9;

  GPIOPA_CRH &= ~GPIOPA_CRH_10_MASK;
  GPIOPA_CRH |= GPIOPA_CRH_10;

  DMA1_CCR5 = DMA1_CCR5_RESET_MASK;
  DMA1_CCR5 |= DMA1_CCR5_TCIE;
  DMA1_CCR5 |= DMA1_CCR5_CIRC;
  DMA1_CCR5 |= DMA1_CCR5_MINC;
  DMA1_CNDTR5 = DMA1_CNDTR5_16_BYTE_BUFFER;
  DMA1_CPAR5 = (uintptr_t) &USART1_DR;
  DMA1_CMAR5 = (uintptr_t) dma_buffer;
  DMA1_CCR5 |= DMA1_CCR5_EN;
  NVIC_ISER0 |= NVIC_ISER0_DMA1_CHANNEL5;

  USART_CR1 &= ~USART1_CR1_UE;
  USART_CR1 |= USART1_CR1_RE;
  USART_CR1 |= USART1_CR1_TE;
  USART_CR3 |= USART1_CR3_DMAR;

  /*
    Select Baud rate (BR) = 115200
    USARTDIV = PCLK / (16 * BR)
             = 4.3402
    Mantissa = 4
    Fraction = round(0.3 * 16)
             = 5 
  */
  USART1_BRR = USART1_BRR_115200_BR;

  USART_CR1 |= USART1_CR1_UE;
}

void usart1_write_char(uint8_t byte) {
  while ((USART1_SR & USART1_SR_TXE) == 0U) {}
  USART1_DR = byte;
}

usart1_status_t usart1_read_char(uint8_t *byte) {
  if (dma_buffer_index == (DMA1_CNDTR5_16_BYTE_BUFFER - DMA1_CNDTR5)) {
    return USART1_STATUS_READ_WAIT;
  }

  *byte = dma_buffer[dma_buffer_index];
  dma_buffer_index = (dma_buffer_index + 1) & (DMA1_CNDTR5_16_BYTE_BUFFER - 1);

  return USART1_STATUS_READ_OK;
}

void DMA1_Channel5_IRQHandler(void) {
  DMA1_IFCR |= DMA1_IFCR_CGIF5;
  rounds++;
}