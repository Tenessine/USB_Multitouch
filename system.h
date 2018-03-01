#ifndef __SYSTEM
#define __SYSTEM

#include <stdint.h>

void pllConfig(void);
void gpioInit(void);
void clockEnable(void);
void timerInit(void);
void timDelay(uint16_t);


#endif
