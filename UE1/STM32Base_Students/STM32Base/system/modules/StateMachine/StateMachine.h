/**
 * @author Simon Offenberger
 * @version 0.1
 * @date 2025-10-25
 * 
 */
#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

/**
 * @brief Defintion of the FuncPTR of the Transition Function -> Enter and Exit
 * 
 */
typedef void (*StateTransitionFunction_t)(void);

/**
 * @brief Definition of the State Process FunctionPTR
 * 
 */
typedef struct State_s const * (*StateFunction_t)(void);

// State Struct defintion
typedef struct State_s {
  StateTransitionFunction_t Enter;
  StateFunction_t Process;
  StateTransitionFunction_t Exit;
} State_t;

/**
 * @brief With his function we enter the State Machine 
 * @attention This function never returns!!!
 * 
 * @param state Initial State of the Statemachine | State Enter will be called!
 */
void StateMachine_Run(State_t const * state);

#endif