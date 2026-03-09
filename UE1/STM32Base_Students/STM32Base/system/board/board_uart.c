/**
  ******************************************************************************
  * @file    board_uart.c
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
#include "board_uart.h"


/**
 * @brief Function for transmitting a char over UART
 * @brief in blocked mode
 * 
 * @param c character to be transmitted over the UART
 */
static void VCOM_TX_Char_blocked(const char c){
  
  // Wait till the Transmit Shift Register is empty
  while(! (USART3->ISR & USART_ISR_TXE));
  
  // copy the value to the Transmit Register
  USART3->TDR = c;
  
}

/**
 * @brief Transmit a complete String over the Uart
 * 
 * @param str pointer to string which should be transmittedt
 */
void VCOM_TX_String_blocked(const char * const str ){

  // nullptr check
  if(str == 0) return;

  // Transmit all characters in string till Zero Terminator is read.
  for(int i =0; str[i] != '\0'; i++){

    // Transmit Char
    VCOM_TX_Char_blocked(str[i]);
  }

}

/**
 * @brief Receives a Character from the UART
 * @brief non blocking
 * 
 * @return int NO_RX_DATA if no data is in the receive buffer
 * @return int ASCII value of the character
 */
int VCOM_RX_Char(){
  int c = NO_RX_DATA;

  // if Receive Register not empty get Byte
  if(USART3->ISR & USART_ISR_RXNE){

    // get Data from the Receive Buffer
    c = (USART3->RDR & USART_RDR_RDR);

    // Normally the RXNE Bit is clear by reading the RDR
    // But it can also be cleared with the RXFRQ bit in RQR 
    USART3->RQR |= USART_RQR_RXFRQ;
  }
  
  return c;
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
 * @brief Function to Initialize the USART3 peripherial inorder to communicate via a VCOM Port
 * @brief It is Configured with (2 Stop , 8 Data, 1 Stopbit) 
 */
void UART_VCOM_Init(){
  // Initialize The UART Pins for the VCOM
  VCOM_PIN_Initialize();

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

  // Set Baudrate
  USART3->BRR = BaudRateRegisterVal;

  // Enable USART
  USART3->CR1 |= USART_CR1_UE;
}


