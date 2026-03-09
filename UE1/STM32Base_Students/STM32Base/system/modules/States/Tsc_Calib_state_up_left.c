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
#include "Tsc_Calib_state_up_right.h"
#include "EventQueue.h"
#include <stm32f072xb.h>

void Enter_TSC_Calib_up_left(void);

struct State_s const *TSC_Calib_up_left_Process(void);
void Exit_TSC_Calib_up_left(void);

/** @brief State initialization exported to the state machine. */
State_t TSC_Calib_State_up_left = {
    .Enter = Enter_TSC_Calib_up_left,
    .Exit = Exit_TSC_Calib_up_left,
    .Process = TSC_Calib_up_left_Process};

/**
 * @brief Draw the calibration cross in the top-left corner.
 *
 * Called once when the state becomes active.
 */
void Enter_TSC_Calib_up_left(void)
{
    // Draw target cross at top-left
    Draw_Cross(0, 0, TSC_CALIB_SQUARE_SIZE, TSC_CALIB_SQUARE_SIZE, RGB565_BLUE, RGB565_RED);
}

/**
 * @brief Poll touch controller and store calibration point on new touch in eventQueue.
 * @return Pointer to next state (or this state to continue).
 */
struct State_s const *TSC_Calib_up_left_Process(void)
{

    bool touched = false;
    static bool prev_touched = true;
    uint16_t x = 0;
    uint16_t y = 0;

    // Read TSC State and Coordinates
    if (Tsc_Read(&touched, &x, &y, TSC_CALIB_SAMPLES) != TSC_OK)
    {
        prev_touched = touched;
        return &TSC_Calib_State_up_left;
    }

    if (touched && !prev_touched)
    {
        // Store Calibration Point in Event Queue
        Event_t touch_calib = {0};
        touch_calib.EventType = EVENT_TSC_CALIB_UP_LEFT;
        touch_calib.X = x;
        touch_calib.Y = y;
        touch_calib.Z = 0;
        EventQueue_Put(&touch_calib);
        prev_touched = true;
        return &TSC_Calib_State_up_right;
    }

    prev_touched = touched;

    return &TSC_Calib_State_up_left;
}

/**
 * @brief Clear the calibration target area on exit.
 */
void Exit_TSC_Calib_up_left(void)
{
    Lcd_Fill(0, 0, TSC_CALIB_SQUARE_SIZE, TSC_CALIB_SQUARE_SIZE, RGB565_WHITE);
}
