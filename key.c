/*
	key.c
*/

#include "stm32f10x_gpio.h"

#include "buzzer.h"
#include "lcd.h"
#include "key.h"
#include "var.h"

#define	KEYCHECKTIME	3

volatile char key_flag;
volatile int key_count;


volatile char NewKeyIn;
//volatile char ReleaseKeyIn;
volatile char PressKeyIn;
volatile char PreKeyIn;

volatile u16	KeyCode;
volatile u16	KeyCodePre;

u8		qKEYIN=1;	  //  1: not key in      0: key in
u8		nKEYIN=0;	  //  1: new key         0: not new key
u8 	  rKEYIN=1;	  //  1: key release     0: not key release

unsigned int   loc, loc1;
unsigned char key_pressed=0;




volatile u16	KeyCheckTimer = KEYCHECKTIME;

	int device_address=0;
	int  first_sw, second_sw,third_sw,fourth_sw, fifth_sw;
	
  u16  st_loc,st_loc_2,st_loc_3,st_loc_4,st_loc_5;
	u16   st_scan;
	u16 st_value=0;







void dip_sw_adress(void) // GI-1400 dip switch address input 
{
	 u8 str[50];
	
	device_address=0;
	  if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3) == 0x00)
	  first_sw=1;
		else
		first_sw=0;

  	  if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4) == 0x00)
	  second_sw=2;
		else
		second_sw=0;

			  if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5) == 0x00)
	  third_sw=4;
		else
		third_sw=0;

  	  if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8) == 0x00)
	  fourth_sw=8;
		else
		fourth_sw=0;

  	  if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9) == 0x00)
	  fifth_sw=16;
		else
		fifth_sw=0;
		
   device_address=first_sw+second_sw+third_sw+fourth_sw+fifth_sw;
		
		Display_buffer[0]=dip_switch[device_address/10];
		Display_buffer[1]=dip_switch[device_address%10];
		
		
		
		sprintf(str, "address= %d", device_address);	USART1_puts(str);	 
		Delay_ms(300);
	
	
	
}


void key_init()
{
	PressKeyIn = 0;
	PreKeyIn = 0;
	
	KeyCode = 0;
	KeyCodePre = 0;
	
	KeyCheckTimer = KEYCHECKTIME;
}

#ifdef dpfdp

void CHKEYIN()
{
	NewKeyIn = 0;

	//KeyCode = GPIO_ReadInputData(GPIOD) & 0X00F8 ;	   //0000 0000 0001 0000 

   
  //  KeyCode = GPIO_ReadInputData(GPIOD)>>3;  예전 낙상 콘트롤러 
	KeyCode = GPIO_ReadInputData(GPIOA)>>11;
	KeyCode&=0x001f;


	if(KeyCode == KeyCodePre)
	{
		if(!KeyCheckTimer)
		{
			if(KeyCode == 0x001F) // 아무것도 눌리지 았았으면  
			{
				PressKeyIn = 0;         //not key
				//ReleaseKeyIn = 1;	//release key	
			}
			else 
			{
				//ReleaseKeyIn = 0;
				if(!PressKeyIn) NewKeyIn = 1;  	// new key pressed
				PressKeyIn = 1;
			}
		}
	}
	else KeyCheckTimer = KEYCHECKTIME;
	
	KeyCodePre = KeyCode;
	//PreKeyIn = PressKeyIn;
}

#endif

void CHKEYIN_with_selfcounter()
{
	NewKeyIn = 0;

	KeyCode = GPIO_ReadInputData(GPIOA) & 0x01F0;
	if(KeyCode == KeyCodePre)
	{
		if(KeyCheckTimer) --KeyCheckTimer;
		if(!KeyCheckTimer)
		{
			if(KeyCode == 0x01F0) 
			{
				PressKeyIn = 0;         //not key
				//ReleaseKeyIn = 1;	//release key	
			}
			else 
			{
				//ReleaseKeyIn = 0;
				if(!PressKeyIn) NewKeyIn = 1;  	// new key pressed
				PressKeyIn = 1;
			}
		}
	}
	else KeyCheckTimer = KEYCHECKTIME;
	
	KeyCodePre = KeyCode;
	//PreKeyIn = PressKeyIn;
}

