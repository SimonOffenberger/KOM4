/**
 * @file Adc.h
 * @author Phillip Kastner
 * @brief This file was provided and downloaded from the Elearning platform of FH OOE.
 * @version 0.1
 * @date 2025-12-06
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef ADC_H
#define ADC_H


#include <stdint.h>
#include <stdbool.h>

#define ADC_VREF_VOLTAGE 3.3f

typedef enum Adc_Id_e {
  ADC_ID_CHANNEL0  =  0, ADC_ID_PA0  = ADC_ID_CHANNEL0,
  ADC_ID_CHANNEL1  =  1, ADC_ID_PA1  = ADC_ID_CHANNEL1,
  ADC_ID_CHANNEL2  =  2, ADC_ID_PA2  = ADC_ID_CHANNEL2,
  ADC_ID_CHANNEL3  =  3, ADC_ID_PA3  = ADC_ID_CHANNEL3,
  ADC_ID_CHANNEL4  =  4, ADC_ID_PA4  = ADC_ID_CHANNEL4,
  ADC_ID_CHANNEL5  =  5, ADC_ID_PA5  = ADC_ID_CHANNEL5,
  ADC_ID_CHANNEL6  =  6, ADC_ID_PA6  = ADC_ID_CHANNEL6,
  ADC_ID_CHANNEL7  =  7, ADC_ID_PA7  = ADC_ID_CHANNEL7,
  ADC_ID_CHANNEL8  =  8, ADC_ID_PB0  = ADC_ID_CHANNEL8,
  ADC_ID_CHANNEL9  =  9, ADC_ID_PB1  = ADC_ID_CHANNEL9,
  ADC_ID_CHANNEL10 = 10, ADC_ID_PC0  = ADC_ID_CHANNEL10,
  ADC_ID_CHANNEL11 = 11, ADC_ID_PC1  = ADC_ID_CHANNEL11,
  ADC_ID_CHANNEL12 = 12, ADC_ID_PC2  = ADC_ID_CHANNEL12,
  ADC_ID_CHANNEL13 = 13, ADC_ID_PC3  = ADC_ID_CHANNEL13,
  ADC_ID_CHANNEL14 = 14, ADC_ID_PC4  = ADC_ID_CHANNEL14, ADC_ID_POTENTIOMETER  = ADC_ID_PC4,
  ADC_ID_CHANNEL15 = 15, ADC_ID_PC5  = ADC_ID_CHANNEL15,
  ADC_ID_CHANNEL16 = 16, ADC_ID_TEMPERATURE = ADC_ID_CHANNEL16, // Temperature sensor
  ADC_ID_CHANNEL17 = 17, ADC_ID_VREFINT = ADC_ID_CHANNEL17, // Internal voltage reference Vrefint
  ADC_ID_CHANNEL18 = 18, ADC_ID_VBAT = ADC_ID_CHANNEL18, // VBAT battery voltage
} Adc_Id_t;

int Adc_Initialize(void);
int Adc_Calibrate(uint_fast8_t * calibrationFactor);
int Adc_Enable(void);
int Adc_Disable(void);
int Adc_Convert(int id, float * value, bool raw);


#endif // ADC_H
