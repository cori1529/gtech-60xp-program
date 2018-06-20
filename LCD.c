/*
	lcd.c
	
	2016.07.20 
	 - A/D test에서 A/D값이 5자리가 넘어가면 1의자리가 표시되지 않음.
	        =>void vputnum(char leadingzero, char dp, long num) 함수에  if(num>99999) num %= 100000; 추가하여 1의자리 ~ 1만의자리까지 표시하도록 수정.
*/

#include "stm32f10x_type.h"
#include "stm32f10x_gpio.h"

#include "stdio.h"

#include "delay.h"
#include "lcd.h"
#include "set.h"
#include "var.h"

//   AAAAA
//	 F	 B
//	 F G B
//	 E   C
//   E	 C
//	 DDDDD  DP
const u16 font_fnd[128]= {NUM_0,NUM_1,NUM_2,NUM_3,NUM_4,NUM_5,NUM_6,NUM_7,NUM_8,NUM_9,  0,  0, 0, 0,  0,   0,    // 0x00 ~0x0f
                  	0,  0,  0,  0,   0,   0,  0,  0,  0,  0,  0,  0, 0, 0,  0,   0, 	 // 0x10 ~0x1f
					0,  0,  0,  0,   0,   0,  0,  0,  0,  0,  0,  0, 0,CH_BAR, SEGMENT_DOT, ZERO_DOT, 	 // 0x20 ~0x2f															 //	0x10 ~0x1f
					NUM_0,NUM_1 ,NUM_2 ,NUM_3 ,NUM_4 ,NUM_5 ,NUM_6 ,NUM_7 ,NUM_8 ,NUM_9 ,  0,  0, 0, CH_EQUAL,  0,   0,   // 0x30 ~0x3f 
					SEGMENT_ALL,CH_A,CH_B,CH_C,CH_D,CH_E,CH_F,CH_G,CH_H,CH_I,CH_J,CH_K,CH_L,CH_M,CH_N,CH_O,   // 0x40 ~0x4f				 
   					CH_P ,CH_Q,CH_R,CH_S,CH_T,CH_U,CH_V,CH_W,CH_X,CH_Y,CH_Z,0,0,0,0,0, //0X50 ~ 0X5F
					SEGMENT_ALL,CH_a,CH_b,CH_c,CH_d,CH_e,CH_f,CH_g,CH_h,CH_i,CH_j,CH_k,CH_l,CH_m,CH_n,CH_o,   // 0x60 ~0x6f				 
   					CH_p ,CH_q,CH_r,CH_s,CH_t,CH_u,CH_v,CH_w,CH_x,CH_y,CH_z,0,0,0,0,0 //0X70 ~ 0X7F
};


 const u16 dip_switch[]={NUM_0 ,NUM_1 ,NUM_2 ,NUM_3 ,NUM_4 ,NUM_5 ,NUM_6 ,NUM_7 ,NUM_8 ,NUM_9 };




void Display_Weight_test(long weight)
{
	vputnum( 1, gxlDecimalPoint, weight);
}

void Display_Weight(long weight)
{
	vputnum( 0, gxlDecimalPoint, weight);
}
	
void vputchar(char digit, u16 ch)
{
	Display_buffer[digit] = font_fnd[ch];
}

void vputs(char *str)
{
	vputchar (5, str[0]);
	vputchar (6, str[1]);
	vputchar (7, str[2]);
	vputchar (8, str[3]);						  
	vputchar (9, str[4]);
	//vputchar (5, str[5]);
    //vputchar (6, str[6]);
}
void vputs_3(char *str)
{
	vputchar (2, str[0]);
	vputchar (3, str[1]);
	vputchar (4, str[2]);
}




void vcls()
{
	Display_buffer[0]=0;
	Display_buffer[1]=0;
	Display_buffer[2]=0;
	Display_buffer[3]=0;
	Display_buffer[4]=0;
	
	
	vputs("     ");	//display, lamp clear
  Display_buffer[10]=0;
	Display_buffer[11]=0;
	
}

