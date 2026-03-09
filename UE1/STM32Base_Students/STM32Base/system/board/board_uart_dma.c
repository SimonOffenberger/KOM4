/**
  ******************************************************************************
  * @file    board_uart_dma.c
  * @author  Simon Offenberger
  * @version V1.0
  * @date    18-November-2025
  * @brief   APUS Board uart driver
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_usart.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_dma.h"
#include "board_uart_dma.h"
#include <string.h>


/**
 * @brief Transmit a complete String over the Uart
 * 
 * @param str pointer to string which should be transmittedt
 */
void VCOM_DMA_TX_String(const char * const str ){

  // wait until previous transfer is complete to not overwrite the DMA settings
  // Flag is cleared in the IRQ Handler
  while((DMA1->ISR & DMA_ISR_TCIF7)==0 && (DMA1_Channel7->CCR & DMA_CCR_EN)==1); // wait until previous transfer is complete

  // nullptr check
  if(str == 0) return;

  // Get the length of the string using strlen
  uint32_t len = strlen(str);

  if(len == 0) return; // nothing to send
  
  // Disable Channel before Configuration
  DMA_Cmd(DMA1_Channel7, DISABLE); 

  // Configure the DMA with the String Address and Length
  DMA1_Channel7->CMAR = (uint32_t)str;

  // Set the Number of Data to Transfer
  DMA1_Channel7->CNDTR = len;

  // Enable the DMA Channel
  DMA_Cmd(DMA1_Channel7, ENABLE);

}



/* GPIO Pin identifier */
typedef struct _GPIO_PIN 
{
  GPIO_TypeDef *port;
  uint16_t      pin;
  uint8_t       AF_PINSOURCE;
  uint8_t       AF;
} GPIO_PIN;


/* VCOM GPIO Pins */
static const GPIO_PIN VCOM_PIN[] = 
{
    { GPIOD, GPIO_Pin_9 , GPIO_PinSource9, GPIO_AF_0  },	// User Button 0
    { GPIOD, GPIO_Pin_8,  GPIO_PinSource8, GPIO_AF_0  },	// User Button 1
};


/**
  * @brief  Initialize The UART Pins for Alternate Function.
  * @param  None
  * @retval None
*/
static void VCOM_PIN_Initialize(void) 
{
    GPIO_InitTypeDef initStruct;
    uint32_t idx = 0;

    /* GPIO Ports Clock Enable */
    RCC->AHBENR |= RCC_AHBENR_GPIODEN;	// Clock for User Button
    /* initialize the GPIO struct */
    GPIO_StructInit(&initStruct);
    
    // Initialize all the Pins for the UART Periph
    for (idx = 0; idx < sizeof(VCOM_PIN) / sizeof(GPIO_PIN); idx++)
    {
      /* Configure GPIO pin */
      initStruct.GPIO_Pin = VCOM_PIN[idx].pin;
      initStruct.GPIO_Mode = GPIO_Mode_AF;
      initStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
      initStruct.GPIO_OType = GPIO_OType_PP; // <= will be ignored for RX Pin!
      GPIO_Init(VCOM_PIN[idx].port, &initStruct);

      // Set the Alternate Function Source for each Pin
      GPIO_PinAFConfig(VCOM_PIN[idx].port,VCOM_PIN[idx].AF_PINSOURCE,VCOM_PIN[idx].AF);
    }

}

/**
 * @brief Initialize the DMA for UART3 TX
 * 
 */
static void DMA_UART3_Init(void){
  // Enable Clock for DMA1
  RCC_AHBPeriphClockCmd(RCC_AHBENR_DMA1EN, ENABLE);


  // Disable Channel before Configuration
  DMA_Cmd(DMA1_Channel7, DISABLE); 
  // Configure DMA for UART3 TX
  DMA_InitTypeDef DMA_InitStruct = {0};
  DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
  DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&(USART3->TDR);
  DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;
  // Note: Buffer size and MemoryBaseAddr will be set in the TX function
  DMA_Init(DMA1_Channel7, &DMA_InitStruct);
  // Clear the Interrupt Flag
  DMA1->IFCR = DMA_IFCR_CTCIF7;
  
  // alternativ konfiguration via register
//  // Configure DMA for UART3 TX using Registers directly
//  // Disable DMA Channel set Memory to Peripheral, No Circular, No Peripheral Increment, No Memory Increment, Byte Size
//  DMA1_Channel1->CCR &= ~ (DMA_CCR_EN);
//  DMA1_Channel1->CCR &= ~ (DMA_CCR_MEM2MEM | DMA_CCR_PL | DMA_CCR_MSIZE | DMA_CCR_PSIZE | DMA_CCR_PINC | DMA_CCR_CIRC);
//  // Peripheral to Memory, Memory Increment, Transfer Complete Interrupt Enable
//  DMA1_Channel7->CCR = DMA_CCR_DIR | DMA_CCR_MINC | DMA_CCR_TCIE; 
//  DMA1_Channel7->CPAR = (uint32_t)&(USART3->TDR); // Peripheral Address
 
 // Enable DMA Interrupt for Transfer Complete
 DMA_ITConfig(DMA1_Channel7, DMA_IT_TC, ENABLE);
 NVIC_EnableIRQ(DMA1_Channel4_5_6_7_IRQn);

}


/**
 * @brief Function to Initialize the USART3 peripherial inorder to communicate via a VCOM Port
 * @brief It is Configured with (2 Stop , 8 Data, 1 Stopbit) 
 */
void UART_DMA_VCOM_Init(){
  // Initialize The UART Pins for the VCOM
  VCOM_PIN_Initialize();
	DMA_UART3_Init();

  // Enable Clock for the UART Peripheral
  RCC_APB1PeriphClockCmd(RCC_APB1ENR_USART3EN, ENABLE);

  // Configure using Registers instead of the comfortable STD-Periph Lib.

  // Clear Bit M1 and M0 in Control Register -> 1 Start Bit u 8 Data Bits
  // Clear PCE Bit -> No Parity
  USART3->CR1 &= ~(USART_CR1_M1 | USART_CR1_M | USART_CR1_PCE | USART_CR1_OVER8);

  // Enable the Transmitter and Enable Receiver
  USART3->CR1 |= (USART_CR1_TE | USART_CR1_RE);

  // Set to 2 Stop Bits
  USART3-> CR2 &= ~(USART_CR2_STOP);
  USART3->CR2 |= (USART_CR2_STOP_1);
  
  // Disable Overrun error
  // If data is in RDR and new Data is Received
  // the old data is overwritten
  // be aware if you test the programm und send 
  // 1\r\n than 1 will be overwritten by \r
  // and \r by \n !
  // If this feature is not set than the uart enters an 
  // error state on overrun!
  USART3->CR3 |= USART_CR3_OVRDIS;
  // Enable DMA for Transmitter
  USART3->CR3 |= USART_CR3_DMAT;
  
  // Set Baudrate
  USART3->BRR = BaudRateRegisterVal;

  // Enable USART
  USART3->CR1 |= USART_CR1_UE;
}


