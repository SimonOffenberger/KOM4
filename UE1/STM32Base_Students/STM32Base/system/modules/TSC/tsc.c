/**
 * @file tsc.c
 * @author Simon Offenberger
 * @brief Module for interfacing and calibrating the Touch Sense Controller on the LCD
 * @version 1.0
 * @date 2025-11-22
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <stdbool.h>
#include "tsc.h"
#include "Gpio.h"
#include <stm32f072xb.h>

// Makros
#define RETURN_ON_ERROR(expression) \
  {                                 \
    int ___retval = expression;     \
    if (___retval != TSC_OK)        \
      return ___retval;             \
  }

/** @brief Struct that holds the Calibration Information */
typedef struct
{
  float x_scale;
  float y_scale;
  float x_offset;
  float y_offset;
} Scale_t;

/** @brief Defintion of the Scale Struct */
static Scale_t TFT_Scale = {0};

/** @brief Maximum Index of a Bit in a Byte */
#define MAX_BIT_INDEX_IN_BYTE 7

/**
 * @brief Function that initializes the GPIO Pins Used for interfacing with the TSC
 *
 * @return int Error Code
 */
int Tsc_Initialize(void)
{

  // Chip Select is low active -> Idle Level high
  RETURN_ON_ERROR(Gpio_Initialize(GPIO_ID_TOUCH_CS, GPIO_MODE_OUTPUT, true));

  // Clock line Idle = High
  RETURN_ON_ERROR(Gpio_Initialize(GPIO_ID_TOUCH_DCLK, GPIO_MODE_OUTPUT, true));

  // Data In Line High active -> Idle Level low
  RETURN_ON_ERROR(Gpio_Initialize(GPIO_ID_TOUCH_DIN, GPIO_MODE_OUTPUT, false));

  // Data OUT is Input
  RETURN_ON_ERROR(Gpio_Initialize(GPIO_ID_TOUCH_DOUT, GPIO_MODE_INPUT, false));

  // Busy is Input
  RETURN_ON_ERROR(Gpio_Initialize(GPIO_ID_TOUCH_BUSY, GPIO_MODE_INPUT, false));

  // PenIRQ With Pull up to ensure Inactive level if the TSC does not drive this line
  RETURN_ON_ERROR(Gpio_Initialize(GPIO_ID_TOUCH_PENIRQ, GPIO_MODE_INPUT_PULL_UP, true));

  return TSC_OK;
}

/**
 * @brief Low Level SPI Communication Function for Writing a BYTE to the TSC
 *
 * @param data Data that should be written to the TSC
 */
static void Tsc_WriteByte(const uint8_t data)
{

  // Serialize the data in data and clock it out
  for (int8_t i = MAX_BIT_INDEX_IN_BYTE; i >= 0; --i)
  {
    // Set Bit and Reset Clock
    GPIOE->BSRR = DIN_Reset | DCLK_Reset | DIN(((data >> i) & 0x1));
    _Delay_200ns();
    // Set Clock Lin
    GPIOE->BSRR = DCLK(High);
    _Delay_200ns();
  }

  // Set Clock and Data Line to Reset Val;
  GPIOE->BSRR = DCLK_Reset;
  GPIOE->BSRR = DIN_Reset;
}

/**
 * @brief Low Level SPI Communication Function for Reading a BYTE from the TSC
 *
 * @return Data byte read from the TSC
 */
static uint8_t Tsc_ReadByte(void)
{

  uint8_t input_data = 0;

  for (int8_t i = MAX_BIT_INDEX_IN_BYTE; i >= 0; --i)
  {
    // Set Clock High
    GPIOE->BSRR = DCLK(High);
    _Delay_200ns();
    // Read the Input Data from the IDR Register of the DOUT Port Pin
    input_data |= ((GPIOE->IDR & DOUT) >> Pin_DOUT_Pos) << i;
    // Pull Clock Low
    GPIOE->BSRR = DCLK_Reset;
    _Delay_200ns();
  }

  return input_data;
}

/**
 * @brief Function for Reading the Raw Touch Data from the TSC
 *
 * @param touched Indicates if a Touch was detected
 * @param x X Coordinate of the touch
 * @param y Y Coordinate of the touch
 * @return int Error Code
 */
