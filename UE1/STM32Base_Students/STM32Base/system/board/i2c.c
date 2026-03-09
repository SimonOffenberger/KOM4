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
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_rcc.h"
#include "i2c.h"
#include "sysdelay.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define I2C_TIMEOUT     100

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static int32_t I2C_WaitForFlagOrNackToBeSet(I2C_TypeDef *I2C_Handle, uint32_t flag);

/**
  * @brief  Initialize I2C including GPIO ports.
  * @param  None
  * @retval The result of the operation.
*/
I2C_RetType I2C_Initialize(I2C_TypeDef *I2C_Handle)
{
    I2C_InitTypeDef initStructI2C;
    GPIO_InitTypeDef initStructGPIO;

    if (I2C_Handle == I2C_RTC_TEMP) 
    {
        /* GPIO Ports Clock Enable */
        RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOBEN, ENABLE);

        GPIO_StructInit(&initStructGPIO);
    
        /* Configure GPIO pins for I2C: PB6 PB7 */
        initStructGPIO.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
        initStructGPIO.GPIO_Mode = GPIO_Mode_AF;
        GPIO_Init(GPIOB, &initStructGPIO);
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_1);
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_1);
        
        /* init I2C1 */
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
        
        I2C_StructInit(&initStructI2C);
        /* Master-Mode, 100kHz, Analog Filter Delay = On, Rise time = 140ns, Fall time = 40ns */
        /* calculated with I2C_Timing_Configuration_V1.0.1.xls */
        initStructI2C.I2C_Timing = 0x10905F86;
        I2C_Init(I2C_Handle, &initStructI2C);
        /* enable I2C */
        I2C_Cmd(I2C_Handle, ENABLE);
    }    
    
    return I2C_OK;
}

/**
  * @brief  Write data to a i2c slave device.
  * @param  slave_address: The adress of the i2c slave device.
  * @param  sendData: The data to be sent to the i2c slave device.
  * @param  bytesToSend: The number of bytes to be sent to the i2c slave device.
  * @retval The result of the operation.
*/
I2C_RetType I2C_Write(I2C_TypeDef *I2C_Handle, uint16_t slave_address, uint8_t *sendData, uint8_t bytesToSend)
{
    uint32_t idx = 0;
    int32_t ret = 0;

    /* null pointer check */
    if (sendData == 0 || bytesToSend == 0)
    {
        return I2C_ERR_INVALID_PARAM;
    }
    
    // Configure I2C and then send Start Condition
    I2C_TransferHandling(I2C_Handle,slave_address, bytesToSend, I2C_AutoEnd_Mode, I2C_Generate_Start_Write);

    ret = I2C_WaitForFlagOrNackToBeSet(I2C_Handle, I2C_FLAG_TXIS);

    if((ret & I2C_FLAG_TXIS) != I2C_FLAG_TXIS)
    {
        // got NACK or timeout
        return I2C_ERR_SLAVE_ADDRESS;
    }

    /* send data */
    for (idx = 0; idx < bytesToSend; idx++)
    {   
        // send data and wait until we get a status back
        I2C_SendData(I2C_Handle, sendData[idx]);

        // only wait on TXIS until the second to last byte 
        // Last byte does not get a TXIS!!
        // only for SoftEndMode
        if((idx+1) < (bytesToSend)){
            // wait for TXIS flag which is send when the next value can be copied to the data register
            ret = I2C_WaitForFlagOrNackToBeSet(I2C_Handle, I2C_FLAG_TXIS);
            if((ret & I2C_FLAG_TXIS) != I2C_FLAG_TXIS)
            {
                // got NACK or timeout
                return I2C_ERR_NACK;
            }
        }
    }
    
    return I2C_OK;
}

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
I2C_RetType I2C_WriteRead(I2C_TypeDef *I2C_Handle, uint16_t slave_address, uint8_t *sendData, uint8_t bytesToSend, uint8_t *recData, uint8_t bytesToRead)
{
    int32_t idx = 0;
    int32_t ret = 0;
    
    /* null pointer check */
    if (sendData == 0 || recData == 0 || bytesToRead == 0 || bytesToSend == 0)
    {
        return I2C_ERR_INVALID_PARAM;
    }
    
    // Configure I2C and then send Start Condition
    I2C_TransferHandling(I2C_Handle,slave_address, bytesToSend, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);

    ret = I2C_WaitForFlagOrNackToBeSet(I2C_Handle, I2C_FLAG_TXIS);

    if((ret & I2C_FLAG_TXIS) != I2C_FLAG_TXIS)
    {
        // got NACK or timeout
        return I2C_ERR_SLAVE_ADDRESS;
    }

    /* send data */
    for (idx = 0; idx < bytesToSend; idx++)
    {   
        // send data and wait until we get a status back
        I2C_SendData(I2C_Handle, sendData[idx]);

        // for Last Byte
        if((idx + 1) == bytesToSend){
            // at last byte Wait for TC Flag
            ret = I2C_WaitForFlagOrNackToBeSet(I2C_Handle, I2C_FLAG_TC);
            if((ret & I2C_FLAG_TC) != I2C_FLAG_TC)
            {
                // got NACK or timeout
                return I2C_ERR_NACK;
            }
        } 
        // only wait on TXIS until the second to last byte 
        // Last byte does not get a TXIS!!
        else{
            // wait for TXIS flag which is send when the next value can be copied to the data register
            ret = I2C_WaitForFlagOrNackToBeSet(I2C_Handle, I2C_FLAG_TXIS);
            if((ret & I2C_FLAG_TXIS) != I2C_FLAG_TXIS)
            {
                // got NACK or timeout
                return I2C_ERR_NACK;
            }
        }
    }

    // Configure I2C and then send Start Condition for Reading
    I2C_TransferHandling(I2C_Handle,slave_address, bytesToRead, I2C_AutoEnd_Mode, I2C_Generate_Start_Read);
    
    /* receive data */
    for (idx = 0; idx < bytesToRead; idx++)
    {   
        // for Reading wait for RXNE Flag
        ret = I2C_WaitForFlagOrNackToBeSet(I2C_Handle, I2C_FLAG_RXNE);

        if((ret & I2C_FLAG_RXNE) != I2C_FLAG_RXNE)
        {
            // got NACK or timeout
            return I2C_ERR_NACK;
        }
        else{            
            // received data 
            recData[idx] = I2C_ReceiveData(I2C_Handle);
        }
    }

    return I2C_OK;
}

/* Private functions ---------------------------------------------------------*/
static int32_t I2C_WaitForFlagOrNackToBeSet(I2C_TypeDef *I2C_Handle, uint32_t flag)
{
    uint32_t initialTicks = 0;
    uint32_t isrState = 0;
    
    initialTicks = SysDelay_GetTicks();
    
    /* wait until the given flag gets set or we have a timeout or a NACK*/
    do 
    {
        isrState = I2C_ReadRegister(I2C_Handle, I2C_Register_ISR);
        /* check for a timeout */
        if ((SysDelay_GetTicks() - initialTicks) > I2C_TIMEOUT) 
        {
            return 0x00;
        }
        if ((isrState & I2C_FLAG_NACKF) == I2C_FLAG_NACKF)
        {
            return I2C_FLAG_NACKF;
        }
    } while ((isrState & flag) != flag);
    
    return isrState & flag;
}
