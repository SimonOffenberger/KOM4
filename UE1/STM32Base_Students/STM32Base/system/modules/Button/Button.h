/**
 * @author Simon Offenberger
 * @version 0.1
 * @date 2025-10-25
 * 
 */
#ifndef BUTTON_H
#define BUTTON_H

// Num of buttons in the button Array
#define NUM_OF_BUTTON 3

// Interval of the Button Sampling
#define BUTTON_CHECK_INTERVAL 50 //ms

// Makro for Calculation the Counter for each time
#define MAX_TIME_BUTTON_VALID ((1000) / BUTTON_CHECK_INTERVAL) 
#define MIN_TIME_BUTTON_VALID ((100)  / BUTTON_CHECK_INTERVAL)

// Event Type
#define EVENT_BUTTON_PRESSED 1



// Typedef of a Button Struct
typedef struct{
  const int GPIO_ID;
  const int EVENT_INFO;
  int cntBtnPressed;
}Button_T;

/**
 * @brief Function that initializes all the Buttons on the board
 * 
 * @return int returnvalue of the GPIO_initialize Function
 */
int ButtonInitialize();


/**
 * @brief Function that starts the Button Sample Task
 * 
 * @param id ID of the Timer which is used to trigger the sampling of the button
 * @return int 
 */
int StartButtonSampleTask(int id);

#endif