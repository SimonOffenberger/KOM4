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
#define I2C_RTC_TEMP I2C1

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
 * @param  I2C_Handle Pointer to I2C peripheral instance.
 * @retval The result of the operation.
 */
I2C_RetType I2C_Initialize(I2C_TypeDef *I2C_Handle);

/**
 * @brief  Write data to a i2c slave device.
 * @param  I2C_Handle Pointer to I2C peripheral instance.
 * @param  slave_address Address of the I2C slave device.
 * @param  sendData Data to be sent to the I2C slave device.
 * @param  bytesToSend Number of bytes to send to the I2C slave device.
 * @retval The result of the operation.
 */
I2C_RetType I2C_Write(I2C_TypeDef *I2C_Handle, const uint16_t slave_address, const uint8_t *sendData, const uint8_t bytesToSend);

/**
 * @brief  First writes data to a i2c slave device and then reads back the content.
 *         Therefore the first i2c frame is sent without a STOP bit.
 *         The read transfer starts with a repeated START condition.
 * @param  I2C_Handle Pointer to I2C peripheral instance.
 * @param  slave_address Address of the I2C slave device.
 * @param  sendData Data to be sent to the I2C slave device.
 * @param  bytesToSend Number of bytes to send to the I2C slave device.
 * @param  recData Pointer to store received data from the I2C slave device.
 * @param  bytesToRead Number of bytes to read from the I2C slave device.
 * @retval The result of the operation.
 */
I2C_RetType I2C_WriteRead(I2C_TypeDef *I2C_Handle, const uint16_t slave_address, const uint8_t *sendData, const uint8_t bytesToSend, uint8_t *recData, const uint8_t bytesToRead);

#endif
