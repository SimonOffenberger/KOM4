/**
 * @author Simon Offenberger
 * @version 0.1
 * @date 2025-10-25
 * 
 */
#include "Button.h"
#include "Gpio.h"
#include "Timer.h"
#include "EventQueue.h"

/**
 * @brief Internal Definition and Initialization of the Buttons Struct
 * 
 */
static Button_T Buttons[NUM_OF_BUTTON] = {
	{GPIO_ID_BUTTON_USER0,		EVENT_INFO_BUTTON_USER0, 	0 },
	{GPIO_ID_BUTTON_USER1,		EVENT_INFO_BUTTON_USER1,  0 },
	{GPIO_ID_BUTTON_WAKE_UP,	EVENT_INFO_BUTTON_WAKEUP, 0 },
};

/**
 * @brief Function that initializes all the Buttons on the board
 * 
 * @return int returnvalue of the GPIO_initialize Function
 */
int ButtonInitialize(){
  int retval = 0;
  
	// initialize all the buttons
	for(int i =0; i<NUM_OF_BUTTON; i++){
		// retain the return values of all the Init Calls
		// this may alter the real retval but retains an error if one occurs
		retval |= Gpio_Initialize(Buttons[i].GPIO_ID ,GPIO_MODE_INPUT,false);
	}

  return retval;
}

/**
 * @brief Function that Checks wheter a button is pressed or not
 * 
 * @param id ID of the Button to check 
 * @param counter Internal pressed Counter of the button
 * @return true Button was pressen for 100ms to 1000ms 
 * @return false Button wa not pressed or pressen too long 
 */
static bool Check_Button(const int id, int * counter){
	
	bool retval = false;
	bool ButtonState=false;
	
	// Get the State of the GPIO Pin
	Gpio_Get(id, &ButtonState);
	
	// if button is pressed increment the counter
	if(ButtonState){
		(*counter) ++;
	}
	
	// on release of the button determin if it was a valid press
	if(ButtonState == false){
		if((*counter) > MIN_TIME_BUTTON_VALID && (*counter) < MAX_TIME_BUTTON_VALID){
			retval = true;
		}
		else{
			retval = false;
		}
		
		*counter = 0;
	}
	
	return retval;
}


#define UNUSED(x) (void)(x)

/**
 * @brief Callback which is periodically called to deterimin if a button is pressed
 * 
 * @param IDUnsued Unused ID -> must be there because the Functionpointer must be type of Callback
 */
void Button_Check_Callback(int IDUnsued){

	UNUSED(IDUnsued);

	// Check all the buttons
	for(int i=0;i<NUM_OF_BUTTON;i++){
		if(Check_Button(Buttons[i].GPIO_ID,&Buttons[i].cntBtnPressed)){
			Event_t Event;

			Event.EventType = EVENT_BUTTON_PRESSED;
			Event.X 				= Buttons[i].EVENT_INFO;
			Event.Y					= 0;
			Event.Z					= 0;
			
			// Store an event if a button was pressed
			EventQueue_Put(&Event);
		}
	}
}

/**
 * @brief Function that starts the Button Sample Task
 * 
 * @param id ID of the Timer which is used to trigger the sampling of the button
 * @return int 
 */
int  StartButtonSampleTask(int id){

  int retval =0 ;

  retval |=  Timer_Initialize(id);

  // Start Timer with Interrupt Cycletime 50 ms with priodic Relead
  retval |= Timer_Enable(id,Button_Check_Callback,BUTTON_CHECK_INTERVAL,TIME_UNIT_MS,true);

  return retval;
}


