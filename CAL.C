/*
	cal.c
	
	v_ei_multiply_factor = 1;	//10; 2016.3.12 // 카운트 값(50g 당 10개의 카운트	
	
	
	2016.05.26
	 - CAL_1, CAL_2 모드 division 설정 단계에서 C키 입력시 이전(소수점 설정)으로 돌아가도록 수정(기존 CAL모드를 빠져나갔었음)
	
	
*/

#include "stdio.h"

#include "adc.h"
#include "delay.h"
#include "func.h"
#include "i2c_ee.h"
#include "key.h"
#include "lcd.h"
#include "set.h"
#include "var.h"
#include "usart.h"
#include "buzzer.h"
#include "var.h"
#include "stm32f10x_lib.h"
#include "stm32f10x_it.h"
#include "stm32f10x_rcc.h"


#define CAL_SAMPLING_NUMBER	20

#define ZERO_LOW_LIMIT		100L   		/* 500000L */
#define ZERO_HIGH_LIMIT		1000000L		/* 600000L */

#define ZERO_ERROR_LIMIT    20000000L
#define FULL_LIMIT			1000000L	/* 500000L */


#define	CAL_ZERO_AD_SET	0
#define	CAL_SPAN_AD_SET	1

#define	CAL_ZERO_AD_SET_RETURN_ERROR	-1
#define	CAL_ZERO_AD_SET_RETURN_CANCEL	-10
#define  ENTER_BUTTON_ON        if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0x00)


//void zero_set(void);
void zero_set(void);
void span_set(void);

long cal_zero_span_ad_set(int which);
int calc_factor(void);
int little_span_adjust(void);

s32  v_adc1_buf;      //  = 0 
s32  v_adc1_org; //  = 0 
s32	 v_adc_org;
unsigned long  v_zero;
unsigned long  v_full;
unsigned long  v_span;


unsigned long  v_e_resolution;
unsigned long  v_i_resolution;
unsigned long  v_capacity; unsigned char   v_decimal_point;

unsigned long  v_setting_weight; //   = 5000; 
unsigned long  v_setting_rated_output;


s32 Current_temp; //  현재온도 

unsigned int  min_set(unsigned int init_value);
unsigned int  capacity_set(unsigned int init_value);
//void zero_set();
//void span_set();



long v_division;
long v_ei_multiply_factor;

//unsigned char v_decimal_point;
unsigned char SIGNFLAG;


u32	dZeroValue;	// zero calibration data - gain 1 - 64 
u32	dFullValue;	// full calibration data - gain 1 - 64 
unsigned char	bRepeat;


extern volatile	u16	 KeyCode;

//int cal_state;


u32 power(int x, u8 n)
{
	u32 p;

	if (n == 0) return (u32)1;
	p = (u32)x;
	while(--n) p = p * x;
	return p;
} /* of DWORD power(int x, byte n) */


//분동무게 구하기   415
int Set_Weight()
{
	int exit_flag=0;
	long 	ret_val;
	long	blink = 0;
	
	s16 value_buffer[5];
	u16 digit_buffer = 0;
//	s32 span_weight;

//	vputs("LOAD  ");
	vputmsg("LOAD  ");	//2016.3.29
	Delay_ms(500);

	ret_val = gnlCapacity;	//capa 의 100%로 기본 설정
	value_buffer[0] = (ret_val%100000)/10000;
	value_buffer[1] = (ret_val%10000)/1000;
	value_buffer[2] = (ret_val%1000)/100;
	value_buffer[3] = (ret_val%100)/10;
	value_buffer[4] = (ret_val%10);

	do
	{
		ret_val = value_buffer[0]*10000 + value_buffer[1]*1000 + value_buffer[2]*100 + value_buffer[3]*10 + value_buffer[4];
//		vputnum( 0, 0, ret_val);

		// 자릿수 깜박거리기
		blink++;
		if((blink == 35000)) vputchar(4-digit_buffer, ' '); //자릿수 깜박거리기
		else if(70000 < blink)
		{
			blink = 0;
//			vputnum( 0, 0, ret_val);
			vputnum_ndigit( 0, 0, ret_val, 4-digit_buffer);
		}		

		if(keypush())
		{		 
			switch(KeyCode)
			{
				case KEY_ZERO:
					value_buffer[4-digit_buffer]++;
					value_buffer[4-digit_buffer] %= 10;
					break;

				case KEY_TARE:
					if(value_buffer[4-digit_buffer]) value_buffer[4-digit_buffer]--;
					else value_buffer[4-digit_buffer] = 9;
					break;

				case KEY_SETTING:
					digit_buffer++;
					digit_buffer %= 5;
					break;				 

				case KEY_CANCEL:
					exit_flag = 1;
//					cal_state = 2;
					ret_val = SET_BACKWARD;
					break;

				case KEY_ENTER:
					if(ret_val > gnlCapacity)// capa보다 크게 설정된 경우
					{
						exit_flag=1;
//						cal_state = 3;
						ret_val = SET_AGAIN;

						vputerr("ERR23 ");
					}
					else if(ret_val < (gnlCapacity*0.1))//capa의 10% 미만으로 설정한 경우
					{
						exit_flag=1;
//분동무게 상관없이 넘어가기  2016.3.30
						if(ret_val < 1) ret_val = SET_AGAIN;
						vputerr("ERR26 ");
					}
					else
					{
						exit_flag = 1;
//						cal_state = 4;

						vputnum( 0, 0, ret_val); Delay_ms(300); vcls(); Delay_ms(300);
						vputnum( 0, 0, ret_val); Delay_ms(300); vcls(); Delay_ms(300);

//						ret_val = SET_FORWARD;
					}
			}
		}
	} while(!exit_flag);

	return ret_val;
}
		
	
int Set_capa_Weight()//  capa 구하기   ////////////////////10.16 김정호가 수정
{
	int exit_flag = 0;
	
	s16 value_buffer[5];
	s16 digit_buffer = 0;
	long e_res;
	long ret_val;
	long blink = 0;

//	vputs("CAPA  ");
	vputmsg("CAPA  ");	//2016.3.29
	Delay_ms(500);
		
	ret_val = gnlCapacity;	//v_capacity;
	value_buffer[0] = (ret_val%100000)/10000;
	value_buffer[1] = (ret_val%10000)/1000;
	value_buffer[2] = (ret_val%1000)/100;
	value_buffer[3] = (ret_val%100)/10;
	value_buffer[4] = (ret_val%10);
   
	do
	{
		ret_val = value_buffer[0]*10000 + value_buffer[1]*1000 + value_buffer[2]*100 + value_buffer[3]*10 + value_buffer[4];
		
//		vputnum( 0, 0, ret_val);

		// 자릿수 깜박거리기
		blink++;
		if((blink == 35000)) vputchar(4-digit_buffer, ' '); //자릿수 깜박거리기
		else if(70000 < blink)
		{
			blink = 0;
			//vputnum( 0, 0, ret_val);
			vputnum_ndigit( 0, 0, ret_val, 4-digit_buffer);
		}		
		 
		if(keypush())
		{
			switch(KeyCode)
			{
			case KEY_ZERO:
				value_buffer[4-digit_buffer]++;
				value_buffer[4-digit_buffer] %= 10;
				break;				 

			case KEY_TARE:
				if(value_buffer[4-digit_buffer]) value_buffer[4-digit_buffer]--;
				else value_buffer[4-digit_buffer] = 9;
				break;

			case KEY_SETTING:
				digit_buffer++;
				digit_buffer %= 5;
				break;

			case KEY_CANCEL:
//				cal_state = 0; 
				exit_flag = 1;
				ret_val = SET_BACKWARD;
				break;

			case KEY_ENTER:
				exit_flag = 1;

//				capa_weight = value_buffer[0]*10000 + value_buffer[1]*1000 + value_buffer[2]*100 + value_buffer[3]*10 + value_buffer[4];
				ret_val = value_buffer[0]*10000 + value_buffer[1]*1000 + value_buffer[2]*100 + value_buffer[3]*10 + value_buffer[4];
				e_res = ret_val * power(10, gxlDecimalPoint);				
				if(30000 < (u32)(e_res/v_division))
				{
//					cal_state = 1;
//분해도가 30000 넘어가도 동작하게
//cal_state = 2;
					ret_val = SET_AGAIN;


					vputerr("ERR21 ");
				}
				else
				{
//					cal_state = 2;
					
					vputnum( 0, 0, ret_val); Delay_ms(300); vcls(); Delay_ms(300);
					vputnum( 0, 0, ret_val); Delay_ms(300); vcls(); Delay_ms(300);
				}
				break;
			} //switch
		} //if(keypush())
	} while(!exit_flag);

	return ret_val;
}
	

