					 /*
	matrix.c

*/

 #include "stm32f10x_type.h"
#include "stm32f10x_gpio.h"

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

 char a5b1_input=0;	 // 5행1열 
 char a5b2_input=0;
 char a5b3_input=0;
 char a5b4_input=0;
 char a5b5_input=0;	// 5행 5열 

 char first_line=0;	     // 첫번째(머리쪽)이     점유 여부 1 : 점유, 0 : 비점유 
 char second_line=0;	 // 두번째(머리쪽)이     점유 여부 1 : 점유, 0 : 비점유 
 char third_line=0;	     // 세번째(머리쪽)이     점유 여부 1 : 점유, 0 : 비점유 
 char fourth_line=0;	 // 내번째(머리쪽)이     점유 여부 1 : 점유, 0 : 비점유 
 char fifth_line=0;	     // 다섯번째(머리쪽)이   점유 여부 1 : 점유, 0 : 비점유 

 int occupied_line=0; 


 char first_column=0;	     // 첫번째열(맨오른쪽)     점유 여부 1 : 점유, 0 : 비점유 
 char second_column=0;	    //  두번째열                점유 여부 1 : 점유, 0 : 비점유 
 char third_column=0;	    //  세번째(가운데 )       점유 여부 1 : 점유, 0 : 비점유 
 char fourth_column=0;	    //  내번째(머리쪽)이        점유 여부 1 : 점유, 0 : 비점유 
 char fifth_column=0;	    //  5번째열(맨왼쪽)        점유 여부 1 : 점유, 0 : 비점유 

 int occupied_column=0;       //  점유된 열의수  







