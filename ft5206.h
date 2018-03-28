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

typedef enum
{
	FT5206Gest_MoveUp 		= 0x10,
	FT5206Gest_MoveDown 	= 0x18,
	FT5206Gest_MoveLeft 	= 0x14,
	FT5206Gest_MoveRight 	= 0x1C,
	FT5206Gest_ZoomIn 		= 0x48,
	FT5206Gest_ZoomOut 		= 0x49,
	FT5206Gest_NoGesture 	= 0
} FT5206Gest;

//typedef enum
//{
//	FT5206InterruptMode_Polling,
//	FT5206InterruptMode_Trigger,
//} FT5206InterruptMode;

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
uint8_t getGestureId(void);
uint8_t registerRead(uint8_t reg);

#endif
