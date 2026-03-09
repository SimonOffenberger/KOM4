/**
  ******************************************************************************
  * @file    timer_it.c
  * @author  Simon Offenberger
  * @version V1.0
  * @date    14-October-2025
  * @brief   APUS Board Source File for configuring a Timer for the RGB LED 
  ******************************************************************************
  */



/* Includes ------------------------------------------------------------------*/

#include "timer_it.h"
#include "stm32f0xx_tim.h"
#include "stm32f0xx.h"
#include "board_led.h"

/**
 * @brief Function that initializes the Timer Peripheral to output a PWM 
 * 
 * @param LEDBlinkFrequency Frequency of the Blink Sequence
 * @param LEDDutyCycle Duty Cycle of the Blink Sequence
 */
void Timer_IT_Initialize(const uint32_t Interrupt_frequency) {

  __enable_irq(); // Enable global interrupts

   // Turn on Clock for TIM3
   RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

  // Determine required Auto Reload value without Prescaler to realize the PWM frequ
  uint32_t CounterARR =  (SystemCoreClock / Interrupt_frequency);
  uint16_t CounterPSC = 0;
  
   // Check if the Determined ARR Value can be realized without the usage of the prescaler
  if(CounterARR > ARR_MAX_VALUE){
    // if not determin the value of the prescaler
    CounterPSC = (CounterARR / ARR_MAX_VALUE);

    // PSC + 1 because the real Prescaler is always one higher than the register value 
    // because the Prescaler is also a counter that overflows at the PSC value
    // and start at value 0 this is the reason that +1 is needed
    // -1 for the ARR value is because of the same reason but here in reverse
    CounterARR = (CounterARR / (CounterPSC + 1))-1;
  }
  
  
  // Initialize the Timer using the Functions of the STD-Peripheral LIB
  TIM_TimeBaseInitTypeDef TimInitStruct = {0};
  TimInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
  TimInitStruct.TIM_Prescaler = CounterPSC;
  TimInitStruct.TIM_Period = CounterARR;
	
	 // Initialize Timer with provided Struct
  TIM_TimeBaseInit(TIM3,&TimInitStruct);
	
	TIM3->SR &= ~TIM_SR_UIF;
	
	TIM_ITConfig(TIM3, TIM_IT_Update,ENABLE);
	
  NVIC_EnableIRQ(TIM3_IRQn);  // Enable Timer 3 Interrupt in NVIC
  NVIC_SetPriority(TIM3_IRQn, 1); // Set Priority of Timer 3 Interrupt in NVIC



//  // Enable the Timer Peripheral
//  TIM_Cmd(TIM3,ENABLE);
}


void TIM3_IRQHandler(void) {
  // Check if Update Interrupt Flag is set
  if (TIM3->SR & TIM_SR_UIF) {
    // Clear Update Interrupt Flag
    TIM3->SR &= ~TIM_SR_UIF;
    
    LED_Toggle(LED3);
  }
}