void CHKEYIN() //2018 new  선별기
{
	


 /*
  char str[50];
   loc=keypush_first();	
	 Delay_ms(50);
   loc1=keypush_first();
	 sprintf(str, " loc=%2d-loc1=%2d",(int)loc,(int)loc1); 
   USART1_puts(str);
	)*/
	
	 loc=st_value;
	// Delay_ms(10);
	 loc1=st_value;
	 
	if (loc != loc1) // noise filtering process 
		goto Final; 

 if(loc==0) //   눌리지 않았다..
 {
	  rKEYIN = 1;	  // 키를 테고 
		qKEYIN = 1;	  //  키가 들어 올지 않고 
		nKEYIN = 0;	  //새로운 키도 아니다 
	 
	 ////USART1_puts("aaa");
 } 
else// 값이 들어왔다.
	{
		if (rKEYIN == 1) // 키가 떨어진 상태에서 
		{
			rKEYIN = 0;	 // 키가 눌린거고 
			nKEYIN = 1;	 // 새로운 키다.
    //// USART1_puts("bbb");			
		}
		else
		{
			nKEYIN = 0;	// 새로운 키가 아니다.
		////  USART1_puts("ccc");
		}
		qKEYIN = 0;	 // 키는 들어와 있고 
	}	
	

	Final:
}



unsigned char keypush()	
{
	CHKEYIN();	 //check key in
	if (qKEYIN || !nKEYIN) 
	{	
	 ////USART1_puts("ddd");	
	 return 0;  // not key in   not new key 
	 //USART1_puts("bbb");
  }	

if (loc == 0) 
	 {
		 ////USART1_puts("xxxx");
	    return 0;
		  
	 }
 if(loc)	 
	 beep(100);
	 
	 
	return loc;


	
} /* of keypush() */


unsigned char keypush_first(void)	
{
	  st_loc=GPIO_ReadInputData(GPIOA);
		st_loc>>=4;  // 0000 1111 
    st_loc&=0x07;
			 if(st_loc)
       {
	    switch(st_loc)
					{
						case 1:
							  switch(Fnd_rotation)
								{
									case 5:
									  st_value=1;
									   break;
									case 6:
									  st_value=2;
									   break;
									case 7:
									  st_value=3;
									   break;
									case 8:
									  st_value=4;
									   break;
									case 9:
									  st_value=5;
									   break;
									default: break;
							  }		
             break;
              case 2:
							  switch(Fnd_rotation)
								{
									case 5:
									  st_value=6;
									   break;
									case 6:
									  st_value=7;
									   break;
									case 7:
									  st_value=8;
									   break;
									case 8:
									  st_value=9;
									   break;
									case 9:
									  st_value=10;
									   break;
									default: break;
							  }		
             break;
						case 4:
							  switch(Fnd_rotation)
								{
									case 5:
									  st_value=11;
									   break;
									case 6:
									  st_value=12;
									   break;
									case 7:
									  st_value=13;
									   break;
									case 8:
									  st_value=14;
									   break;
									case 9:
									  st_value=15;
									   break;
									default: break;
							  }		
             break;
									default: break;
					}	
			}
			 else 
				 st_value=0;
	return st_value;
} /* of keypush() */




  // 1번 L/C 테스트 

//  #define ZERO_KEY		 1	  //  영점키
//#define START_KEY		 2	  //  시작키 
//#define HOLD_KEY		 3	  //  홀드키 
//#define RESET_KEY		 4	  //  리셋키 
//#define SAVE_KEY		 5	  //  절전키 

/*
	    No_patient_signal();
 		Safety_signal();
 		Falling_Warning_signal();
 	    Falling_signal();
 		 No_patient_signal();
		All_signal_Off();// 무선 모니터롤 보내는 모든 신호 off 
  */

/*
void key_test()
{
	char exit_flag = 0;

	vputs("TEST1");
//	All_signal_Off();

	do {
		if(keypush())
		{
			switch(KeyCode)
			{
				case ZERO_KEY : vputs("LEFT "); beep(10); break;
				case START_KEY : vputs("RIGHT");beep(10); break;
				case HOLD_KEY : vputs("CLEAR"); beep(10); break;
				case RESET_KEY : vputs("FUNCT");beep(10); break;
				case SAVE_KEY: exit_flag = 1; beep(10);break;
			}
		}
	} while(!exit_flag);
}
*/

