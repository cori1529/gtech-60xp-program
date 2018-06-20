/* 
	test.c
*/	

#include "stm32f10x_type.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_it.h"

#include "adc.h"
#include "buzzer.h"
#include "dac.h"
#include "delay.h"
#include "func.h"
#include "key.h"
#include "lcd.h"
#include "relay.h"
#include "set.h"
#include "usart.h"
#include "var.h"
#include "stdio.h"
#include "matrix.h"
#include "cal.h"



void display_test(void);
void ad_test(void);
void ext_input_test(void);
void relay_test(void);
void  battery_value(void);
void ZigBee_Test(void);
void voice_test(void);
void Ble_test(void);

//void Voice_Play(int number);


/*
	 {
		  int battery_value;
		 battery_value=ADCConvertedValue;
		 return battery_value;
		 
		 
	 }
*/
//void matrix_input_test(void);


void serial_test(void);
void analog_out_test(void);
void current_loop_test(void);

extern volatile u16	KeyCode;
vu8 Usart4_Interrupt_flag_test=0;

//vu16  ADCConvertedValue;



//  낙상 matrix 좌표 
 /*
 char a1b1_input=0;	 // 1행1열 
 char a1b2_input=0;
 char a1b3_input=0;
 char a1b4_input=0;
 char a1b5_input=0;	// 1행 5열 

 char a2b1_input=0;	 // 2행1열 
 char a2b2_input=0;
 char a2b3_input=0;
 char a2b4_input=0;
 char a2b5_input=0;	// 2행 5열 
 
 char a3b1_input=0;	 // 3행1열 
 char a3b2_input=0;
 char a3b3_input=0;
 char a3b4_input=0;
 char a3b5_input=0;	// 3행 5열 
 
 char a4b1_input=0;	 // 4행1열 
 char a4b2_input=0;
 char a4b3_input=0;
 char a4b4_input=0;
 char a4b5_input=0;	// 4행 5열 

 char a5b1_input;	 // 5행1열 
 char a5b2_input;
 char a5b3_input;
 char a5b4_input;
 char a5b5_input;	// 5행 5열 

  */

/*
#define ZERO_KEY		 1	  //  영점키
#define START_KEY		 2	  //  시작키 
#define HOLD_KEY		 3	  //  홀드키 
#define RESET_KEY		 4	  //  리셋키 
#define SAVE_KEY		 5	  //  절전키 
*/


void test_mode(void)
{
	char exit_flag = 0;
	char step = 0;
	long test_value;
  
	vputs("TEST ");
  Delay_ms(500);

	do
	{
		 key_test();
		 display_test();      //표시부 테스트 	
	   ad_test();
		 sram_test();
		 voice_test();
		Ble_test();
		
	 
	  exit_flag=1; 
	
	
	} while(!exit_flag);
}



//void test_mode(void);

/*
#define RIGHT_ARROW     ZERO_KEY	//	 1	  //  영점키
#define LEFT_ARROW      START_KEY	//	 2	  //  시작키 
#define CLEAR           HOLD_KEY	//	 3	  //  홀드키 
#define FUNCTOIN_KEY    RESET_KEY	//	 4	  //  리셋키 
#define ENTER_KEY       SAVE_KEY	//	 5	  //  절전키 

 */

void te_set(void)
 {
	u8 exit_flag=0;
   do
	{
	 
	  if(keypush())
		{
			switch(KeyCode)
			{
   			case RIGHT_ARROW:				   //
					 test_mode();
					 break;
			
				case FUNCTOIN_KEY :
					 set_mode();
					 break;
				
				case CLEAR:
					exit_flag = 1;
					break;
			} //switch
		} //if(keypush())
	 exit_flag=1; 
	} while(!exit_flag);







 }



/*
// 낙상침대 
#define ZERO_KEY		 1	  //  영점키
#define START_KEY		 2	  //  시작키 
#define HOLD_KEY		 3	  //  홀드키 
#define RESET_KEY		 4	  //  리셋키 
#define SAVE_KEY		 5	  //  절전키 

// 낙상침대 
  */



