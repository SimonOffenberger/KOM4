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

#include "SoftKeys.h"
#include <stdbool.h>
#include <assert.h>
#include "tsc.h"

typedef struct {
  uint32_t enabled;
  uint16_t was_touched;
  uint16_t x;
  uint16_t y;
  uint16_t width;
  uint16_t height;
  const char * text;
  SoftkeyCallback_t callback;
} Softkey_t;

// Internal Container for Softkeys
static Softkey_t softkeys[MAX_SOFTKEYS];

/**
 * @brief Draws a softkey button with text on the LCD.
 * @param button Pointer to the softkey structure (const).
 * @param border Pointer to border color (const).
 * @param infill Pointer to fill color (const).
 * @return OK on success, error code on failure.
 */
static int DrawButton(const Softkey_t * const button, const RGB565_t * const border, const RGB565_t * const infill)
{
  // Nullptr checks
  if(button == NULL || border == NULL || infill == NULL) 
    return ERROR_INVALID_ARGUMENT;

  uint32_t text_length = 0;

  // Center the text within the button
  for(uint16_t i = 0; button->text[i] != '\0'; i++) {
    
    // Check for unsupported characters
    if(button->text[i] == '\n') return ERROR_UNSUPPORTED_CHAR;
    if(button->text[i] == '\r') return ERROR_UNSUPPORTED_CHAR;
    
    // Calculate text length considering tabs
    if(button->text[i] == TAB_CHAR) {
      text_length += FONT_SOFTKEY.GetGlyph(' ')->Width * TAB_TO_SPACE_COUNT; 
    } else
    text_length += FONT_SOFTKEY.GetGlyph(button->text[i])->Width;

  }
  
  // Check if text fits within button
  if(text_length > (button->width - 2 * BORDER_SIZE)) {
    return ERROR_INVALID_TEXT_WIDTH;
  }
  
  // Draw the Border
  Lcd_Fill(button->x, button->y, button->width, button->height, *border);
  // Place Infill inside Border
  Lcd_Fill(button->x + BORDER_SIZE, button->y + BORDER_SIZE, button->width - 2 * BORDER_SIZE, button->height - 2 * BORDER_SIZE, *infill);
  // Print the Text in the middle of the Button
  Lcd_Print(button->x + (button->width - text_length) / 2, button->y + (button->height - FONT_SOFTKEY.LineHeight) / 2, button->text, RGB565_BLACK, *infill, &Consolas);

  return OK;
}

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
int Display_SetSoftkey(const int id, const uint16_t x, const uint16_t y, const uint16_t width, const uint16_t height, const char * const text, const SoftkeyCallback_t callback)
{
  // Validate parameters
  if(text == NULL || callback == NULL) {
    return ERROR_INVALID_ARGUMENT; 
  }

  if (id < 0 || id >= MAX_SOFTKEYS) {
    return ERROR_INVALID_SOFTKEY_ID; 
  }

  // Store softkey information
  softkeys[id].x = x;
  softkeys[id].y = y;
  softkeys[id].width = width;
  softkeys[id].height = height;
  softkeys[id].text = text;
  softkeys[id].callback = callback;
  softkeys[id].enabled = false;

  // Draw the softkey with initial colors
  RETURN_ON_ERROR(DrawButton(&softkeys[id], &BORDER_COLOR, &DISABLED_INFIL_COLOR));
  
  return OK;
}

/**
 * @brief Disables a softkey and greys out its appearance.
 * @param id Softkey ID to disable.
 * @return OK on success, ERROR_INVALID_SOFTKEY_ID on invalid ID.
 */
int Display_DisableSoftkey(const int id)
{
  // Check for valid ID
  if (id < 0 || id >= MAX_SOFTKEYS) {
    return ERROR_INVALID_SOFTKEY_ID; 
  }
  
  // Disable the softkey
  softkeys[id].enabled = false;
  
  // Redraw key with disabled colors
  RETURN_ON_ERROR(DrawButton(&softkeys[id], &BORDER_COLOR, &DISABLED_INFIL_COLOR));

  return OK;
}

/**
 * @brief Enables a softkey and restores its normal appearance.
 * @param id Softkey ID to enable.
 * @return OK on success, ERROR_INVALID_SOFTKEY_ID on invalid ID.
 */
int Display_EnableSoftkey(const int id)
{
  // Check for valid ID
  if (id < 0 || id >= MAX_SOFTKEYS) {
    return ERROR_INVALID_SOFTKEY_ID; 
  }
  
  // Enable the softkey
  softkeys[id].enabled = true;
  
  // Redraw key with normal colors
  RETURN_ON_ERROR(DrawButton(&softkeys[id], &BORDER_COLOR, &INFILL_COLOR));

  return OK;
}

// Suppress unused parameter warning
#define UNUSED(x) (void)(x)

/**
 * @brief Checks for touch input and executes softkey callbacks.
 * @param unused Unused parameter for timer callback compatibility.
 */
void Display_CheckSoftkeys(const int unused)
{
  // Suppress unused parameter warning
  // this parameter is required for timer callback compatibility
  UNUSED(unused);
  
  bool touch = false;
  uint16_t x = 0;
  uint16_t y = 0;

  // read the touch coordinates scaled to the LCD resolution
  Tsc_Read_scaled(&touch, &x, &y, TSC_TOUCH_AVERAGES);

  // Check each softkey
  for(int i = 0; i < MAX_SOFTKEYS; i++) {

    // if touched check if the key was previously not touched and if the touch is within the key boundaries
    if(touch) {
      if(softkeys[i].enabled && softkeys[i].was_touched == false) {
        if(x >= softkeys[i].x && x <= (softkeys[i].x + softkeys[i].width) &&
          y >= softkeys[i].y && y <= (softkeys[i].y + softkeys[i].height)) {
            // Execute the callback function
            if(softkeys[i].callback != NULL) {
              softkeys[i].callback(i);
            }

            // Mark the key as touched
            softkeys[i].was_touched = true;
        }

        // If not within boundaries, ensure was_touched is false
        else {
          softkeys[i].was_touched = false;
        }
      }
    }
    // if not touched, reset the was_touched flag
    else {
      softkeys[i].was_touched = false;
    }
  }
}
