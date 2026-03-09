/**
  ******************************************************************************
  * @file    3bit_cnt.c
  * @author  Simon Offenberger
  * @version V1.0
  * @date    14-October-2017
  * @brief   APUS Board 3Bit CNT source File.
  ******************************************************************************
  */

#include <stdbool.h>
#include "stm32f0xx_gpio.h"
#include "board_led.h"
#include "board_button.h"
#include "alarm.h"
#include "sysdelay.h"


void change_sysclock_to_24Mhz(void){
  
    // Switch to HSE for configuration of the PLL Mul
    RCC->CFGR &= ~ RCC_CFGR_SW;
    RCC->CFGR |=   RCC_CFGR_SW_HSE;
  
    /* Wait till HSE is ready */
    while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSE);
  
    /* Disable PLL */
    RCC->CR &= ~RCC_CR_PLLON;
  
    /* PLL configuration */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLMULL));
    RCC->CFGR |= (uint32_t)( RCC_CFGR_PLLMULL3);
  
    /* Enable PLL */
    RCC->CR |= RCC_CR_PLLON;
  
      /* Wait till PLL is ready */
    while((RCC->CR & RCC_CR_PLLRDY) == 0)
    {
    }

    /* Select PLL as system clock source */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;    

    /* Wait till PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)RCC_CFGR_SWS_PLL)
    {
    }
}

// Makro to set the current state of the statemachine
#define SET_STATE(x)  state = x;

/**
  * @brief  Implimentation of the 3bit counter which
  * @brief  displays the number on the LEDs
  * @param  None
  * @retval None
  */
void main_alarm(void){
  
  // Initialize the LEDS and Button
  LED_Initialize();
  Button_Initialize();
  
  // Set initial State to Alarm_OFF
  uint8_t state = STATE_ALARM_OFF;
  // status flag indication if the clock was changed
  bool sys_clock_changed = false;
  
  while(1){
  // read the current states of the buttons
  const uint32_t button_states = Button_GetState();
  static uint32_t led_states=0;
    
    // State Machine
    switch(state){
      
      // Current State is STATE_ALARM_OFF
      case(STATE_ALARM_OFF):{
        
        // if User Button0 is pressed activate LED4 -> indication an Alarm in Zone 1
        if(button_states & BUTTON_USER0_MASK){
          // Switch to STATE Alarm ON
          SET_STATE(STATE_ALARM_ON);
          // Turn on LED
          led_states |= LED4_MASK;
        }
        
        // if User Button1 is pressed activate LED5 -> indication an Alarm in Zone 2
        if(button_states & BUTTON_USER1_MASK){
          // Switch to Alarm on
          SET_STATE(STATE_ALARM_ON);
          // Turn on LED
          led_states |= LED5_MASK;
        }
        
        break;
      }
      
      // Current State is STATE_ALARM_ON
      case(STATE_ALARM_ON):{
        
        // Toggle LED 3
        if(led_states & LED3_MASK) led_states &=  ~LED3_MASK;
        else led_states |= LED3_MASK;

        // Turn of Alarm with the Wakeup Button
        if(button_states & BUTTON_WAKEUP_MASK){
          // Set State to Off
          SET_STATE(STATE_ALARM_OFF);
          // turn of all the LEDs
          led_states &= ~(LED3_MASK|LED4_MASK|LED5_MASK);
          
          // switch to 24Mhz Sysclock only once
          if(!sys_clock_changed){
            change_sysclock_to_24Mhz();
            sys_clock_changed=true;
          }
        }
        
        break;
      }
    }
  
    // set the leds to the desired state indicated by led_states
    LED_SetOut(led_states);
    
    // delay the while 1 inorder to see the blinking of the alarm led
    SysDelay_Delay(100);
  }
}