/**
 * @file SoftKeys.c
 * @author Simon Offenberger
 * @brief Implementation of softkey buttons for the LCD interface.
 * @version 1.0
 * @date 2025-11-28
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef SOFTKEYS_H
#define SOFTKEYS_H

#include <stdint.h>
#include "Lcd.h"

/**
 * @brief Maximum number of softkeys supported.
 * 
 */
#define MAX_SOFTKEYS 10

// Error Codes
#define ERROR_INVALID_SOFTKEY_ID -1
#define ERROR_INVALID_ARGUMENT -2
#define ERROR_INVALID_TEXT_WIDTH -3
#define ERROR_UNSUPPORTED_CHAR -4

#define FONT_SOFTKEY Consolas

#define BORDER_SIZE 5
#define BORDER_COLOR RGB565_BLUE
#define INFILL_COLOR RGB565_LIGHT_BLUE
#define DISABLED_INFIL_COLOR RGB565_GREY

// Average count for touch reading
#define TSC_TOUCH_AVERAGES 16

/**
 * @typedef SoftkeyCallback_t
 * @brief Function pointer type for softkey callback handlers.
 * @param id The ID of the touched softkey.
 */
typedef void (*SoftkeyCallback_t)(const int id);

/**
 * @brief Displays and registers a softkey button on the LCD.
 * @param id Softkey ID (0 to MAX_SOFTKEYS-1).
 * @param x X-coordinate of the button.
 * @param y Y-coordinate of the button.
 * @param width Button width in pixels.
 * @param height Button height in pixels.
 * @param text Zero-terminated string to display on the button.
 * @param callback Function pointer called when the button is touched.
 * @return OK on success, error code on failure.
 */
int Display_SetSoftkey(
  const int id,
  const uint16_t x,
  const uint16_t y,
  const uint16_t width,
  const uint16_t height,
  const char * const text,
  const SoftkeyCallback_t callback);

/**
 * @brief Disables a softkey and greys out its appearance.
 * @param id Softkey ID to disable.
 * @return OK on success, ERROR_INVALID_SOFTKEY_ID on invalid ID.
 */
int Display_DisableSoftkey(const int id);

/**
 * @brief Enables a softkey and restores its normal appearance.
 * @param id Softkey ID to enable.
 * @return OK on success, ERROR_INVALID_SOFTKEY_ID on invalid ID.
 */
int Display_EnableSoftkey(const int id);

/**
 * @brief Checks for touch input and executes softkey callbacks.
 * @param unused Unused parameter for timer callback compatibility.
 */
void Display_CheckSoftkeys(const int unused);

#endif