void matrix_input_test()
{
//	 char str[100];
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
	   matrix_input_display();
       matrix_input_usart();

	} while(!exit_flag);
}



 void matrix_input_display(void)
 {

       occupied_line=0;    // 점유된 라인의 수를 reset 하고
	   occupied_column=0;  // 점유된 열의   수를 reset 하고 
       
	   
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

 	  /*
       if(a1b1_input||a1b2_input||a1b3_input||a1b4_input||a1b5_input) //5번째 줄에서 하나라도 점유돼으면  
	    { fifth_line=1; occupied_line++;}
	   else fifth_line=0;
	   */
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
      	
		 /******************a5행 *******************************/ // 머리 
      	 if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_10) == 0x00) // a5b1 입력 on 
		     { if(!a5b1_input){ a5b1_input=1; Display_buffer[0]+=SEGMENT_A ;}}
		  else { if(a5b1_input) {a5b1_input=0; Display_buffer[0]-=SEGMENT_A;}}
 	      
		  if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_11) == 0x00) // a5b2 입력 on 
		     { if(!a5b2_input){ a5b2_input=1; Display_buffer[1]+=SEGMENT_A ;}}
		  else { if(a5b2_input) {a5b2_input=0; Display_buffer[1]-=SEGMENT_A;}}
  		
		 if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_12) == 0x00) // a5b3 입력 on 
		     { if(!a5b4_input){ a5b4_input=1; Display_buffer[3]+=SEGMENT_A ;}}
		  else { if(a5b4_input) {a5b4_input=0; Display_buffer[3]-=SEGMENT_A;}}
          
		 if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_13) == 0x00) // a5b4 입력 on 
		     { if(!a5b5_input){ a5b5_input=1; Display_buffer[4]+=SEGMENT_A ;}}
		  else{ if(a5b5_input) {a5b5_input=0; Display_buffer[4]-=SEGMENT_A;}}
		   /******************a5행 *******************************/ // 머리 

		
	
    /*********************점유된 행의 갯수 구하기  ****************************************/	
	
         if(a1b1_input||a1b2_input||a1b3_input||a1b4_input||a1b5_input) //5번째 줄에서 하나라도 점유돼으면    // 다리 
	    { fifth_line=1; occupied_line++;}
	    else fifth_line=0;
	  	
		if(a2b1_input||a2b2_input||a2b3_input||a2b4_input||a2b5_input) //4번째 줄에서 하나라도 점유돼으면  	// 종아리 
	     {  fourth_line=1; occupied_line++;}
	    else fourth_line=0;

	   if(a3b1_input||a3b2_input||a3b3_input||a3b4_input||a3b5_input) //3번째 줄에서 하나라도 점유돼으면  
	        { third_line=1; occupied_line++;}
	    else third_line=0;

			  
        if(a4b1_input||a4b2_input||a4b3_input||a4b4_input||a4b5_input) //2번째 줄에서 하나라도 점유돼으면  
	      { second_line=1; occupied_line++;}
	    else second_line=0;

		if(a5b1_input||a5b2_input||a5b4_input||a5b5_input) //1번째 줄(머리)에서 하나라도 점유돼으면   // 머리 
	      {first_line=1; occupied_line++;}
	      else first_line=0;


	     if(fifth_line==1&&fourth_line==0) // 다리가 점유된 상태에서 종아리가 없으면  
			occupied_line++;


 /*********************점유된 행의 갯수 구하기  ****************************************/	
		
  
  /*********************점유된 열의 갯수 구하기  ****************************************/	
	
         if(a1b1_input||a2b1_input||a3b1_input||a4b1_input||a5b1_input) //5번째 줄에서 하나라도 점유돼으면  
	    { fifth_column=1; occupied_column++;}
	    else fifth_column=0;
	  	
	    if(a1b2_input||a2b2_input||a3b2_input||a4b2_input||a5b2_input) //4번째 줄에서 하나라도 점유돼으면  
	     {  fourth_column=1; occupied_column++;}
	    else fourth_column=0;

	    if(a1b3_input||a2b3_input||a3b3_input||a4b3_input) //3번째 줄에서 하나라도 점유돼으면  
	   	    { third_column=1; occupied_column++;}
	    else third_column=0;

	     if(a1b4_input||a2b4_input||a3b4_input||a4b4_input||a5b4_input) //2번째 줄에서 하나라도 점유돼으면  
	   	  { second_column=1; occupied_column++;}
	    else second_column=0;

	    if(a1b5_input||a2b5_input||a3b5_input||a4b5_input||a5b5_input) //1번째 줄에서 하나라도 점유돼으면  
		  {first_column=1; occupied_column++;}
	      else first_column=0;

 /*********************점유된 열의 갯수 구하기  ****************************************/	

 }


 void matrix_input(void)
 {

       occupied_line=0;    // 점유된 라인의 수를 reset 하고
	   occupied_column=0;  // 점유된 열의   수를 reset 하고 
       
	   
	    if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_6) == 0x00) // a1b1 입력 on 
		     { if(!a1b1_input){ a1b1_input=1; }}
		  else { if(a1b1_input) {a1b1_input=0;}}
 	      if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_7) == 0x00) // a1b2 입력 on 
		     { if(!a1b2_input){ a1b2_input=1; }}
		  else { if(a1b2_input) {a1b2_input=0; }}
  		 if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_8) == 0x00) // a1b3 입력 on 
		     { if(!a1b3_input){ a1b3_input=1; }}
		  else { if(a1b3_input) {a1b3_input=0;}}
         if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_9) == 0x00) // a1b4 입력 on 
		     { if(!a1b4_input){ a1b4_input=1; }}
		  else{ if(a1b4_input) {a1b4_input=0; }}
         if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_10) == 0x00) // a1b5 입력 on 
		     { if(!a1b5_input){ a1b5_input=1; }}
		  else{ if(a1b5_input) {a1b5_input=0; }}
     /******************a1행 *******************************/

	     /******************a2행 *******************************/
      	  if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_11) == 0x00) // a2b1 입력 on 
		     { if(!a2b1_input){ a2b1_input=1; }}
		  else { if(a2b1_input) {a2b1_input=0;}}
 	      if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_12) == 0x00) // a2b2 입력 on 
		     { if(!a2b2_input){ a2b2_input=1; }}
		  else { if(a2b2_input) {a2b2_input=0;}}
  		 if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_13) == 0x00) // a2b3 입력 on 
		     { if(!a2b3_input){ a2b3_input=1; }}
		  else { if(a2b3_input) {a2b3_input=0;}}
         if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_14) == 0x00) // a2b4 입력 on 
		     { if(!a2b4_input){ a2b4_input=1; }}
		  else{ if(a2b4_input) {a2b4_input=0; }}
         if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_15) == 0x00) // a2b5 입력 on 
		     { if(!a2b5_input){ a2b5_input=1; }}
		  else{ if(a2b5_input) {a2b5_input=0; }}
        
		
		 /******************a2행 *******************************/
      	// #ifdef dpfpd 
		 /******************a3행 *******************************/
      	  if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_0) == 0x00) // a3b1 입력 on 
		     { if(!a3b1_input){ a3b1_input=1; }}
		  else { if(a3b1_input) {a3b1_input=0; }}
 	      if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_1) == 0x00) // a3b2 입력 on 
		     { if(!a3b2_input){ a3b2_input=1; }}
		  else { if(a3b2_input) {a3b2_input=0; }}
  		 if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2) == 0x00) // a3b3 입력 on 
		     { if(!a3b3_input){ a3b3_input=1; }}
		  else { if(a3b3_input) {a3b3_input=0; }}
         if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3) == 0x00) // a3b4 입력 on 
		     { if(!a3b4_input){ a3b4_input=1; }}
		  else{ if(a3b4_input) {a3b4_input=0; }}
         if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4) == 0x00) // a3b5 입력 on 
		     { if(!a3b5_input){ a3b5_input=1; }}
		  else{ if(a3b5_input) {a3b5_input=0; }}

	

         /******************a3행 *******************************/
      	
		 /******************a4행 *******************************/
		 if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_5) == 0x00) // a4b1 입력 on 
		     { if(!a4b1_input){ a4b1_input=1; }}
		  else { if(a4b1_input) {a4b1_input=0;}}
 	      if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_6) == 0x00) // a4b2 입력 on 
		     { if(!a4b2_input){ a4b2_input=1; }}
		  else { if(a4b2_input) {a4b2_input=0;}}
  		 if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_7) == 0x00) // a4b3 입력 on 
		     { if(!a4b3_input){ a4b3_input=1; }}
		  else { if(a4b3_input) {a4b3_input=0;}}
         if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_8) == 0x00) // a4b4 입력 on 
		     { if(!a4b4_input){ a4b4_input=1; }}
		  else{ if(a4b4_input) {a4b4_input=0; }}
         if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_9) == 0x00) // a4b5 입력 on 
		     { if(!a4b5_input){ a4b5_input=1; }}
		  else{ if(a4b5_input) {a4b5_input=0; }}
      

		
		 /******************a4행 *******************************/
      	
		 /******************a5행 *******************************/ // 머리 
      	 if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_10) == 0x00) // a5b1 입력 on 
		     { if(!a5b1_input){ a5b1_input=1; }}
		  else { if(a5b1_input) {a5b1_input=0;}}
 	      
		  if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_11) == 0x00) // a5b2 입력 on 
		     { if(!a5b2_input){ a5b2_input=1; }}
		  else { if(a5b2_input) {a5b2_input=0;} }
  		
		 if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_12) == 0x00) // a5b3 입력 on 
		     { if(!a5b4_input){ a5b4_input=1; }}
		  else { if(a5b4_input) {a5b4_input=0; }}
          
		 if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_13) == 0x00) // a5b4 입력 on 
		     { if(!a5b5_input){ a5b5_input=1; }}
		  else{ if(a5b5_input) {a5b5_input=0; }}
		   /******************a5행 *******************************/ // 머리 

		
	
    /*********************점유된 행의 갯수 구하기  ****************************************/	
	
         if(a1b1_input||a1b2_input||a1b3_input||a1b4_input||a1b5_input) //5번째 줄에서 하나라도 점유돼으면    // 다리 
	    { fifth_line=1; occupied_line++;}
	    else fifth_line=0;
	  	
		if(a2b1_input||a2b2_input||a2b3_input||a2b4_input||a2b5_input) //4번째 줄에서 하나라도 점유돼으면  	// 종아리 
	     {  fourth_line=1; occupied_line++;}
	    else fourth_line=0;

	   if(a3b1_input||a3b2_input||a3b3_input||a3b4_input||a3b5_input) //3번째 줄에서 하나라도 점유돼으면  
	        { third_line=1; occupied_line++;}
	    else third_line=0;

			  
        if(a4b1_input||a4b2_input||a4b3_input||a4b4_input||a4b5_input) //2번째 줄에서 하나라도 점유돼으면  
	      { second_line=1; occupied_line++;}
	    else second_line=0;

		if(a5b1_input||a5b2_input||a5b4_input||a5b5_input) //1번째 줄(머리)에서 하나라도 점유돼으면   // 머리 
	      {first_line=1; occupied_line++;}
	      else first_line=0;


	    if(first_line|| second_line	|| third_line) // 사람이 누워있는 상태에서 
		 {
		   if(fifth_line==1&&fourth_line==0) // 다리가 점유된 상태에서 종아리가 없으면  
			 occupied_line++;
		 }

 /*********************점유된 행의 갯수 구하기  ****************************************/	
		
  
  /*********************점유된 열의 갯수 구하기  ****************************************/	
	
         if(a1b1_input||a2b1_input||a3b1_input||a4b1_input||a5b1_input) //5번째 줄에서 하나라도 점유돼으면  
	    { fifth_column=1; occupied_column++;}
	    else fifth_column=0;
	  	
	    if(a1b2_input||a2b2_input||a3b2_input||a4b2_input||a5b2_input) //4번째 줄에서 하나라도 점유돼으면  
	     {  fourth_column=1; occupied_column++;}
	    else fourth_column=0;

	    if(a1b3_input||a2b3_input||a3b3_input||a4b3_input) //3번째 줄에서 하나라도 점유돼으면  
	   	    { third_column=1; occupied_column++;}
	    else third_column=0;

	     if(a1b4_input||a2b4_input||a3b4_input||a4b4_input||a5b4_input) //2번째 줄에서 하나라도 점유돼으면  
	   	  { second_column=1; occupied_column++;}
	    else second_column=0;

	    if(a1b5_input||a2b5_input||a3b5_input||a4b5_input||a4b5_input) //1번째 줄에서 하나라도 점유돼으면  
		  {first_column=1; occupied_column++;}
	      else first_column=0;

 /*********************점유된 열의 갯수 구하기  ****************************************/	

 }
