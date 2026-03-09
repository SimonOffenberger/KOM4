/**
 * @author Simon Offenberger
 * @version 0.1
 * @date 2025-10-25
 * 
 */
#include "CriticalSection.h"
#include <stm32f072xb.h>

/**
 * @brief Enter Critical Section by disabling Interrupts
 * 
 * @return uint32_t Primmask
 */
uint32_t CriticalSection_Enter(void){
  const uint32_t primmask  = __get_PRIMASK();
  __disable_irq();
  return primmask;
}

/**
 * @brief Reenable Interrupts if they where previously enabled
 * 
 * @param primask Primask
 */
void CriticalSection_Exit(const uint32_t primask){
  if(!primask){
    __enable_irq();
  }
}