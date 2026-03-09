/**
  ******************************************************************************
  * @file    rgb_timer.c
  * @author  Simon Offenberger
  * @version V1.0
  * @date    14-October-2025
  * @brief   APUS Board Source File for configuring a Timer for the RGB LED 
  ******************************************************************************
  */



/* Includes ------------------------------------------------------------------*/

#include "rgb_timer.h"
#include "stm32f0xx_tim.h"
#include "stm32f0xx.h"


/**
 * @brief Function that changes the timer settings of the Timer to the RGB LED
 * 
 * @param LEDBlinkFrequency Frequency of the Blink Sequence
 * @param LEDDutyCycle DutyCycle of the PWM
 */
void RGB_Timer_ChangeInterval(const uint32_t LEDBlinkFrequency, const uint16_t LEDDutyCycle) {
  
  // subtract 1 because the counter counts from 0 to ARR value
  uint32_t CounterARR =  (SystemCoreClock / LEDBlinkFrequency);
  uint16_t CounterPSC = 0;
  
  // determine if a presacaler is needed
  if(CounterARR > ARR_MAX_VALUE){
    // determin the Prescaler 
    CounterPSC = (CounterARR / ARR_MAX_VALUE);

    // PSC + 1 because the real Prescaler is always one higher than the register value 
    // because the Prescaler is also a counter that overflows at the PSC value
    // and start at value 0 this is the reason that +1 is needed
    // -1 for the ARR value is because of the same reason but here in reverse
    CounterARR = (CounterARR / (CounterPSC +1))-1;
  }
  
  // determine the CCR value depending on the duty cycle
  const uint16_t CounterCRR =  ((CounterARR+1) * LEDDutyCycle)/MAX_DUTY_CYCLE;
  
  // Update the Prescaler, the ARR and the CCR register
  // Inorder to change the timer frequency and duty cycle
  TIM3->PSC = CounterPSC;
  TIM3->ARR = CounterARR;
  TIM3->CCR3 = CounterCRR;

  // Set the UG bit to update the Prescaler and the ARR register
  // UG bit generates an update event to reload the Prescaler and the ARR value
  TIM3->EGR |= TIM_EGR_UG;
}

/**
 * @brief Function that initializes the Timer Peripheral to output a PWM 
 * 
 * @param LEDBlinkFrequency Frequency of the Blink Sequence
 * @param LEDDutyCycle Duty Cycle of the Blink Sequence
 */
void RGB_Timer_Initialize(const uint32_t LEDBlinkFrequency, const uint16_t LEDDutyCycle) {

   // Turn on Clock for TIM3
   RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

  // Determine required Auto Reload value without Prescaler to realize the PWM frequ
  uint32_t CounterARR =  (SystemCoreClock / LEDBlinkFrequency);
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
  
  // Determine the Compare Value for creating the required duty cycle 
  const uint16_t CounterCRR =  ((CounterARR+1) * LEDDutyCycle)/MAX_DUTY_CYCLE;
  
  // Initialize the Timer using the Functions of the STD-Peripheral LIB
  TIM_TimeBaseInitTypeDef TimInitStruct = {0};
  TimInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
  TimInitStruct.TIM_Prescaler = CounterPSC;
  TimInitStruct.TIM_Period = CounterARR;
  
  // Initialize Timer with provided Struct
  TIM_TimeBaseInit(TIM3,&TimInitStruct);
  
  // Initialize the Output Compare Part of the Timer
  TIM_OCInitTypeDef TimOCInitStruct = {0};
  TimOCInitStruct.TIM_Pulse = CounterCRR;
  TimOCInitStruct.TIM_OCMode = TIM_OCMode_PWM2; // TIM->CNT < TIM->CCR -> Pin is Reset LED is LOW Active !
  TIM_OC3Init(TIM3,&TimOCInitStruct);
  
  // Enable the Timer Channel
  TIM_CCxNCmd(TIM3,TIM_Channel_3,ENABLE);

  // Enable the Timer Peripheral
  TIM_Cmd(TIM3,ENABLE);
}