void matrix_input_usart(void)
 {
  char str[100];
  #define SENDING_TIMER		1

          USART1_puts("****** HEAD********");
 		   
	       sprintf(str, "%c%c %c%c",(a5b1_input+0x30),(a5b2_input+0x30),(a5b3_input+0x30),(a5b4_input+0x30));     //행 
	       USART1_puts(str);
 		   
		   sprintf(str, "%c%c%c%c%c",(a4b1_input+0x30),(a4b2_input+0x30),(a4b3_input+0x30),(a4b4_input+0x30),(a4b5_input+0x30));//행
	       USART1_puts(str);
           
		   sprintf(str, "%c%c%c%c%c",(a3b1_input+0x30),(a3b2_input+0x30),(a3b3_input+0x30),(a3b4_input+0x30),(a3b5_input+0x30)); //행 
	       USART1_puts(str);
           
		   sprintf(str, "%c%c%c%c%c",(a2b1_input+0X30),(a2b2_input+0X30),(a2b3_input+0X30),(a2b4_input+0X30),(a2b5_input+0X30)); //행
	       USART1_puts(str);
           
		   sprintf(str, "%c%c%c%c%c",(a1b1_input+0X30),(a1b2_input+0X30),(a1b3_input+0X30),(a1b4_input+0X30),(a1b5_input+0X30)); //행 
	       USART1_puts(str);
           USART1_puts("******LEG***********");
		   Delay_ms(SENDING_TIMER);
 		
      
	       sprintf(str, "%c%c%c%c%c",(first_line+0X30),(second_line+0X30),(third_line+0X30),(fourth_line+0X30),(fifth_line+0X30)); //행 
	       USART1_puts(str);
           sprintf(str, "line=%d",occupied_line); //행 
	       USART1_puts(str);

		   sprintf(str, "%c%c%c%c%c",(first_column+0X30),(second_column+0X30),(third_column+0X30),(fourth_column+0X30),(fifth_column+0X30)); //행 
	       USART1_puts(str);
           sprintf(str, "column=%d",occupied_column); //행 
	       USART1_puts(str);

		 

		  /*
	  	    USART1_puts("*********************");
 		  	USART1_puts("                     ");
 		  	USART1_puts("                     ");
 		    USART1_puts("                     ");
 		    USART1_puts("                     ");
 		    USART1_puts("                     ");
 		  	USART1_puts("*********************");
 		  	Delay_ms(200);
	       */

 }






 
/* end of file */
