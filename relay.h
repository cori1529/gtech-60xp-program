/*
	relay.h
*/

#ifndef __RELAY_H
#define __RELAY_H
																																										   
#define	RELAY_ZERO		1

#define	RELAY_HIGH		2
#define	RELAY_LOW		3

#define	RELAY_PASS		1
#define	RELAY_NG_HIGH	2
#define	RELAY_NG_LOW	3


#define	LOWER_R		RELAY_LOW
#define	UPPER_R		RELAY_HIGH
#define	PASS_R		RELAY_PASS


#define    FALLING_SIGNAL_ON           GPIO_ResetBits(GPIOC, GPIO_Pin_0);   // 낙상          신호 ON 
#define    BEFORE_FALLING_SIGNAL_ON    GPIO_ResetBits(GPIOC, GPIO_Pin_1);   // 낙상          위험 신호 ON 
#define    SAFETY_GUARD_SIGNAL_ON      GPIO_ResetBits(GPIOC, GPIO_Pin_2);   //  안전가이드   신호 ON 
#define    NO_PATIENT_SIGNAL_ON        GPIO_ResetBits(GPIOC, GPIO_Pin_3);   //  환자없음    신호 ON 


#define    FALLING_SIGNAL_OFF           GPIO_SetBits(GPIOC, GPIO_Pin_0);   // 낙상          신호 Off 
#define    BEFORE_FALLING_SIGNAL_OFF    GPIO_SetBits(GPIOC, GPIO_Pin_1);   // 낙상          위험 신호 Off 
#define    SAFETY_GUARD_SIGNAL_OFF      GPIO_SetBits(GPIOC, GPIO_Pin_2);   //  안전가이드   신호 Off 
#define    NO_PATIENT_SIGNAL_OFF        GPIO_SetBits(GPIOC, GPIO_Pin_3);   //  환자없음    신호 Off 





void No_patient_signal(void);
void Safety_signal(void);
void Falling_Warning_signal(void);
void Falling_signal(void);
void All_signal(void);
void All_signal_Off(void);


void relay_out(void);
void relay_on(char relay);
void relay_off(char relay);
void relay_all_off(void);


void  fet_33_on(void);   // 3.3 volt enable 
void  fet_5d_on(void);   // digital 5volt enable
void  fet_5a_on(void);   // analog 5 volt enable

void  fet_33_off(void);   // 3.3 volt disable 
void  fet_5d_off(void);   // digital 5volt disable
void  fet_5a_off(void);   // analog 5 volt disable




#endif //#ifndef __RELAY_H

/* end of file */
