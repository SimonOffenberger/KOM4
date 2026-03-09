#ifndef GPIO_H
#define GPIO_H


// Include C standard library header files.
#include <stdbool.h>





/*******************************************************************************
Defines the IDs for all GPIOs.

Enumeration constants may use the same numeric (int) value;
For example, there is a name for the GPIO pin A0 (e.g. GPIO_ID_PA0)
and a name for the signal connected to it (e.g. GPIO_ID_BUTTON_WAKE_UP).
*******************************************************************************/
typedef enum Gpio_Id_e {
  GPIO_ID_PA0  = 0x00, GPIO_ID_BUTTON_WAKE_UP = GPIO_ID_PA0,
  GPIO_ID_PA1  = 0x01,
  GPIO_ID_PA2  = 0x02,
  GPIO_ID_PA3  = 0x03,
  GPIO_ID_PA4  = 0x04,
  GPIO_ID_PA5  = 0x05,
  GPIO_ID_PA6  = 0x06,
  GPIO_ID_PA7  = 0x07,
  GPIO_ID_PA8  = 0x08,
  GPIO_ID_PA9  = 0x09,
  GPIO_ID_PA10 = 0x0A,
  GPIO_ID_PA11 = 0x0B,
  GPIO_ID_PA12 = 0x0C,
  GPIO_ID_PA13 = 0x0D,
  GPIO_ID_PA14 = 0x0E,
  GPIO_ID_PA15 = 0x0F,
  GPIO_ID_PB0  = 0x10,
  GPIO_ID_PB1  = 0x11,
  GPIO_ID_PB2  = 0x12,
  GPIO_ID_PB3  = 0x13,
  GPIO_ID_PB4  = 0x14,
  GPIO_ID_PB5  = 0x15,
  GPIO_ID_PB6  = 0x16,
  GPIO_ID_PB7  = 0x17,
  GPIO_ID_PB8  = 0x18,
  GPIO_ID_PB9  = 0x19,
  GPIO_ID_PB10 = 0x1A,
  GPIO_ID_PB11 = 0x1B,
  GPIO_ID_PB12 = 0x1C,
  GPIO_ID_PB13 = 0x1D,
  GPIO_ID_PB14 = 0x1E,
  GPIO_ID_PB15 = 0x1F,
  GPIO_ID_PC0  = 0x20, GPIO_ID_LED3 = GPIO_ID_PC0,
  GPIO_ID_PC1  = 0x21, GPIO_ID_LED4 = GPIO_ID_PC1,
  GPIO_ID_PC2  = 0x22, GPIO_ID_LED5 = GPIO_ID_PC2,
  GPIO_ID_PC3  = 0x23,
  GPIO_ID_PC4  = 0x24, GPIO_ID_POTENTIOMETER = GPIO_ID_PC4,
  GPIO_ID_PC5  = 0x25,
  GPIO_ID_PC6  = 0x26, GPIO_ID_LED6R = GPIO_ID_PC6,
  GPIO_ID_PC7  = 0x27, GPIO_ID_LED6G = GPIO_ID_PC7,
  GPIO_ID_PC8  = 0x28, GPIO_ID_LED6B = GPIO_ID_PC8, 
  GPIO_ID_PC9  = 0x29,
  GPIO_ID_PC10 = 0x2A, GPIO_ID_UART_X3_TX = GPIO_ID_PC10,
  GPIO_ID_PC11 = 0x2B, GPIO_ID_UART_X3_RX = GPIO_ID_PC11,
  GPIO_ID_PC12 = 0x2C,
  GPIO_ID_PC13 = 0x2D,
  GPIO_ID_PC14 = 0x2E,
  GPIO_ID_PC15 = 0x2F,
  GPIO_ID_PD0  = 0x30, GPIO_ID_BUTTON_USER0 = GPIO_ID_PD0,
  GPIO_ID_PD1  = 0x31, GPIO_ID_BUTTON_USER1 = GPIO_ID_PD1,
  GPIO_ID_PD2  = 0x32,
  GPIO_ID_PD3  = 0x33,
  GPIO_ID_PD4  = 0x34,
  GPIO_ID_PD5  = 0x35,
  GPIO_ID_PD6  = 0x36,
  GPIO_ID_PD7  = 0x37,
  GPIO_ID_PD8  = 0x38, GPIO_ID_UART_VCOM_TX = GPIO_ID_PD8,
  GPIO_ID_PD9  = 0x39, GPIO_ID_UART_VCOM_RX = GPIO_ID_PD9,
  GPIO_ID_PD10 = 0x3A,
  GPIO_ID_PD11 = 0x3B,
  GPIO_ID_PD12 = 0x3C,
  GPIO_ID_PD13 = 0x3D,
  GPIO_ID_PD14 = 0x3E,
  GPIO_ID_PD15 = 0x3F,
  GPIO_ID_PE0  = 0x40, GPIO_ID_TFT_D0 = GPIO_ID_PE0,
  GPIO_ID_PE1  = 0x41, GPIO_ID_TFT_D1 = GPIO_ID_PE1,
  GPIO_ID_PE2  = 0x42, GPIO_ID_TFT_D2 = GPIO_ID_PE2,
  GPIO_ID_PE3  = 0x43, GPIO_ID_TFT_D3 = GPIO_ID_PE3,
  GPIO_ID_PE4  = 0x44, GPIO_ID_TFT_D4 = GPIO_ID_PE4,
  GPIO_ID_PE5  = 0x45, GPIO_ID_TFT_D5 = GPIO_ID_PE5,
  GPIO_ID_PE6  = 0x46, GPIO_ID_TFT_D6 = GPIO_ID_PE6,
  GPIO_ID_PE7  = 0x47, GPIO_ID_TFT_D7 = GPIO_ID_PE7,
  GPIO_ID_PE8  = 0x48, GPIO_ID_TFT_CS = GPIO_ID_PE8,
  GPIO_ID_PE9  = 0x49, GPIO_ID_TFT_DC = GPIO_ID_PE9,
  GPIO_ID_PE10 = 0x4A, GPIO_ID_TFT_WR = GPIO_ID_PE10,
  GPIO_ID_PE11 = 0x4B, GPIO_ID_TFT_RD = GPIO_ID_PE11,
  GPIO_ID_PE12 = 0x4C, GPIO_ID_TOUCH_CS     = GPIO_ID_PE12,
  GPIO_ID_PE13 = 0x4D, GPIO_ID_TOUCH_DCLK   = GPIO_ID_PE13,
  GPIO_ID_PE14 = 0x4E, GPIO_ID_TOUCH_DOUT   = GPIO_ID_PE14,
  GPIO_ID_PE15 = 0x4F, GPIO_ID_TOUCH_DIN    = GPIO_ID_PE15,
  GPIO_ID_PF0  = 0x50,
  GPIO_ID_PF1  = 0x51,
  GPIO_ID_PF2  = 0x52, GPIO_ID_TOUCH_PENIRQ = GPIO_ID_PF2,
  GPIO_ID_PF3  = 0x53, GPIO_ID_TOUCH_BUSY   = GPIO_ID_PF3,
  GPIO_ID_PF4  = 0x54,
  GPIO_ID_PF5  = 0x55,
  GPIO_ID_PF6  = 0x56,
  GPIO_ID_PF7  = 0x57,
  GPIO_ID_PF8  = 0x58,
  GPIO_ID_PF9  = 0x59,
  GPIO_ID_PF10 = 0x5A, GPIO_ID_TFT_RES = GPIO_ID_PF10,
  GPIO_ID_PF11 = 0x5B,
  GPIO_ID_PF12 = 0x5C,
  GPIO_ID_PF13 = 0x5D,
  GPIO_ID_PF14 = 0x5E,
  GPIO_ID_PF15 = 0x5F
} Gpio_Id_t;

