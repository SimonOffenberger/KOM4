/**
 * @author Simon Offenberger
 * @version 0.1
 * @date 2025-10-25
 * 
 */
#include "StateMachine.h"
#include <stm32f072xb.h>

/**
 * @brief With his function we enter the State Machine 
 * @attention This function never returns!!!
 * 
 * @param state Initial State of the Statemachine | State Enter will be called!
 */
void StateMachine_Run(State_t const * state)
{
    // call State Enter on Entering
	state->Enter();
	
    // endless loop!
    while(1){
        // next State is returned by the state Process Function
        State_t const * nextState = state->Process();

        // if nullptr is returned exit Statemachine
        if(nextState == 0){ 
            state->Exit();
            break;
        }
        // if the next state differs from the current state excecute Exit / Enter
        if (nextState != state)
        {
            // if Exit Function exists -> call it
            if (state->Exit)
                state->Exit();
            // if Enter Function exists -> Call it
            if (nextState->Enter)
                nextState->Enter();

        }

        // update the current state
        state = nextState;
    }
}