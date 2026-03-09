#include "Gpio.h"

#include <stm32f072xb.h>

#include <stddef.h>
#include <stdbool.h>


#define MODER_MASK(PIN)         (((uint32_t)3) << ((PIN) * 2))
#define MODER_VALUE(PIN, VALUE) ((((uint32_t)(VALUE)) << ((PIN) * 2)) & MODER_MASK(PIN))

#define OTYPER_MASK(PIN)         (((uint32_t)1) << (PIN))
#define OTYPER_VALUE(PIN, VALUE) ((((uint32_t)(VALUE)) << (PIN)) & OTYPER_MASK(PIN))

#define PUPDR_MASK(PIN)         (((uint32_t)3) << ((PIN) * 2))
#define PUPDR_VALUE(PIN, VALUE) ((((uint32_t)(VALUE)) << ((PIN) * 2)) & PUPDR_MASK(PIN))

#define OSPEEDR_MASK(PIN)         (((uint32_t)3) << ((PIN) * 2))
#define OSPEEDR_VALUE(PIN, VALUE) ((((uint32_t)(VALUE)) << ((PIN) * 2)) & OSPEEDR_MASK(PIN))


/**********************************************************************************************
Validates the specified GPIO id and returns the associated GPIO peripheral and pin number.

id: The ID of the GPIO pin.
gpio: The GPIO peripheral associated with the GPIO pin.
pin: The pin number of the GPIO pin.

returns: Zero on success, an error code otherwise.
	0: Success
  1: Invalid ID
  2: GPIO pin not supported by this driver
	3: gpio is NULL
	4: pin is NULL
**********************************************************************************************/
static int GetContext(int id, GPIO_TypeDef * * gpio, uint_fast8_t * pin)
{
  switch(id >> 4)
  {
    case 0: // GPIOA
      switch(id & 0xF)
      {
        case 13:
        case 14:
          // PA13 and PA14 are used for SW debug port.
          return 2;
      }
      (*gpio) = GPIOA;
      break;

    case 1: // GPIOB
      (*gpio) = GPIOB;
      break;

    case 2: // GPIOC
      (*gpio) = GPIOC;
      break;

    case 3: // GPIOD
      (*gpio) = GPIOD;
      break;

    case 4: // GPIOE
      (*gpio) = GPIOE;
      break;

    case 5: // GPIOF
      (*gpio) = GPIOF;
      break;

    default:
      (*gpio) = NULL;
      (*pin) = 0;
      return 1;
  }
	
  if(gpio == NULL)
  {
    return 3;
  }

  if(pin == NULL)
  {
    return 4;
  }

  (*pin) = (id & 0xF);
  return 0;
}


int Gpio_Initialize(int id, Gpio_Mode_t mode, bool value)
{
	// Map the provided id to the actual GPIO peripheral and pin number.
  GPIO_TypeDef * gpio;
  uint_fast8_t pin;
  if(id < 0 || GetContext(id, &gpio, &pin) != 0)
  {
    return 1;
  }

  switch(mode)
  {
    case GPIO_MODE_OUTPUT:
    case GPIO_MODE_OUTPUT_OPEN_DRAIN:
    case GPIO_MODE_ALTERNATE_FUNCTION:
    case GPIO_MODE_ALTERNATE_FUNCTION_OPEN_DRAIN:
    case GPIO_MODE_INPUT_ANALOG:
    case GPIO_MODE_INPUT:
    case GPIO_MODE_INPUT_PULL_DOWN:
    case GPIO_MODE_INPUT_PULL_UP:
      break;

    default:
      return 2;
  }

  // Enable peripheral clock.
  switch(id >> 4)
  {
    case 0: // GPIOA
      RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
      break;

    case 1: // GPIOB
      RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
      break;

    case 2: // GPIOC
      RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
      break;

    case 3: // GPIOD
      RCC->AHBENR |= RCC_AHBENR_GPIODEN;
      break;

    case 4: // GPIOE
      RCC->AHBENR |= RCC_AHBENR_GPIOEEN;
      break;

    case 5: // GPIOF
      RCC->AHBENR |= RCC_AHBENR_GPIOFEN;
      break;
  }

  // Determine configuration.
  uint_fast32_t moder;
  uint_fast32_t otyper;
  uint_fast32_t ospeedr = 3; // 11
  uint_fast32_t pupdr;
  switch(mode)
  {
    case GPIO_MODE_OUTPUT:
      // Set output register (ODR) value via port bit set/reset register (BSRR) to prevent race conditions.
      gpio->BSRR = (1 << (value ? pin : (pin + 16)));
      moder  = 1; // 01
      otyper = 0; //  0
      pupdr  = 0; // 00
      break;

    case GPIO_MODE_OUTPUT_OPEN_DRAIN:
      // Set output register (ODR) value via port bit set/reset register (BSRR) to prevent race conditions.
      gpio->BSRR = (1 << (value ? pin : (pin + 16)));
      moder  = 1; // 01
      otyper = 1; //  1
      pupdr  = 0; // 00
      break;

    case GPIO_MODE_ALTERNATE_FUNCTION:
      moder  = 2; // 10
      otyper = 0; //  0
      pupdr  = 0; // 00
      break;

    case GPIO_MODE_ALTERNATE_FUNCTION_OPEN_DRAIN:
      moder  = 2; // 10
      otyper = 1; //  1
      pupdr  = 0; // 00
      break;

    case GPIO_MODE_INPUT_ANALOG:
      moder  = 3; // 11
      otyper = 0; //  -
      pupdr  = 0; // 00
      break;

    case GPIO_MODE_INPUT:
      moder  = 0; // 00
      otyper = 0; //  -
      pupdr  = 0; // 00
      break;

    case GPIO_MODE_INPUT_PULL_DOWN:
      moder  = 0; // 00
      otyper = 0; //  -
      pupdr  = 2; // 10
      break;

    case GPIO_MODE_INPUT_PULL_UP:
      moder  = 0; // 00
      otyper = 0; //  -
      pupdr  = 0; // 01
      break;
  }
  
  // Apply configuration.
  // Note: ODR has already been set via BSRR above.
  gpio->OTYPER  = ((gpio->OTYPER  & (~OTYPER_MASK(pin)))  | OTYPER_VALUE(pin, otyper));
  gpio->PUPDR   = ((gpio->PUPDR   & (~PUPDR_MASK(pin)))   | PUPDR_VALUE(pin, pupdr));
  gpio->OSPEEDR = ((gpio->OSPEEDR & (~OSPEEDR_MASK(pin))) | OSPEEDR_VALUE(pin, ospeedr));
  gpio->MODER   = ((gpio->MODER   & (~MODER_MASK(pin)))   | MODER_VALUE(pin, moder));
  
  return 0;
}

int Gpio_Set(int id, bool value)
{
  GPIO_TypeDef * gpio;
  uint_fast8_t pin;
  if(id < 0 || GetContext(id, &gpio, &pin) != 0)
  {
    return 1;
  }

  // Set output register (ODR) value via port bit set/reset register (BSRR) to prevent race conditions.
  gpio->BSRR = (1 << (value ? pin : (pin + 16)));
  return 0;
}

int Gpio_Get(int id, bool * value)
{
  GPIO_TypeDef * gpio;
  uint_fast8_t pin;
  if(id < 0 || GetContext(id, &gpio, &pin) != 0)
  {
    return 1;
  }

  (*value) = (gpio->IDR & (1 << pin)) != 0;
  return 0;
}