void display_test()
{
	 u16 key_code;
	
	unsigned char exit_flag=0, i;
	vputs("TEST2");
	do
	{
		key_code=keypush();
		if (key_code)
		{
			switch (key_code)
			{
				case SET_K:
					exit_flag =1;
					break;
				default:
					for(i=0; i<14; i++) { Display_buffer[i] = 0xFF; Delay_ms(50); }
					 Delay_ms(1000);
					for(i=0; i<14; i++) { Display_buffer[i] = 0x00; Delay_ms(50); }
					 Delay_ms(200);
					break;
			}
		} // of if (keypush())
		else vputs("TEST2");
	} while (!exit_flag);

	
	
	
	
} //display_test()



void ad_test()  //0319
{
	u32 raw_data;
	u8 str[20];
	u8 key_code=0;
  unsigned int	index=0;
	unsigned char	exit_flag=0;
	long int		adc_signed_data=0, zero_raw=0;
	long int		temp_long=0;
	long			lcount = 0;
	u8 gxhfilter_temp;
	u8			bTemp;
	vputs("TEST3");
	delay(500);
	bTemp = v_decimal_point;
	v_decimal_point = 0;
	SIGNFLAG = 0;
	gxhfilter_temp=gxhFilter;
	gxhFilter = 50;
	
	INIT_CS5555();
	adc_timer=0;
	
	do
	{
		if(adc_timer>=_80SPEED)
  			
		{
	     adc_timer=0;
			 raw_data = read_filtered_adc();
			 vputnum_6digit(raw_data);
		 }
	 
	key_code=keypush();
	if(key_code==SET_K)
     exit_flag=1;		
  
	} while(!exit_flag);
	
	gxhFilter=gxhfilter_temp;
	
} //ad_test()


void ZigBee_Test()
{
	char exit_flag = 0;
	char str[30];
	long raw_data;
	
	
	Display_buffer[0]=CH_T ;
	Display_buffer[1]=NUM_5 ;
	Delay_ms(1500);

	do
	{
		 {
	    //sprintf(str, "adc=%5u",ADvalue);
	    //USART2_puts("AT+UNICAST=0000,@01DATA00345!");
			 USART1_puts("Zigbee  Serial  Sending!!");
      Delay_ms(1000);			 
		 }
	 
		PUSH_BUTTON_ON
		exit_flag = 1;

  
	} while(!exit_flag);
} //ad_test()





void memory_test_not_used()
{
#ifdef ___fjdk___
			else if(test_state==MEMORY_TEST)//사용 안함.
			{
				while(1)
				{
						vputs("PT  1 "); Delay_ms(500);
						temp.dw=test_memory;//=> 유니언 변수 temp.dw 에 test_memory 에 저장된 값을 저장
						I2C_EE_BufferWrite(temp.c, ADR_v_zero, 4);//=> ADR_v_zero 가리키는 메모리 주소에 temp.c[0], temp.c[1], temp[2], temp[3] 에 저장된 값을 차례대로 쓴다.
						vputs("PT  2 "); Delay_ms(500);	
	
						

						I2C_EE_BufferRead( temp.c, ADR_v_zero, 4); // => ADR_v_zero가 카리키는 메모리에 저장된 값을 temp.c[0] ~ c[3] 까지 1byte 씩 저장한다.
						test_memory_2=temp.dw;  //=> temp.dw 에 저장된 값을 gxlCurrent_BED라는 변수에 저장한다.
							
						vputs("PT  3 "); Delay_ms(500);	
					
						if(test_memory==test_memory_2) vputs("OK OK ");
						else vputs("FAIL  ");
						/*
						sprintf(str,"%4ld",seven);// 
							vputs(str);

						 if(seven==1234)// write, read ok
							 vputs("ok ok ");
						 else
							vputs("fail  ");
					*/
	
	
				}
			}
#endif
}


		/*
 		No_patient_signal();
 		Safety_signal();
 		Falling_Warning_signal();
 	    Falling_signal();
 		 No_patient_signal();
		All_signal_Off();// 무선 모니터롤 보내는 모든 신호 off 
	
		  */