void key_test()	   //0619
{
	 u16 key_code=0;
	 unsigned char exit_flag = 0;
	 vputs("TEST1");
	 Delay_ms(500);
	 do
	 {
	   key_code=keypush(); 
		 if (key_code)
	  
	   {
		 switch(key_code)
		 {

		   case SET_K: // 선별키, GRADE_K
				beep(50);
				Delay_ms(500);
				exit_flag =1;
				break;
			 case POJANG_K:       vputs("PJANG"); break; 
			 case WHICH_FRUIT_K:  vputs("FRUIT"); break; 
			 case HUNDRED_K:      vputs(" 100 "); break; 
			 case TEN_K:          vputs(" 10  "); break; 
			 case ONE_K:          vputs(" 1   "); break; 
			 case WHICH_GRAED_K:  vputs("GRADE"); break; 
			 case GRAED_SPEED_K:  vputs("SPEED"); break; 
			 case WEIGH_K:        vputs("WEIGH"); break; 
			 case SULJUNG_K:      vputs("SJUNG"); break; 
			 case DEUNGUB_K:      vputs("COUNT"); break; 
			 case ZERO_K:         vputs("ZERO "); break; 
			 case POWER_K:        vputs("POWER"); break; 
			 case VOICEUP_K:      vputs("VOL_U"); break;
			 case VOICEDOWN_K:     vputs("VOL_D"); break;
			 


		 }
	   } // of if (keypush())
	 } while (!(exit_flag));
} // of key_test


void Voice_Reset(void) //0312
{	
 GPIO_ResetBits(GPIOD, GPIO_Pin_2);// low
 GPIO_SetBits(GPIOD, GPIO_Pin_2); Delay_ms(2);
 GPIO_ResetBits(GPIOD, GPIO_Pin_2);// low
}

	void Voice_Data_Loading(int number)
	{
    int  final_signal=number;
    int  temp;
		char digit[8];
 	
    temp=(number&0x01)^0x0001;  //  1이면(data 가 있으면)  0 을 반환    // 0001 1110 
    if(!temp) digit[0]=1; else      digit[0]=0;
    
		temp=(number&0x02)^0x0002;  //  1이면(data 가 있으면)  0 을 반환
    if(!temp) digit[1]=1; else      digit[1]=0;
    
		temp=(number&0x04)^0x0004;  //  1이면(data 가 있으면)  0 을 반환
    if(!temp) digit[2]=1; else      digit[2]=0;
    
		temp=(number&0x08)^0x0008;  //  1이면(data 가 있으면)  0 을 반환
    if(!temp) digit[3]=1; else      digit[3]=0;
    
		temp=(number&0x10)^0x0010;  //  1이면(data 가 있으면)  0 을 반환
    if(!temp) digit[4]=1; else      digit[4]=0;
    
		temp=(number&0x20)^0x0020;  //  1이면(data 가 있으면)  0 을 반환
    if(!temp) digit[5]=1; else      digit[5]=0;
    
		temp=(number&0x40)^0x0040;  //  1이면(data 가 있으면)  0 을 반환
    if(!temp) digit[6]=1; else      digit[6]=0;
    
		temp=(number&0x80)^0x0080;  //  1이면(data 가 있으면)  0 을 반환
    if(!temp) digit[7]=1; else      digit[7]=0;
   
	 //signal loading  
		//digit[0]=1; digit[2]=1;
		
		if(digit[0]) GPIO_SetBits(GPIOA, GPIO_Pin_7); 
		else         GPIO_ResetBits(GPIOA, GPIO_Pin_7); 
		
	  if(digit[1]) GPIO_SetBits(GPIOA, GPIO_Pin_8); 
		else         GPIO_ResetBits(GPIOA, GPIO_Pin_8); 
	
    if(digit[2]) GPIO_SetBits(GPIOB, GPIO_Pin_8); 
		else         GPIO_ResetBits(GPIOB, GPIO_Pin_8); 
	
    if(digit[3]) GPIO_SetBits(GPIOB, GPIO_Pin_9); 
		else         GPIO_ResetBits(GPIOB, GPIO_Pin_9); 
	
    if(digit[4]) GPIO_SetBits(GPIOB, GPIO_Pin_10); 
		else         GPIO_ResetBits(GPIOB, GPIO_Pin_10); 
	
    if(digit[5]) GPIO_SetBits(GPIOB, GPIO_Pin_11); 
		else         GPIO_ResetBits(GPIOB, GPIO_Pin_11); 
	
    if(digit[6]) GPIO_SetBits(GPIOB, GPIO_Pin_12); 
		else         GPIO_ResetBits(GPIOB, GPIO_Pin_12); 
	
    if(digit[7]) GPIO_SetBits(GPIOC, GPIO_Pin_13); 
		else         GPIO_ResetBits(GPIOC, GPIO_Pin_13); 
		
		
		//GPIO_SetBits(GPIOA, GPIO_Pin_7);
	//	while(1);
		
		/*
  GPIO_SetBits(GPIOA, GPIO_Pin_7); 
	GPIO_ResetBits(GPIOA, GPIO_Pin_8); 
	GPIO_ResetBits(GPIOB, GPIO_Pin_8); 
	GPIO_ResetBits(GPIOB, GPIO_Pin_9); 
	GPIO_ResetBits(GPIOB, GPIO_Pin_10); 
	GPIO_ResetBits(GPIOB, GPIO_Pin_11); 
	GPIO_ResetBits(GPIOB, GPIO_Pin_12); 
	GPIO_ResetBits(GPIOC, GPIO_Pin_10);
 */
  }

