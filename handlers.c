#include "handlers.h"
#include "stm32f10x.h"

extern uint8_t int_flag_pb4;

void EXTI4_IRQHandler()
{
	if(!int_flag_pb4)
		int_flag_pb4 = 1;
	GPIOB->ODR ^=1<<5;
	EXTI->PR |= EXTI_PR_PR4;
}