int Set_decimalpoint()//  decimal point 구하기   default 0
{
    u8 exit_flag = 0;
//	u8 iz;
	int which;
	int	ret_val = 0;

	vputmsg("dp    ");	//2016.3.30
	Delay_ms(500);

	which = gxlDecimalPoint;
	vputnum( 0, 0, (long)which);
	vputnum( 0, 0, gxlDecimalPoint);
/*
	vputchar( 0, 'd');
	vputchar( 1, 'p');
	vputchar( 2, '=');
*/
	do
	{
		if(keypush())
		{
			switch(KeyCode)
			{
				case KEY_ZERO:
					which++;
					which %= 4;
					break;

				case KEY_TARE:
					if(which) which--;
					else which = 3;
					break;

				case KEY_CANCEL:
					exit_flag = 1;
					ret_val = SET_BACKWARD;
					break;

				case KEY_ENTER:
					exit_flag=1;
					ret_val = which;

		vputnum( 0, 0, ret_val); Delay_ms(300); vcls(); Delay_ms(300);
		vputnum( 0, 0, ret_val); Delay_ms(300); vcls(); Delay_ms(300);

					break;					
			}

			vputnum( 0, 0, (long)which);
			vputchar( 0, 'd');
			vputchar( 1, 'p');
			vputchar( 2, '=');

		}
	} while(!exit_flag);

	return ret_val;
}


int Set_division()//  division 구하기   default 5
{
    u8 exit_flag = 0;
	u8 iz;
	u8 which;
	int test_weight[]={1,2,5,10,20,50};
	int	ret_val = 0;

//	vputs("  DIV ");
	vputmsg("div   ");	//2016.3.29
	Delay_ms(500);


	//기존 설정값으로 표시하기 위함.
	for(iz=0; iz<6; iz++) if(test_weight[iz] == v_division) break;
	if(iz < 6) which = iz;
	else which = 0;

	vputnum( 0, 0, test_weight[which]);
	vputchar( 0, 'd');
	vputchar( 1, '=');

	do
	{
		if(keypush())
		{
			switch(KeyCode)
			{
				case KEY_ZERO:
					which++;
					which %= 6;
					break;

				case KEY_TARE:
					if(which) which--;
					else which = 5;
					break;

				case KEY_CANCEL:
					exit_flag = 1;
//					cal_state = 8;
					ret_val = SET_BACKWARD;
					break;

				case KEY_ENTER:
					exit_flag=1;
//					cal_state = 1;
					ret_val = test_weight[which];

		vputnum( 0, 0, ret_val); Delay_ms(300); vcls(); Delay_ms(300);
		vputnum( 0, 0, ret_val); Delay_ms(300); vcls(); Delay_ms(300);

					break;					
			}

			vputnum( 0, 0, test_weight[which]);
			vputchar( 0, 'd');
			vputchar( 1, '=');
		}
	} while(!exit_flag);

	return ret_val;
}


void cal_mode()
{
 char str[30];
	v_ei_multiply_factor=10;
	vputs("CAL 1");  Delay_ms(500);
	v_division = min_set(v_division);	//
	
	
	sprintf(str, "v_division : %d", v_division);	USART1_puts(str);
	sprintf(str, "v_decimal_point : %d", v_decimal_point);	USART1_puts(str);
	
	

	vputs("CAL 2");  Delay_ms(500);
	v_capacity = (long)capacity_set((unsigned int)v_capacity);
	sprintf(str, "v_capacity : %d", v_capacity);	USART1_puts(str);
	
	//if(v_key == ESC_K) return;

	
	
	v_e_resolution = (v_capacity * power(10, v_decimal_point)) / v_division;
	if (v_e_resolution > 15000) {  vputs("ER 21");  delay(500);  }
	v_i_resolution = v_e_resolution *  v_ei_multiply_factor;

  sprintf(str, "v_e_resolution : %d", v_e_resolution);	USART1_puts(str);
	sprintf(str, "v_i_resolution : %d", v_i_resolution);	USART1_puts(str);
	
	
	
	
	
	label_cal3:
	vputs("CAL 3");  Delay_ms(500);
	bRepeat = 0;
	zero_set(); ////if(v_key == ESC_K) return;

	if (v_capacity <= 5) v_setting_weight = 5;
	else if (v_capacity <= 15) v_setting_weight = 10;
	else if (v_capacity <= 60) v_setting_weight = 20;
	else  v_setting_weight = 100;
	vputs("CAL 5");  delay(500);
	span_set(); ////if(v_key == ESC_K) return;
	if (bRepeat) goto label_cal3;

	//adc_initial();

	vputs("CAL 6");  Delay_ms(500);
	little_span_adjust();

	v_adc1_org = v_zero;
	v_adc_org = (long)(((float)v_adc1_org) * gnfFactor);

	
	
	
	////write_factors();  /*  adc_normal_mode() */
	write_cal_factors();
	vputs("C END"); Delay_ms(1000);
	vcls();
} /* of cal_mode */