void vputerr(char *err_msg)
{
	vputs(err_msg); Delay_ms(200); vcls();	Delay_ms(200);
	vputs(err_msg); Delay_ms(200); vcls();	Delay_ms(200);
	vputs(err_msg); Delay_ms(200); vcls();	Delay_ms(200);
	Delay_ms(300);
}

void vputmsg(char *msg)
{
	vputs(msg); Delay_ms(500); vcls();	Delay_ms(500);
	vputs(msg); Delay_ms(500); vcls();	Delay_ms(500);
	vputs(msg); Delay_ms(500); vcls();	Delay_ms(500);
	vputs(msg); Delay_ms(500);
}

#ifdef dpfpd 

//2016.3.2
void vputnum(char leadingzero, char dp, long num)
{
	char sign = 0;
	char zstr[10];
	char *fmt[2] = { "%5ld", "%05ld" };
	
	if(num < 0)
	{
		sign = 1;
		num *= -1;
	}
	
	if(num>99999) num %= 100000;
	sprintf(zstr, fmt[leadingzero], num);
	
	vputchar(0, zstr[0]);
	if(sign)
	{	if(zstr[0] == '0') vputchar(0, ' ');
			if(num<10000) Display_buffer[0] = CH_BAR;
	}

	switch(dp)
	{
		case 1 : 
			Display_buffer[1]= font_fnd[zstr[1]];
			Display_buffer[2]= font_fnd[zstr[2]];

			if(zstr[3] == ' ') Display_buffer[3] = font_fnd[0] + SEGMENT_DOT;
			else Display_buffer[3] = font_fnd[zstr[3]] + SEGMENT_DOT;
			break;
		
		case 2:
			Display_buffer[1]= font_fnd[zstr[1]];

			if(zstr[2] == ' ') Display_buffer[2] = font_fnd[0] + SEGMENT_DOT;
			else Display_buffer[2] = font_fnd[zstr[2]] + SEGMENT_DOT;

			if(zstr[3] == ' ') Display_buffer[3] = font_fnd[0]; else Display_buffer[3]= font_fnd[zstr[3]];
			break;
					
		case 3:
			if(zstr[1] == ' ') Display_buffer[1] = font_fnd[0] + SEGMENT_DOT;
			else Display_buffer[1] = font_fnd[zstr[1]] + SEGMENT_DOT;

			if(zstr[2] == ' ') Display_buffer[2] = font_fnd[0];  else Display_buffer[2]= font_fnd[zstr[2]];
			if(zstr[3] == ' ') Display_buffer[3] = font_fnd[0];  else Display_buffer[3]= font_fnd[zstr[3]];
			break;
		
		case 4:
			if(zstr[0] == ' ') Display_buffer[0] = font_fnd[0] + SEGMENT_DOT;
			else Display_buffer[0] = font_fnd[zstr[0]] + SEGMENT_DOT;
			if(sign) Display_buffer[0] += CH_BAR;

			if(zstr[1] == ' ') Display_buffer[1] = font_fnd[0];  else Display_buffer[1]= font_fnd[zstr[1]];
			if(zstr[2] == ' ') Display_buffer[2] = font_fnd[0];  else Display_buffer[2]= font_fnd[zstr[2]];
			if(zstr[3] == ' ') Display_buffer[3] = font_fnd[0];  else Display_buffer[3]= font_fnd[zstr[3]];
			break;
		
		default : 
			Display_buffer[1]= font_fnd[zstr[1]];
			Display_buffer[2]= font_fnd[zstr[2]];
			Display_buffer[3]= font_fnd[zstr[3]];
			break;
	}

	vputchar (4, zstr[4]);	
}

#endif