void ext_input_test()
{
	char exit_flag = 0;
	char flag = 0;	//beep flag
	//vputs("TEST3"); Delay_ms(1500);
  
	Display_buffer[0]=CH_T ;
	Display_buffer[1]=NUM_2 ;
	Delay_ms(1500);

	do
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
		
   device_address=first_sw+second_sw+third_sw+fourth_sw+fifth_sw+1; //1 means  my address is '1'
		
		Display_buffer[0]=dip_switch[device_address/10];
		Display_buffer[1]=dip_switch[device_address%10];
		
		
		
		sprintf(str, "address= %d", device_address);	USART1_puts(str);	 
		Delay_ms(300);
	
	  PUSH_BUTTON_ON
		exit_flag = 1;

		
		
		
		
	
	} while(!exit_flag);
}

void relay_test()
{
	
	char exit_flag = 0;
    All_signal_Off();
	vputs("TEST4");
	Delay_ms(2000);

	do
	{
		if(keypush())
		{
			switch(KeyCode)
			{
				case RIGHT_ARROW : vputs("OUT_1 "); relay_all_off(); relay_on(1);  USART2_puts("FALL"); beep(10); Delay_ms(2000);break;
				case LEFT_ARROW:   vputs("OUT_2 "); relay_all_off(); relay_on(2);  USART2_puts("WARN"); beep(10); Delay_ms(2000);break;
				case CLEAR: vputs("OUT_3 "); relay_all_off(); relay_on(3);         USART2_puts("MOVE"); beep(10); Delay_ms(2000);break;
                case FUNCTOIN_KEY: vputs("OUT_4 "); relay_all_off(); relay_on(4);  USART2_puts("SAFE"); beep(10); Delay_ms(2000); break;
 
				case KEY_ENTER: exit_flag = 1; break;
				default :/* vputs("OUT_0 ");*/ relay_all_off(); break; 
			}
		}
	} while(!exit_flag);
}

