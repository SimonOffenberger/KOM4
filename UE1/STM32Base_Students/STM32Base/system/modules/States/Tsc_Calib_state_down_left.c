/**
 * @author Simon Offenberger
 * @version 0.1
 * @date 2025-10-25
 *
 */

#include "tsc.h"
#include "Lcd.h"
#include "Tsc_Calib_state_down_right.h"
#include "EventQueue.h"
#include <stm32f072xb.h>

/** @brief Forward Declaration */
void Enter_TSC_Calib_down_left(void);
struct State_s const *TSC_Calib_down_left_Process(void);
void Exit_TSC_Calib_down_left(void);

/** @brief TSC Calibration State Down Left Definition */
State_t TSC_Calib_State_down_left = {
    .Enter = Enter_TSC_Calib_down_left,
    .Exit = Exit_TSC_Calib_down_left,
    .Process = TSC_Calib_down_left_Process};

/**
 * @brief Draw the calibration cross at the bottom-left corner.
 *
 * Called when entering this state.
 */
void Enter_TSC_Calib_down_left(void)
{
    // Draw Calibration Cross at Down Left Corner
    Draw_Cross(0, LCD_HEIGHT - TSC_CALIB_SQUARE_SIZE, TSC_CALIB_SQUARE_SIZE, TSC_CALIB_SQUARE_SIZE, RGB565_BLUE, RGB565_RED);
}

/**
 * @brief Poll touch controller and store calibration point on new touch in eventQueue.
 * @return Pointer to next state (down-right) or this state to continue polling.
 */
struct State_s const *TSC_Calib_down_left_Process(void)
{

    bool touched = false;
    static bool prev_touched = true;
    uint16_t x = 0;
    uint16_t y = 0;

    // Read TSC State and Coordinates
    if (Tsc_Read(&touched, &x, &y, TSC_CALIB_SAMPLES) != TSC_OK)
    {
        prev_touched = touched;
        return &TSC_Calib_State_down_left;
    }

    // On Touch Event -> Store Calibration Point and go to Next State
    if (touched && !prev_touched)
    {
        // Store Calibration Point in Event Queue
        Event_t touch_calib = {0};
        touch_calib.EventType = EVENT_TSC_CALIB_LOW_LEFT;
        touch_calib.X = x;
        touch_calib.Y = y;
        touch_calib.Z = 0;
        // Enqueue the Calibration Point
        EventQueue_Put(&touch_calib);

        // Set prev_touched to true to avoid multiple detections
        prev_touched = true;

        // Go to Next State
        return &TSC_Calib_State_down_right;
    }

    prev_touched = touched;

    // Stay in this State
    return &TSC_Calib_State_down_left;
}

/**
 * @brief Clear the calibration cross area on exit.
 */
void Exit_TSC_Calib_down_left(void)
{
    Lcd_Fill(0, LCD_HEIGHT - TSC_CALIB_SQUARE_SIZE, TSC_CALIB_SQUARE_SIZE, TSC_CALIB_SQUARE_SIZE, RGB565_WHITE);
}
