/**
 * @file tsc.h
 * @author Simon Offenberger
 * @brief Module for interfacing and calibrating the Touch Sense Controller on the LCD
 * @version 1.0
 * @date 2025-11-22
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef TSC_H
#define TSC_H

#include <stdint.h>
#include <stdbool.h>

/** @brief Return Values */
#define TSC_OK 0
#define TSC_NOK 1
#define ERROR_NULLPTR 2
#define ERROR_NO_TOUCH 3

/** @brief Defines for the BSRR Macros */
#define High 1
#define Low 0

/** @brief Shift Amount to reach the Bit Reset Field of th BSR Register */
#define SHAMT_BitReset 16

/** @brief Mask for resseting all the SPI Pins for the TSC */
#define BRR_Mask (0xF << 28)

/** @brief Definition of the Position in the BSR Register */
#define Pin_DCLK_Pos 13
#define Pin_DIN_Pos 15
#define Pin_DOUT_Pos 14
#define Pin_CS_Pos 12
#define Pin_PENIRQ_Pos 2

/** @brief Macros for getting the Mask to set Pins with BSRR */
#define DCLK(x) (x << Pin_DCLK_Pos)
#define DIN(x) (x << Pin_DIN_Pos)
#define DOUT (1 << Pin_DOUT_Pos)
#define ChipSel (1 << Pin_CS_Pos)

/** @brief Rest Mask for BSRR */
#define DCLK_Reset ((1 << Pin_DCLK_Pos) << SHAMT_BitReset)
#define DIN_Reset ((1 << Pin_DIN_Pos) << SHAMT_BitReset)
#define DOUT_Reset ((1 << Pin_DOUT_Pos) << SHAMT_BitReset)
#define ChipSel_Reset ((1 << Pin_CS_Pos) << SHAMT_BitReset)

/** Commands for the TSC */
#define CMD_SampleX 0xD0
#define CMD_SampleY 0x90

/** @brief Coordinates if no touch is detected */
#define POS_NO_TOUCH 0xFFFF

/** @brief Mask for clearing the highest bit in a 16bit integer */
#define CLEAR_UPPER_BIT_MSK (~(1 << 15))

/**
 * @brief Defines for Generation Delay Using NOPS
 *
 * With 48 Mhz one NOP takes around 20ns.
 * So by applying multiple NOPS one can generate Delays in 20ns increments
 *
 */
#define NOP() __NOP()
#define NOP2() \
  NOP();       \
  NOP()
#define NOP4() \
  NOP2();      \
  NOP2()
#define NOP5() \
  NOP4();      \
  NOP()
#define NOP8() \
  NOP4();      \
  NOP4()
#define NOP10() \
  NOP8();       \
  NOP2()
#define NOP16() \
  NOP8();       \
  NOP8()

#define _Delay_200ns() NOP10();
#define _Delay_100ns() NOP5();
#define _Delay_20ns() __NOP();

/** @brief Defines for the TSC Calibration */
#define TSC_CALIB_SQUARE_SIZE 25
#define TSC_CALIB_SAMPLES 255

/**
 * @brief Function that initializes the GPIO Pins Used for interfacing with the TSC
 *
 * @return int Error Code
 */
int Tsc_Initialize(void);

/**
 * @brief Function that reads and averages the coordinates of the TSC
 *
 * @param touched Flag if a touch was detected
 * @param x X Coordinate
 * @param y Y Coordinate
 * @param samples Number of Samples that should be averaged
 * @return int Error Code
 */
int Tsc_Read(bool *const touched, uint16_t *const x, uint16_t *const y, const uint8_t samples);

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
int Tsc_Read_scaled(bool *const touched, uint16_t *const x, uint16_t *const y, const uint8_t samples);

/**
 * @brief Function for setting the Scale and Offset for Mapping to the TFT Coordinates
 *
 * @param x_offset X Offset
 * @param y_offset Y Offset
 * @param x_scale X Scale
 * @param y_scale Y Scale
 */
void Tsc_set_Cal(const float x_offset, const float y_offset, const float x_scale, const float y_scale);

#endif