/*
  //new gi-520 

void vputnum(char leadingzero, char dp, long num)
{
	char sign = 0;
	char zstr[10];
	char *fmt[2] = { "%7ld", "%07ld" };
	
	if(num < 0)
	{
		sign = 1;
		num *= -1;
	}
	
	if(num>9999999) num %= 100000;
	sprintf(zstr, fmt[leadingzero], num);
	
	vputchar(0, zstr[0]);
	if(sign)	        //  - 표시 
	{	if(zstr[0] == '0') vputchar(0, ' ');
			if(num<10000) Display_buffer[0] = CH_BAR;
	}
	   // G3 인디게이터 5 DIGIT 
	   // GI-525 7 DIGIT
	switch(dp)
	{
		case 1 : 
			Display_buffer[1]= font_fnd[zstr[1]];
			Display_buffer[2]= font_fnd[zstr[2]];
            Display_buffer[3]= font_fnd[zstr[3]];
			Display_buffer[4]= font_fnd[zstr[4]];
		    Display_buffer[5]= font_fnd[zstr[5]];
		


			if(zstr[5] == ' ') Display_buffer[5] = font_fnd[0] + SEGMENT_DOT;
			else Display_buffer[5] = font_fnd[zstr[5]] + SEGMENT_DOT;
			break;
		
		case 2:
			Display_buffer[1]= font_fnd[zstr[1]];

			if(zstr[2] == ' ') Display_buffer[2] = font_fnd[0] + SEGMENT_DOT;
			else Display_buffer[2] = font_fnd[zstr[2]] + SEGMENT_DOT;

			if(zstr[3] == ' ') Display_buffer[3] = font_fnd[0]; else Display_buffer[3]= font_fnd[zstr[3]];
			break;
					
		case 3:
			if(zstr[1] == ' ') Display_buffer[1] = font_fnd[0] + SEGMENT_DOT;
			else Display_buffer[1] = font_fnd[zstr[1]] + SEGMENT_DOT;

			if(zstr[2] == ' ') Display_buffer[2] = font_fnd[0];  else Display_buffer[2]= font_fnd[zstr[2]];
			if(zstr[3] == ' ') Display_buffer[3] = font_fnd[0];  else Display_buffer[3]= font_fnd[zstr[3]];
			break;
		
		case 4:
			if(zstr[0] == ' ') Display_buffer[0] = font_fnd[0] + SEGMENT_DOT;
			else Display_buffer[0] = font_fnd[zstr[0]] + SEGMENT_DOT;
			if(sign) Display_buffer[0] += CH_BAR;

			if(zstr[1] == ' ') Display_buffer[1] = font_fnd[0];  else Display_buffer[1]= font_fnd[zstr[1]];
			if(zstr[2] == ' ') Display_buffer[2] = font_fnd[0];  else Display_buffer[2]= font_fnd[zstr[2]];
			if(zstr[3] == ' ') Display_buffer[3] = font_fnd[0];  else Display_buffer[3]= font_fnd[zstr[3]];
			break;
		
		default : 		 // DP 없을대 
			Display_buffer[1]= font_fnd[zstr[1]];
			Display_buffer[2]= font_fnd[zstr[2]];
			Display_buffer[3]= font_fnd[zstr[3]];
		    Display_buffer[4]= font_fnd[zstr[4]];
			Display_buffer[5]= font_fnd[zstr[5]];
		
			break;
	}

	vputchar (6, zstr[6]);//맨마지막  DIGIT 는  그냥 표시 	
}

 */



  //new gi-520 


void vputnum_5digit(long num)
{

 char zstr[10];
 
 sprintf(zstr,"%5ld", num); 
/*
 Display_buffer[0]= font_fnd[zstr[0]];
 Display_buffer[1]= font_fnd[zstr[1]];
 Display_buffer[2]= font_fnd[zstr[2]];
 Display_buffer[3]= font_fnd[zstr[3]];
 Display_buffer[4]= font_fnd[zstr[4]];
*/

 Display_buffer[5]= font_fnd[zstr[0]];
 Display_buffer[6]= font_fnd[zstr[1]];
 Display_buffer[7]= font_fnd[zstr[2]];
 Display_buffer[8]= font_fnd[zstr[3]];
 Display_buffer[9]= font_fnd[zstr[4]];
}


