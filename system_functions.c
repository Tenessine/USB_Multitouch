#include "system_functions.h"
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"

void enableIRQ(int irqNumber)
{
  NVIC->ISER[(((uint32_t)(int32_t)irqNumber) >> 5UL)] = (uint32_t)(1UL << (((uint32_t)(int32_t)irqNumber) & 0x1FUL));
}