void calc_equ_factor()
{
	v_capacity = gnlCapacity * power(10, gxlDecimalPoint);
	v_ei_multiply_factor=10; // 1div당  10개의 카운트
	v_e_resolution = v_capacity / v_division;	 
	v_i_resolution = v_e_resolution *  (long)v_ei_multiply_factor;			     // 50000 내부 resolution
	gnfFactor = (float)((float)(10000/(float)gxlEquivalInput) * gnfRefFactor * (float)(v_e_resolution/10000.0));
}

void cal_mode_2()	// G3 등가입력 캘리브레이션
{
//	u16 i;
//	u32 sum_1=0;
//	u32 dummy;
	char str[30];
//	union { u32 dw; u16 u[2]; u8 c[4];} temp;
//	float set_ratio;
//	char k;
//	char cal_step_state;
	int exit_flag = 0;
	long	ret_val;
	int	cal_step = 0;
	
//	cal_state = 0;
	
/*
#define	SET_FORWARD		-101	//저장 안하고 다음 번호로
#define	SET_BACKWARD	-102	//저장 안하고 앞으로
#define SET_END		 	-100	//저장 안하고 SET MODE 종료
*/
	do
	{
		
		 #ifdef dfdfd
		switch(cal_step)
		{
			case 0:
				ret_val = Set_decimalpoint();
				if(ret_val == SET_BACKWARD) exit_flag = -1;
				else gxlDecimalPoint = ret_val;

				cal_step++;

				sprintf(str, "gxlDecimalPoint : %d", gxlDecimalPoint);
				USART1_puts(str);
				break;

			case 1:
//				v_division = Set_division();
				ret_val = Set_division();
				if(ret_val == SET_BACKWARD) cal_step--;
				else { v_division = ret_val; cal_step++; }

				sprintf(str, "v_division : %d", v_division);
				USART1_puts(str);
				break;
				 
			case 2:
///				v_capacity = Set_capa_Weight();//capa
//				gnlCapacity = Set_capa_Weight();	//capa

				ret_val = Set_capa_Weight();
				if(ret_val == SET_BACKWARD) cal_step--;
				else if(ret_val != SET_AGAIN) 
				{
					cal_step++;

					gnlCapacity = ret_val;
					v_capacity = gnlCapacity * power(10, gxlDecimalPoint);
					sprintf(str, "v_capacity : %d", v_capacity);
					USART1_puts(str);
	
					/*	
					v_decimal_point=gxlDecimalPoint;                                                                         
					//Set_decimal_point();	 // division 50 g
					
					sprintf(str, "v_decimal_point : %d", v_decimal_point);
					USART1_puts(str);
					*/
					
					v_ei_multiply_factor=10; // 1div당  10개의 카운트
					sprintf(str, "v_ei_multiply_factor : %d", v_ei_multiply_factor);
					USART1_puts(str);
		
					v_e_resolution = v_capacity / v_division;	 
					sprintf(str, "v_e_resolution : %d", v_e_resolution);
					USART1_puts(str);
					
					v_i_resolution = v_e_resolution *  (long)v_ei_multiply_factor;			     // 50000 내부 resolution
					sprintf(str, "v_i_resolution : %d", v_i_resolution);
					USART1_puts(str);
				}				
				break;
				
			case 3:
//				zero_set(); ////if(v_key == KEY_ENTER)	 // 무부하상태에서의  a/d 값 
				ret_val = cal_zero_span_ad_set(CAL_ZERO_AD_SET);
				if(ret_val == SET_BACKWARD) cal_step--;
				else if(ret_val == SET_RESTART) cal_step = 0;
				else if(ret_val == SET_AGAIN) ;
				else
				{
					cal_step++;
					dZeroValue = ret_val;
					sprintf(str, "dZeroValue : %d", dZeroValue);
					USART1_puts(str);
				}				
				break;
				
			case 4:
//				gxlEquivalInput = lValueSetMode_5digit_with_dp( 0, gxlEquivalInput, MAX_gxlEquivalInput, 4);
				ret_val = lValueSetMode_dp( 0, gxlEquivalInput, MAX_gxlEquivalInput, 4);	
				if(0 < ret_val) gxlEquivalInput = ret_val;
				if(ret_val == SET_FORWARD) cal_step--;
				else cal_step++;
					
				sprintf(str, "gxlEquivalInput : %d", gxlEquivalInput);
				USART1_puts(str);
//vputnum(0,0, gxlEquivalInput);	Delay_ms(5000);
				break;
	
			case 5:
				cal_step++;
				//calc_factor();// calibration 계산하는 곣  gnfFactor
				//v_setting_weight = v_capacity / 10;			
				//v_span = (Vmv/gxlEquivalInput/1000) * mv_factor(ad값) * 10(v);
				//ratio = (float)((float)v_capacity/(float)v_setting_weight);// 250/40 =16
				//gnfFactor = (float)(((float)v_i_resolution)/((float)v_span*0.1));
				//v_span = (gxlEquivalInput/1000) * gnfVoltPerMv * 10(v);
				//v_span = (gxlEquivalInput/100) * gnfVoltPerMv;
///				gnfFactor = (float)(((float)v_i_resolution)/((float)(gxlEquivalInput/100.0) * gnfVoltPerMv));

///	gnfFactor = (float)((float)(10000/(float)gxlEquivalInput) * gnfRefFactor * (float)(v_e_resolution/10000.0));
				calc_equ_factor();


/** 8400C
	v_e_resolution = (v_capacity * power(10, v_decimal_point)) / v_division;
	if (v_e_resolution > 10000 ) {  vputs("ERR  21");  delay(500);  }
    if (v_e_resolution > 4999) uEiResol = 5;
	v_i_resolution = v_e_resolution * uEiResol;

//	gnfFactor = (float)((1.0/(float)v_mv) * gnfRefFactor);
	gnfFactor = (float)((float)(1000/(float)v_mv) * gnfRefFactor * (float)(v_e_resolution/10000.0));
//	fast_evalue((long)(gnfFactor*10000000));
//	delay(2000);

	v_adc_org = (long)gndwZero * gnfFactor / gnfRefFactor;
**/

/*
			vputnum(0,0, gxlEquivalInput/10);	Delay_ms(5000);
			vputnum(0,1, gnfVoltPerMv*100000);	Delay_ms(5000);
			vputnum(0,2, v_i_resolution);	Delay_ms(5000);
			vputnum(0,3, gnfFactor*100000);	Delay_ms(5000);
*/			
				sprintf(str, "gxlEquivalInput=%ld", gxlEquivalInput); USART1_puts(str);
				sprintf(str, "gnfRefFactor=%f", gnfRefFactor); USART1_puts(str);
				sprintf(str, "v_i_resolution=%ld", v_i_resolution); USART1_puts(str);
				sprintf(str, "gnfFactor=%f", gnfFactor); USART1_puts(str);

				sprintf(str, "cal_step : %d", cal_step);
				USART1_puts(str);
				break;

			case 6: 
				v_zero = dZeroValue;
				ret_val = little_span_adjust();	//무게환산해서 표시해주는곳 
				if(ret_val == SET_BACKWARD) cal_step--;
				else if(ret_val == SET_RESTART) cal_step = 0;
				else if(ret_val == SET_AGAIN) ;
				else
				{
					cal_step++;

					v_adc1_org = v_zero;
					v_adc_org = (long)(((float)v_adc1_org) * gnfFactor); // 무부하상태에서의 카운트값 
					//GLCD_string(0,0, " 무게조정 종료 "); Delay_ms(2000);
					// lcd_cls();
				}
				break;
			
			case 7: 
				exit_flag = 1;
				gdwWhichCal = 1;
				write_cal_factors(gdwWhichCal);  // cal data  저장 
//				vputnum( 1, 4, gxlEquivalInput);
//				Delay_ms(1000);

				vputs("-END- ");
				Delay_ms(3000);
				beep(10); 
				//while(1){};

				break;
			
			case 8 : 
			default : exit_flag = 1; break;
		}
		
		#endif
	} while(!exit_flag);
}


 void cal_mode_eq_ref()	//등가입력 기준 캘 (로드셀 입력에 1mv 입력 할것)
{
    char str[30];
//	union { u32 dw; u16 u[2]; u8 c[4];} temp;

//	u16 i;
//	u32 sum_1=0;
//	long dummy;
	long ret_val;
//	float set_ratio;
//	char k;
	char cal_step = 0;
	int exit_flag = 0;
	
	vputmsg("CAL R ");	//2016.3.31
	Delay_ms(500);

	do
	{
		switch(cal_step)
		{
			case 0: //무부하상태에서의  a/d 값 
				ret_val = cal_zero_span_ad_set(CAL_ZERO_AD_SET);
				if(ret_val == SET_BACKWARD) exit_flag = -1;
				else if(ret_val == SET_RESTART) cal_step = 0;
				else if(ret_val == SET_AGAIN) ;
				else
				{
					cal_step++;
					dZeroValue = ret_val;
					sprintf(str, "dZeroValue : %ld", dZeroValue);
					USART1_puts(str);

					v_division = 1;

					gnlCapacity = 1;
					gxlDecimalPoint = 4;

					v_capacity = gnlCapacity * power(10, gxlDecimalPoint);
					v_setting_weight = 1 * power(10, gxlDecimalPoint);// 기준분동무게 구하기
					v_ei_multiply_factor = 1;
					v_e_resolution = v_capacity / v_division;	 
					v_i_resolution = v_e_resolution * (long)v_ei_multiply_factor;
				
//vputnumwithled(1,0, v_i_resolution); Delay_ms(10000);
//vputnumwithled(1,1, v_setting_weight); Delay_ms(10000);

				}
				break;

			case 1 : //분동무게(40)를 올렸을때의 a/d 값을 구한다.
				//span_set();
				ret_val = cal_zero_span_ad_set(CAL_SPAN_AD_SET);
				if(ret_val == SET_BACKWARD) cal_step--;
				else if(ret_val == SET_RESTART) cal_step = 0;
				else if(ret_val == SET_AGAIN) ;
				else
				{
					cal_step++;
					dFullValue = ret_val;
					sprintf(str, "dFullValue : %d", dFullValue);
					USART1_puts(str);
				}
				break;
			
			case 2 : 
				ret_val = calc_factor();// calibration 계산하는 곣  gnfFactor
				if(ret_val == SET_BACKWARD) cal_step--;
				else if(ret_val == SET_RESTART) cal_step = 0;
				else if(ret_val == SET_AGAIN) ;
				else
				{
					cal_step++;
/*			
vputnum(0,0,gnfFactor*(float)power(10, 9)); Delay_ms(1000);	
vputnum(0,1,gnfFactor*(float)power(10, 9)); Delay_ms(1000);	
vputnum(0,2,gnfFactor*(float)power(10, 9)); Delay_ms(1000);	
vputnum(0,3,gnfFactor*(float)power(10, 9)); Delay_ms(1000);	
*/					
					sprintf(str, "cal_step : %d", cal_step);
					USART1_puts(str);
				}
				break;

			case 3 : 
				ret_val = little_span_adjust();	//무게환산해서 표시해주는곳 
				if(ret_val == SET_BACKWARD) cal_step--;
				else if(ret_val == SET_RESTART) cal_step = 0;
				else if(ret_val == SET_AGAIN) ;
				else
				{
					cal_step++;

					v_adc1_org = v_zero;
					v_adc_org = (long)(((float)v_adc1_org) * gnfFactor); // 무부하상태에서의 카운트값 
					//GLCD_string(0,0, " 무게조정 종료 "); Delay_ms(2000);
					// lcd_cls();
				}
				break;
			
			case 4 : 
				exit_flag = -1;
//				cal_step++;
				gdwWhichCal = 2;
				write_cal_factors();  // cal data  저장 
/*	
vputnum(0,0,gnfRefFactor*(float)power(10, 9)); Delay_ms(1000);	
vputnum(0,1,gnfRefFactor*(float)power(10, 9)); Delay_ms(1000);	
vputnum(0,2,gnfRefFactor*(float)power(10, 9)); Delay_ms(1000);	
vputnum(0,3,gnfRefFactor*(float)power(10, 9)); Delay_ms(1000);	
*/

	vputs("-END- ");
	while(1){};

//				break;
				
			case 6 : 
			default : exit_flag = 1; break;
		 }
	} while(!exit_flag);
}


