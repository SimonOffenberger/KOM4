/**
 ******************************************************************************
 * @file    Board_MCP_Temp.h
 * @author  Simon Offenberger
 * @version V1.0
 * @date    9-March-2026
 * @brief   Driver for the MCP9800 Temperature Sensor on the APUS Board.
 ******************************************************************************
 */

#ifndef __BOARD_MCP_TEMP_H
#define __BOARD_MCP_TEMP_H

/* Includes ------------------------------------------------------------------*/
#include "i2c.h"

/* Defines -----------------------------------------------------------------*/
/** @brief 7-bit MCP9800 slave address shifted for STM32 SPL I2C API. */
#define MCP9800_SLAVE_ADDRESS 0x90

/** @brief MCP9800 temperature register address. */
#define MCP_REG_TEMP 0x00
/** @brief MCP9800 configuration register address. */
#define MCP_REG_CONFIG 0x01
/** @brief MCP9800 hysteresis register address. */
#define MCP_REG_HYSTERESIS 0x02
/** @brief MCP9800 limit register address. */
#define MCP_REG_LIMIT 0x03

#define ONE_SHOT_POS 7
#define ADC_RESOLUTION_POS 5
#define FAULT_QUEUE_POS 3
#define ALERT_POLARITY_POS 2
#define ALERT_MODE_POS 1
#define SHUTDOWN_MODE_POS 0

/* Exported types ------------------------------------------------------------*/
/**
 * @brief  One-shot conversion mode.
 */
typedef enum
{
  MCP9800_ONE_SHOT_DISABLE = 0,
  MCP9800_ONE_SHOT_ENABLE = 1
} MCP9800_OneShot_Enum;

/**
 * @brief  ADC resolution selection.
 */
typedef enum
{
  MCP9800_ADC_RESOLUTION_0_5C = 0x00,
  MCP9800_ADC_RESOLUTION_0_25C = 0x01,
  MCP9800_ADC_RESOLUTION_0_125C = 0x02,
  MCP9800_ADC_RESOLUTION_0_0625C = 0x03
} MCP9800_Resolution_Enum;

/**
 * @brief  Number of consecutive fault events before alert.
 */
typedef enum
{
  MCP9800_FAULT_QUEUE_1 = 0x00,
  MCP9800_FAULT_QUEUE_2 = 0x01,
  MCP9800_FAULT_QUEUE_4 = 0x02,
  MCP9800_FAULT_QUEUE_6 = 0x03
} MCP9800_FaultQueue_Enum;

/**
 * @brief  Alert output polarity.
 */
typedef enum
{
  MCP9800_ALERT_POLARITY_ACTIVE_LOW = 0,
  MCP9800_ALERT_POLARITY_ACTIVE_HIGH = 1
} MCP9800_AlertPolarity_Enum;

/**
 * @brief  Alert mode selection.
 */
typedef enum
{
  MCP9800_COMPARATOR_MODE = 0,
  MCP9800_INTERRUPT_MODE = 1
} MCP9800_AlertMode_Enum;

/**
 * @brief  Sensor shutdown mode.
 */
typedef enum
{
  MCP9800_SHUTDOWN_MODE_DISABLE = 0,
  MCP9800_SHUTDOWN_MODE_ENABLE = 1
} MCP9800_ShutdownMode_Enum;

/**
 * @brief  MCP9800 configuration bitfield values.
 */
typedef struct
{
  MCP9800_ShutdownMode_Enum Shutdown_Mode;
  MCP9800_AlertMode_Enum Alert_Mode;
  MCP9800_AlertPolarity_Enum Alert_Polarity;
  MCP9800_FaultQueue_Enum Fault_Queue;
  MCP9800_Resolution_Enum Resolution;
  MCP9800_OneShot_Enum One_Shot;
} MCP9800_Init_StructTypeDef;

/**
 * @brief  MCP9800 driver handle.
 */
typedef struct
{
  I2C_TypeDef *i2c_handle;
  uint16_t adr;
  MCP9800_Init_StructTypeDef init;

} MCP9800_Temp_HandleTypeDef;

/* Exported functions ------------------------------------------------------- */

/**
 * @brief  Initializes the MCP9800 sensor.
 * @param  handle Pointer to MCP9800 handle structure.
 * @return I2C status.
 */
I2C_RetType MCP9800_Temp_Init(MCP9800_Temp_HandleTypeDef *handle);

/**
 * @brief  Reads the current temperature value.
 * @param  handle Pointer to MCP9800 handle structure.
 * @param  temp Pointer to destination temperature in degree Celsius.
 * @return I2C status.
 */
I2C_RetType MCP9800_Temp_ReadTemp(MCP9800_Temp_HandleTypeDef *handle, float *temp);

/**
 * @brief  Sets hysteresis temperature threshold.
 * @param  handle Pointer to MCP9800 handle structure.
 * @param  temp Hysteresis threshold in degree Celsius.
 * @return I2C status.
 */
I2C_RetType MCP9800_Temp_Set_THYST(MCP9800_Temp_HandleTypeDef *handle, float temp);

/**
 * @brief  Sets alert temperature threshold.
 * @param  handle Pointer to MCP9800 handle structure.
 * @param  temp Alert threshold in degree Celsius.
 * @return I2C status.
 */
I2C_RetType MCP9800_Temp_Set_TSET(MCP9800_Temp_HandleTypeDef *handle, float temp);

#endif
