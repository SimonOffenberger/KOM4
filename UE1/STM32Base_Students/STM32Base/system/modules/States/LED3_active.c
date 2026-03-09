/**
 * @author Simon Offenberger
 * @version 0.1
 * @date 2025-10-25
 * 
 */
#include "StateMachine.h"
#include "LED3_active.h"
#include "LED4_active.h"
#include "EventQueue.h"
#include "Led.h"
#include "Button.h"
#include <stm32f072xb.h>

// Forward Deklarations
void EnterLED3_active(void);
struct State_s const * LED3_active_Process(void);
void ExitLED3_active(void);

// Defintion of the State
State_t LED3_active_State = {
	.Enter = EnterLED3_active,
	.Exit = ExitLED3_active,
	.Process = LED3_active_Process
};

/**
 * @brief On Enter LED3 active -> Turn on LED3
 * 
 */
void EnterLED3_active(void){
	LedOn(GPIO_ID_LED3);
}

/**
 * @brief Determin when to swich to LED4_active State
 * 
 * @return struct State_s const*  Return the next State
 */
struct State_s const * LED3_active_Process(void){
	
	Event_t Event;
	if(!EventQueue_Get(&Event)){
		if(Event.EventType == EVENT_BUTTON_PRESSED && Event.X == EVENT_INFO_BUTTON_USER1){
			return &LED4_active_State;
		}
	}
	
	
	return &LED3_active_State;
}

/**
 * @brief On Exit LED3 active -> Turn off LED3
 * 
 */
void ExitLED3_active(void){
	LedOff(GPIO_ID_LED3);
}

