/**
 * @file Eval_Minimum_State.c
 * @author Simon Offenberger
 * @brief Implementation of minimum value evaluation state for waveform analysis.
 * @version 0.1
 * @date 2025-10-25
 *
 * @copyright Copyright (c) 2025
 */
#include "Led.h"
#include "StateMachine.h"
#include "tsc.h"
#include "Lcd.h"
#include "Eval_Minimum_State.h"
#include "ADC_Sampling_State.h"
#include "Oszilloskop_State.h"
#include "Wave_Display_State.h"
#include "EventQueue.h"
#include "SoftKeys.h"
#include "Timer.h"
#include "Adc.h"
#include <stm32f072xb.h>


// Forward Declarations
/**
 * @brief Enters the minimum value evaluation state.
 */
void Enter_Eval_Minimum_State(void);

/**
 * @brief Processes events in the minimum value evaluation state.
 * @return Pointer to the next state.
 */
struct State_s const *Eval_Minimum_State_Process(void);

/**
 * @brief Exits the minimum value evaluation state.
 */
void Exit_Eval_Minimum_State(void);

/** @brief Eval_Minimum State Definition */
State_t Eval_Minimum_State = {
        .Enter = Enter_Eval_Minimum_State,
        .Exit = Exit_Eval_Minimum_State,
        .Process = Eval_Minimum_State_Process};


/**
 * @brief Enters the minimum value evaluation state.
 */
void Enter_Eval_Minimum_State(void)
{
  Display_DisableSoftkey(Minimum_Button_ID);
}


/**
 * @brief Processes events in the minimum value evaluation state.
 *
 * Finds the minimum ADC sample value and marks it with a box on the display.
 * Transitions to Wave_Display_State after processing.
 *
 * @return Pointer to Wave_Display_state.
 */
struct State_s const *Eval_Minimum_State_Process(void)
{
  uint16_t min_index = 0;
  float min_value = ADC_VREF_VOLTAGE;

  // Find Minimum in ADC Sample Buffer
  for(uint16_t i = 0; i < ADC_SAMPLES; i++){
    if(ADC_Sample_Buffer[i] < min_value){
      min_value = ADC_Sample_Buffer[i];
      min_index = i;
    }
  }

  // Calculate position on display
  const uint16_t x_pos = (uint16_t)(min_index * (Graph_Width / ADC_SAMPLES));
  const uint16_t y_pos = LCD_HEIGHT - (uint16_t)((min_value / ADC_VREF_VOLTAGE) * Graph_Height);

  // Draw Hollow Box around the Minimum Point
  Lcd_Draw_HollowBox(x_pos - MINBoxSize/2, y_pos - MINBoxSize/2, MINBoxSize, MINBoxSize, MINBoxBorderWidth, RGB565_BLUE);

  return &Wave_Display_State;
}

/**
 * @brief Exits the minimum value evaluation state.
 */
void Exit_Eval_Minimum_State(void)
{
}
