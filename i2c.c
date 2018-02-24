#include "i2c.h"

void i2cInit()
{
    I2C_InitTypeDef  I2C_InitStructure;
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    /* Configure I2C_EE pins: SCL and SDA */
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* I2C configuration */
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0xFF;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = 100000;

    /* I2C Peripheral Enable */
    I2C_Cmd(I2C1, ENABLE);
    /* Apply I2C configuration after enabling it */
    I2C_Init(I2C1, &I2C_InitStructure);
}

uint8_t i2cReadBytes(uint8_t slave_addr, uint8_t *data, size_t size)
{
	uint8_t tAddr = (slave_addr<<1)^1;
	uint8_t *tData = data;
	uint8_t bytes_counter = 0;
	
	//Recive	
	I2C_GenerateSTART(I2C1,ENABLE);
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	
	I2C_Send7bitAddress(I2C1, tAddr, I2C_Direction_Receiver);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	
	//Todo: DMA without cycle
	for(;tData<=data+(size-1);tData++)
	{
		while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
		*tData = I2C_ReceiveData(I2C1);
		bytes_counter++;
	}

  I2C_GenerateSTOP(I2C1,ENABLE);
  I2C_AcknowledgeConfig(I2C1,DISABLE);

  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
  I2C_ReceiveData(I2C1);
	
	return bytes_counter;
}

uint8_t i2cWriteBytes(uint8_t slave_addr, uint8_t *data, size_t size)
{
	uint8_t tAddr = (slave_addr<<1)|1;
	uint8_t *tData = data;
	uint8_t bytes_counter = 0;
	
	//Transmit
	I2C_GenerateSTART(I2C1,ENABLE);
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
	
	I2C_Send7bitAddress(I2C1, tAddr, I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	
	//Todo: DMA without cycle
	for(;tData<=data+(size-1);tData++)
	{
		I2C_SendData(I2C1,*tData);
		while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
		bytes_counter++;
	}
	
	I2C_GenerateSTOP(I2C1,ENABLE);
  while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
	return bytes_counter;
}
