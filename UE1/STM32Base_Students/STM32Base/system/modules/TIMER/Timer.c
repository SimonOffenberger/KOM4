// Include module header files.
#include "Timer.h"

// Include toolchain header files.
#include <stm32f072xb.h>

// Include C standard library header files.
#include <stddef.h>
#include <stdbool.h>


/*******************************************************************************
  Private
*******************************************************************************/

typedef struct Timer_Context_s {
  volatile uint32_t * const ClockEnableRegister;
  const uint32_t            ClockEnable;
  volatile uint32_t * const ResetRegister;
  const uint32_t            Reset;
  TIM_TypeDef * const       Timer;
  IRQn_Type const           InterruptNumber;
  bool                      Initialized;
  uint16_t:16; // Prevent warning: padding struct 'struct Timer_Context_s' with 2 bytes to align 'Callback' [-Wpadded]
  Timer_Callback_t          Callback;
  uint32_t                  TickCount;
} Timer_Context_t;

#define TIMER_COUNT 9

static Timer_Context_t Timer_Context[TIMER_COUNT] = {
  
  { &RCC->APB1ENR, RCC_APB1ENR_TIM2EN,  &RCC->APB1RSTR, RCC_APB1RSTR_TIM2RST,  TIM2,  TIM2_IRQn,     false, NULL, 0 },
  { &RCC->APB1ENR, RCC_APB1ENR_TIM3EN,  &RCC->APB1RSTR, RCC_APB1RSTR_TIM3RST,  TIM3,  TIM3_IRQn,     false, NULL, 0 },
  
  { &RCC->APB1ENR, RCC_APB1ENR_TIM6EN,  &RCC->APB1RSTR, RCC_APB1RSTR_TIM6RST,  TIM6,  TIM6_DAC_IRQn, false, NULL, 0 },
  { &RCC->APB1ENR, RCC_APB1ENR_TIM7EN,  &RCC->APB1RSTR, RCC_APB1RSTR_TIM7RST,  TIM7,  TIM7_IRQn,     false, NULL, 0 },
  
  { &RCC->APB1ENR, RCC_APB1ENR_TIM14EN, &RCC->APB1RSTR, RCC_APB1RSTR_TIM14RST, TIM14, TIM14_IRQn,    false, NULL, 0 },
  
  { &RCC->APB2ENR, RCC_APB2ENR_TIM15EN, &RCC->APB2RSTR, RCC_APB2RSTR_TIM15RST, TIM15, TIM15_IRQn,    false, NULL, 0 },
  { &RCC->APB2ENR, RCC_APB2ENR_TIM16EN, &RCC->APB2RSTR, RCC_APB2RSTR_TIM16RST, TIM16, TIM16_IRQn,    false, NULL, 0 },
  { &RCC->APB2ENR, RCC_APB2ENR_TIM17EN, &RCC->APB2RSTR, RCC_APB2RSTR_TIM17RST, TIM17, TIM17_IRQn,    false, NULL, 0 }
};
                
static inline Timer_Context_t * Timer_GetContext(const int id)
{
  if(id < 0 || id >= TIMER_COUNT)
  {
    return NULL;
  }
  
  return &Timer_Context[id];
}


/*******************************************************************************
  Public
*******************************************************************************/

int Timer_Initialize(int id)
{
  volatile Timer_Context_t * context = Timer_GetContext(id);
  if(context == NULL)
  {
    return -1;
  }
  
  // Activate reset.
  *context->ResetRegister |= context->Reset;
  
  // Enable peripheral clock.
  *context->ClockEnableRegister |= context->ClockEnable;
  
  // Deactivate reset.
  *context->ResetRegister &= ~context->Reset;
  
  // Configure and enable interrupt (NVIC).
  NVIC_SetPriority(context->InterruptNumber, 1);
  NVIC_EnableIRQ(context->InterruptNumber);
  
  context->Initialized = true;
  
  return 0;
}

