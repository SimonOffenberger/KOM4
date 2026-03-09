/**
  ******************************************************************************
  * @file    i2c.c
  * @author  Andreas Oyrer
  * @version V1.0
  * @date    10-March-2022
  * @brief   APUS Board I2C control Source File.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "Board_MCP_Temp.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
MCP9800_Temp_HandleTypeDef MCP9800_Temp_Handle={
  .i2c_handle = I2C_RTC_TEMP,
  .adr = MCP_SLAVE_ADDRESS
};

I2C_RetType MCP9800_Temp_Init(void)
{
  return I2C_Initialize(MCP9800_Temp_Handle.i2c_handle);
}

I2C_RetType MCP9800_Temp_ReadTemp(float *temp)
{
    int16_t recData;
    uint8_t  sendData = MCP_REG_TEMP;

    I2C_RetType ret = I2C_WriteRead(MCP9800_Temp_Handle.i2c_handle, MCP9800_Temp_Handle.adr, &sendData, 1, (uint8_t*)&recData, 2);
    
    if (ret != I2C_OK)
    {
        return ret;
    }
    
    *temp = (float)recData / 256;
    
    return I2C_OK;
}