#ifdef dpfdpf
void matrix_input_test()
{
	
	char exit_flag = 0;
	char result[5];	      // 위 3줄
    char result_lower[5]; // 아래 2줄 

    All_signal_Off();
	result[0]=0;
    result[1]=0;
	result[2]=0;
	result[3]=0;
	result[4]=0;

	vputs("TEST5");
	Delay_ms(2000);
	vputs("     ");
	Display_buffer[0]=0;

    //    AAAA
	//	 F	  B
	//	 F	  B
	//	   GGG
	//	 E	  C
	//	 E	  C
	//	  DDD
	do
	{
		 /******************a1행 *******************************/
      	  if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_6) == 0x00) // a1b1 입력 on 
		     { if(!a1b1_input){ a1b1_input=1; Display_buffer[0]+=SEGMENT_D ;}}
		  else { if(a1b1_input) {a1b1_input=0; Display_buffer[0]-=SEGMENT_D;}}
 	      if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_7) == 0x00) // a1b2 입력 on 
		     { if(!a1b2_input){ a1b2_input=1; Display_buffer[1]+=SEGMENT_D ;}}
		  else { if(a1b2_input) {a1b2_input=0; Display_buffer[1]-=SEGMENT_D;}}
  		 if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_8) == 0x00) // a1b3 입력 on 
		     { if(!a1b3_input){ a1b3_input=1; Display_buffer[2]+=SEGMENT_D ;}}
		  else { if(a1b3_input) {a1b3_input=0; Display_buffer[2]-=SEGMENT_D;}}
         if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_9) == 0x00) // a1b4 입력 on 
		     { if(!a1b4_input){ a1b4_input=1; Display_buffer[3]+=SEGMENT_D ;}}
		  else{ if(a1b4_input) {a1b4_input=0; Display_buffer[3]-=SEGMENT_D;}}
         if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_10) == 0x00) // a1b5 입력 on 
		     { if(!a1b5_input){ a1b5_input=1; Display_buffer[4]+=SEGMENT_D ;}}
		  else{ if(a1b5_input) {a1b5_input=0; Display_buffer[4]-=SEGMENT_D;}}
         /******************a1행 *******************************/
    
	     /******************a2행 *******************************/
      	  if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_11) == 0x00) // a2b1 입력 on 
		     { if(!a2b1_input){ a2b1_input=1; Display_buffer[0]+=SEGMENT_C ;}}
		  else { if(a2b1_input) {a2b1_input=0; Display_buffer[0]-=SEGMENT_C;}}
 	      if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_12) == 0x00) // a2b2 입력 on 
		     { if(!a2b2_input){ a2b2_input=1; Display_buffer[1]+=SEGMENT_C ;}}
		  else { if(a2b2_input) {a2b2_input=0; Display_buffer[1]-=SEGMENT_C;}}
  		 if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_13) == 0x00) // a2b3 입력 on 
		     { if(!a2b3_input){ a2b3_input=1; Display_buffer[2]+=SEGMENT_C ;}}
		  else { if(a2b3_input) {a2b3_input=0; Display_buffer[2]-=SEGMENT_C;}}
         if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_14) == 0x00) // a2b4 입력 on 
		     { if(!a2b4_input){ a2b4_input=1; Display_buffer[3]+=SEGMENT_C ;}}
		  else{ if(a2b4_input) {a2b4_input=0; Display_buffer[3]-=SEGMENT_C;}}
         if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_15) == 0x00) // a2b5 입력 on 
		     { if(!a2b5_input){ a2b5_input=1; Display_buffer[4]+=SEGMENT_C ;}}
		  else{ if(a2b5_input) {a2b5_input=0; Display_buffer[4]-=SEGMENT_C;}}
         /******************a2행 *******************************/
      	// #ifdef dpfpd 
		 /******************a3행 *******************************/
      	  if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_0) == 0x00) // a3b1 입력 on 
		     { if(!a3b1_input){ a3b1_input=1; Display_buffer[0]+=SEGMENT_G ;}}
		  else { if(a3b1_input) {a3b1_input=0; Display_buffer[0]-=SEGMENT_G;}}
 	      if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_1) == 0x00) // a3b2 입력 on 
		     { if(!a3b2_input){ a3b2_input=1; Display_buffer[1]+=SEGMENT_G ;}}
		  else { if(a3b2_input) {a3b2_input=0; Display_buffer[1]-=SEGMENT_G;}}
  		 if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2) == 0x00) // a3b3 입력 on 
		     { if(!a3b3_input){ a3b3_input=1; Display_buffer[2]+=SEGMENT_G ;}}
		  else { if(a3b3_input) {a3b3_input=0; Display_buffer[2]-=SEGMENT_G;}}
         if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3) == 0x00) // a3b4 입력 on 
		     { if(!a3b4_input){ a3b4_input=1; Display_buffer[3]+=SEGMENT_G ;}}
		  else{ if(a3b4_input) {a3b4_input=0; Display_buffer[3]-=SEGMENT_G;}}
         if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4) == 0x00) // a3b5 입력 on 
		     { if(!a3b5_input){ a3b5_input=1; Display_buffer[4]+=SEGMENT_G ;}}
		  else{ if(a3b5_input) {a3b5_input=0; Display_buffer[4]-=SEGMENT_G;}}
         /******************a3행 *******************************/
      	
		 /******************a4행 *******************************/
		 if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_5) == 0x00) // a4b1 입력 on 
		     { if(!a4b1_input){ a4b1_input=1; Display_buffer[0]+=SEGMENT_B ;}}
		  else { if(a4b1_input) {a4b1_input=0; Display_buffer[0]-=SEGMENT_B;}}
 	      if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_6) == 0x00) // a4b2 입력 on 
		     { if(!a4b2_input){ a4b2_input=1; Display_buffer[1]+=SEGMENT_B ;}}
		  else { if(a4b2_input) {a4b2_input=0; Display_buffer[1]-=SEGMENT_B;}}
  		 if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_7) == 0x00) // a4b3 입력 on 
		     { if(!a4b3_input){ a4b3_input=1; Display_buffer[2]+=SEGMENT_B ;}}
		  else { if(a4b3_input) {a4b3_input=0; Display_buffer[2]-=SEGMENT_B;}}
         if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_8) == 0x00) // a4b4 입력 on 
		     { if(!a4b4_input){ a4b4_input=1; Display_buffer[3]+=SEGMENT_B ;}}
		  else{ if(a4b4_input) {a4b4_input=0; Display_buffer[3]-=SEGMENT_B;}}
         if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_9) == 0x00) // a4b5 입력 on 
		     { if(!a4b5_input){ a4b5_input=1; Display_buffer[4]+=SEGMENT_B ;}}
		  else{ if(a4b5_input) {a4b5_input=0; Display_buffer[4]-=SEGMENT_B;}}
      	  
         /******************a4행 *******************************/
      	
		 /******************a5행 *******************************/
      	 if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_10) == 0x00) // a5b1 입력 on 
		     { if(!a5b1_input){ a5b1_input=1; Display_buffer[0]+=SEGMENT_A ;}}
		  else { if(a5b1_input) {a5b1_input=0; Display_buffer[0]-=SEGMENT_A;}}
 	      
		
		  if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_11) == 0x00) // a5b2 입력 on 
		     { if(!a5b2_input){ a5b2_input=1; Display_buffer[1]+=SEGMENT_A ;}}
		  else { if(a5b2_input) {a5b2_input=0; Display_buffer[1]-=SEGMENT_A;}}
  		 
		
		
		
		 if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_12) == 0x00) // a5b3 입력 on 
		     { if(!a5b3_input){ a5b3_input=1; Display_buffer[3]+=SEGMENT_A ;}}
		  else { if(a5b3_input) {a5b3_input=0; Display_buffer[3]-=SEGMENT_A;}}
          
		 if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_13) == 0x00) // a5b4 입력 on 
		     { if(!a5b4_input){ a5b4_input=1; Display_buffer[4]+=SEGMENT_A ;}}
		  else{ if(a5b4_input) {a5b4_input=0; Display_buffer[4]-=SEGMENT_A;}}
        /* 
		 if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_14) == 0x00) // a5b5 입력 on 
		     { if(!a5b5_input){ a5b5_input=1; Display_buffer[4]+=SEGMENT_A ;}}
		  else{ if(a5b5_input) {a5b5_input=0; Display_buffer[4]-=SEGMENT_A;}}
      	 */
		 
		 /******************a5행 *******************************/
      	      	
		

		
 
	  	


	} while(!exit_flag);
}

  #endif





