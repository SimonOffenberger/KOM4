/** @file Tsc_Calib_state.c
 *
 *  @brief Touchscreen Calibration State Implementation
 *
 *  @author Simon Offenberger
 */

#include "StateMachine.h"
#include "tsc.h"
#include "Lcd.h"
#include "Tsc_Calib_state_up_left.h"
#include "Tsc_Calib_state.h"
#include "Oszilloskop_State.h"
#include "EventQueue.h"
#include <stm32f072xb.h>

/** @brief Forward Declaration */
void Enter_TSC_Calib(void);
struct State_s const *TSC_Calib_Process(void);
void Exit_TSC_Calib(void);

/** @brief Vector structure to hold X, Y, Z coordinates */
typedef struct
{
  float X;
  float Y;
  float Z;
} vector_t;

/** @brief Structure to hold calibration vectors for each corner */
typedef struct
{
  vector_t up_left;
  vector_t up_right;
  vector_t down_left;
  vector_t down_right;
} calib_vec_t;

/** @brief TSC Calibration State Definition */
State_t TSC_Calib_State = {
    .Enter = Enter_TSC_Calib,
    .Exit = Exit_TSC_Calib,
    .Process = TSC_Calib_Process};

/** * @brief On Enter TSC Calibration State -> Clear Screen
 */
void Enter_TSC_Calib(void)
{
  Lcd_Fill(0, 0, LCD_WIDTH, LCD_HEIGHT, RGB565_BLACK);
}

/** * @brief TSC Calibration State Process Function
 *
 * @return Next State Pointer
 */
struct State_s const *TSC_Calib_Process(void)
{

  // Start with the first Corner for Calibration
  State_t *start_State = &TSC_Calib_State_up_left;

  // Run the Internal Calibration State Machine
  StateMachine_Run(start_State);

  // After Calibration is done go to Paint State
  return &Oszilloskop_State;
}

/** * @brief On Exit TSC Calibration State -> Calculate Calibration Parameters and Set them in TSC Module
 */
void Exit_TSC_Calib(void)
{

  calib_vec_t TSC_Cal_Vector = {0};

  Event_t touch_calib = {0};

  // Collect all the Calibration Points from the Event Queue
  while (EventQueue_Get(&touch_calib) != EVENT_QUEUE_EMPTY)
  {

    switch (touch_calib.EventType)
    {
    case EVENT_TSC_CALIB_UP_LEFT:
      TSC_Cal_Vector.up_left.X = touch_calib.X;
      TSC_Cal_Vector.up_left.Y = touch_calib.Y;
      TSC_Cal_Vector.up_left.Z = touch_calib.Z;
      break;
    case EVENT_TSC_CALIB_UP_RIGHT:
      TSC_Cal_Vector.up_right.X = touch_calib.X;
      TSC_Cal_Vector.up_right.Y = touch_calib.Y;
      TSC_Cal_Vector.up_right.Z = touch_calib.Z;
      break;
    case EVENT_TSC_CALIB_LOW_LEFT:
      TSC_Cal_Vector.down_left.X = touch_calib.X;
      TSC_Cal_Vector.down_left.Y = touch_calib.Y;
      TSC_Cal_Vector.down_left.Z = touch_calib.Z;
      break;
    case EVENT_TSC_CALIB_LOW_RIGHT:
      TSC_Cal_Vector.down_right.X = touch_calib.X;
      TSC_Cal_Vector.down_right.Y = touch_calib.Y;
      TSC_Cal_Vector.down_right.Z = touch_calib.Z;
      break;
    }
  }

  // Calculate X-Offset
  float x_offset = ((TSC_Cal_Vector.up_left.X + TSC_Cal_Vector.down_left.X) / 2);

  // Calculate Y-Offset
  float y_offset = ((TSC_Cal_Vector.up_left.Y + TSC_Cal_Vector.up_right.Y) / 2);

  // Calculate average x Distance across the screen
  float x_scale = (((TSC_Cal_Vector.up_right.X - TSC_Cal_Vector.up_left.X) +
                    (TSC_Cal_Vector.down_right.X - TSC_Cal_Vector.down_left.X)) /
                   2);

  // calculate the x Scale Factor to convert TSC Coordinates to LCD Coordinates
  x_scale = (LCD_WIDTH - TSC_CALIB_SQUARE_SIZE) / x_scale;

  // Calculate average y Distance across the screen
  float y_scale = (((TSC_Cal_Vector.down_left.Y - TSC_Cal_Vector.up_left.Y) +
                    (TSC_Cal_Vector.down_right.Y - TSC_Cal_Vector.up_right.Y)) /
                   2);

  // calculate the x Scale Factor to convert TSC Coordinates to LCD Coordinates
  y_scale = (LCD_HEIGHT - TSC_CALIB_SQUARE_SIZE) / y_scale;

  // Adjust the offsets for the with of the Square
  // The user touches the middle of the Squares not the pixel in the edge of the screen
  x_offset = x_offset - (TSC_CALIB_SQUARE_SIZE / 2) / x_scale;
  y_offset = y_offset - (TSC_CALIB_SQUARE_SIZE / 2) / y_scale;

  // Set the Calibration in the TSC Module
  Tsc_set_Cal(x_offset, y_offset, x_scale, y_scale);
  
}
