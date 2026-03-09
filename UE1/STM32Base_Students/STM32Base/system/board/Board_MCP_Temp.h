/**
  ******************************************************************************
  * @file    BOARD_MCP_TEMP.h
  * @author  Andreas Oyrer
  * @version V1.0
  * @date    10-March-2022
  * @brief   APUS Board I2C control Header File.
  ******************************************************************************
  */

#ifndef __BOARD_MCP_TEMP_H
#define __BOARD_MCP_TEMP_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "i2c.h"

/* Exported types ------------------------------------------------------------*/
typedef struct{
  I2C_TypeDef * i2c_handle;
  const uint16_t adr;  
}MCP9800_Temp_HandleTypeDef;

extern MCP9800_Temp_HandleTypeDef MCP9800_Temp_Handle;


/* Exported functions ------------------------------------------------------- */

/* Defines -----------------------------------------------------------------*/
#define MCP_SLAVE_ADDRESS 0x90

#define MCP_REG_TEMP 0x00
#define MCP_REG_CONFIG 0x01
#define MCP_REG_HYSTERESIS 0x02
#define MCP_REG_LIMIT 0x03


#endif
