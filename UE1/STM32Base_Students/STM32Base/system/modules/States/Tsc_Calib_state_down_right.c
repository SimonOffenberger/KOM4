/**
 * @author Simon Offenberger
 * @version 0.1
 * @date 2025-10-25
 *
 */
#include "Led.h"
#include "StateMachine.h"
#include "tsc.h"
#include "Lcd.h"
#include "EventQueue.h"
#include <stm32f072xb.h>

void Enter_TSC_Calib_down_right(void);
struct State_s const *TSC_Calib_down_right_Process(void);
void Exit_TSC_Calib_down_right(void);

/** @brief State initialization for the down-right calibration step. */
State_t TSC_Calib_State_down_right = {
    .Enter = Enter_TSC_Calib_down_right,
    .Exit = Exit_TSC_Calib_down_right,
    .Process = TSC_Calib_down_right_Process};

/**
 * @brief Draw the calibration cross in the bottom-right corner.
 *
 * Executed on state entry.
 */
void Enter_TSC_Calib_down_right(void)
{
    Draw_Cross(LCD_WIDTH - TSC_CALIB_SQUARE_SIZE, LCD_HEIGHT - TSC_CALIB_SQUARE_SIZE, TSC_CALIB_SQUARE_SIZE, TSC_CALIB_SQUARE_SIZE, RGB565_BLUE, RGB565_RED);
}

/**
 * @brief Poll touch controller and enqueue final calibration point on new touch.
 * @return NULL when calibration finished, otherwise pointer to this state.
 * @return NULL means exit this Substatemachine
 */
struct State_s const *TSC_Calib_down_right_Process(void)
{

    bool touched = false;
    static bool prev_touched = true;
    uint16_t x = 0;
    uint16_t y = 0;

    // Read TSC State and Coordinates
    if (Tsc_Read(&touched, &x, &y, TSC_CALIB_SAMPLES) != TSC_OK)
    {
        prev_touched = touched;
        return &TSC_Calib_State_down_right;
    }

    if (touched && !prev_touched)
    {
        // Store Calibration Point in Event Queue
        Event_t touch_calib = {0};
        touch_calib.EventType = EVENT_TSC_CALIB_LOW_RIGHT;
        touch_calib.X = x;
        touch_calib.Y = y;
        touch_calib.Z = 0;
        EventQueue_Put(&touch_calib);
        prev_touched = true;
        return 0;
    }

    prev_touched = touched;

    return &TSC_Calib_State_down_right;
}

/**
 * @brief Clear the calibration target area when leaving the state.
 */
void Exit_TSC_Calib_down_right(void)
{
    Lcd_Fill(LCD_WIDTH - TSC_CALIB_SQUARE_SIZE, LCD_HEIGHT - TSC_CALIB_SQUARE_SIZE, TSC_CALIB_SQUARE_SIZE, TSC_CALIB_SQUARE_SIZE, RGB565_WHITE);
}
