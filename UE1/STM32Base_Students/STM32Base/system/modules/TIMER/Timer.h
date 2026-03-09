#ifndef TIMER_H
#define TIMER_H


#include <stdbool.h>
#include <stdint.h>


typedef enum Time_Unit_e {
  TIME_UNIT_US,
  TIME_UNIT_MS
} Time_Unit_t;

typedef enum Timer_Id_e {
  TIMER_ID_TIM2  = 0,
  TIMER_ID_TIM3  = 1,
  TIMER_ID_TIM6  = 2,
  TIMER_ID_TIM7  = 3,
  TIMER_ID_TIM14 = 4,
  TIMER_ID_TIM15 = 5,
  TIMER_ID_TIM16 = 6,
  TIMER_ID_TIM17 = 7
} Timer_Id_t;

typedef void (*Timer_Callback_t)(int id);

int Timer_Initialize(int id);
int Timer_Enable(int id, Timer_Callback_t callback, int interval, Time_Unit_t unit, bool periodic);
int Timer_Disable(int id);
int Timer_GetTickCount(int id, uint32_t * tickCount);


#endif // TIMER_H
