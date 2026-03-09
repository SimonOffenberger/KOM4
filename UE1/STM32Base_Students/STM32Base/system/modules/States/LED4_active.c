/**
 * @author Simon Offenberger
 * @version 0.1
 * @date 2025-10-25
 * 
 */
#include "Led.h"
#include "StateMachine.h"
#include "LED3_active.h"
#include "LED4_active.h"
#include "EventQueue.h"
#include "Button.h"
#include <stm32f072xb.h>

void EnterLED4_active(void);
struct State_s const * LED4_active_Process(void);
void ExitLED4_active(void);

State_t LED4_active_State = {
	.Enter = EnterLED4_active,
	.Exit = ExitLED4_active,
	.Process = LED4_active_Process
};

/**
 * @brief On Enter LED4 active -> Turn on LED4
 * 
 */
void EnterLED4_active(void){
	LedOn(GPIO_ID_LED4);
}

/**
 * @brief Determin when to swich to LED3_active State
 * 
 * @return struct State_s const*  Return the next State
 */
struct State_s const * LED4_active_Process(void){
	
	Event_t Event;
	if(!EventQueue_Get(&Event)){
		if(Event.EventType == EVENT_BUTTON_PRESSED && Event.X == EVENT_INFO_BUTTON_USER0){
			return &LED3_active_State;
		}
	}
	
	
	return &LED4_active_State;
}

/**
 * @brief On Exit LED3 active -> Turn off LED3
 * 
 */
void ExitLED4_active(void){
	LedOff(GPIO_ID_LED4);
}

