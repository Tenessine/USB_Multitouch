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
	
	//Main cycle
	for(;systemStopFlag==0;)
	{
		
	}
	
	return 0;
}