/*******************************************************************************
Defines the available modes for GPIO pins.
*******************************************************************************/
typedef enum Gpio_Mode_e {
   GPIO_MODE_OUTPUT,
   GPIO_MODE_OUTPUT_OPEN_DRAIN,
   GPIO_MODE_ALTERNATE_FUNCTION,
   GPIO_MODE_ALTERNATE_FUNCTION_OPEN_DRAIN,
   GPIO_MODE_INPUT_ANALOG,
   GPIO_MODE_INPUT,
   GPIO_MODE_INPUT_PULL_DOWN,
   GPIO_MODE_INPUT_PULL_UP
} Gpio_Mode_t;

/*******************************************************************************
Initializes the specified GPIO pin.

id: The ID of the GPIO pin.
mode: The mode in which the GPIO pin will be used.
value: The output value of the GPIO pin; ignored if the GPIO pin is used as input pin.
  false: The pin output is set to low.
  true: The pin output is set to high. 

returns: Zero on success, an error code otherwise.
	0: Success
  1: GPIO pin not supported by this driver.
	2: mode is invalid

The GPIO pin is not intialized if an error code is returned.
*******************************************************************************/
int Gpio_Initialize(int id, Gpio_Mode_t mode, bool value);

/*******************************************************************************
Sets the value of the specified GPIO pin.

id: The ID of the GPIO pin.
value: The output value of the GPIO pin.
  false: The pin output is set to low.
  true: The pin output is set to high. 
	
returns: Zero on success, an error code otherwise.
	0: Success
  1: GPIO pin not supported by this driver.

The pin output is not changed if an error code is returned.
*******************************************************************************/
int Gpio_Set(int id, bool value);

/*******************************************************************************
Gets the value of the specified GPIO pin.

id: The ID of the GPIO pin.
value: The value of the pin.
  false: The pin is currently low.
  true: The pin is currently high. 

returns: Zero on success, an error code otherwise.
	0: Success
  1: GPIO pin not supported by this driver.
*******************************************************************************/
int Gpio_Get(int id, bool * value);


#endif // GPIO_H

