/*
	buzzer.c

*/

#include "stm32f10x_lib.h"

#include "buzzer.h"
#include "delay.h"

void beep(unsigned long duration)
{
	buzzer_on();
	Delay_ms(duration);
	buzzer_off();
}

void buzzer_on(void)
{
  GPIO_ResetBits(BUZZER_PORT, BUZZER_PIN);
}

void buzzer_off(void)
{
  GPIO_SetBits(BUZZER_PORT, BUZZER_PIN);
}





void wireless_on(void)
{
  GPIO_SetBits(GPIOB, GPIO_Pin_8);
}

void wireless_off(void)
{
  GPIO_ResetBits(GPIOB, GPIO_Pin_8);
}

void security_on(void)
{
  GPIO_SetBits(GPIOB, GPIO_Pin_9);
}

void security_off(void)
{
  GPIO_ResetBits(GPIOB, GPIO_Pin_9);
}




 
/* end of file */
