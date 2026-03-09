/**
  ******************************************************************************
  * @file    Uart_Vcom.c
  * @author  Simon Offenberger
  * @version V1.0
  * @date    18-November-2025
  * @brief   Project File for the Exercise 6 in MPT3
  ******************************************************************************
  */
 
#include "board_uart.h"
#include "board_led.h"
#include "board_button.h"
#include "Uart_Vcom.h"
#include "sysdelay.h"

void main_Uart_VCOM(){

  // Initialize Peripheral
  UART_VCOM_Init();
  LED_Initialize();
  Button_Initialize();
  
  // Send Hello text right after init
  VCOM_TX_String_blocked("MPT3 Hello World from VCOM APUS Board \n");

  while(1){

    static uint32_t prev_button_State = 0;
    // read the current button states
    const uint32_t button_States = Button_GetState();

    // read Data if there is data in the USART
    const int c = VCOM_RX_Char();

    // if data is read
    if(c!=NO_RX_DATA){
      switch(c){
        case C_Toggle_LED4: LED_Toggle(LED4); break;
        case C_Toggle_LED5: LED_Toggle(LED5); break;
        case C_Toggle_LED3: LED_Toggle(LED3); break;
      }
    }

    // Send the corresponding Command if a button is pressed
    if(rising_edge(button_States,BUTTON_USER0_MASK,prev_button_State)){
      VCOM_TX_String_blocked(CMD_BUTTON0);
    }
    if(rising_edge(button_States,BUTTON_USER1_MASK,prev_button_State)){
      VCOM_TX_String_blocked(CMD_BUTTON1);
    }
    if(rising_edge(button_States,BUTTON_WAKEUP_MASK,prev_button_State)){
      VCOM_TX_String_blocked(CMD_BUTTON_WKUP);
    }

    // store prev state for edge detection
    prev_button_State = button_States;

    // delay for edge detection -> to supress button bouncing
    SysDelay_Delay(LOOP_DELAY);
  }
} 