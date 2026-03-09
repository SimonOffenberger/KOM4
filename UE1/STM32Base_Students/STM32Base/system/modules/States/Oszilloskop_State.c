/**
 * @file Oszilloskop_State.c
 * @author Simon Offenberger
 * @brief Implementation of oscilloscope state.
 * @version 0.1
 * @date 2025-10-25
 *
 * @copyright Copyright (c) 2025
 */
#include "Led.h"
#include "StateMachine.h"
#include "tsc.h"
#include "Lcd.h"
#include "Oszilloskop_State.h"
#include "ADC_Sampling_State.h"
#include "EventQueue.h"
#include "SoftKeys.h"
#include "Timer.h"
#include "Adc.h"
#include <stm32f072xb.h>

// Forward Declarations
/**
 * @brief Enters the oscilloscope state.
 */
void Enter_Oszilloskop_State(void);

/**
 * @brief Processes events in the oscilloscope state.
 * @return Pointer to the next state.
 */
struct State_s const *Oszilloskop_State_Process(void);

/**
 * @brief Exits the oscilloscope state.
 */
void Exit_Oszilloskop_State(void);

/** @brief Oszilloskop State Definition */
State_t Oszilloskop_State = {
    .Enter = Enter_Oszilloskop_State,
    .Exit = Exit_Oszilloskop_State,
    .Process = Oszilloskop_State_Process};

/**
 * @brief Callback function for softkey button press events.
 * @param id The ID of the pressed softkey button.
 */
void Soft_Key_Clicked(const int id)
{
  Event_t touch_event = {0};

  switch (id)
  {
  case Start_Button_ID:
    touch_event.EventType = EVENT_Soft_Key_START;
    break;
  case Clear_Button_ID:
    touch_event.EventType = EVENT_Soft_Key_CLEAR;
    break;
  case Minimum_Button_ID:
    touch_event.EventType = EVENT_Soft_Key_MINIMUM;
    break;
  case Maximum_Button_ID:
    touch_event.EventType = EVENT_Soft_Key_MAXIMUM;
    break;
  }

  EventQueue_Put(&touch_event);
}

/**
 * @brief Displays the measurement grid on the LCD.
 * 
 * Draws grid lines both solid and dotted for waveform reference.
 */
static void Display_Graph_Grid(void)
{
  
  // Clear Graph Area
  Lcd_Fill(0, LCD_HEIGHT - Graph_Height, Graph_Width, Graph_Height, RGB565_WHITE);
  
  // Draw solid center lines 
  Lcd_Fill(0, LCD_HEIGHT - Graph_Height, Graph_Width, Grid_Line_Width, RGB565_BLACK);
  Lcd_Fill(Graph_Width / 2, LCD_HEIGHT - Graph_Height, Grid_Line_Width, Graph_Height, RGB565_BLACK);
  Lcd_Fill(0, LCD_HEIGHT - Graph_Height / 2, Graph_Width, Grid_Line_Width, RGB565_BLACK);

  // Draw grid minor lines as dotted lines
  Lcd_Draw_DottedLine(0, LCD_HEIGHT - Graph_Height / 6, Graph_Width, Grid_Line_Width, RGB565_BLACK, RGB565_WHITE);
  Lcd_Draw_DottedLine(0, LCD_HEIGHT - 5 * Graph_Height / 6, Graph_Width, Grid_Line_Width, RGB565_BLACK, RGB565_WHITE);
  Lcd_Draw_DottedLine(0, LCD_HEIGHT - 4 * Graph_Height / 6, Graph_Width, Grid_Line_Width, RGB565_BLACK, RGB565_WHITE);
  Lcd_Draw_DottedLine(0, LCD_HEIGHT - 2 * Graph_Height / 6, Graph_Width, Grid_Line_Width, RGB565_BLACK, RGB565_WHITE);

  // Draw vertical grid minor lines as dotted lines
  Lcd_Draw_DottedLine(Graph_Width / 6, LCD_HEIGHT - Graph_Height, Grid_Line_Width, Graph_Height, RGB565_BLACK, RGB565_WHITE);
  Lcd_Draw_DottedLine(5 * Graph_Width / 6, LCD_HEIGHT - Graph_Height, Grid_Line_Width, Graph_Height, RGB565_BLACK, RGB565_WHITE); 
  Lcd_Draw_DottedLine(4 * Graph_Width / 6, LCD_HEIGHT - Graph_Height, Grid_Line_Width, Graph_Height, RGB565_BLACK, RGB565_WHITE); 
  Lcd_Draw_DottedLine(2 * Graph_Width / 6, LCD_HEIGHT - Graph_Height, Grid_Line_Width, Graph_Height, RGB565_BLACK, RGB565_WHITE); 
}

/**
 * @brief Enters the oscilloscope state.
 * 
 * Initializes the ADC, LCD display, softkey buttons, and timer for UI polling.
 */
void Enter_Oszilloskop_State(void)
{
  // Initialize ADC for Potentiometer Input and Calibrate the ADC
  Adc_Initialize();
  Adc_Calibrate(NULL);

  // Clear whole screen to white
  Lcd_Fill(0, 0, LCD_WIDTH, LCD_HEIGHT, RGB565_WHITE);

  // Setup softkey buttons
  Display_SetSoftkey(Start_Button_ID, Start_Button_X, Start_Button_Y, Button_Width, Button_Height, "Start", Soft_Key_Clicked);
  Display_SetSoftkey(Clear_Button_ID, Clear_Button_X, Clear_Button_Y, Button_Width, Button_Height, "Clear", Soft_Key_Clicked);
  Display_SetSoftkey(Minimum_Button_ID, Minimum_Button_X, Minimum_Button_Y, Button_Width, Button_Height, "Minimum", Soft_Key_Clicked);
  Display_SetSoftkey(Maximum_Button_ID, Maximum_Button_X, Maximum_Button_Y, Button_Width, Button_Height, "Maximum", Soft_Key_Clicked);

  // Only Enable Start Button at the beginning
  Display_EnableSoftkey(Start_Button_ID);
  Display_DisableSoftkey(Clear_Button_ID);
  Display_DisableSoftkey(Minimum_Button_ID);
  Display_DisableSoftkey(Maximum_Button_ID);

  // Display Graph Grid
  Display_Graph_Grid();

  // Initialize Timer for Softkey Checking
  Timer_Initialize(TIMER_ID_TIM6);

  // Enable Timer to periodically check softkey states
  Timer_Enable(TIMER_ID_TIM6, Display_CheckSoftkeys, SoftKey_Check_Cycle_Time_MS, TIME_UNIT_MS, true);
}

/**
 * @brief Processes events in the oscilloscope state.
 * 
 * Handles softkey events and transitions to sampling state.
 *
 * @return Pointer to next state.
 */
struct State_s const *Oszilloskop_State_Process(void)
{
  Event_t event = {0};
  
  // Check for new Events in the Event Queue
  if (EventQueue_Get(&event) != EVENT_QUEUE_EMPTY)
  {
    switch (event.EventType)
    {
    case EVENT_Soft_Key_START:
      return &ADC_Sampling_State;
      break;
    }
  }

  return &Oszilloskop_State;
}

/**
 * @brief On Exit oscilloscope state.
 * 
 */
void Exit_Oszilloskop_State(void)
{
}