long cal_zero_span_ad_set(int which)
{
	int	iz;
	int	exit_flag = 0;
	long	ret_val = 0;
	u32  gxhFilter_backup;		/* gxhFilter Backup */
	long law_data;
	u8 str[50];

	gxhFilter =10;
	#define DIVISION_FACTOR 70

//0927
	do
	{
		
		//	 if(key_timer>101) // Sampling rate : 10 hz   at least gurantee more 100 mser
    	 if(key_timer>13) // Sampling rate : 10 hz   at least gurantee more 100 mser
				
			 {
			 
			 key_timer=0;
 		
			 //GPIO_WriteBit(GPIOA, GPIO_Pin_8, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_8))); 
			 
		   ret_val= read_filtered_adc();
		   sprintf(str, "adc=%5u",(ret_val));
  		 USART1_puts(str);
		 }
			 
		PUSH_BUTTON_ON
		{
			    exit_flag = 1;
		}
		
		/*
		if(keypush())
		{
			switch(KeyCode)
			{
				case KEY_CANCEL:
					exit_flag = -1;
					ret_val = SET_BACKWARD;
					break;

				case KEY_ENTER:
				    exit_flag = 1;
					ret_val = 0;
					for (iz=0; iz<CAL_SAMPLING_NUMBER; iz++) ret_val += read_filtered_adc(); 
					ret_val = ret_val / CAL_SAMPLING_NUMBER;
					break;

				default:
					retry_message();
					break;
			}
		} //of keypush
		*/
	} while(!exit_flag);
		return (ret_val);
} /* of zero_set() */