void vputnum_6digit(long num)
{

 char zstr[10];
 
 sprintf(zstr,"%6ld", num); 
/*
 Display_buffer[0]= font_fnd[zstr[0]];
 Display_buffer[1]= font_fnd[zstr[1]];
 Display_buffer[2]= font_fnd[zstr[2]];
 Display_buffer[3]= font_fnd[zstr[3]];
 Display_buffer[4]= font_fnd[zstr[4]];
*/
Display_buffer[4]= font_fnd[zstr[0]];
 Display_buffer[5]= font_fnd[zstr[1]];
 Display_buffer[6]= font_fnd[zstr[2]];
 Display_buffer[7]= font_fnd[zstr[3]];
 Display_buffer[8]= font_fnd[zstr[4]];
 Display_buffer[9]= font_fnd[zstr[5]];
}


void vputgrade2digit(long num)// 등급
 {

 char zstr[5];
 sprintf(zstr,"%2ld", num); 

 Display_buffer[0]= font_fnd[zstr[0]];
 Display_buffer[1]= font_fnd[zstr[1]];
 } 

void vputgwa3digit(long num) //  몇과
  {

 char zstr[5];
 sprintf(zstr,"%3ld", num); 

 Display_buffer[2]= font_fnd[zstr[0]];
 Display_buffer[3]= font_fnd[zstr[1]];
 Display_buffer[4]= font_fnd[zstr[2]];

	} 



void vputpojang2digit(long num)// 포장
 {

 char zstr[5];
 sprintf(zstr,"%2ld", num); 

 Display_buffer[10]= font_fnd[zstr[0]];
 Display_buffer[11]= font_fnd[zstr[1]];
 } 



#ifdef dpfdpf
void vputnum(char leadingzero, char dp, long num)
{
	char sign = 0;
	char zstr[10];
	char *fmt[2] = { "%7ld", "%07ld" };
	
	if(num < 0)
	{
		sign = 1;
		num *= -1;
	}
	
	if(num>999999) num %= 10000;
	sprintf(zstr, fmt[leadingzero], num);
	
	vputchar(0, zstr[0]);
	if(sign)	        //  - 표시 
	{	if(zstr[0] == '0') vputchar(0, ' ');
			if(num<10000) Display_buffer[0] = CH_BAR;
	}
	   // G3 인디게이터 5 DIGIT 
	   // GI-525 7 DIGIT

	switch(dp)
	{
		case 1 : 
			Display_buffer[0]= font_fnd[zstr[0]];
			Display_buffer[1]= font_fnd[zstr[1]];
            Display_buffer[2]= font_fnd[zstr[3]];
			Display_buffer[3]= font_fnd[zstr[4]];
		    Display_buffer[4]= font_fnd[zstr[5]];
		


			if(zstr[5] == ' ') Display_buffer[5] = font_fnd[0] + SEGMENT_DOT;
			else Display_buffer[5] = font_fnd[zstr[5]] + SEGMENT_DOT;
			break;
		
		case 2:
			Display_buffer[1]= font_fnd[zstr[1]];

			if(zstr[2] == ' ') Display_buffer[2] = font_fnd[0] + SEGMENT_DOT;
			else Display_buffer[2] = font_fnd[zstr[2]] + SEGMENT_DOT;

			if(zstr[3] == ' ') Display_buffer[3] = font_fnd[0]; else Display_buffer[3]= font_fnd[zstr[3]];
			break;
					
		case 3:
			if(zstr[1] == ' ') Display_buffer[1] = font_fnd[0] + SEGMENT_DOT;
			else Display_buffer[1] = font_fnd[zstr[1]] + SEGMENT_DOT;

			if(zstr[2] == ' ') Display_buffer[2] = font_fnd[0];  else Display_buffer[2]= font_fnd[zstr[2]];
			if(zstr[3] == ' ') Display_buffer[3] = font_fnd[0];  else Display_buffer[3]= font_fnd[zstr[3]];
			break;
		
		case 4:
			if(zstr[0] == ' ') Display_buffer[0] = font_fnd[0] + SEGMENT_DOT;
			else Display_buffer[0] = font_fnd[zstr[0]] + SEGMENT_DOT;
			if(sign) Display_buffer[0] += CH_BAR;

			if(zstr[1] == ' ') Display_buffer[1] = font_fnd[0];  else Display_buffer[1]= font_fnd[zstr[1]];
			if(zstr[2] == ' ') Display_buffer[2] = font_fnd[0];  else Display_buffer[2]= font_fnd[zstr[2]];
			if(zstr[3] == ' ') Display_buffer[3] = font_fnd[0];  else Display_buffer[3]= font_fnd[zstr[3]];
			break;
		
		default : 		 // DP 없을대 
			Display_buffer[1]= font_fnd[zstr[1]];
			Display_buffer[2]= font_fnd[zstr[2]];
			Display_buffer[3]= font_fnd[zstr[3]];
		    Display_buffer[4]= font_fnd[zstr[4]];
			Display_buffer[5]= font_fnd[zstr[5]];
		
			break;
	}

	vputchar (6, zstr[6]);//맨마지막  DIGIT 는  그냥 표시 	
}

