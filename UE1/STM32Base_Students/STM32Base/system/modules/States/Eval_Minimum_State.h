/**
 * @file Eval_Minimum_State.h
 * @author Simon Offenberger
 * @brief Minimum value evaluation state machine interface for waveform analysis.
 * @version 0.1
 * @date 2025-10-25
 *
 * @copyright Copyright (c) 2025
 */
#ifndef Eval_Minimum_STATE_H
#define Eval_Minimum_STATE_H

#include "StateMachine.h"

/**
 * @brief Minimum value evaluation state instance for state machine.
 */
extern State_t Eval_Minimum_State;

/**
 * @brief Border width for minimum value indicator box in pixels.
 */
#define MINBoxBorderWidth 2

/**
 * @brief Size of the minimum value indicator box in pixels.
 */
#define MINBoxSize 12

#endif