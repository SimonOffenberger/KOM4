/**
 * @file Eval_Maximum_State.c
 * @author Simon Offenberger
 * @brief Implementation of maximum value evaluation state for waveform analysis.
 * @version 0.1
 * @date 2025-10-25
 *
 * @copyright Copyright (c) 2025
 */
#include "Led.h"
#include "StateMachine.h"
#include "tsc.h"
#include "Lcd.h"
#include "Eval_Maximum_State.h"
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
 * @brief Enters the maximum value evaluation state.
 */
void Enter_Eval_Maximum_State(void);

/**
 * @brief Processes events in the maximum value evaluation state.
 * @return Pointer to the next state.
 */
struct State_s const *Eval_Maximum_State_Process(void);

/**
 * @brief Exits the maximum value evaluation state.
 */
void Exit_Eval_Maximum_State(void);

/** @brief Eval_Maximum State Definition */
State_t Eval_Maximum_State = {
        .Enter = Enter_Eval_Maximum_State,
        .Exit = Exit_Eval_Maximum_State,
        .Process = Eval_Maximum_State_Process};


/**
 * @brief Enters the maximum value evaluation state.
 */
void Enter_Eval_Maximum_State(void)
{
  Display_DisableSoftkey(Maximum_Button_ID);
}


/**
 * @brief Processes events in the maximum value evaluation state.
 *
 * Finds the maximum ADC sample value and marks it with a box on the display.
 * Transitions to Wave_Display_State after processing.
 *
 * @return Pointer to Wave_Display_State.
 */
struct State_s const *Eval_Maximum_State_Process(void)
{
  uint16_t max_index = 0;
  float max_value = -1.0f;

  // Find Maximum in ADC Sample Buffer
  for(uint16_t i = 0; i < ADC_SAMPLES; i++){
    if(ADC_Sample_Buffer[i] > max_value){
      max_value = ADC_Sample_Buffer[i];
      max_index = i;
    }
  }

  const uint16_t x_pos = (uint16_t)(max_index * (Graph_Width / ADC_SAMPLES));
  const uint16_t y_pos = LCD_HEIGHT - (uint16_t)((max_value / ADC_VREF_VOLTAGE) * Graph_Height);

  Lcd_Draw_HollowBox(x_pos - MAXBoxSize/2, y_pos - MAXBoxSize/2, MAXBoxSize, MAXBoxSize, MAXBoxBorderWidth, RGB565_RED);

    return &Wave_Display_State;
}

/**
 * @brief Exits the maximum value evaluation state.
 */
void Exit_Eval_Maximum_State(void)
{
}