#endif





void vputnum(char leadingzero, char dp, long num)
{
	char sign = 0;
	char zstr[10];
	
	//leadingzero=1;
	if(num < 0)
	{
		sign = 1;
		num *= -1;
	}
	
	if(leadingzero) sprintf(zstr, "%05ld", num);   //   5   ,   4,   3   , 2    ,1   , 0 
	else sprintf(zstr, "%5ld", num);   //   5   ,   4,   3   , 2    ,1   , 0 
	
	vputchar(5, zstr[0]);
	if(sign) Display_buffer[5] += CHAR_BAR;
	
	switch(dp)
	{
		case 1 : 
			Display_buffer[6]= font_fnd[zstr[1]];
			Display_buffer[7]= font_fnd[zstr[2]];

			if(zstr[3] == ' ') Display_buffer[8] = font_fnd[0] + SEGMENT_DOT;
			else Display_buffer[8] = font_fnd[zstr[3]] + SEGMENT_DOT;
			break;
		
		case 2:
			Display_buffer[6]= font_fnd[zstr[1]];

			if(zstr[2] == ' ') Display_buffer[7] = font_fnd[0] + SEGMENT_DOT;
			else Display_buffer[7] = font_fnd[zstr[2]] + SEGMENT_DOT;

			if(zstr[3] == ' ') Display_buffer[8] = font_fnd[0]; else Display_buffer[8]= font_fnd[zstr[3]];
			break;
					
		case 3:
			if(zstr[1] == ' ') Display_buffer[6] = font_fnd[0] + SEGMENT_DOT;
			else Display_buffer[6] = font_fnd[zstr[1]] + SEGMENT_DOT;

			if(zstr[2] == ' ') Display_buffer[7] = font_fnd[0];  else Display_buffer[7]= font_fnd[zstr[2]];
			if(zstr[3] == ' ') Display_buffer[8] = font_fnd[0];  else Display_buffer[8]= font_fnd[zstr[3]];
			break;
		
		case 4:
			if(zstr[0] == ' ') Display_buffer[5] = font_fnd[0] + SEGMENT_DOT;
			else Display_buffer[5] = font_fnd[zstr[0]] + SEGMENT_DOT;
			if(sign) Display_buffer[5] += CHAR_BAR;

			if(zstr[1] == ' ') Display_buffer[6] = font_fnd[0];  else Display_buffer[6]= font_fnd[zstr[1]];
			if(zstr[2] == ' ') Display_buffer[7] = font_fnd[0];  else Display_buffer[7]= font_fnd[zstr[2]];
			if(zstr[3] == ' ') Display_buffer[8] = font_fnd[0];  else Display_buffer[8]= font_fnd[zstr[3]];
			break;
		
		default : 
			Display_buffer[6]= font_fnd[zstr[1]];
			Display_buffer[7]= font_fnd[zstr[2]];
			Display_buffer[8]= font_fnd[zstr[3]];
			break;
	}

	vputchar (9, zstr[4]);	
}










