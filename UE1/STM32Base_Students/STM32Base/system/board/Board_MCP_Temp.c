/**
 ******************************************************************************
 * @file    Board_MCP_Temp.c
 * @author  Simon Offenberger
 * @version V1.0
 * @date    9-March-2026
 * @brief   Driver for the MCP9800 Temperature Sensor on the APUS Board.
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "board_MCP_temp.h"

/* Private define ------------------------------------------------------------*/
#define INT_FLOAT_CONV 256.0f
#define ONLY_MSB_BIT 0x80
#define NUM_OF_TEMP_BYTES 2

/**
 * @brief  Initializes the MCP9800 sensor and writes the configuration register.
 * @param  handle Pointer to MCP9800 handle structure.
 * @return I2C status.
 */
I2C_RetType MCP9800_Temp_Init(MCP9800_Temp_HandleTypeDef *handle)
{

  if (handle == 0)
  {
    return I2C_ERR_INVALID_PARAM;
  }

  // Build MCP9800 config register from user-selected bitfields.
  const uint8_t config_reg = ((uint8_t)handle->init.Shutdown_Mode << SHUTDOWN_MODE_POS) |
                             ((uint8_t)handle->init.Alert_Mode << ALERT_MODE_POS) |
                             ((uint8_t)handle->init.Alert_Polarity << ALERT_POLARITY_POS) |
                             ((uint8_t)handle->init.Fault_Queue << FAULT_QUEUE_POS) |
                             ((uint8_t)handle->init.Resolution << ADC_RESOLUTION_POS) |
                             ((uint8_t)handle->init.One_Shot << ONE_SHOT_POS);

  I2C_RetType retval = I2C_Initialize(handle->i2c_handle);

  if (retval != I2C_OK)
  {
    return retval;
  }

  const uint8_t sendData[2] = {MCP_REG_CONFIG, config_reg};

  retval = I2C_Write(handle->i2c_handle, handle->adr, sendData, sizeof(sendData) / sizeof(sendData[0]));

  return retval;
}

/**
 * @brief  Reads the current temperature from MCP9800.
 * @param  handle Pointer to MCP9800 handle structure.
 * @param  temp Pointer to destination temperature in degree Celsius.
 * @return I2C status.
 */
I2C_RetType MCP9800_Temp_ReadTemp(MCP9800_Temp_HandleTypeDef *handle, float *temp)
{
  if (handle == 0 || temp == 0)
  {
    return I2C_ERR_INVALID_PARAM;
  }
  uint8_t recData[NUM_OF_TEMP_BYTES];
  uint8_t sendData = MCP_REG_TEMP;

  // Write temperature register address, then read back 2 bytes with repeated START.
  I2C_RetType ret = I2C_WriteRead(handle->i2c_handle, handle->adr, &sendData, 1, (uint8_t *)&recData, sizeof(recData) / sizeof(recData[0]));

  if (ret != I2C_OK)
  {
    return ret;
  }

  // MCP9800 temperature format is Q8.8, convert raw value to degree Celsius.
  *temp = ((float)(recData[0] << 8 | recData[1])) / INT_FLOAT_CONV;

  return ret;
}

/**
 * @brief  Sets the MCP9800 overtemperature limit (TSET).
 * @param  handle Pointer to MCP9800 handle structure.
 * @param  temp Temperature limit in degree Celsius.
 * @return I2C status.
 */
I2C_RetType MCP9800_Temp_Set_TSET(MCP9800_Temp_HandleTypeDef *handle, float temp)
{

  if (handle == 0 || temp == 0)
  {
    return I2C_ERR_INVALID_PARAM;
  }

  uint16_t temp_int = (uint16_t)(temp * INT_FLOAT_CONV);
  // THYST/TSET use MSB plus bit7 of LSB according to MCP9800 register format.
  const uint8_t sendData[3] = {MCP_REG_LIMIT, (temp_int >> 8) & 0xFF, temp_int & ONLY_MSB_BIT};

  return I2C_Write(handle->i2c_handle, handle->adr, sendData, sizeof(sendData) / sizeof(sendData[0]));
}

/**
 * @brief  Sets the MCP9800 hysteresis temperature threshold (THYST).
 * @param  handle Pointer to MCP9800 handle structure.
 * @param  temp Hysteresis temperature in degree Celsius.
 * @return I2C status.
 */
I2C_RetType MCP9800_Temp_Set_THYST(MCP9800_Temp_HandleTypeDef *handle, float temp)
{
  if (handle == 0 || temp == 0)
  {
    return I2C_ERR_INVALID_PARAM;
  }

  uint16_t temp_int = (uint16_t)(temp * INT_FLOAT_CONV);
  // THYST/TSET use MSB plus bit7 of LSB according to MCP9800 register format.
  const uint8_t sendData[3] = {MCP_REG_HYSTERESIS, (temp_int >> 8) & 0xFF, temp_int & ONLY_MSB_BIT};

  return I2C_Write(handle->i2c_handle, handle->adr, sendData, sizeof(sendData) / sizeof(sendData[0]));
}