void serial_test()
{
	char exit_flag = 0;

	do
	{
		vputs("READY ");

		if(keypush())
		{					
			switch(KeyCode)
			{
				case KEY_ZERO: vputs("TRANS "); USART2_print("GTECH!!"); Delay_ms(300); break;
				case KEY_TARE:	USART2_print("GI-600!!"); break;
				case KEY_ENTER: exit_flag = 1; break;
			}
		}
	} while(!exit_flag);
}

void analog_out_test()
{
	char exit_flag = 0;
	char step = 0;

	vputs("   0V ");
	dac_data(0);

	do
	{
		if(keypush())
		{
			switch(KeyCode)
			{
				case KEY_ZERO:
					step++;
					step %= 3;
				 	switch(step)
					{
						 case 0: vputs("   0V "); dac_data(0); break;
						 case 1: vputs("   5V "); dac_data(32768); break;
						 case 2: vputs("  10V "); dac_data(65535); break;
					}
					break;

				case KEY_ENTER:
					exit_flag = 1;
					dac_data(0);
					break;
			}
		}			
	} while(!exit_flag);
}

void current_loop_test()
{
	
	char exit_flag = 0;
	char str[20];
	char i;
	
	sprintf(str, "ST,NT,+123.456kg");
	
	do
	{
		
		{ USART4_puts_DMA(str); Delay_ms(200);}
		
		if(keypush())
		{
			switch(KeyCode)
			{
				case KEY_ZERO:
					
					vputs("TRANS "); sprintf(str, "ST,NT,+123.456kg"); USART4_puts_DMA(str); Delay_ms(500);break;	
				

				case KEY_ENTER:
					exit_flag = 1;
					break;
			}
		}			
	} while(!exit_flag);
	
	
}


 void battery_value(void)
 
	 {
		  int   battery_value;
		  char exit_flag=0;
		  char str[20];
		  Display_buffer[0]=CH_T ;
	    Display_buffer[1]=NUM_4 ;
	    Delay_ms(1500);

		  
		 
		  do
			{
		   battery_value=ADCConvertedValue;
			 sprintf(str, "battery= %2d.%2d v", (battery_value/100),(battery_value%100));	USART1_puts(str);	 
	     Delay_ms(300);
				PUSH_BUTTON_ON
		   exit_flag = 1;

		  }while(!exit_flag);
		 
	 }


