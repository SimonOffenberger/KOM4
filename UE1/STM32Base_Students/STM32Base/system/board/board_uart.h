/**
  ******************************************************************************
  * @file    board_uart.h
  * @author  Simon Offenberger
  * @version V1.0
  * @date    18-November-2025
  * @brief   APUS Board uart driver
  ******************************************************************************
  */


#ifndef __BOARD_UART_H
#define __BOARD_UART_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported types ------------------------------------------------------------*/

// Return Value for No Rx Data available
#define NO_RX_DATA -1

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
void VCOM_TX_String_blocked(const char * const str );

/**
 * @brief Receives a Character from the UART
 * @brief non blocking
 * 
 * @return int NO_RX_DATA if no data is in the receive buffer
 * @return int ASCII value of the character
 */
int VCOM_RX_Char();

/**
 * @brief Function to Initialize the USART3 peripherial inorder to communicate via a VCOM Port
 * @brief It is Configured with (2 Stop , 8 Data, 1 Stopbit) 
 */
void UART_VCOM_Init();

#endif /* __BOARD_UART_H */
