/*
	buzzer.h
*/

#ifndef __BUZZER_H
#define __BUZZER_H
				
#define	BUZZER_PORT		GPIOC
//#define	BUZZER_PIN		GPIO_Pin_10 	//pcb version 0.3
#define	BUZZER_PIN		GPIO_Pin_14 	//pcb version PP후 수정할 것
									  
void beep(unsigned long duration);
void buzzer_on(void);
void buzzer_off(void);


void wireless_on(void);
void wireless_off(void);

void security_on(void);
void security_off(void);





#endif // BUZZER_H

/* end of file */