void sram_test()
{
	unsigned char exit_flag=0, error_flag = 0;
	u8 key_code=0;
	union taddr {
					unsigned int  ui;
					unsigned char ch[2];
				} taddr, ret;
	vputs("TEST4");
	do
	{
		key_code=keypush();
		if (key_code)
			switch (key_code)
			{
				case SET_K:
					exit_flag =1;
					break;
				default:
		     /*	
    				error_flag = 0;
					for( taddr.ui = 0; taddr.ui < 0x2000; taddr.ch[0]++)
					{
					////	VfdPrintf("M=%02X00", (UINT)taddr.ch[0]);// DP2 = 0;
						ret.ui = checkbyte(taddr.ui);
						if(ret.ch[0])
						{
						////	VfdPrintf("M=%02X00", (UINT)taddr.ch[0]); //DP2 = 0;
							retry_message(); delay(50);
							error_flag=1;
							break;
						}
						delay(10);
					}
				*/
				   vputs("GooD ");


					Delay_ms(1000);
					vputs("TEST4");
					break;
			} // of switch
	}  while (!exit_flag);
} // of sram_test()

void Ble_test()		 //0524
{
	unsigned char exit_flag=0, error_flag = 0;
	u8 key_code=0;
	
	vputs("TEST6");
	do
	{
		
		if(Usart4_Interrupt_flag_test==1)
	 {
		 
		 beep(300);
 	  Usart4_Interrupt_flag_test=0;
		 Voice_Play(172);
		  vputs("GOOD "); Delay_ms(1500);
		  vputs("TEST6");
//void vputgrade2digit(long num);// 등급//
//void vputgwa3digit(long num); //  몇과
		 
		 
		 
	 }
		
		
		
		
		key_code=keypush();
		if (key_code)
			switch (key_code)
			{
				case SET_K:
					exit_flag =1;
					break;
				default: break;
			}	
		    // of switch
	}  while (!exit_flag);
}