void zero_set()
{
	int	  i,j;
	char str[30];
	u8 exit_flag = 0;
	u32 sum_temp = 0L, temp, temp_buff, i_diff,sum;
	u8  hBUgxhFilter;		/* gxhFilter Backup */
  u8 key_code;
	SIGNFLAG = 0;
	hBUgxhFilter = gxhFilter;
	gxhFilter = 50;
mainload:
	temp      =  v_zero;
	vputs("CAL 3"); Delay_ms(200);
	vputs("ULOAD");
  adc_timer=0;
	i=0;
	do
     {
       if(adc_timer>=_80SPEED)// 
	       {
		      i++;
	          adc_timer=0;
 		        temp_buff=read_filtered_adc();// get weight and judge stable
		        sprintf(str, "zero_value : %d", temp_buff);	USART1_puts(str);
		        sum_temp+=temp_buff;
				 }// of key timey  
       if(i>50)
				 
		   break;
     }while(1);
		  
		 temp_buff=(sum_temp/50);
		 sprintf(str, "final zero_value : %d", temp_buff);	USART1_puts(str);
		 Delay_ms(500);
		 
		 adc_timer=0;
		 i=0;
		 sum_temp=0;
		 
	do
	{
	    if(adc_timer>=_80SPEED)// 
	       {
		        adc_timer=0;
 		        temp=read_filtered_adc();// get weight and judge stable
					  vputnum_6digit(temp);
		        sprintf(str, "zero=%6d", (temp/1));	USART1_puts(str);
		        i++;
					  sum_temp+=temp;
				 }// of key timey  
  /*
    if(i>=10)
		{
			i=0;
			sum=sum_temp;
			sum_temp=0;
			vputnum_5digit(sum/10);
		}	
	*/			 
				 

		key_code=keypush();
		
		if (key_code)
		{
			switch(key_code)
			{
				default:  /* ANY key */
					vcls();
					sum_temp = 0;
				
			  	
				 for (j=0; j < CAL_SAMPLING_NUMBER; j++)
					{
						switch (j/5)
						{
							case 0:	Display_buffer[5] = 0x01; break; /* a ON */
							case 1: Display_buffer[6] = 0x08; break; /* d ON */
							case 2: Display_buffer[7] = 0x40; break; /* g ON */
							case 3: Display_buffer[8] = 0x01; break; /* a ON */
							case 4: Display_buffer[9] = 0x08; break; /* d ON */
						}
						Delay_ms(100);
					 /*	
						temp = read_filtered_adc();
						if (temp_buff > temp) i_diff = temp_buff - temp;
						else i_diff = temp - temp_buff;
						if (i_diff > 50)
						{
							vputs("ER 28");  //  unstable  
							retry_message();
							Delay_ms(1000);
							goto mainload;
						}  // if (i_diff > 2500) 
						sum_temp += temp;
					 */
					} // for j 
					
					
					////temp = sum_temp / CAL_SAMPLING_NUMBER;

					
					dZeroValue = temp;
					if (dZeroValue < ZERO_LOW_LIMIT)
					{
						vputs("ER 27"); Delay_ms(1000);  /* zero low */
						vputnum( 0, 0, temp); Delay_ms(300);
						
						dZeroValue = ZERO_ERROR_LIMIT;
						goto mainload;
					}
					else if (ZERO_HIGH_LIMIT < dZeroValue)
					{
						vputs("ER 26"); Delay_ms(1000); // zero high 
						vputnum( 0, 0, temp); Delay_ms(1000);
						
						dZeroValue = ZERO_ERROR_LIMIT;
						goto mainload;
					}
					vputs("SUCCS");  Delay_ms(500);
					exit_flag =1; /* go to LOAD */
			} /* switch (v_key) */
		} /* of if (keypush()) */
	////	vputs("ULOAD");
	} while (!exit_flag);
	gxhFilter = hBUgxhFilter;
} /* of zero_set() */