void 	Voice_Play(int number)
{	
 
 GPIO_ResetBits(GPIOC, GPIO_Pin_12); // LOW SBT
 Delay_ms(1);
 Delay_ms(1);
 Delay_ms(1);
 //i++;
 Voice_Data_Loading(number-1);// 
 Delay_ms(10);
Delay_ms(10);
Delay_ms(10);	
 
 GPIO_SetBits(GPIOC, GPIO_Pin_12); // HIGH SBT
 
 Delay_ms(10);
 Delay_ms(10);
 Delay_ms(10);
 GPIO_ResetBits(GPIOC, GPIO_Pin_12); // LOW SBT

} 
	

void  Potential_Increment_Set(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_0); // set to higt chip select
	GPIO_ResetBits(GPIOB, GPIO_Pin_1); // set to higt chip select
	//Delay_ms(1);
	register_delay(30);
	GPIO_SetBits(GPIOB, GPIO_Pin_1); // set to higt chip select
  //Delay_ms(1);
	register_delay(30);
 	GPIO_ResetBits(GPIOB, GPIO_Pin_0); // set to higt chip select
	
}	
	
void  Potential_Decrement_Set(void) 
	
{
 GPIO_SetBits(GPIOB, GPIO_Pin_0); // set to higt chip select
 GPIO_SetBits(GPIOB, GPIO_Pin_1); // set to higt chip select
 //Delay_ms(1);
register_delay(30); 	
 GPIO_ResetBits(GPIOB, GPIO_Pin_1); // set to higt chip select
 //Delay_ms(1);
	register_delay(30);
 GPIO_ResetBits(GPIOB, GPIO_Pin_0); // set to higt chip select
	
}	



void  Potential_Voice_Adjustment(char i)
{
  char rotation;
 for(rotation=0;rotation<i;rotation++)
 {	
 GPIO_ResetBits(GPIOB, GPIO_Pin_1); // set to higt chip select
 register_delay(30);
 GPIO_SetBits(GPIOB, GPIO_Pin_1); // set to higt chip select
	
 register_delay(30);	
 GPIO_ResetBits(GPIOB, GPIO_Pin_1); // set to higt chip select
 }	
	
}	

void register_delay(int time)

{
	int	 duration;

	for(duration=0; duration<time; duration++) {}
}


void  Potential_ZeroSet(void)
{
	 USART1_puts("start decrement!");
 
 Potential_Decrement_Set(); 
	Potential_Voice_Adjustment(17);
 /* 
 for(i=0;i<17;i++)
 {
  Potential_Voice_Adjustment(); 
	sprintf(str, "decrement=%d", i);	USART1_puts(str);
 
	Delay_ms(10);
 }
 */
 
 USART1_puts("end decrement!");

	
	
	
}	







/* end of file */
