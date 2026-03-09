/**
  ******************************************************************************
  * @file    board_uart_dma.h
  * @author  Simon Offenberger
  * @version V1.0
  * @date    18-November-2025
  * @brief   APUS Board uart driver
  ******************************************************************************
  */


#ifndef __BOARD_UART_DMA_H
#define __BOARD_UART_DMA_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported types ------------------------------------------------------------*/

// Define for the M1 Field in the Control Register 1
// This define is not present in the STD Periph lib
#define USART_CR1_M1 (1 << 28)

// Calculated Baudrate Register Value for a Baudrate of 115200 bd/s
#define BaudRateRegisterVal 417

/* Exported functions ------------------------------------------------------- */

/**
 * @brief Transmit a complete String over the Uart
 * 
 * @param str pointer to string which should be transmittedt
 */
void VCOM_DMA_TX_String(const char * const str );


/**
 * @brief Function to Initialize the USART3 peripherial inorder to communicate via a VCOM Port
 * @brief It is Configured with (2 Stop , 8 Data, 1 Stopbit) 
 */
void UART_DMA_VCOM_Init();

#endif /* __BOARD_UART_DMA_H */
