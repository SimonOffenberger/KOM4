/**
 * @author Simon Offenberger
 * @version 0.1
 * @date 2025-10-25
 *
 */
#include "Led.h"
#include "StateMachine.h"
#include "tsc.h"
#include "Lcd.h"
#include "Wave_Display_State.h"
#include "ADC_Sampling_State.h"
#include "Oszilloskop_State.h"
#include "Eval_Minimum_State.h"
#include "Eval_Maximum_State.h"
#include "EventQueue.h"
#include "SoftKeys.h"
#include "Timer.h"
#include "Adc.h"
#include <stm32f072xb.h>


// Forward Declaration
void Enter_Wave_Display_State(void);
struct State_s const *Wave_Display_State_Process(void);
void Exit_Wave_Display_State(void);

/** @brief Wave_Display State Definition */
State_t Wave_Display_State = {
		.Enter = Enter_Wave_Display_State,
		.Exit = Exit_Wave_Display_State,
		.Process = Wave_Display_State_Process};




/**
 * @brief On Enter Wave_Display State -> Clear Screen
 * @brief and Initialize User Button for Erase Functionality
 */
void Enter_Wave_Display_State(void)
{


}

/**
 * @brief Wave_Display State Process Function
 *
 * @return Next State Pointer = Wave_Display State (stay in this state)
 */
struct State_s const *Wave_Display_State_Process(void)
{

  Event_t event = {0};
  // Check for new Events in the Event Queue
  if(EventQueue_Get(&event) != EVENT_QUEUE_EMPTY){
    switch(event.EventType){
      case EVENT_Soft_Key_CLEAR:
        return &Oszilloskop_State;
        break;
      case EVENT_Soft_Key_MINIMUM:
        return &Eval_Minimum_State;
        break;
      case EVENT_Soft_Key_MAXIMUM:
        return &Eval_Maximum_State;
        break;
    }
  }

	return &Wave_Display_State;
}

/**
 * @brief On Exit Wave_Display State -> No specific action
 *
 */
void Exit_Wave_Display_State(void)
{
}
