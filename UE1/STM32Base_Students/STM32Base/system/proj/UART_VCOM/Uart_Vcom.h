/**
  ******************************************************************************
  * @file    Uart_Vcom.c
  * @author  Simon Offenberger
  * @version V1.0
  * @date    18-November-2025
  * @brief   Project File for the Exercise 6 in MPT3
  ******************************************************************************
  */

#ifndef __MAIN_VCOM_H
#define __MAIN_VCOM_H

#include <stdint.h>
// Makro for Rising Edge detection
#define rising_edge(state,bit,prevstate) ((state & bit)&&(!(prevstate&bit)))

#define ms *1

// Delay of the while(1) loop -> for debouncing
#define LOOP_DELAY 1 ms

// Command Charactor for toggling the leds
#define C_Toggle_LED4 '0'
#define C_Toggle_LED5 '1'
#define C_Toggle_LED3 'W'

// Comands that are sent if a button is pressed
#define CMD_BUTTON0     "0\0"
#define CMD_BUTTON1     "1\0"
#define CMD_BUTTON_WKUP "W\0"

/**
 * @brief Project for interfacing with the UART Periph
 * 
 */
void main_Uart_VCOM(void);

#endif /* __MAIN_VCOM_H */
