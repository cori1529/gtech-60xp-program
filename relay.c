/*
	relay.c
*/


#include "stm32f10x_lib.h"

#include "relay.h"
#include "usart.h"


void relay_out()
{
}

/*
void relay_on(char relay)
{
	switch(relay)
	{
		case 1: GPIO_SetBits(GPIOB, GPIO_Pin_0); break;
		case 2: GPIO_SetBits(GPIOB, GPIO_Pin_1); break;
		case 3: GPIO_SetBits(GPIOB, GPIO_Pin_8); break;


	}
}

void relay_off(char relay)
{
	switch(relay)
	{
		case 1: GPIO_ResetBits(GPIOB, GPIO_Pin_0); break;
		case 2: GPIO_ResetBits(GPIOB, GPIO_Pin_1); break;
		case 3: GPIO_ResetBits(GPIOB, GPIO_Pin_8); break;
	}
}

void relay_all_off()
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_0);
	GPIO_ResetBits(GPIOB, GPIO_Pin_1);
	GPIO_ResetBits(GPIOB, GPIO_Pin_8);
}

*/


void relay_on(char relay)
{
	switch(relay)
	{
		case 1: GPIO_ResetBits(GPIOC, GPIO_Pin_0); break;
		case 2: GPIO_ResetBits(GPIOC, GPIO_Pin_1); break;
		case 3: GPIO_ResetBits(GPIOC, GPIO_Pin_2); break;
		case 4: GPIO_ResetBits(GPIOC, GPIO_Pin_3); break;
	}
}

void relay_off(char relay)
{
	switch(relay)
	{
		case 1: GPIO_SetBits(GPIOC, GPIO_Pin_0); break;
		case 2: GPIO_SetBits(GPIOC, GPIO_Pin_1); break;
		case 3: GPIO_SetBits(GPIOC, GPIO_Pin_2); break;
		case 4: GPIO_SetBits(GPIOC, GPIO_Pin_3); break;
	}
}

void relay_all_off()
{
		 GPIO_SetBits(GPIOC, GPIO_Pin_0);// break;
		 GPIO_SetBits(GPIOC, GPIO_Pin_1);// break;
		 GPIO_SetBits(GPIOC, GPIO_Pin_2);// break;
		 GPIO_SetBits(GPIOC, GPIO_Pin_3);// break;



}


/**************GI-1400*********************/

void  fet_33_on(void)   // 3.3 volt enable 
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_11);
}	

void  fet_5d_on(void)   // digital 5 volt  volt enable 
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_12);
}	

void  fet_5a_on(void)   // analog 5 volt enable 
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_14);
}	


void  fet_33_off(void)   // 3.3 volt enable 
{
	GPIO_SetBits(GPIOA, GPIO_Pin_11);
}	

void  fet_5d_off(void)   // digital 5 volt  volt enable 
{
	GPIO_SetBits(GPIOA, GPIO_Pin_12);
}	

void  fet_5a_off(void)   // analog 5 volt enable 
{
	GPIO_SetBits(GPIOA, GPIO_Pin_14);
}	







/*
void  fet_5d_on(void);   // digital 5volt enable
void  fet_5a_on(void);   // analog 5 volt enable

void  fet_33_off(void);   // 3.3 volt disable 
void  fet_5d_off(void);   // digital 5volt disable
void  fet_5a_off(void);   // analog 5 volt disable
*/









/*
#define    FALLING_SIGNAL_ON           GPIO_ResetBits(GPIOE, GPIO_Pin_0);   // ����          ��ȣ ON 
#define    BEFORE_FALLING_SIGNAL_ON    GPIO_ResetBits(GPIOE, GPIO_Pin_1);   // ����          ���� ��ȣ ON 
#define    SAFETY_GUARD_SIGNAL_ON      GPIO_ResetBits(GPIOE, GPIO_Pin_2);   //  �������̵�   ��ȣ ON 
#define    NO_PATIENT_SIGNAL_ON        GPIO_ResetBits(GPIOE, GPIO_Pin_3);   //  ȯ�ھ���    ��ȣ ON 


#define    FALLING_SIGNAL_OFF           GPIO_SetBits(GPIOE, GPIO_Pin_0);   // ����          ��ȣ Off 
#define    BEFORE_FALLING_SIGNAL_OFF    GPIO_SetBits(GPIOE, GPIO_Pin_1);   // ����          ���� ��ȣ Off 
#define    SAFETY_GUARD_SIGNAL_OFF      GPIO_SetBits(GPIOE, GPIO_Pin_2);   //  �������̵�   ��ȣ Off 
#define    NO_PATIENT_SIGNAL_OFF        GPIO_SetBits(GPIOE, GPIO_Pin_3);   //  ȯ�ھ���    ��ȣ Off 
 */


#define SIGNAL_ON_DELAY  200

void No_patient_signal(void)
    {
	   NO_PATIENT_SIGNAL_ON
       USART1_puts("Signal of no patient !");
	   Delay_ms(SIGNAL_ON_DELAY );
	   NO_PATIENT_SIGNAL_OFF
	}
void Safety_signal(void)
    {
	  SAFETY_GUARD_SIGNAL_ON
      USART1_puts("Signal of safety guard !");
	  Delay_ms(SIGNAL_ON_DELAY );
	  SAFETY_GUARD_SIGNAL_OFF
	}

void Falling_Warning_signal(void)
    {
      USART1_puts("Signal of falling warning !");
	  BEFORE_FALLING_SIGNAL_ON
	  Delay_ms(SIGNAL_ON_DELAY );
	  BEFORE_FALLING_SIGNAL_OFF
	}


void Falling_signal(void)
    {
	   USART1_puts("Signal of falling !!");
	  FALLING_SIGNAL_ON
	  Delay_ms(SIGNAL_ON_DELAY );
	  FALLING_SIGNAL_OFF
	}
void All_signal(void)
    {


	#define  SIGNAL_DELAY   30000
	 No_patient_signal();
	 Delay_ms(SIGNAL_DELAY);
	 Safety_signal();
	 Delay_ms(SIGNAL_DELAY);
	 Falling_Warning_signal();
	 Delay_ms(SIGNAL_DELAY);
	 Falling_signal();
	 Delay_ms(SIGNAL_DELAY);

	}
void All_signal_Off(void)
   {
      FALLING_SIGNAL_OFF           //  GPIO_SetBits(GPIOE, GPIO_Pin_0);   // ����          ��ȣ Off 
      BEFORE_FALLING_SIGNAL_OFF    //GPIO_SetBits(GPIOE, GPIO_Pin_1);   // ����          ���� ��ȣ Off 
      SAFETY_GUARD_SIGNAL_OFF      //GPIO_SetBits(GPIOE, GPIO_Pin_2);   //  �������̵�   ��ȣ Off 
      NO_PATIENT_SIGNAL_OFF        //GPIO_SetBits(GPIOE, GPIO_Pin_3);   //  ȯ�ھ���    ��ȣ Off 
   }






/* end of file */
