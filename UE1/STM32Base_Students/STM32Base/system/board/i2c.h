/**
  ******************************************************************************
  * @file    i2c.h
  * @author  Andreas Oyrer
  * @version V1.0
  * @date    10-March-2022
  * @brief   APUS Board I2C control Header File.
  ******************************************************************************
  */

#ifndef __I2C_H
#define __I2C_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "stm32f0xx_i2c.h"

/* Exported types ------------------------------------------------------------*/
#define I2C_RTC_TEMP        I2C1

typedef enum 
{
    I2C_OK = 0,
    I2C_ERR_NACK,
    I2C_ERR_SLAVE_ADDRESS,
    I2C_ERR_INVALID_PARAM,
    I2C_ERR_TIMEOUT
} I2C_RetType;


/* Exported functions ------------------------------------------------------- */


/**
  * @brief  Initialize I2C including GPIO ports.
  * @param  None
  * @retval The result of the operation.
*/
I2C_RetType I2C_Initialize(I2C_TypeDef *I2C_Handle);

/**
  * @brief  Write data to a i2c slave device.
  * @param  slave_address: The adress of the i2c slave device.
  * @param  sendData: The data to be sent to the i2c slave device.
  * @param  bytesToSend: The number of bytes to be sent to the i2c slave device.
  * @retval The result of the operation.
*/
I2C_RetType I2C_Write(I2C_TypeDef *I2C_Handle, uint16_t slave_address, uint8_t *sendData, uint8_t bytesToSend);

/**
  * @brief  First writes data to a i2c slave device and then reads back the content.
  *         Therefore the first i2c frame is sent without a STOP bit.
  *         The read transfer starts with a repeated START condition.
  * @param  slave_address: The adress of the i2c slave device.
  * @param  sendData: The data to be sent to the i2c slave device.
  * @param  bytesToSend: The number of bytes to be sent to the i2c slave device.
  * @param  recData: A pointer to store the received data from the i2c slave device.
  * @param  bytesToRead: The number of bytes to be read from the i2c slave device.
  * @retval The result of the operation.
*/
I2C_RetType I2C_WriteRead(I2C_TypeDef *I2C_Handle, uint16_t slave_address, uint8_t *sendData, uint8_t bytesToSend, uint8_t *recData, uint8_t bytesToRead);


#endif