static int Tsc_ReadRaw(bool * const touched, uint16_t * const x, uint16_t * const  y)
{

  // check for Nullptr
  if (touched == 0 || x == 0 || y == 0)
    return ERROR_NULLPTR;

  uint16_t x_val = 0;
  uint16_t y_val = 0;

  // if no touch occurred set touched to false
  // and set the coordinates to not touch pos
  // a touch is indicated via the Low Active Pending IRQ Pin
  if (GPIOF->IDR & (1 << Pin_PENIRQ_Pos))
  {
    *touched = false;
    *x = POS_NO_TOUCH;
    *y = POS_NO_TOUCH;
    return ERROR_NO_TOUCH;
  }
  // if a touch was detected set the touched Flag
  else
    *touched = true;

  // Pull Chipselect low -> activate Chip
  GPIOE->BSRR = ChipSel_Reset;
  _Delay_20ns();

  // Write Command to sample X Coordinate
  Tsc_WriteByte(CMD_SampleX);

  // read the Sampled X coordinate from the chip
  // 16 bit value !
  x_val = Tsc_ReadByte() << 8;
  x_val |= Tsc_ReadByte();
  _Delay_20ns();

  // Write command to sample y coordinate
  Tsc_WriteByte(CMD_SampleY);
  y_val = Tsc_ReadByte() << 8;
  y_val |= Tsc_ReadByte();

  // pull chip select high to deactivate chip
  GPIOE->BSRR = ChipSel;

  // delay 100ns so that no timing violation can occur
  _Delay_100ns();

  // check if the lcd is still touched and not released in the meantime
  // if it was leased during sampling set touched = false
  // and set coordinates to no touch
  if (GPIOF->IDR & (1 << Pin_PENIRQ_Pos))
  {
    *touched = false;
    *x = POS_NO_TOUCH;
    *y = POS_NO_TOUCH;
    return ERROR_NO_TOUCH;
  }
  else
    *touched = true;

  // Mask out the upper Bit -> this is not part of the data
  x_val &= CLEAR_UPPER_BIT_MSK;
  y_val &= CLEAR_UPPER_BIT_MSK;

  // Assign the values to the return parameter
  *x = x_val;
  *y = y_val;

  return TSC_OK;
}

/**
 * @brief Function that reads and averages the coordinates of the TSC
 *
 * @param touched Flag if a touch was detected
 * @param x X Coordinate
 * @param y Y Coordinate
 * @param samples Number of Samples that should be averaged
 * @return int Error Code
 */
int Tsc_Read(bool *const touched, uint16_t *const x, uint16_t *const y, const uint8_t samples)
{

  // Check for nullptr
  if (touched == 0 || x == 0 || y == 0)
    return ERROR_NULLPTR;

  // check for touch
  if (GPIOF->IDR & (1 << Pin_PENIRQ_Pos))
  {
    *x = POS_NO_TOUCH;
    *y = POS_NO_TOUCH;
    *touched = false;
  }

  uint32_t x_val_sum = 0;
  uint32_t y_val_sum = 0;

  // accumulate the samples
  for (uint16_t i = 0; i < samples; i++)
  {

    RETURN_ON_ERROR(Tsc_ReadRaw(touched, x, y));

    // if touch was release between samples abort
    if (!(*touched))
    {
      *x = POS_NO_TOUCH;
      *y = POS_NO_TOUCH;
      *touched = false;
      return ERROR_NO_TOUCH;
    }

    // accumulate the samples
    x_val_sum += (*x);
    y_val_sum += (*y);
  }

  // calculate average
  x_val_sum = x_val_sum / samples;
  y_val_sum = y_val_sum / samples;

  // assign the averaged values
  *x = (uint16_t)x_val_sum;
  *y = (uint16_t)y_val_sum;

  return TSC_OK;
}

/**
 * @brief Function that returns the Scaled Coordinates of the TSC
 *
 * These Coordinates are Scaled to match with the Coordinates of the TFT Display.
 * Bevor this Function the TFT Cali should be performed.
 * The Cal Information is transferred using the  Tsc_set_Cal() Function
 *
 * @param touched Flag indicating a touch
 * @param x Scaled X Coordinate
 * @param y Scaled Y Coordinate
 * @param samples No of Averages
 * @return int Error_Code
 */
int Tsc_Read_scaled(bool *const touched, uint16_t *const x, uint16_t *const y, const uint8_t samples)
{
  int retval = TSC_OK;

  // check for nullptr
  if (touched == 0 || x == 0 || y == 0)
    return ERROR_NULLPTR;

  // read the TSC Data
  retval = Tsc_Read(touched, x, y, samples);

  // Scale X and Y Coordinates
  *x = TFT_Scale.x_scale * ((*x) - TFT_Scale.x_offset);
  *y = TFT_Scale.y_scale * ((*y) - TFT_Scale.y_offset);

  return retval;
}

/**
 * @brief Function for setting the Scale and Offset for Mapping to the TFT Coordinates
 *
 * @param x_offset X Offset
 * @param y_offset Y Offset
 * @param x_scale X Scale
 * @param y_scale Y Scale
 */
void Tsc_set_Cal(const float x_offset, const float y_offset, const float x_scale, const float y_scale)
{

  // Set Scale Struct
  TFT_Scale.x_scale = x_scale;
  TFT_Scale.y_scale = y_scale;
  TFT_Scale.x_offset = x_offset;
  TFT_Scale.y_offset = y_offset;
}
