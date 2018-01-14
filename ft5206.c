#include "ft5206.h"

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
#define FT5206_REGISTER_TOUCH5_XH 		(0x1B)

#define FT5206_REGISTER_ID_G_PERIODACTIVE (0x88)	/* The period of active status, value 3 to 14 (defualt 12) */
#define FT5206_REGISTER_ID_G_CIPHER				(0xA3)  /* Chip vendor ID */
#define FT5206_REGISTER_ID_G_MODE					(0xA4)	/* Interrupt mode */
#define FT5206_REGISTER_ID_G_FT5201ID			(0xA8)	/* CTPM vendor ID */

#define FT5206_ADDRESS				(0x38)
#define FT5206_CHIP_VENDOR_ID	(0x55)
#define FT5206_CTPM_VENDOR_ID	(0x86)


uint32_t FT5206_GetNumOfTouchPoints()
{
	return 0;
}
