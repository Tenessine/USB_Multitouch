#include "ft5206.h"
#include "i2c.h"
#include "system.h"

/* Private defines -----------------------------------------------------------*/
#define FT5206_REGISTER_DEVICE_MODE 	(0x00)
#define FT5206_REGISTER_GEST_ID 			(0x01)
#define FT5206_REGISTER_TD_STATUS 		(0x02)

#define FT5206_REGISTER_TOUCH1_XH 		(0x03)
#define FT5206_REGISTER_TOUCH1_XL 		(0x04)
#define FT5206_REGISTER_TOUCH1_YH 		(0x05)
#define FT5206_REGISTER_TOUCH1_YL 		(0x06)

#define FT5206_REGISTER_TOUCH2_XH 		(0x09)
#define FT5206_REGISTER_TOUCH2_XL 		(0x0A)
#define FT5206_REGISTER_TOUCH2_YH 		(0x0B)
#define FT5206_REGISTER_TOUCH2_YL 		(0x0C)

#define FT5206_REGISTER_TOUCH3_XH 		(0x0F)
#define FT5206_REGISTER_TOUCH3_XL 		(0x10)
#define FT5206_REGISTER_TOUCH3_YH 		(0x11)
#define FT5206_REGISTER_TOUCH3_YL 		(0x12)

#define FT5206_REGISTER_TOUCH4_XH 		(0x15)
#define FT5206_REGISTER_TOUCH4_XL 		(0x16)
#define FT5206_REGISTER_TOUCH4_YH 		(0x17)
#define FT5206_REGISTER_TOUCH4_YL 		(0x18)
#define FT5206_REGISTER_TOUCH5_XH 		(0x1B)
#define FT5206_REGISTER_TOUCH5_XL 		(0x1C)

#define FT5206_REGISTER_TOUCH5_YH 		(0x1D)
#define FT5206_REGISTER_TOUCH5_YL 		(0x1E)

#define FT5206_REGISTER_ID_G_PERIODACTIVE (0x88)	/* The period of active status, value 3 to 14 (defualt 12) */
#define FT5206_REGISTER_ID_G_CIPHER				(0xA3)  /* Chip vendor ID */
#define FT5206_REGISTER_ID_G_MODE					(0xA4)	/* Interrupt mode */
#define FT5206_REGISTER_ID_G_FT5201ID			(0xA8)	/* CTPM vendor ID */

#define FT5206_ADDRESS				(0x38)
#define FT5206_CHIP_VENDOR_ID	(0x55)
#define FT5206_CTPM_VENDOR_ID	(0x86)

#define FT5206_ID_G_THGROUP 		0x80//valid touching detect threshold [R/W]
#define FT5206_ID_G_THPEAK 			0x81//valid touching peak detect threshold [R/W]
#define FT5206_ID_G_THCAL 			0x82//the threshold when calculating the focus of touching[R/W]
#define FT5206_ID_G_THDIFF	 		0x85//the threshold where the coordinatis differs from orig[R/W]
#define FT5206_ID_G_CLTR	 			0x86//power control mode[R/W]

#define I2C (I2C1)		/*i2c peritherial address*/
#define GPIO_RST 		GPIOB

/* Private typedefs ----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
typedef __packed struct
{
	uint8_t msb;
	uint8_t lsb;
}bytePair;
const uint8_t bytePairElemCount = sizeof(bytePair)/sizeof(uint8_t);

typedef __packed struct
{
	bytePair first;
	bytePair second;
}valuePair;
const uint8_t valuePairElemCount = sizeof(valuePair)/sizeof(bytePair);

static const bytePair xTouchPointsRegistersPairs[5] = {
		{FT5206_REGISTER_TOUCH1_XH,FT5206_REGISTER_TOUCH1_XL},
		{FT5206_REGISTER_TOUCH2_XH,FT5206_REGISTER_TOUCH2_XL},
		{FT5206_REGISTER_TOUCH3_XH,FT5206_REGISTER_TOUCH3_XL},
		{FT5206_REGISTER_TOUCH4_XH,FT5206_REGISTER_TOUCH4_XL},
		{FT5206_REGISTER_TOUCH5_XH,FT5206_REGISTER_TOUCH5_XL}
};

static const bytePair yTouchPointsRegistersPairs[5] = {
		{FT5206_REGISTER_TOUCH1_YH,FT5206_REGISTER_TOUCH1_YL},
		{FT5206_REGISTER_TOUCH2_YH,FT5206_REGISTER_TOUCH2_YL},
		{FT5206_REGISTER_TOUCH3_YH,FT5206_REGISTER_TOUCH3_YL},
		{FT5206_REGISTER_TOUCH4_YH,FT5206_REGISTER_TOUCH4_YL},
		{FT5206_REGISTER_TOUCH5_YH,FT5206_REGISTER_TOUCH5_YL}
};

void ft5x06Init()
{
	//Switch RST line to VCC
	GPIO_RST->ODR |= 1<<5;
	
	//Reset ft5206
	//Pull down
	GPIO_RST->ODR ^= 1<<5;
	//Wait
	//Todo: here need timer
	//for(int i=0;i<10000;i++){__ASM("nop");}
	timDelay(50);
	
	//Pull-up
	GPIO_RST->ODR |= 1<<5;
	//Todo: After reset 
	//Need wait here.
}

uint8_t registerRead(uint8_t reg)
{
	uint8_t data = 0;
	//Set data address
	i2cWriteBytes((uint8_t)FT5206_ADDRESS,&reg,sizeof(data));
	i2cReadBytes((uint8_t)FT5206_ADDRESS,&data,sizeof(data));
	return data;
}

DeviceMode getDeviceMode()
{
	uint8_t data = FT5206_REGISTER_DEVICE_MODE;
	i2cWriteBytes((uint8_t)FT5206_ADDRESS,&data,sizeof(data));
	data = 0;
	i2cReadBytes((uint8_t)FT5206_ADDRESS,&data,sizeof(data));
	return  (DeviceMode)data;
}

uint32_t getNumOfTouchPoints()
{
	uint8_t data = FT5206_REGISTER_TD_STATUS;
	i2cWriteBytes((uint8_t)FT5206_ADDRESS,&data,sizeof(data));
	data = 0;
	i2cReadBytes((uint8_t)FT5206_ADDRESS,&data,sizeof(data));
	return data;
}

uint32_t getTouchDataForPoint(FT5206TouchCoordinate* pCoordinate, FT5206Point Point)
{
	valuePair pair;
	pair.first = xTouchPointsRegistersPairs[Point-1];
	pair.second = yTouchPointsRegistersPairs[Point-1];
	
	for(bytePair *bPair = &(pair.first);bPair<((&(pair.first))+sizeof(valuePair));bPair+=sizeof(bytePair))
	{	
		uint8_t* s = 	(uint8_t*)(bPair);
		uint8_t* e = 	(uint8_t*)(bPair+sizeof(bytePair));

		for(uint8_t* bytePtr = s; bytePtr<e; bytePtr++)
		{	
			i2cWriteBytes((uint8_t)FT5206_ADDRESS,bytePtr,1);
			i2cReadBytes((uint8_t)FT5206_ADDRESS,bytePtr,1);	
		}
	}
	pCoordinate->x = ((pair.first.msb & 0x0F) << 8) | pair.first.lsb;
	pCoordinate->y = ((pair.second.msb & 0x0F) << 8) | pair.second.lsb;

	return (uint32_t)(pair.first.msb &0xC0);
}

uint8_t getGestureId()
{
	return registerRead((uint8_t)FT5206_REGISTER_GEST_ID);
}

