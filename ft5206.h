#ifndef __FT5206
#define __FT5206

#include "stdint.h"

/* Defines -------------------------------------------------------------------*/
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

uint32_t getNumOfTouchPoints(void);
uint32_t getTouchDataForPoint(/*FT5206Event* pEvent,*/FT5206TouchCoordinate* pCoordinate, FT5206Point Point);

#endif
