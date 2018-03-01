#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_i2c.h"
#include "system.h"
#include "handlers.h"
#include "ft5206.h"
#include "i2c.h"

//----------------------------------------------------Flags--------------------------------------------------
uint8_t int_flag_pb4 = 0;
DeviceMode currentMode = DeviceMode_TestMode;

int main()
{
	pllConfig();	
	SystemCoreClockUpdate();
	clockEnable();		
	gpioInit();	
	i2cInit();
	
	__enable_irq();
	
	timerInit();
	
	//FT5X06 Section
	ft5x06Init();
	timDelay(100);
	//Add timer awainting
	currentMode =  getDeviceMode();
	
	for(;;)
	{
		if(currentMode == DeviceMode_NormalOperatingMode)
		{
			if(int_flag_pb4)
			{
				int_flag_pb4 = 0;
			}
		}
		if(currentMode == DeviceMode_SystemInfoMode)
		{
			//Sestem info mode
		}
		if(currentMode == DeviceMode_TestMode)
		{
			//Test mode
		}
	}
	
	return 0;
}
