/**
 * @file ADC_Sampling_State.c
 * @author Simon Offenberger
 * @brief Implementation of ADC sampling state for waveform acquisition and display.
 * @version 0.1
 * @date 2025-10-25
 *
 * @copyright Copyright (c) 2025
 */
#include "Led.h"
#include "StateMachine.h"
#include "tsc.h"
#include "Lcd.h"
#include "Wave_Display_State.h"
#include "Oszilloskop_State.h"
#include "EventQueue.h"
#include "SoftKeys.h"
#include "Timer.h"
#include "Adc.h"
#include <stdlib.h>
#include <stm32f072xb.h>
#include "ADC_Sampling_State.h"

float ADC_Sample_Buffer[ADC_SAMPLES] = {0};

// Forward Declarations
/**
 * @brief Enters the ADC sampling state.
 */
void Enter_ADC_Sampling_State(void);
static void Display_Line_Graph(float curr_Sample,uint16_t smp_idx);
/**
 * @brief Processes events in the ADC sampling state.
 * @return Pointer to the next state.
 */
struct State_s const *ADC_Sampling_State_Process(void);

/**
 * @brief Exits the ADC sampling state.
 */
void Exit_ADC_Sampling_State(void);

/** @brief ADC_Sampling State Definition */
State_t ADC_Sampling_State = {
        .Enter = Enter_ADC_Sampling_State,
        .Exit = Exit_ADC_Sampling_State,
        .Process = ADC_Sampling_State_Process};


#define UNUSED(x) (void)(x)

/**
 * @brief Collects a single ADC sample from the potentiometer.
 * 
 * Timer callback function that collects one ADC sample per callback.
 * When all samples are collected, posts EVENT_SAMPLING_COMPLETE to event queue.
 * 
 * @param id_unused Unused parameter for timer callback compatibility.
 */
void ADC_Collect_Samples(const int id_unused)
{
  // Suppress unused parameter warning
  UNUSED(id_unused);

  static uint16_t sample_index = 0;

  Adc_Convert(ADC_ID_POTENTIOMETER, &ADC_Sample_Buffer[sample_index], false);

  Display_Line_Graph(ADC_Sample_Buffer[sample_index], sample_index);

  sample_index++;

  // Check if all samples have been collected
  // if yes, post event and stop timer
  if(sample_index >= ADC_SAMPLES){
    sample_index = 0;
    Event_t adc_event = {0};
    adc_event.EventType = EVENT_SAMPLING_COMPLETE;
    EventQueue_Put(&adc_event);
    Timer_Disable(TIMER_ID_TIM7);
  } 
}

/**
 * @brief Enters the ADC sampling state.
 * 
 * Initializes ADC, disables softkeys, and starts the sampling timer.
 */
void Enter_ADC_Sampling_State(void)
{
  Adc_Enable();

  Display_DisableSoftkey(Start_Button_ID);
  Display_DisableSoftkey(Clear_Button_ID);
  Display_DisableSoftkey(Minimum_Button_ID);
  Display_DisableSoftkey(Maximum_Button_ID);
    
  // Initialize Timer for ADC Sampling
  Timer_Initialize(TIMER_ID_TIM7);
  // Start ADC Sampling Timer to collect samples periodically
  Timer_Enable(TIMER_ID_TIM7, ADC_Collect_Samples, ADC_SAMPLING_INTERVAL, TIME_UNIT_MS, true);
}


/**
 * @brief Processes events in the ADC sampling state.
 *
 * Checks event queue for sampling completion and transitions to wave display state.
 *
 * @return Pointer to Wave_Display_State on completion, or ADC_Sampling_State to stay.
 */
struct State_s const *ADC_Sampling_State_Process(void)
{
   Event_t event = {0};
  // Check for new Events in the Event Queue
  if(EventQueue_Get(&event) != EVENT_QUEUE_EMPTY){
    switch(event.EventType){
      case EVENT_SAMPLING_COMPLETE :
          return &Wave_Display_State;
      break;
        }
  }

    return &ADC_Sampling_State;
}


/**
 * @brief Draws a line graph based on ADC sample data.
 *
 * Scales ADC voltage values to graph height and draws connected line segments
 * between consecutive samples using Bresenham's line algorithm.
 */
static void Display_Line_Graph(float curr_Sample,uint16_t smp_idx)
{
    // Scale factor: ADC 0...3.3V mapped to graph height
    const float scale = (float)Graph_Height / ADC_VREF_VOLTAGE;

    // Horizontal spacing between ADC samples
    const int dx = Graph_Width / ADC_SAMPLES;

    // Initial graph point (first sample)
    static int prev_x = 0;
    static int prev_y = 0;

    if(smp_idx == 0){
        prev_x = 0;
        prev_y = LCD_HEIGHT - (int)(curr_Sample * scale);
    }


    const int x1 = (smp_idx + 1) * dx;
    const int y1 = LCD_HEIGHT - (int)(curr_Sample * scale);

    // Draw line from previous sample to the current one
    Lcd_DrawLine_Bresenham(prev_x, prev_y, x1, y1, Graph_Line_Width, RGB565_GREEN);

    // Update previous point
    prev_x = x1;
    prev_y = y1;
    
}


/**
 * @brief Exits the ADC sampling state.
 * 
 * Draws the collected ADC samples as a line graph on the LCD.
 */
void Exit_ADC_Sampling_State(void)
{
  Display_DisableSoftkey(Start_Button_ID);
  Display_EnableSoftkey(Clear_Button_ID);
  Display_EnableSoftkey(Minimum_Button_ID);
  Display_EnableSoftkey(Maximum_Button_ID);
}