int Timer_Enable(int id, Timer_Callback_t callback, int interval, Time_Unit_t unit, bool periodic)
{
  Timer_Context_t * context = Timer_GetContext(id);
  if(context == NULL)
  {
    return 1;
  }
  if(!context->Initialized || (context->Timer->CR1 & TIM_CR1_CEN))
  {
    return 2;
  }
  if(interval < 1)
  {
    return 3;
  }
  
  // 1 s  ... 48,000,000 cycles
  // 1 ms ...     48,000 cycles
  // 1 us ...         48 cycles
  uint16_t psc;
  switch(unit)
  {
    case TIME_UNIT_US:
      psc = 48-1;
      break;
      
    case TIME_UNIT_MS:
      psc = 48000-1;
      break;
      
    default:
      return -3;
  }
  
  // Configure timer.
  context->Timer->PSC = psc;
  context->Timer->CNT = 0x0000;
  context->Timer->ARR = ((uint32_t)interval - 1);
  context->Timer->SR &= ~TIM_SR_UIF;
  context->Timer->DIER |= TIM_DIER_UIE;
  
  // Reset context.
  context->Callback = callback;
  context->TickCount = 0;
  
  // Configure and enable timer.
  uint32_t cr1 = context->Timer->CR1 & ~(
      TIM_CR1_ARPE_Msk
    | TIM_CR1_OPM_Msk
    | TIM_CR1_URS_Msk
    | TIM_CR1_UDIS_Msk
    | TIM_CR1_CEN_Msk );
  if(!periodic)
  {
    cr1 |= TIM_CR1_OPM;
  }
  cr1 |= TIM_CR1_CEN;
  context->Timer->CR1 = cr1;
  
  return 0;
}

int Timer_Disable(int id)
{
  Timer_Context_t * context = Timer_GetContext(id);
  if(context == NULL)
  {
    return -1;
  }
  if(!context->Initialized)
  {
    return -2;
  }
  
  // Disable timer.
  context->Timer->CR1 &= ~(TIM_CR1_CEN);
  
  // Disable Update interrupt.
  context->Timer->DIER &= ~TIM_DIER_UIE;
  
  return 0;
}

int Timer_GetTickCount(int id, uint32_t * tickCount)
{
  Timer_Context_t * context = Timer_GetContext(id);
  if(context == NULL)
  {
    return -1;
  }
  if(!context->Initialized)
  {
    return -2;
  }
  if(tickCount == NULL)
  {
    return -3;
  }
  
  if(context->Timer->PSC == 0)
  {
    *tickCount = context->Timer->CNT;
  }
  else
  {
    *tickCount = context->TickCount;
  }
  
  return 0;
}

static void Timer_ISR(const int id){
  // get the context for the provided id
  Timer_Context_t * context = Timer_GetContext(id);
  
	// check if the UIF Flag is set in the Status Register
	if(context->Timer->SR & TIM_SR_UIF){
		// clear the UIF Flag if it is set
		context->Timer->SR &= ~TIM_SR_UIF;
		
		// increment Tickcount
		context->TickCount ++;
		
		// Call Calback Function if it is set
		if(context->Callback != NULL){
			context->Callback(id);
		}
	}
}

// Interrupt Handler for the Timer Peripheral
void TIM2_IRQHandler(void){Timer_ISR(TIMER_ID_TIM2);}
void TIM3_IRQHandler(void){Timer_ISR(TIMER_ID_TIM3);}
void TIM6_DAC_IRQHandler(void){Timer_ISR(TIMER_ID_TIM6);}
void TIM7_IRQHandler(void){Timer_ISR(TIMER_ID_TIM7);}
void TIM14_IRQHandler(void){Timer_ISR(TIMER_ID_TIM14);}
void TIM15_IRQHandler(void){Timer_ISR(TIMER_ID_TIM15);}
void TIM16_IRQHandler(void){Timer_ISR(TIMER_ID_TIM16);}
void TIM17_IRQHandler(void){Timer_ISR(TIMER_ID_TIM17);}
