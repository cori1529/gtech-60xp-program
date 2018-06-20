/*
	delay.h

*/

#ifndef	__DELAY_H
#define	__DELAY_H

#include "stm32f10x_type.h"

extern vu32 multiplier;

void TM_Delay_Init(void);
void TM_DelayMicros(u32 micros);
void TM_DelayMillis(u32 millis);

void Delay_us(u32 nTime);
void Delay_ms(u32 nTime);
void TimingDelay_Decrement(void);

 
#endif

/* end of file */