void span_set()
{
	 u8 str[30];
	int	  i,j;
	float ratio;
  u8  exit_flag = 0;
	u32 sum_temp = 0L, temp, temp_buff, temp_full, i_diff;
	u32 low_limit,		/* span low limit */
		  high_limit;		/* span high limit */
	long  v_full, v_span;
	u8  hBUgxhFilter;		/* gxhFilter Backup */
  u8 key_code;
	SIGNFLAG = 0;
	hBUgxhFilter = gxhFilter;
	gxhFilter =50;
mainload:
	temp_full =  v_full;
	temp      =  v_zero;
	vputs("CAL 5"); Delay_ms(500);
	vputs(" UP  "); Delay_ms(1000);

	i=0;
		do
     {
       if(adc_timer>=_80SPEED)// 
	       {
		      i++;
	          adc_timer=0;
 		        temp_buff=read_filtered_adc();// get weight and judge stable
		        sprintf(str, "zero_value : %d", temp_buff);	USART1_puts(str);
		  	 }// of key timey  
       if(i>100)
				 
		   break;
     }while(1);

	
	i=0;
		do
     {
       if(adc_timer>=_80SPEED)// 
	       {
		      i++;
	          adc_timer=0;
 		        temp_buff=read_filtered_adc();// get weight and judge stable
		        sprintf(str, "zero_value : %d", temp_buff);	USART1_puts(str);
		  	 }// of key timey  
       if(i>100)
				 
		   break;
     }while(1);

	
	
	
	
	
	do
	{ 
		
		 if(adc_timer>=_80SPEED)// 
	       {
		        adc_timer=0;
 		        temp=read_filtered_adc();// get weight and judge stable
					  vputnum_6digit(temp);
		        sprintf(str, "zero=%6d", (temp/1));	USART1_puts(str);
		     }//
		
		
		
		key_code=keypush();
		if (key_code)
		{
//			v_key = key_disp[KEYCODE];
//			switch (v_key)
			switch(key_code)
			{
/*
				case SET_K:
				case ESC_K:
				case ONOFF_K:
					onoff_test();
					exit_flag = 1;
					break;
*/
				default:  /* ANY key */
					vcls();
					sum_temp = 0;
					{
						switch (j/5)
						{
							case 0:	Display_buffer[5] = 0x01; break; /* a ON */
							case 1: Display_buffer[6] = 0x08; break; /* d ON */
							case 2: Display_buffer[7] = 0x40; break; /* g ON */
							case 3: Display_buffer[8] = 0x01; break; /* a ON */
							case 4: Display_buffer[9] = 0x08; break; /* d ON */
						}
						//temp = read_filtered_adc();
						if (temp_buff > temp) i_diff = temp_buff - temp;
						else i_diff = temp - temp_buff;
					 	/*
						if (i_diff > 2550)
						{
							vputs("ER 28");  //unstable  
							retry_message();
							delay(1000);
							goto mainload;
						}  // if (i_diff > 2500) 
						*/
						sum_temp += temp;
					} /* for j */
					//temp = sum_temp / CAL_SAMPLING_NUMBER;
					dFullValue = temp;

					v_zero = dZeroValue;
					if (v_zero == ZERO_ERROR_LIMIT) continue;
					temp_full = dFullValue;
					low_limit =(long)((float)v_i_resolution/((float)v_capacity/(float)v_setting_weight));
					high_limit = temp_full-v_zero;
					high_limit *= ((float)v_capacity/(float)v_setting_weight);
					high_limit += v_zero;
					if ((temp_full - v_zero) < low_limit)
					{
						vputs("ER 24"); /*   span low  */
						retry_message();
						delay(500);
						////fast_evalue(temp_full - v_zero);  delay(1000);
						vputnum( 0, 0, temp); Delay_ms(300);
						bRepeat = 1;
						continue; /* goto mainload; *//* return; */
					} /* if (temp_full) */
					else if((temp_full-v_zero) > ((float)FULL_LIMIT/((float)v_capacity/(float)v_setting_weight)))
					{
						vputs("ER 25");  /* span high */
						retry_message();
						Delay_ms(500);
						////fast_evalue(temp_full - v_zero);  delay(1000);
						bRepeat = 1;
						continue; /* goto mainload; */ /* return; */
					} /* if (temp_full) */
					else if (ADC_SATURATION <= high_limit)
					{
						vputs("ER 29");  /* ad saturation */
						retry_message();
						Delay_ms(500);
						//fast_evalue(temp_full - v_zero);  delay(1000);
						vputnum( 0, 0, (temp_full-v_zero)); Delay_ms(300);
						bRepeat = 1;
						continue; /* goto mainload; */ /* return; */
					}
					else
					{
						v_full = temp_full;
						v_span = v_full- v_zero;
						ratio = (float)((float)v_capacity/(float)v_setting_weight);
						gnfFactor = (float)(((float)v_i_resolution)/((float)v_span*ratio));
						v_adc_org = (long)(((float)v_zero)*gnfFactor);
						bRepeat = 0;
						exit_flag = 1;
						vputs("SUCCS");  Delay_ms(300);
						
						sprintf(str, "v_zero=%6d",(int)v_zero);	USART1_puts(str);
						sprintf(str, "v_full=%6d",(int)v_full);	USART1_puts(str);
						sprintf(str, "v_span=%6d",(int)v_span);	USART1_puts(str);
						sprintf(str, "v_ratio=%1.7f",(float)ratio);	USART1_puts(str);
						sprintf(str, "gnfFactor=%1.7f",(float)gnfFactor);	USART1_puts(str);
						sprintf(str, "v_adc_org=%6d",(int)v_adc_org);	USART1_puts(str);
						
						
						
						
						
						
					}
					break;
			} /* switch (v_key) */
		} /* of if (keypush()) */
		////else  vputs(" UP  ");
	} while (!exit_flag);
	gxhFilter = hBUgxhFilter;
} /* of span_set() */







