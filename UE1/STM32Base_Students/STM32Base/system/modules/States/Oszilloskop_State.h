/**
 * @file Oszilloskop_State.h
 * @author Simon Offenberger
 * @brief Oscilloscope state.
 * @version 0.1
 * @date 2025-10-25
 *
 * @copyright Copyright (c) 2025
 */
#ifndef Oszilloskop_STATE_H
#define Oszilloskop_STATE_H

#include "StateMachine.h"

/**
 * @brief Oscilloscope state instance for state machine.
 */
extern State_t Oszilloskop_State;

#define SoftKey_Check_Cycle_Time_MS 100   ///< Softkey check cycle time in milliseconds

// Graph Configuration
#define Graph_Width 240            ///< Graph width in pixels
#define Graph_Height 240           ///< Graph height in pixels
#define Grid_Line_Width 2          ///< Grid line width in pixels

// UI Layout Configuration
#define Border_Spacing 10          ///< Border spacing in pixels
#define Button_Width 80            ///< Button width in pixels
#define Button_Height 25           ///< Button height in pixels
#define Button_Spacing_Y ((LCD_HEIGHT - Graph_Height) - (2 * Border_Spacing + 2 * Button_Height))         ///< Vertical button spacing in pixels
#define Button_Spacing_X (LCD_WIDTH- 2 * Border_Spacing - 2 * Button_Width)        ///< Horizontal button spacing in pixels

#define Second_Row (Border_Spacing + Button_Height + Button_Spacing_Y)       ///< Y-position of second row
#define Second_Column (Border_Spacing + Button_Width + Button_Spacing_X)     ///< X-position of second column

// Start Button Configuration
#define Start_Button_X Border_Spacing      ///< Start button X-coordinate
#define Start_Button_Y Border_Spacing      ///< Start button Y-coordinate
#define Start_Button_ID 0                  ///< Start button ID

// Clear Button Configuration
#define Clear_Button_X Second_Column       ///< Clear button X-coordinate
#define Clear_Button_Y Border_Spacing      ///< Clear button Y-coordinate
#define Clear_Button_ID 1                  ///< Clear button ID

// Minimum Button Configuration
#define Minimum_Button_X Border_Spacing    ///< Minimum button X-coordinate
#define Minimum_Button_Y Second_Row        ///< Minimum button Y-coordinate
#define Minimum_Button_ID 2                ///< Minimum button ID

// Maximum Button Configuration
#define Maximum_Button_X Second_Column     ///< Maximum button X-coordinate
#define Maximum_Button_Y Second_Row        ///< Maximum button Y-coordinate
#define Maximum_Button_ID 3                ///< Maximum button ID


#endif