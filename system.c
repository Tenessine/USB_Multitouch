#include "system.h"
#include "stm32f10x_i2c.h"

//---------------------------------Private defines---------------------------------- 
#define PLLMUL			(0x4<<18)	//PLL multiplication factor (mul.6)
#define PLLXTPRE 		(0<<17)		//HSE divider for PLL entry (0 - not devided, 1 - devided by 2)
#define PLLSRC 			(1<<16)		//PLL entry clock source (0 - HSI, 1 - HSE)
#define HPRE				(0x8<<4)	//AHB prescaler (dev.2)
#define SW					(0x2)			//PLL selected as system clock
#define PPRE1				(0x4<<8)	//APB1 clock devided by 2
#define PPRE2				(0x4<<11)	//APB2 clock devided by 2
#define USBPRE			(1<<22)		//PLL clock is not divided
#define APB_FREQ		(12&0x3F)	//APB clock frequency value (12Mhz)
#define I2C_CLK_SPD (100000)	//I2C clock speed

void pllConfig(void)
{
	//Read current configuration
	__IO uint32_t t_register = RCC->CFGR;
	
	//If PLL+HSE enabled
	//switch to HSI and
	//disable PLL	
	if((RCC->CR&(1<<24)))
	{
		if((t_register&SW)==SW)
		{
			RCC->CFGR ^= SW;
			t_register = RCC->CFGR;
		}
		RCC->CR ^= 1<<24;
		while(((RCC->CR)&(1<<24))==1){};
	}
	
	RCC->CFGR = 0;
	t_register = RCC->CFGR;
	
	if((t_register&SW)!=SW)
	{
		RCC->CFGR |= SW;
	}
	//Set AHB prescaler
	if((t_register&HPRE)!=HPRE)
	{
			RCC->CFGR |= HPRE;
	}
	
	if((t_register&PLLSRC)!=PLLSRC)
	{
			RCC->CFGR |= PLLSRC;
	}
	
	if((t_register&PLLXTPRE)!=PLLXTPRE)
	{
		if(!PLLXTPRE)
			RCC->CFGR ^=1<<17;
		else
			RCC->CFGR |=1<<17;
  }
	
	//Configure PLL
	RCC->CFGR |= PLLMUL|PPRE1|PPRE2|USBPRE;
	
	//PLL On
	RCC->CR |= (1<<24); 
	while((RCC->CR & RCC_CR_PLLRDY) == 0) {}
	
	//PLL selected as system clock	
	RCC->CFGR |= (1<<1); 
	while ((RCC->CFGR & RCC_CFGR_SWS) != (1<<3)) {}
}

void gpioInit(void)
{
	//Reset GPIOB
	GPIOB->CRL = 0x44444444;
	//Configure GPIO pin : PB4 (INT)
	GPIOB->CRL |= (1<<18);	//Floating input
	
	//Set external interrupt on port B
	AFIO->EXTICR[1] |= AFIO_EXTICR2_EXTI4_PB; //(1<<0)
	//Enable interrupt on EXTI4
	EXTI->IMR |= EXTI_IMR_MR4;
	//Interupt by rising edge
	EXTI->RTSR |= EXTI_RTSR_TR4;
	//NVIC->ISER[0] = NVIC_ISER_SETENA_6;
	NVIC_EnableIRQ(EXTI4_IRQn);	
		
	//Configure GPIO pin : PB5 (RST)
	GPIOB->CRL |= (3<<20);	//Output mode 50 MHz
	
	if((GPIOB->CRL)&0xC00000)
	{
		//Set 0 to CNF5 (Push-pull)
		GPIOB->CRL |= (3<<22);
		GPIOB->CRL ^= (3<<22);
	}
}

void clockEnable(void)
{
	RCC->APB2ENR |= (1<<3); //IOPBEN
	RCC->APB2ENR |= (1<<2); //IOPAEN
	RCC->APB2ENR |= (1<<0); //AFIOEN
		
	RCC->APB1ENR |= (1<<21);//I2C1EN
	RCC->APB1ENR |= (1<<0); //TIM2EN;	
}

void timerInit(void)
{
	//Counter used as downcounter
	TIM2->CR1 |= (1<<4);
	//Set the autoreload value
	//TIM2->ARR = 1000;
	//Set prescaller value
	TIM2->PSC = 24000;
	//Set update generation
	TIM2->EGR = 1;
	//Enable the interrupt sources
  TIM2->DIER |= 0x0001;
	//Enable the TIM Counter
	//TIM2->CR1 |= TIM_CR1_CEN;
}

void timDelay(uint16_t ms)
{
	//Reset tim counter	
	//Register no equal zero because
	//timer configured as downcounter	
	TIM2->CNT = (uint16_t)ms;
	//Set the autoreload value
	TIM2->ARR = (uint16_t)ms;
	//Enable the TIM Counter
	TIM2->CR1 |= TIM_CR1_CEN;
	//Whait flag
	while(((TIM2->SR)&(0x1))!=1){}
	//Reset flag
	TIM2->SR^=1;
	//Disable the TIM Counter
	TIM2->CR1 ^= TIM_CR1_CEN;	
}