/*
//2016.3.29
void vputnum_ndigit(char leadingzero, char dp, long num, int ndigit)
{
	int  iz;
	char sign = 0;
	char zstr[10];
	char *fmt[2] = { "%5ld", "%05ld" };
	
	if(num < 0)
	{
		sign = 1;
		num *= -1;
	}
	
	sprintf(zstr, fmt[leadingzero], num);
	
	vputchar(0, zstr[0]);
	if(sign)
	{	if(zstr[0] == '0') vputchar(0, ' ');
		Display_buffer[0] = CH_BAR;
	}

	switch(dp)
	{
		case 1 : 
			Display_buffer[1]= font_fnd[zstr[1]];
			Display_buffer[2]= font_fnd[zstr[2]];

			if(zstr[3] == ' ') Display_buffer[3] = font_fnd[0] + SEGMENT_DOT;
			else Display_buffer[3] = font_fnd[zstr[3]] + SEGMENT_DOT;
			break;
		
		case 2:
			Display_buffer[1]= font_fnd[zstr[1]];

			if(zstr[2] == ' ') Display_buffer[2] = font_fnd[0] + SEGMENT_DOT;
			else Display_buffer[2] = font_fnd[zstr[2]] + SEGMENT_DOT;

			if(zstr[3] == ' ') Display_buffer[3] = font_fnd[0]; else Display_buffer[3]= font_fnd[zstr[3]];
			break;
					
		case 3:
			if(zstr[1] == ' ') Display_buffer[1] = font_fnd[0] + SEGMENT_DOT;
			else Display_buffer[1] = font_fnd[zstr[1]] + SEGMENT_DOT;

			if(zstr[2] == ' ') Display_buffer[2] = font_fnd[0];  else Display_buffer[2]= font_fnd[zstr[2]];
			if(zstr[3] == ' ') Display_buffer[3] = font_fnd[0];  else Display_buffer[3]= font_fnd[zstr[3]];
			break;
		
		case 4:
			if(zstr[0] == ' ') Display_buffer[0] = font_fnd[0] + SEGMENT_DOT;
			else Display_buffer[0] = font_fnd[zstr[0]] + SEGMENT_DOT;
			if(sign) Display_buffer[0] += CH_BAR;

			if(zstr[1] == ' ') Display_buffer[1] = font_fnd[0];  else Display_buffer[1]= font_fnd[zstr[1]];
			if(zstr[2] == ' ') Display_buffer[2] = font_fnd[0];  else Display_buffer[2]= font_fnd[zstr[2]];
			if(zstr[3] == ' ') Display_buffer[3] = font_fnd[0];  else Display_buffer[3]= font_fnd[zstr[3]];
			break;
		
		default : 
			Display_buffer[1]= font_fnd[zstr[1]];
			Display_buffer[2]= font_fnd[zstr[2]];
			Display_buffer[3]= font_fnd[zstr[3]];
			break;
	}

	for(iz=4; ndigit<=iz; iz--) if(Display_buffer[iz] == 0) Display_buffer[iz]= font_fnd[0];

	vputchar (4, zstr[4]);	
}
 */

 // new gi-525 
