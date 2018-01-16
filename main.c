#include "system.h"
#include "stdint.h"

/*---------------------------------Private flags----------------------------------*/
uint8_t systemStopFlag;

int main()
{
	//Initialise system
	pllConfig();
	rccInit();
	clockEnable();
	gpioInit();
	
	__enable_irq();
	
	//Main cycle
	for(;systemStopFlag==0;)
	{
		
	}
	
	return 0;
}