int calc_factor()
{
	char str[30];
	float ratio;
//	u8  exit_flag = 0;
	u32 temp_full;
	u32 low_limit,		/* span low limit */
		  high_limit;		/* span high limit */
//	char  repeat_flag = 0;
	long  v_full, v_span;
	u8  bAdcLow = 0;			/* zero, full value high */
	u8  bAdcHigh = 0;			/* zero, full value low */
	u8  bAdcSaturation = 0;	/* ad saturation */
	long	ret_val;


	bAdcLow = bAdcHigh = bAdcSaturation = 0;
	ratio = (float)((float)v_capacity/(float)v_setting_weight);// 250/40 =16
	
	{
		v_zero = dZeroValue;		//zero상태의 a/d 값 
		temp_full = dFullValue;		// 부하상태 (분도무게 올렸을때의)a/d 값 
		v_full = temp_full;	  		// 40 에서의 a/d 값 	    zero= 6780   40 kg a/d 값 =12345
		v_span = v_full- v_zero;	// 40 kg 에서의 span 값  = 12345-6780=5565  

		USART2_puts("1234567890");
		sprintf(str, "V_ZERO=%ld,%ld", v_zero, dZeroValue);
		USART2_puts(str);
		sprintf(str, "V_FULL=%ld,%ld", v_full, dFullValue);
		USART2_puts(str);
		sprintf(str, "V_SPAN=%ld", v_span);
		USART2_puts(str);
		sprintf(str, "V_LIMIT=%f", (float)FULL_LIMIT/ratio);
		USART2_puts(str);
		sprintf(str, "F_LIMIT=%ld", FULL_LIMIT);
		USART2_puts(str);
		sprintf(str, "ratio=%f", ratio);
		USART2_puts(str);

/*
vputnumwithled(1,0, v_zero); Delay_ms(10000);
vputnumwithled(1,1, v_full); Delay_ms(10000);
vputnumwithled(1,2, v_span); Delay_ms(10000);
vputnumwithled(1,3, ratio*1000); Delay_ms(10000);
vputnumwithled(1,4, FULL_LIMIT/ratio); Delay_ms(10000);
vputnumwithled(0,0, v_span); Delay_ms(10000);
*/
		/*************************************************************************************************************
		capa 250kg, 1div 50g 일 때 분해도는 1/5000이고 1눈금당 10개의 카운트가 있어야 하므로 디지털 스팬은 50000 이상이어야 한다.
		만약 기준분동을 40kg을 올린다면 디지털 스팬은 50000*(40/250) 이상이어야 한다. 이 값을 low_limit 라는 변수에 담는것이다.
		*************************************************************************************************************/
		low_limit = (float)v_i_resolution/ratio;// v_capacity : v_i_resolution = v_setting_weight : low_limit
		sprintf(str, "low_limit : %d", low_limit);
		USART1_puts(str);

		high_limit = v_span * ratio + v_zero;
		sprintf(str, "high_limit : %d", high_limit);
		USART1_puts(str);

		sprintf(str, "err25 : %f", ((float)FULL_LIMIT/(ratio)));
		USART1_puts(str);

		sprintf(str, "FULL_LIMIT : %d", FULL_LIMIT);
		USART1_puts(str);

		if (v_span < low_limit)
		{
			bAdcLow++;
//			goto mainload;
		} /* if (temp_full) */
		else if(((float)FULL_LIMIT/ratio) < v_span)
		{
///vputnumwithled(0,1, ((float)FULL_LIMIT/ratio)); Delay_ms(10000);
			bAdcHigh++;
//			goto mainload;
		} /* if (temp_full) */
//		else if (ADC_SATURATION <= high_limit)
//		{
//			bAdcSaturation++;
//			goto mainload;
//		}
		else  //  cal에 성공하면 
		{
			gnfFactor = (float)(((float)v_i_resolution)/((float)v_span*ratio));
			sprintf(str, "gnfFactor : %f", gnfFactor);
			USART1_puts(str);

			// =50000/(5565*16)=0.56148231   ->span값 (250 kg )에다가 gnfFactor 값을 곱하면 50000( 분해도 *10눈금 ) 
/*
			ctemp.f = gnfFactor;
			write_eep(ADR_gnfFactor, ctemp.u[0]);
			Delay_ms_time2(10);
			write_eep(ADR_gnfFactor+1, ctemp.u[1]);
			Delay_ms_time2(10);
								
			gnfFactor = gnfFactor;
*/			
			v_adc_org = (long)(((float)v_zero)*gnfFactor);	 // 무부하상태에서의 카운트값 **
			bAdcLow = bAdcHigh = bAdcSaturation = 0;
/*
			sprintf(str, "v_zero = %8ld g ", v_zero); GLCD_string (0, 2, str); Delay_ms(5000);
			sprintf(str, "v_full = %8ld g ", v_full); GLCD_string (0, 2, str); Delay_ms(5000);
			sprintf(str, "ratio  = %8.5f g ", ratio); GLCD_string (0, 2, str); Delay_ms(5000);
			sprintf(str, "v_e_re = %8ld   ", v_e_resolution); GLCD_string (0, 2, str); Delay_ms(5000);
			sprintf(str, "v_i_re = %8ld   ", v_i_resolution); GLCD_string (0, 2, str); Delay_ms(5000);
			sprintf(str, "v_span = %8ld   ", v_span); GLCD_string (0, 2, str); Delay_ms(5000);
			sprintf(str, "factor = %8ld ", (long)(gnfFactor*1000000)); GLCD_string (0, 2, str); Delay_ms(20000);
//          sprintf(str, "factor 01 = %8f ", i, gnfFactor); GLCD_string (0, 2, str); Delay_ms(20000);
*/
//			repeat_flag = 0;
//			exit_flag = 1;
//			cal_state = 6;
			//GLCD_string(0,2, "  SUCCESS      ");
			//GLCD_string(0,2, "  SUCCESS      ");
			//	GLCD_string(0,2, " 무게조정 성공");
			vputs("OK    ");
			
			
			Delay_ms(2000);
		}
	} /* for (hIndex = 0; hIndex < 7; hIndex++) */
	
	if (bAdcLow)
	{
		//GLCD_string(0,1, "ERROR 24 : SPAN LOW ");
		//retry_message();
		///vputs("ERR24 ");
		///Delay_ms(500);
		vputerr("ERR24 ");

		//VfdPrintf("GAIN%2u", (u16)hLoop); Delay_ms(1000);
		//fast_evalue(temp_full - v_zero);  Delay_ms(1000);
		//flag = 0;  /* cal 3 */
		//repeat_flag = 1;
		//cal_state = 4;
		
//		repeat_flag = 0;
//		exit_flag = 1;
		ret_val = SET_RESTART;
//		cal_state = 0;
	}
	else if (bAdcHigh)
	{
		//GLCD_string(0,2, "ERROR 25 : SPAN HIGH");
		///vputs("ERR25 ");
		//retry_message();
		///Delay_ms(500);
		vputerr("ERR25 ");

		//VfdPrintf("GAIN%2u", (u16)hLoop); Delay_ms(1000);
		//fast_evalue(temp_full - v_zero);  Delay_ms(1000);
		//flag = 0;  /* cal 3 */
//		repeat_flag = 0;
//		exit_flag = 1;
//		cal_state = 0;
		ret_val = SET_RESTART;
	}
	else if (bAdcSaturation)
	{
		//GLCD_string(0,2, "ERROR 29 : AD OVER  ");
		///vputs("ERR29 ");
		//retry_message();
		///Delay_ms(500);
		vputerr("ERR29 ");

		//VfdPrintf("GAIN%2u", (u16)hLoop); Delay_ms(1000);
		//fast_evalue(temp_full - v_zero);  Delay_ms(1000);
		//flag = 0;  /* cal 3 */
//		repeat_flag = 0;
//		exit_flag = 1;
//		cal_state = 0;
		ret_val = SET_RESTART;
	}
	else
	{
		ret_val = SET_FORWARD;
	}

	return ret_val;
} /* of zero_set() */


