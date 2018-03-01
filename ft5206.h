#ifndef __FT5206
#define __FT5206

#include "stdint.h"

/* Defines -------------------------------------------------------------------*/
typedef enum
{
	DeviceMode_NormalOperatingMode = 0,
	DeviceMode_SystemInfoMode = 1,
	DeviceMode_TestMode = 4,
} DeviceMode;

typedef enum
{
	FT5206Point_1 = 1,
	FT5206Point_2,
	FT5206Point_3,
	FT5206Point_4,
	FT5206Point_5,
} FT5206Point;

typedef enum
{
	FT5206Event_PutDown = 0,
	FT5206Event_PutUp = 1,
	FT5206Event_Contact = 2,
} FT5206Event;

typedef struct
{
	uint16_t x;
	uint16_t y;
} FT5206TouchCoordinate;

typedef struct
{
	FT5206Event Event;
	FT5206TouchCoordinate Coordinates;
} FT5206TouchEvent;

void ft5x06Init(void);
DeviceMode getDeviceMode(void);
uint32_t getNumOfTouchPoints(void);
uint32_t getTouchDataForPoint(FT5206TouchCoordinate* pCoordinate, FT5206Point Point);

#endif
