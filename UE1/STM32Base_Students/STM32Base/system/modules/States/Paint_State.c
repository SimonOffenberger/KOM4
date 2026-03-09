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
#include "Paint_State.h"
#include "EventQueue.h"
#include <stm32f072xb.h>

// Forward Declaration
void Enter_Paint_State(void);
struct State_s const *Paint_State_Process(void);
void Exit_Paint_State(void);

/** @brief Paint State Definition */
State_t Paint_State = {
		.Enter = Enter_Paint_State,
		.Exit = Exit_Paint_State,
		.Process = Paint_State_Process};

/**
 * @brief On Enter Paint State -> Clear Screen
 * @brief and Initialize User Button for Erase Functionality
 */
void Enter_Paint_State(void)
{
	// Clear Screen to White
	Lcd_Fill(0, 0, LCD_WIDTH, LCD_HEIGHT, RGB565_WHITE);

	// Initialize User Button for Erase Functionality
	Gpio_Initialize(GPIO_ID_BUTTON_USER0, GPIO_MODE_INPUT, false);
}

#define AVG_SAMPLES 16

/**
 * @brief Paint State Process Function
 *
 * @return Next State Pointer = Paint State (stay in this state)
 */
struct State_s const *Paint_State_Process(void)
{
	bool touched = false;
	bool erase_button = false;
	uint16_t x = 0;
	uint16_t y = 0;

	// Read TSC State and Coordinates Scaled to match LCD Coordinates
	Tsc_Read_scaled(&touched, &x, &y, AVG_SAMPLES);

	// Read Erase Button State
	Gpio_Get(GPIO_ID_BUTTON_USER0, &erase_button);

	// If Touched Paint on the Screen
	if (touched)
	{
		// Erase Mode if Button is Pressed
		if (erase_button)
		{
			// underflow is not a problem here as Lcd_Fill will clip the values!
			Lcd_Fill(x-PAINT_ERASE_SIZE/2, y-PAINT_ERASE_SIZE/2, PAINT_ERASE_SIZE, PAINT_ERASE_SIZE, RGB565_WHITE);
		}
		// Normal Paint Mode
		else
		{
			Lcd_Fill(x-PAINT_SQUARE_SIZE/2, y-PAINT_SQUARE_SIZE/2, PAINT_SQUARE_SIZE, PAINT_SQUARE_SIZE, RGB565_BLUE);
		}
	}

	// Stay in Paint State
	return &Paint_State;
}

/**
 * @brief On Exit Paint State -> No specific action
 *
 */
void Exit_Paint_State(void)
{
}
