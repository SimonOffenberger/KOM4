/**
 * @file Eval_Maximum_State.h
 * @author Simon Offenberger
 * @brief Maximum value evaluation state machine interface for waveform analysis.
 * @version 0.1
 * @date 2025-10-25
 *
 * @copyright Copyright (c) 2025
 */
#ifndef Eval_Maximum_STATE_H
#define Eval_Maximum_STATE_H

#include "StateMachine.h"

/**
 * @brief Maximum value evaluation state instance for state machine.
 */
extern State_t Eval_Maximum_State;

/**
 * @brief Border width for maximum value indicator box in pixels.
 */
#define MAXBoxBorderWidth 2

/**
 * @brief Size of the maximum value indicator box in pixels.
 */
#define MAXBoxSize 12

#endif