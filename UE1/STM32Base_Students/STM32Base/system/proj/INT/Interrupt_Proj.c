/**
  ******************************************************************************
  * @file    Interrupt_Proj.c
  * @author  Simon Offenberger
  * @version V1.0
  * @date    14-October-2017
  * @brief   Project File for the Exercise 5 in MPT - Interrupts
  ******************************************************************************
  */
#include "board_led.h"
#include "sysdelay.h"

// includes for STM32F0xx Standard Peripheral Library
#include "stm32f0xx.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_exti.h"
#include "stm32f0xx_syscfg.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_tim.h"


/**
  * @brief  Initialize buttons on PD0 and PD1 to generate external interrupts (EXTI).
  *         This function enables required clocks, configures GPIO inputs,
  *         maps EXTI lines to these pins, and enables the interrupt in the NVIC.
*/
void Button_Initialize_EXTI(void) 
{
  // Enable peripheral clocks 
  // Peripherals are not accessible until their clocks are enabled.
  // SYSCFG is needed to map EXTI lines to GPIO pins !
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_AHBENR_GPIODEN, ENABLE);


  GPIO_InitTypeDef initStruct;

  // Load default values into structure
  GPIO_StructInit(&initStruct);

  // Configure Button Pins for External Interrupts
  initStruct.GPIO_Pin  = GPIO_Pin_0 | GPIO_Pin_1;  
  initStruct.GPIO_Mode = GPIO_Mode_IN;             
  initStruct.GPIO_PuPd = GPIO_PuPd_DOWN;           
  GPIO_Init(GPIOD, &initStruct);                

  // Connect EXTI lines to GPIO pins
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource0);  // EXTI0 -> PD0
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource1);  // EXTI1 -> PD1

  // Configure EXTI lines
  EXTI_InitTypeDef extiInitStruct;
  extiInitStruct.EXTI_Line    = EXTI_Line0 | EXTI_Line1;  
  extiInitStruct.EXTI_Mode    = EXTI_Mode_Interrupt;    
  extiInitStruct.EXTI_Trigger = EXTI_Trigger_Rising;      // Interrupt on rising edge (button press)
  extiInitStruct.EXTI_LineCmd = ENABLE;                  
  EXTI_Init(&extiInitStruct);

  // Enable interrupt request in NVIC
  NVIC_EnableIRQ(EXTI0_1_IRQn);
}



// Max Auto Reload Register Value for 16 Bit Timer
#define ARR_MAX_VALUE       0xFFFF  

/**
 * @brief Function that initializes the Timer Peripheral to generate
 * @brief an Update Interrupt with the provided frequency
 * 
 * @param Interrupt_frequency Frequency of the Timer Interrupt in Hz
 */
void Timer3_IT_Initialize(const uint32_t Interrupt_frequency) {

   // Turn on Clock for TIM3
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  // Determine required Auto Reload value without Prescaler to realize Frequency
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
	
  // Clear the Update Interrupt Flag to not generate an immediate interrupt
  // after enabeling the interrupt
  TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	
  // Configure that the Update Event generates an interrupt
	TIM_ITConfig(TIM3, TIM_IT_Update,ENABLE);
	
  NVIC_EnableIRQ(TIM3_IRQn);  // Enable Timer 3 Interrupt in NVIC
}

/**
 * @brief Interrupt Handler for Timer 3 Interrupt
 * 
 */
void TIM3_IRQHandler(void) {
  // Check if Update Interrupt Flag is set
  if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
    // Clear Update Interrupt Flag
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);

    // Toggle LED3 on each Timer Interrupt
    // Timer Interrupts with 10 Hz frequency
    // so with this the LED will blink with 5 Hz and 50% duty cycle
    LED_Toggle(LED3);
  }
}

/**
  * @brief  This function handles EXTI Line0 and Line1 interrupts.
  */
void EXTI0_1_IRQHandler(void) {
    // Check if EXTI Line0 has an Pending Interrupt
    if (EXTI->PR & EXTI_PR_PR0) {
        // Clear the EXTI Line0 pending bit no read modify write!!!
				// Because it would reset another pending flag as well!
        // 1 written to the pending register clears the pending bit
        EXTI->PR = EXTI_PR_PR0;

        // Toggle LED above Button 0
        LED_Toggle(LED4);
        // Enable the blinking of LED3 by starting Timer 3
				TIM_Cmd(TIM3,ENABLE);
    }
    if (EXTI->PR & EXTI_PR_PR1) {

        // Clear the EXTI Line1 pending bit
        EXTI->PR = EXTI_PR_PR1;

        // Toggle LED above Button 1
				LED_Toggle(LED5);

        // Handle Button 1 press event
        // Disable the blinking of LED3 by stopping Timer 3
				TIM_Cmd(TIM3,DISABLE);
        // Turn the led Off if it was on
				LED_Off(LED3);
    }
}



/**
  * @brief  Implimentation of an blinking LED using Timer Interrupts and EXTI Interrupts 
  */
void main_interrupt_proj(void){
  // Enable global interrupts
  __enable_irq();
  // Initialize the LEDs
  LED_Initialize();
	// Initialize Buttons with EXTI Interrupts
  Button_Initialize_EXTI();
  // Initialize Timer 3 to generate Update Interrupts with 10 Hz frequency
	Timer3_IT_Initialize(10);

  // Main Loop
  while(1){
    // Do nothing, everything is handled in the Interrupts
  }

}