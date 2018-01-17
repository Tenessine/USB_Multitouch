#include "handlers.h"
#include "stm32f10x.h"

void EXTI4_IRQHandler()
{
	GPIOB->ODR ^=1<<5;
	EXTI->PR |= EXTI_PR_PR4;
}
