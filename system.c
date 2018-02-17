#include "system.h"
#include "stm32f10x.h"
#include "system_functions.h"

/*---------------------------------Private defines----------------------------------*/
#define PLLMUL			(0x4<<18)	//PLL multiplication factor (mul.6)
#define PLLXTPRE 		(0<<17)		//HSE divider for PLL entry (0 - not devided, 1 - devided by 2)
#define PLLSRC 			(1<<16)		//PLL entry clock source (0 - HSI, 1 - HSE)
#define HPRE				(0x8<<4)	//AHB prescaler (dev.2)
#define SW					0x2				//PLL selected as system clock
#define PPRE1				(0x4<<8)	//APB1 clock devided by 2
#define PPRE2				(0x4<<11)	//APB2 clock devided by 2
#define USBPRE			(1<<22)		//PLL clock is not divided
#define APB_FREQ		(12&0x3F)	//APB clock frequency value (12Mhz)
#define I2C_CLK_SPD (100000)	//I2C clock speed

void rccInit(void)
{
	//Read current configuration
	__IO uint32_t t_register = RCC->CR;
	
	//HSE On
	if(!(t_register&(1<<17)))
	{
		RCC->CR |= 1<<16;
		while(((RCC->CR)&(1<<17))==0){};
		t_register = RCC->CR;
	}
	
	//HSI off
	if(t_register&(1<<0)&&(RCC->CFGR&0xC)!=0)
	{
		RCC->CR ^= (1<<0);
		while(((RCC->CR)&(1<<0))==1){};		
		t_register = RCC->CR;
	}
	
	//PLL on
	if(!(t_register&(1<<24)))
	{
		RCC->CR |= 1<<24;
		while(((RCC->CR)&(1<<25))==0){};
		if(t_register&(1<<0))
		{
			RCC->CR ^= (1<<0);
			while(((RCC->CR)&(1<<0))==1){};
		}
	}
	
	//Todo: clock security system enable
}

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
		//t_register = RCC->CFGR;
	}
	
	if((t_register&HPRE)!=HPRE)
	{
			RCC->CFGR |= HPRE;
			//t_register = RCC->CFGR;
	}
	
	if((t_register&PLLSRC)!=PLLSRC)
	{
			RCC->CFGR |= PLLSRC;
			//t_register = RCC->CFGR;
	}
	
	if((t_register&PLLXTPRE)!=PLLXTPRE)
	{
		if(!PLLXTPRE)
			RCC->CFGR ^=1<<17;
		else
			RCC->CFGR |=1<<17;
  }
	
	RCC->CFGR |= PLLMUL|PPRE1|PPRE2|USBPRE;
}

void clockEnable(void)
{
	RCC->APB2ENR |= (1<<3); //IOPBEN
	RCC->APB2ENR |= (1<<2); //IOPAEN
	RCC->APB2ENR |= (1<<0); //AFIOEN
		
	RCC->APB1ENR |= (1<<21); //I2C1EN
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
	GPIOB->CRL |= (2<<20);	//Output mode 2 MHz
	
	if((GPIOB->CRL)&0xC00000)
	{
		//Set 0 to CNF5 (Push-pull)
		GPIOB->CRL |= (3<<22);
		GPIOB->CRL ^= (3<<22);
	}
}

void i2c_init(void)
{
	//Configure GPIO pin : PB6(SCL) PB7(SDA)
	GPIOB->CRL |= (0xF<<26);	//Alternate function Open-Drain,
																			//output mode 50 MHz
	//Enable I2C1 clock 
	RCC->APB1ENR |= (1<<21);
	//Set APB clock frequency value
	I2C1->CR2 |= APB_FREQ;
	//Set maximum rise time in master mode
	I2C1->TRISE = (uint16_t)APB_FREQ+1;
	//Configure clock control register
	I2C1->CCR = 0x3C;//((1/I2C_CLK_SPD)/2)/(1/APB_FREQ*10^6)
	//Enable I2C1 peripheral
	I2C1->CR1 |= (1<<0);
}
