/**
  ******************************************************************************
  * @file    3bit_cnt.h
  * @author  Simon Offenberger
  * @version V1.0
  * @date    14-October-2025
  * @brief   APUS Board 3Bit Cnt Header.
  ******************************************************************************
  */

#ifndef __ALARM_H
#define __ALARM_H

#include <stdint.h>



#define STATE_POWER_ON		0
#define STATE_ALARM_OFF 	1
#define STATE_ALARM_ON 		2

// main of the project 3 Bit Counter
void main_alarm(void);

#endif /* __ALARM_H */
