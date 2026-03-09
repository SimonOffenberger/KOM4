/**
 * @file ADC_Sampling_State.h
 * @author Simon Offenberger
 * @brief ADC sampling state machine interface for waveform acquisition.
 * @version 0.1
 * @date 2025-10-25
 *
 * @copyright Copyright (c) 2025
 */
#ifndef ADC_Sampling_STATE_H
#define ADC_Sampling_STATE_H

#include "StateMachine.h"

/**
 * @brief ADC sampling state instance for state machine.
 */
extern State_t ADC_Sampling_State;

/**
 * @brief Number of ADC samples to collect per acquisition.
 */
#define ADC_SAMPLES 48

/**
 * @brief ADC sampling interval in milliseconds.
 */
#define ADC_SAMPLING_INTERVAL 100

/**
 * @brief Buffer to store ADC samples (voltage values in volts).
 */
extern float ADC_Sample_Buffer[ADC_SAMPLES];

/**
 * @brief Line width for graph rendering in pixels.
 */
#define Graph_Line_Width 2

#endif