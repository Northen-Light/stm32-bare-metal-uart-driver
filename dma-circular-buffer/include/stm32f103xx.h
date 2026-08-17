#include <stdint.h>

#define RCC_AHBENR                                *((volatile uint32_t *)0x40021014U)
#define RCC_APB2ENR                               *((volatile uint32_t *)0x40021018U)
#define GPIOPA_CRH                                *((volatile uint32_t *)0x40010804U)
#define USART1_SR                                 *((volatile uint32_t *)0x40013800U)  
#define USART1_DR                                 *((volatile  uint8_t *)0x40013804U)  
#define USART1_BRR                                *((volatile uint32_t *)0x40013808U)  
#define USART1_CR1                                 *((volatile uint32_t *)0x4001380CU)
#define USART1_CR3                                 *((volatile uint32_t *)0x40013814U)
#define NVIC_ISER0                                *((volatile uint32_t *)0xE000E100U)
#define GPIOC_CRH                                 *((volatile uint32_t *)0x40011004U)
#define GPIOC_BSRR                                *((volatile uint32_t *)0x40011010U)
#define DMA1_IFCR                                 *((volatile uint32_t *)0x40020004U)
#define DMA1_CCR5                                 *((volatile uint32_t *)0x40020058U)
#define DMA1_CNDTR5                               *((volatile uint32_t *)0x4002005CU)
#define DMA1_CPAR5                                *((volatile uint32_t *)0x40020060U)
#define DMA1_CMAR5                                *((volatile uint32_t *)0x40020064U)



#define RCC_APB2ENR_IOPAEN                        (1U << 2)
#define RCC_APB2ENR_IOPCEN                        (1U << 4)
#define RCC_APB2ENR_USART1EN                      (1U << 14)
#define RCC_AHBENR_DMA1EN                         (1U << 0)

#define GPIOPA_CRH_9                              (0xAU << 4)
#define GPIOPA_CRH_10                             (0x4U << 8)
#define GPIOPA_CRH_9_MASK                         (0xFU << 4)
#define GPIOPA_CRH_10_MASK                        (0xFU << 8)
#define GPIOPC_CRH_13                             (1U << 20)
#define GPIOC_BSRR_SET_13                         (1U << 13)
#define GPIOC_BSRR_RESET_13                       (1U << (13 + 16))
#define GPIOPC_CRH_13_MASK                        (0xFU << 20)

#define USART1_CR1_RE                             (1U << 2)
#define USART1_CR1_TE                             (1U << 3)
#define USART1_CR1_UE                             (1U << 13)
#define USART1_CR3_DMAR                           (1U << 6)
#define USART1_SR_RXNE                            (1U << 5)
#define USART1_SR_TXE                             (1U << 7)
#define USART1_BRR_115200_BR                      ((0x4U << 4) | 0x5U)

#define DMA1_CCR5_RESET_MASK                      (0U)
#define DMA1_CCR5_EN                              (1U << 0)
#define DMA1_CCR5_TCIE                            (1U << 1)
#define DMA1_CCR5_CIRC                            (1U << 5)
#define DMA1_CCR5_MINC                            (1U << 7)
#define DMA1_IFCR_CGIF5                           (1U << 16)


#define NVIC_ISER0_DMA1_CHANNEL5                  (1U << 15)
