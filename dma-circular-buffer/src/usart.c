#include "usart.h"
#include "stm32f103xx.h"

#define DMA_BUFFER_SIZE                     32

static volatile uint8_t dma_buffer[DMA_BUFFER_SIZE];
volatile uint32_t write_cycles = 0; 
uint32_t read_cycles = 0;
uint32_t read_index = 0;

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
  DMA1_CNDTR5 = DMA_BUFFER_SIZE;
  DMA1_CPAR5 = (uintptr_t) &USART1_DR;
  DMA1_CMAR5 = (uintptr_t) dma_buffer;
  DMA1_CCR5 |= DMA1_CCR5_EN;
  NVIC_ISER0 |= NVIC_ISER0_DMA1_CHANNEL5;

  USART1_CR1 &= ~USART1_CR1_UE;
  USART1_CR1 |= USART1_CR1_RE;
  USART1_CR1 |= USART1_CR1_TE;
  USART1_CR3 |= USART1_CR3_DMAR;

  /*
    Select Baud rate (BR) = 115200
    USARTDIV = PCLK / (16 * BR)
             = 4.3402
    Mantissa = 4
    Fraction = round(0.3 * 16)
             = 5 
  */
  USART1_BRR = USART1_BRR_115200_BR;

  USART1_CR1 |= USART1_CR1_UE;
}

void usart1_write_char(uint8_t byte) {
  while ((USART1_SR & USART1_SR_TXE) == 0) {}
  USART1_DR = byte;
}

usart1_status_t usart1_read_char(uint8_t *byte) {
  uint32_t write_cycles_before = 0;
  uint32_t write_cycles_after = 0;
  uint32_t write_index = 0;
  uint32_t cyclic_difference;

  do {
    write_cycles_before = write_cycles;
    write_index = (DMA_BUFFER_SIZE - DMA1_CNDTR5);
    write_cycles_after = write_cycles;
  } while (write_cycles_before != write_cycles_after);

  cyclic_difference = write_cycles_after - read_cycles;

  if ((cyclic_difference > 1) || ((cyclic_difference == 1) && (write_index > read_index))) {
    read_index = write_index;
    read_cycles = write_cycles;
    return USART1_STATUS_DMA_BUFFER_OVERRUN;
  }

  if ((read_cycles == write_cycles_after) && (read_index == write_index)) {
    return USART1_STATUS_NO_DATA;
  }

  *byte = dma_buffer[read_index];
  read_index = (read_index + 1) & (DMA_BUFFER_SIZE - 1);

  if (read_index == 0) {
    read_cycles++;
  }

  return USART1_STATUS_OK;
}

void usart1_enable_rx(void) {
  USART1_CR1 |= USART1_CR1_RE;
}

void usart1_disable_rx(void) {
  USART1_CR1 &= ~USART1_CR1_RE;
}

void DMA1_Channel5_IRQHandler(void) {
  DMA1_IFCR = DMA1_IFCR_CGIF5;
  write_cycles++;
}