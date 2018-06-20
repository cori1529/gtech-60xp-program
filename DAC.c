/*
	dac.c
*/

#include "stm32f10x_gpio.h"

#include "delay.h"

void dac_data(u16 outdata)
{
	char k;
	
	for(k=0; k<16; k++)
	{
		if(((outdata<<k)&0x8000) == 0) GPIO_WriteBit(GPIOB, GPIO_Pin_9, Bit_RESET);
		else GPIO_WriteBit(GPIOB, GPIO_Pin_9, Bit_SET);;

		TM_DelayMicros(2);	//	Delay_us(1);

		GPIO_WriteBit(GPIOD, GPIO_Pin_2, Bit_SET);
		TM_DelayMicros(2);	//	Delay_us(1);
		GPIO_WriteBit(GPIOD, GPIO_Pin_2, Bit_RESET);
//		TM_DelayMicros(2);	//	Delay_us(1);
	}

	GPIO_WriteBit(GPIOC, GPIO_Pin_12, Bit_SET);	
	TM_DelayMicros(2);	//	Delay_us(1);
	GPIO_WriteBit(GPIOC, GPIO_Pin_12, Bit_RESET);	
}

/* end of file */