void voice_test()
{
  u8  key_code;
  int		iVoice = 0;
	int		iVoiceDsp;
	int		iVoiceDelay = 0;

	vputs("TEST5"); delay(450);
	vputs("V=  0");
	buzzer_off();
	
	//Potential_Voice_Adjustment();
	Potential_ZeroSet();
	Potential_Increment_Set();
	Potential_Voice_Adjustment(15);
	
	do
	{
		key_code=keypush();
		if (key_code)
		{

			switch (key_code)
			{
				case COUNT_K:
					   iVoice++;
			   if(iVoice>180)
					 iVoice=1;
        //     iVoice = iVoice % 65; // total 64 voice segment
             iVoice = iVoice % 181; // total 64 voice segment
 
					switch(iVoice)
					 {
					  case 1:vputs("V=  1");break;
					  case 2:vputs("V=  2");break;
					  case 3:vputs("V=  3");break;
					  case 4:vputs("V=  4");break;
					  case 5:vputs("V=  5");break;
					  case 6:vputs("V=  6");break;
					  case 7:vputs("V=  7");break;
					  case 8:vputs("V=  8");break;
					  case 9:vputs("V=  9");break;
					  case 10:vputs("V= 10");break;
					  case 11:vputs("V= 11");break;
					  case 12:vputs("V= 12 ");break;
					  case 13:vputs("V= 13");break;
					  case 14:vputs("V= 14");break;
					  case 15:vputs("V= 15");break;
					  case 16:vputs("V= 16");break;
					  case 17:vputs("V= 17");break;
					  case 18:vputs("V= 18");break;
                      case 19:vputs("V= 19");break;
					  case 20:vputs("V= 20");break;
					  case 21:vputs("V= 21");break;
					  case 22:vputs("V= 22");break;
					  case 23:vputs("V= 23");break;
					  case 24:vputs("V= 24");break;
					  case 25:vputs("V= 25");break;
					  case 26:vputs("V= 26");break;
					  case 27:vputs("V= 27");break;
					  case 28:vputs("V= 28");break;
					  case 29:vputs("V= 29");break;
					  case 30:vputs("V= 30");break;
					  case 31:vputs("V= 31");break;
					  case 32:vputs("V= 32");break;
					  case 33:vputs("V= 33");break;
					  case 34:vputs("V= 34");break;
					  case 35:vputs("V= 35");break;
					  
					  case 36:vputs("V= 36");break;
					  case 37:vputs("V= 37");break;
					  case 38:vputs("V= 38");break;
					  case 39:vputs("V= 39");break;
					  case 40:vputs("V= 40");break;
					  case 41:vputs("V= 41");break;
					  case 42:vputs("V= 42");break;
					  case 43:vputs("V= 43");break;
					  case 44:vputs("V= 44");break;
					  case 45:vputs("V= 45");break;
					  case 46:vputs("V= 46");break;
					  




					  case 47:vputs("V= 47");break;
					  case 48:vputs("V= 48");break;
					  case 49:vputs("V= 49");break;
					  case 50:vputs("V= 50");break;
					  case 51:vputs("V= 51");break;
					  case 52:vputs("V= 52");break;
					  case 53:vputs("V= 53");break;
					  case 54:vputs("V= 54");break;
					  case 55:vputs("V= 55");break;
					  case 56:vputs("V= 56");break;
					  case 57:vputs("V= 57");break;
					  
					  case 58:vputs("V= 58");break;
					  case 59:vputs("V= 59");break;
					  case 60:vputs("V= 60");break;
					  case 61:vputs("V= 61");break;
					  case 62:vputs("V= 62");break;
					  case 63:vputs("V= 63");break;
					  case 64:vputs("V= 64");break;
			
					  case 65:vputs("V= 65");break;
					  case 66:vputs("V= 66");break;
					  case 67:vputs("V= 67");break;
					  case 68:vputs("V= 68");break;
					  case 69:vputs("V= 69");break;
					  case 70:vputs("V= 70");break;
			
					  
						
						case 71:vputs("V= 71");break;
					  case 72:vputs("V= 72");break;
					  case 73:vputs("V= 73");break;
					  case 74:vputs("V= 74");break;
					  case 75:vputs("V= 75");break;
					  case 76:vputs("V= 76");break;
					  case 77:vputs("V= 77");break;
					  case 78:vputs("V= 78");break;
					  case 79:vputs("V= 79");break;
					  case 80:vputs("V= 80");break;
					  
						case 81:vputs("V= 81");break;
					  case 82:vputs("V= 82");break;
					  case 83:vputs("V= 83");break;
					  case 84:vputs("V= 84");break;
					  case 85:vputs("V= 85");break;
					  case 86:vputs("V= 86");break;
					  case 87:vputs("V= 87");break;
					  case 88:vputs("V= 88");break;
					  case 89:vputs("V= 89");break;
					  case 90:vputs("V= 90");break;
					  
						
						case 91:vputs("V= 91");break;
					  case 92:vputs("V= 92");break;
					  case 93:vputs("V= 93");break;
					  case 94:vputs("V= 94");break;
					  case 95:vputs("V= 95");break;
					  case 96:vputs("V= 96");break;
					  case 97:vputs("V= 97");break;
					  case 98:vputs("V= 98");break;
					  case 99:vputs("V= 99");break;
					  case 100:vputs("V=100");break;
					  

          	case 101:vputs("V=101");break;
					  case 102:vputs("V=102");break;
					  case 103:vputs("V=103");break;
					  case 104:vputs("V=104");break;
					  case 105:vputs("V=105");break;
					  case 106:vputs("V=106");break;
					  case 107:vputs("V=107");break;
					  case 108:vputs("V=108");break;
					  case 109:vputs("V=109");break;
					  case 110:vputs("V=110");break;

            case 111:vputs("V=111");break;
					  case 112:vputs("V=112");break;
					  case 113:vputs("V=113");break;
					  case 114:vputs("V=114");break;
					  case 115:vputs("V=115");break;
					  case 116:vputs("V=116");break;
					  case 117:vputs("V=117");break;
					  case 118:vputs("V=118");break;
					  case 119:vputs("V=119");break;
					  case 120:vputs("V=120");break;

            case 121:vputs("V=121");break;
					  case 122:vputs("V=122");break;
					  case 123:vputs("V=123");break;
					  case 124:vputs("V=124");break;
					  case 125:vputs("V=125");break;
					  case 126:vputs("V=126");break;
					  case 127:vputs("V=127");break;
					  case 128:vputs("V=128");break;
					  case 129:vputs("V=129");break;
					  case 130:vputs("V=130");break;

            case 131:vputs("V=131");break;
					  case 132:vputs("V=132");break;
					  case 133:vputs("V=133");break;
					  case 134:vputs("V=134");break;
					  case 135:vputs("V=135");break;
					  case 136:vputs("V=136");break;
					  case 137:vputs("V=137");break;
					  case 138:vputs("V=138");break;
					  case 139:vputs("V=139");break;
					  case 140:vputs("V=140");break;

            case 141:vputs("V=141");break;
					  case 142:vputs("V=142");break;
					  case 143:vputs("V=143");break;
					  case 144:vputs("V=144");break;
					  case 145:vputs("V=145");break;
					  case 146:vputs("V=146");break;
					  case 147:vputs("V=147");break;
					  case 148:vputs("V=148");break;
					  case 149:vputs("V=149");break;
					  case 150:vputs("V=150");break;

            case 151:vputs("V=151");break;
					  case 152:vputs("V=152");break;
					  case 153:vputs("V=153");break;
					  case 154:vputs("V=154");break;
					  case 155:vputs("V=155");break;
					  case 156:vputs("V=156");break;
					  case 157:vputs("V=157");break;
					  case 158:vputs("V=158");break;
					  case 159:vputs("V=159");break;
					  case 160:vputs("V=160");break;

            case 161:vputs("V=161");break;
					  case 162:vputs("V=162");break;
					  case 163:vputs("V=163");break;
					  case 164:vputs("V=164");break;
					  case 165:vputs("V=165");break;
					  case 166:vputs("V=166");break;
					  case 167:vputs("V=167");break;
					  case 168:vputs("V=168");break;
					  case 169:vputs("V=169");break;
					  case 170:vputs("V=170");break;
           
					  case 171:vputs("V=171");break;
					  case 172:vputs("V=172");break;
					  case 173:vputs("V=173");break;
					  case 174:vputs("V=174");break;
					  case 175:vputs("V=175");break;
					  case 176:vputs("V=176");break;
					  case 177:vputs("V=177");break;
					  case 178:vputs("V=178");break;
					  case 179:vputs("V=179");break;
					  case 180:vputs("V=180");break;




defatul: break;






					 }
					break;
				case SET_K:
					return;
					break;
				default:
					retry_message();
					break;
			} //  of switch
			//Voice_Play(iVoice);
			switch (iVoice)
			{
				default: 
					  Voice_Reset();
						Voice_Play(iVoice);
//						VOICE(iVoice);
						break;
			}
			iVoiceDsp = iVoice;
		} // of if read bit
///		if (++iVoiceDelay > 34) VOICE = 0;
///		if (++iVoiceDelay > 64) VOICE = 0;
///		if (++iVoiceDelay > 100) VOICE = 0;
///		if (++iVoiceDelay > 10) VOICE = 0;		//심함.

//		if (++iVoiceDelay > 50)
//		{	VOICE = 0;	//심함
//			VfdDan(0);
//		}
	} while(1);

////	VOICE(0);

//	VOICE_SBT = 0;	//T1 = 0;



}	
	 
	 



/* end of file */