void vputnum_ndigit(char leadingzero, char dp, long num, int ndigit)
{
	int  iz;
	char sign = 0;
	char zstr[10];
	char *fmt[2] = { "%7ld", "%07ld" };
	
	if(num < 0)
	{
		sign = 1;
		num *= -1;
	}
	
	sprintf(zstr, fmt[leadingzero], num);
	
	vputchar(0, zstr[0]);
	if(sign)
	{	if(zstr[0] == '0') vputchar(0, ' ');
		Display_buffer[0] = CH_BAR;
	}

	switch(dp)
	{
		case 1 : 
			Display_buffer[1]= font_fnd[zstr[1]];
			Display_buffer[2]= font_fnd[zstr[2]];

			if(zstr[3] == ' ') Display_buffer[3] = font_fnd[0] + SEGMENT_DOT;
			else Display_buffer[3] = font_fnd[zstr[3]] + SEGMENT_DOT;
			break;
		
		case 2:
			Display_buffer[1]= font_fnd[zstr[1]];

			if(zstr[2] == ' ') Display_buffer[2] = font_fnd[0] + SEGMENT_DOT;
			else Display_buffer[2] = font_fnd[zstr[2]] + SEGMENT_DOT;

			if(zstr[3] == ' ') Display_buffer[3] = font_fnd[0]; else Display_buffer[3]= font_fnd[zstr[3]];
			break;
					
		case 3:
			if(zstr[1] == ' ') Display_buffer[1] = font_fnd[0] + SEGMENT_DOT;
			else Display_buffer[1] = font_fnd[zstr[1]] + SEGMENT_DOT;

			if(zstr[2] == ' ') Display_buffer[2] = font_fnd[0];  else Display_buffer[2]= font_fnd[zstr[2]];
			if(zstr[3] == ' ') Display_buffer[3] = font_fnd[0];  else Display_buffer[3]= font_fnd[zstr[3]];
			break;
		
		case 4:
			if(zstr[0] == ' ') Display_buffer[0] = font_fnd[0] + SEGMENT_DOT;
			else Display_buffer[0] = font_fnd[zstr[0]] + SEGMENT_DOT;
			if(sign) Display_buffer[0] += CH_BAR;

			if(zstr[1] == ' ') Display_buffer[1] = font_fnd[0];  else Display_buffer[1]= font_fnd[zstr[1]];
			if(zstr[2] == ' ') Display_buffer[2] = font_fnd[0];  else Display_buffer[2]= font_fnd[zstr[2]];
			if(zstr[3] == ' ') Display_buffer[3] = font_fnd[0];  else Display_buffer[3]= font_fnd[zstr[3]];
			break;
		
		default : 
			Display_buffer[1]= font_fnd[zstr[1]];
			Display_buffer[2]= font_fnd[zstr[2]];
			Display_buffer[3]= font_fnd[zstr[3]];
			break;
	}

	for(iz=4; ndigit<=iz; iz--) if(Display_buffer[iz] == 0) Display_buffer[iz]= font_fnd[0];

	vputchar (4, zstr[4]);	
}


void vputnumwithled(char leadingzero, char dp, long num)
{
	u16	 ledfont[] = { 0, SEGMENT_A, SEGMENT_B, SEGMENT_C, SEGMENT_D, SEGMENT_E, SEGMENT_F, SEGMENT_G, SEGMENT_DOT };

	vputnum(leadingzero, dp, num);
	Display_buffer[5] = ledfont[num/100000];
}

/****************GI-1400(NEW)*******************************************/
/*
void Led_All_Off(void);
void Led_All_On(void);
void Led1_On(void);
void Led2_On(void);
void Led3_On(void);
void Led4_On(void);
void Led5_On(void);
void Led1_Off(void);
void Led2_Off(void);
void Led3_Off(void);
void Led4_Off(void);
void Led5_Off(void);
*/

void Led1_On(void)
{GPIO_SetBits(GPIOA, GPIO_Pin_8);}	

void Led1_Off(void)
{GPIO_ResetBits(GPIOA, GPIO_Pin_8);}	

void Led2_On(void)
{GPIO_SetBits(GPIOA, GPIO_Pin_15);}	

void Led2_Off(void)
{GPIO_ResetBits(GPIOA, GPIO_Pin_15);}	

void Led3_On(void)
{GPIO_SetBits(GPIOC, GPIO_Pin_10);}	
void Led3_Off(void)
{GPIO_ResetBits(GPIOC, GPIO_Pin_10);}	


void Led4_On(void)
{GPIO_SetBits(GPIOC, GPIO_Pin_11);}	
void Led4_Off(void)
{GPIO_ResetBits(GPIOC, GPIO_Pin_11);}	

void Led5_On(void)
{GPIO_SetBits(GPIOC, GPIO_Pin_12);}	
void Led5_Off(void)
{GPIO_ResetBits(GPIOC, GPIO_Pin_12);}	

void Led_All_Off(void)
{
 Led1_Off();
 Led2_Off();
 Led3_Off();
 Led4_Off();
 Led5_Off();
}	
void Led_All_On(void)
{
 Led1_On();
 Led2_On();
 Led3_On();
 Led4_On();
 Led5_On();
	 
	
}






/* end of file */
