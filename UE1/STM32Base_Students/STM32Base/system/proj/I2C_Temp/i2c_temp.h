/**
 * @file i2c_temp.h
 * @author Simon Offenberger
 * @brief I2C Temperature Sensor Project which indicates a Alert via LED4 and shows the current temperature on the LCD.
 * @version 1.0
 * @date 2026-03-09
 *
 * @copyright Copyright (c) 2026
 *
 */

#ifndef __MAIN_I2C_TEMP_H
#define __MAIN_I2C_TEMP_H

#include <stdint.h>

/** @brief Temperature polling interval in milliseconds. */
#define I2C_POLL_INTERVAL_MS 250

/** @brief Text buffer size for LCD output. */
#define LCD_TEXT_BUFFER_SIZE 32

/** @brief Alert threshold temperature in degree Celsius. */
#define TEMP_ALERT_THRESHOLD  28.5f
/** @brief Alert hysteresis temperature in degree Celsius. */
#define TEMP_ALERT_HYSTERESIS 27.0f

/**
 * @brief Main function for the Project I2C Temperature Sensor.
 */
void main_I2C_Temp(void);

#endif /* __MAIN_I2C_TEMP_H */