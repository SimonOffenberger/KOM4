/**
 * @author Simon Offenberger
 * @version 0.1
 * @date 2025-10-25
 * 
 */
#ifndef CRITITCAL_SECTION_HPP
#define CRITITCAL_SECTION_HPP

#include <stdint.h>

/**
 * @brief Enter Critical Section by disabling Interrupts
 * 
 * @return uint32_t Primmask
 */
uint32_t CriticalSection_Enter(void);

/**
 * @brief Reenable Interrupts if they where previously enabled
 * 
 * @param primask Primask
 */
void CriticalSection_Exit(const uint32_t primask);

#endif