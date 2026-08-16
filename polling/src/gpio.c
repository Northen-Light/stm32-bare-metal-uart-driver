#include "gpio.h"
#include "stm32f103xx.h"

void gpio_portC_init(void) {
  RCC_APB2ENR |= RCC_APB2ENR_IOPCEN;
  GPIOC_CRH &= ~GPIOPC_CRH_13_MASK;
  GPIOC_CRH |= GPIOPC_CRH_13;
}

void gpioPC13_set() {
  GPIOC_BSRR = GPIOC_BSRR_SET_13;
}

void gpioPC13_reset() {
  GPIOC_BSRR = GPIOC_BSRR_RESET_13;
}