int little_span_adjust()	//0320
{
	int  exit_flag=0;	//, sign;
	char  iz;
	char key_code;
	float raw_data;
	s32  gxlGrossWeight_1;
	char str[30];
	u8  gxhFilterBackup;
	s32  v_i_value;
	long ret_val = 0;
	gxhFilter=50;

	v_adc1_org = v_zero;
	v_adc_org = (long)(((float)v_adc1_org) * gnfFactor);	// 제로에 대한 count 값
	sprintf(str, "raw_data : %f", raw_data); USART1_puts(str);
	sprintf(str, "v_adc1_org : %d", v_adc1_org); USART1_puts(str);
	sprintf(str, "v_adc_org : %d", v_adc_org);	USART1_puts(str);
	sprintf(str, "gnfFactor : %f", gnfFactor);	USART1_puts(str);


//0320
	do
	{
		key_code=keypush();
		if(key_code==GRADE_K)
			 exit_flag=1;

		if(adc_timer>=_80SPEED)// 
	    {
		    
		   adc_timer=0;
		   raw_data = read_filtered_adc();		// ad 값으 읽어온다ㅣ
		   v_adc1_buf = (s32)(gnfFactor * raw_data);	// count 값 입력(무게에 대한)
		   v_i_value  =  v_adc1_buf - v_adc_org;	//현재 카운트 값에서 zero 에서의 카운트갓을 뺀다. 

	
		if (0 <= v_i_value) 
		{
			gxlGrossWeight_1 = (v_i_value + (v_ei_multiply_factor/2));  // 10000+(10/2)=10005
			gxlGrossWeight_1=(gxlGrossWeight_1 / v_ei_multiply_factor);	//10005/10=1000
			gxlGrossWeight_1=gxlGrossWeight_1 * v_division;					//1000*5=5000
//			sign=0;
		}
		else  
		{
//			sign=1;
//			v_i_value=v_i_value*-1;
			gxlGrossWeight_1 = (v_i_value + (v_ei_multiply_factor/2));
			gxlGrossWeight_1=(gxlGrossWeight_1 / v_ei_multiply_factor);
			gxlGrossWeight_1=gxlGrossWeight_1 * v_division;
		}

		  // vputnum_5digit(gxlGrossWeight_1);
		gxlDecimalPoint=3;
		Display_Weight(gxlGrossWeight_1);	  //50.00
		
	}	
		
	} while (!exit_flag);
//	gxhFilter = gxhFilterBackup;

	return ret_val;
} /* little() */

unsigned int  min_set(unsigned int init_value)
{
   unsigned char exit_flag =0;
   unsigned char c_string[8]; /* = {'D', '=', ' ', ' ', ' ', ' ', ' '}; */
   unsigned int  ret_value;
   unsigned char key_code;
	ret_value = init_value;   /* index =0; */
	do
	{
		 key_code=keypush(); 
		if (key_code)
		{
//			switch( v_key = key_disp[KEYCODE] )
			switch(key_code)
			{
				case COUNT_K: /* rotate */
					if(ret_value == 10) ret_value = 20;
					else if(ret_value == 20) ret_value = 50;
					else if(ret_value == 50){ ret_value = 1; v_decimal_point = 3;}
					else if(ret_value == 1) ret_value = 2;
					else if(ret_value == 2) ret_value = 5;
					else
					{
						ret_value = 1;
						if(v_decimal_point == 3) v_decimal_point = 2;
						else if(v_decimal_point == 2) v_decimal_point = 1;
						else if(v_decimal_point == 1) v_decimal_point = 0;
						else ret_value = 10;
					}
					break;
				case SET_K: /* Enter Key (PF_key) */
					exit_flag =1;
					break;
/*
				case ONOFF_K:
					onoff_test();
					break;
*/
				default:
					retry_message(); break;
			} /* of switch */
		} /* of if */
		else
		{
			if (v_decimal_point == 3)
			{
				sprintf(c_string, "D%04d", ret_value);
				c_string[1]='/';
				vputs(c_string);
				//Display_buffer[6]=SEGMENT_ZERO+SEGMENT_DOT;
				
			}
			else
			{
				sprintf(c_string, "D%4d", ret_value);
				if (v_decimal_point)
				{
					if (v_decimal_point == 1) 
					     c_string[3] ='/';
					else   { c_string[2] = '/'; /*  0. */ c_string[3] = '0'; }
				}
				vputs(c_string);
			}
		}
	}  while (!exit_flag);
	return(ret_value);
}  /* of min_set */

unsigned int  capacity_set(unsigned int init_value)
{
	unsigned char c_string[8]; 
	unsigned char exit_flag =0;
	unsigned int  ret_value;
  unsigned char key_code;
	ret_value = init_value;   /* index =0; */
////	VfdPrintf("C%4u", ret_value);
	//if(ret_value<15 || ret_value>600)
    ret_value=60;		
	
	do
	{
		 key_code=keypush();
	//	VfdPrintf("C%4u", ret_value);
        sprintf(c_string, "C%4d", ret_value);
				vputs(c_string);
				
		if (key_code)
		{
//			switch(v_key = key_disp[KEYCODE])
			switch(key_code)
			{
				case COUNT_K: /* rotate */
					switch (ret_value)
					{
						case 15:	ret_value = 20;  break;
						case 20:	ret_value = 30;  break;
						case 30:	ret_value = 60;  break;
						case 60:	ret_value = 150; break;
						case 150:	ret_value = 300; break;
						case 300:	ret_value = 600; break;
						case 600:	ret_value = 15;  break;
						default:	ret_value = 60;  break;
					}
				///	VfdPrintf("C%4u", ret_value);
					break;
				case SET_K: /* Enter Key (PF_key) */
					exit_flag =1;
					break;
/*
				case  ONOFF_K:
					 onoff_test();
					 break;
*/					 
				default:
					retry_message(); break;
			} /* of switch */
		} /* of if */
	}  while (!exit_flag);
	return(ret_value);
}  /* of capacity_set */


/* end of file */
