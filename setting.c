/* 
	set.c
*/

#include "stdio.h"

#include "stm32f10x_lib.h"
#include "stm32f10x_it.h"

#include "LCD.h"	  
#include "i2c_ee.h"
#include "adc.h"
#include "cal.h"

#include "key.h"


void setting(void);


char setting_number=0;

extern volatile u16	KeyCode;

extern u16 Display_buffer[];

u32 gxlDecimalPoint;				// gxlDecimalPoin 소수점 위치설정

u32 gxhWeightBackup;

u32 gxhMotionBand;
u32 gxhZTDigit;
u32 gxuAutoZero;
u32 gxhFilter;
u32 gxhZTKStable;
u32 gxlZeroKeyRange;
u32 gxlTareKeyRange;
u32 gxhHoldType;
u32 gxhEIMode;
u32 gxlKeySet;
u32 gxlCodeNumSet;
u32 gxlHoldOffTime;
vu32 gxhRelayMode;
u32 gxlDataOutDelay;
u32 gxlRelayOnDelay;
u32 gxlRelayOnTime;


vu32 gdhParity;
vu32 gxhBaudRate;
vu32 gxhSendMode;
vu32 gxlSerialMethod;
vu32 gxhMyAddress;
vu32 gxhRsFormat;
vu32 gxlBCCSelect;
vu32 gxlTransPerSec;
vu32 gxlWeightUnit;


u32 gxlDisplaySpeed;
u32 gxlStableOnDelay;
u32 gxlPushPull;
u32 gxhAroundZero;
u32 gxlZeroDisplayRange;
u32 gxlZeroMinus;
u32 gxlAnalogOutMax;
u32 gxlEquivalInput;
u32	gdwWhichCal;
u32 gxhGain;

#define	SET_FORWARD		-101	//저장 안하고 다음 번호로
#define	SET_BACKWARD	-102	//저장 안하고 앞으로
#define SET_END		 	-100	//저장 안하고 SET MODE 종료


typedef struct set_list_struct {
	int f_no;	//set no
	long *val;	//설정값의 포인터
	long limit;	//설정값의 최대값
	int	 adr;	//설정값 저장할 eeprom address
	char dp;	//설정값 소숫점
} SET_LIST;

static const SET_LIST set_list[] = {
	{  1, &gxlDecimalPoint, MAX_gxlDecimalPoint, ADR_gxlDecimalPoint, 0 },	// 소수점 위치설정 0~3
	{  2, &gxhWeightBackup, MAX_gxhWeightBackup, ADR_gxhWeightBackup, 0 },	// 영점 기억모드 0~1
//Program Size: Code=25996 RO-data=612 RW-data=704 ZI-data=2408  
	{  3, &gxhMotionBand, MAX_gxhMotionBand, ADR_gxhMotionBand, 0 },	// 안정범위 일정 시간 내에 중량 변화폭이 설정 범위 이내일 때 안정 기능 작동 0~9
	{  4, &gxhZTDigit, MAX_gxhZTDigit, ADR_gxhZTDigit, 0 },	// 설정값 이하로 중량값이 표시되어 안정된다면 그 순간에 “0”kg으로 0~9
	{  5, &gxuAutoZero, MAX_gxuAutoZero, ADR_gxuAutoZero, 0 },	// AutoZero 0~99
	{  6, &gxhFilter, MAX_gxhFilter, ADR_gxhFilter, 0 },	// 디지털 필터 범위 : 01~49
	{  7, &gxhZTKStable, MAX_gxhZTKStable, ADR_gxhZTKStable, 0 },	// 영점, 용기 작동 모드
	{  8, &gxlZeroKeyRange, MAX_gxlZeroKeyRange, ADR_gxlZeroKeyRange, 0 },	// 제로키 동작 범위
	{  9, &gxlTareKeyRange, MAX_gxlTareKeyRange, ADR_gxlTareKeyRange, 0 },	// 용기 키 동작 범위
	{ 10, &gxhHoldType, MAX_gxhHoldType, ADR_gxhHoldType, 0 },	// 홀드기능 설정
	{ 11, &gxhEIMode, MAX_gxhEIMode, ADR_gxhEIMode, 0 },	// 외부입력설정
	{ 12, &gxlKeySet, MAX_gxlKeySet, ADR_gxlKeySet, 0 },	// 전면 키 기능 설정
	{ 13, &gxlCodeNumSet, MAX_gxlCodeNumSet, ADR_gxlCodeNumSet, 0 },	// 코드번호지정
	{ 14, &gxlHoldOffTime, MAX_gxlHoldOffTime, ADR_gxlHoldOffTime, 0 },	// 홀드 off 시간 설정
	{ 15, &gxhRelayMode, MAX_gxhRelayMode, ADR_gxhRelayMode, 0 },	// 릴레이 출력모드
	{ 16, &gxlDataOutDelay, MAX_gxlDataOutDelay, ADR_gxlDataOutDelay, 1 },	// 패커 완료후 통신 전송 지연시간
	{ 17, &gxlRelayOnDelay, MAX_gxlRelayOnDelay, ADR_gxlRelayOnDelay, 1 },	// 계량 판정 Relay 출력 지연시간(checker mode 1)
	{ 18, &gxlRelayOnTime, MAX_gxlRelayOnTime, ADR_gxlRelayOnTime, 1 },	// 계량 판정 Relay 출력 On 시간시간(checker mode 1)
	{ 19, &gdhParity, MAX_gdhParity, ADR_gdhParity, 0 },	// Serial Parity Bit 설정
	{ 20, &gxhBaudRate, MAX_gxhBaudRate, ADR_gxhBaudRate, 0 },	// 통신속도 설정
	{ 21, &gxhSendMode, MAX_gxhSendMode, ADR_gxhSendMode, 0 },	// Serial 통신 모드
	{ 22, &gxlSerialMethod, MAX_gxlSerialMethod, ADR_gxlSerialMethod, 0 },	// Serial stream, command mode
	{ 23, &gxhMyAddress, MAX_gxhMyAddress, ADR_gxhMyAddress, 0 },	// Serial 통신 방식 //0 ~99
	{ 24, &gxhRsFormat, MAX_gxhRsFormat, ADR_gxhRsFormat, 0 },	// 전송데이터 Foramt
	{ 25, &gxlBCCSelect, MAX_gxlBCCSelect, ADR_gxlBCCSelect, 0 },	// BCC 선택 모드
	{ 26, &gxlTransPerSec, MAX_gxlTransPerSec, ADR_gxlTransPerSec, 0 },	// 일반 송신횟수 설정
	{ 27, &gxlWeightUnit, MAX_gxlWeightUnit, ADR_gxlWeightUnit, 0 },	// 중량 단위 설정(통신관련)
	{ 28, &gxlDisplaySpeed, MAX_gxlDisplaySpeed, ADR_gxlDisplaySpeed, 0 },	// 평균 디스플레이 표시 설정 모드
	{ 29, &gxlStableOnDelay, MAX_gxlStableOnDelay, ADR_gxlStableOnDelay, 1 },	// 안정 지연시간

	{ 30, &gxlPushPull, MAX_gxlPushPull, ADR_gxlPushPull, 0 },	// 인장 압축 기능 사용설정
	{ 31, &gxhAroundZero, MAX_gxhAroundZero, ADR_gxhAroundZero, 10 },	// 영점 부근 중량 범위
	{ 32, &gxlZeroDisplayRange, MAX_gxlZeroDisplayRange, ADR_gxlZeroDisplayRange, 10 },	// "0" 표시 중량 범위
	{ 33, &gxlZeroMinus, MAX_gxlZeroMinus, ADR_gxlZeroMinus, 0 },	// 영점 A/D 빼기 기능
	{ 34, &gxlAnalogOutMax, MAX_gxlAnalogOutMax, ADR_gxlAnalogOutMax, 10 },	// 아날로그 최대값 중량 설정
	{ 35, &gxlEquivalInput, MAX_gxlEquivalInput, ADR_gxlEquivalInput, 4 },	// 등가입력값 확인 및 변경
	{ 36, &gdwFactor, MAX_gdwFactor, ADR_gdwFactor, 0 },	// 분동 캘 팩터 확인 및 변경
//Program Size: Code=25996 RO-data=1348 RW-data=512 ZI-data=2408  	
/*				 
	ret = lValueSetMode_5digit_with_dp(35,gxlEquivalInput, MAX_gxlEquivalInput, 4);
	gxlEquivalInput = EE_Write_val(ADR_gxlEquivalInput, ret, 4);	//add 2016.2.23
	EE_Write_val(ADR_gnfFactor, gxlEquivalInput*10000, 4);	//add 2016.2.23

	ret = lValueSetMode_5digit_with_dp(36, gdwFactor/1000, MAX_gdwFactor, 0);		
	gdwFactor = EE_Write_val(ADR_gdwFactor, ret*1000, 4);	//add 2016.2.23
	gdwFactor = EE_Write_val(ADR_gnfFactor, ret*1000, 4);	//add 2016.2.23
*/		
	
	{ -1, NULL, NULL, NULL }	// 데이터 마지막
};

// 설정번호로 있는지 없는지 찾기
char set_find_list(char index)
{
	char iz;
	char ret_val = -1;
	
	for(iz=0; set_list[iz].f_no != -1; iz++)
	{
		if( index == set_list[iz].f_no)
		{
			ret_val = iz;
			break;
		}
	}
	
	return ret_val;
}	


// IValueSetMode(설정 번호, 설정변수명, 최대값)
long lValueSetMode_dp(char f_no ,long init_value, long limit, char dp)
{
	u8  exit_flag =0;
	u8  changed = 0;
	u8  index = 0;	/* only HIDE mode, instead number to '-' */
	s8  func_num_digit_10, func_num_digit_1;
//	s8  func_value_digit_10, func_value_digit_1;
	int func_value;
	s8	limitx100k, limitx10k, limitx1k, limitx100, limitx10, limitx1;
	u8 str[30];
	long curr_num = 0;
	long val;
	int number;

	char curr_digit = 0;
	long blink = 0;
	char func_or_val; // 설정 번호 표시할 건지, 두자리수면 0, 5자리면 1

	char curr_val[5];
	char curr_dp;


	func_num_digit_10 = (f_no%100) / 10;
	func_num_digit_1 = f_no % 10;

	curr_val[0] = (init_value%100000)/10000;
	curr_val[1] = (init_value%10000)/1000;
	curr_val[2] = (init_value%1000)/100;
	curr_val[3] = (init_value%100)/10;
	curr_val[4] = (init_value%10);
	
	limitx100k = (limit%1000000) / 100000;
	limitx10k  = limit / 10000;
	limitx1k   = limit / 1000;
	limitx100  = limit / 100;
	limitx10   = limit / 10;
	limitx1    = limit % 10;	

	func_value = init_value;

	if(limit < 100) func_or_val = 0;
	else func_or_val = 1;
	
	//현재 소숫점으로 표시함.
	if(dp == 10) curr_dp = gxlDecimalPoint;
	else curr_dp = dp;

	if( !func_or_val)
	{
		vputchar(3, curr_val[3]);
		vputchar(4, curr_val[4]);
	}
	else vputnum( 1, curr_dp, func_value);	


	do
	{
		// 자릿수 깜박거리기
		blink++;

		if( !func_or_val)
		{
			vputchar( 0, func_num_digit_10);
			vputchar( 1, func_num_digit_1);
			vputchar( 2, '-');
		}
		
		if((blink == 35000)) vputchar(4-curr_digit, ' '); //자릿수 깜박거리기
		else if(70000 < blink)
		{
			blink = 0;
			if( !func_or_val)
			{
				vputchar( 3, curr_val[3]);
				vputchar( 4, curr_val[4]);
			}
			else vputnum( 1, curr_dp, func_value);
		}		
		
		if(keypush())
		{
			switch(KeyCode)
			{
				case KEY_ZERO:
					curr_val[4-curr_digit]++;
					//if(curr_val[4-curr_digit]>=10 ) curr_val[4-curr_digit] = 0; //최대값이 limit 값으로 제한
					curr_val[4-curr_digit] %= 10;					
					changed = 1;
					break;

				case KEY_TARE:
					if( !curr_val[4-curr_digit]) curr_val[4-curr_digit] = 9;
					else curr_val[4-curr_digit]--;
					changed = 1;
					break;
				
				case KEY_SETTING:
					if(func_or_val)
					{
						curr_digit++;
						curr_digit %= 5;
					}
				 	else
					{
						key_count = 0;
						while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) == 0x00)
						{
							key_count++; Delay_ms(1);
							if(key_count >= 500) break;
						}
						
						if(key_count>=500) 
						{
							if(changed) //설정값 바꿨을 때 길게 누르면 해당 설정번호로 이동
							{
								number=curr_val[3]*10 + curr_val[4];
								
								number = set_find_list(number);
								if (number == -1)
								{
									vputs("ERR28 "); Delay_ms(300);
								}
								else //설정값 안바꼈을 때 길게 누르면 설정번호 뒤로 이동
								{ 
									exit_flag = 1; 
									changed = 1;
									func_value = -number;
								}
							}
							else //설정값 안바꿨을 때 길게 누르면 설정번호 이전로 이동
							{ 
								exit_flag = 1; 
								changed = 1;
								func_value = SET_BACKWARD;
							}
						}
						else 
						{	//if(9 < limit) 
							curr_digit++;
							//else retry_message();
							curr_digit %= 2;
						}
					}
					break;

				case KEY_CANCEL:
					if(changed)//세팅값 초기화
					{
						curr_val[0] = (init_value%100000)/10000;
						curr_val[1] = (init_value%10000)/1000;
						curr_val[2] = (init_value%1000)/100;
						curr_val[3] = (init_value%100)/10;
						curr_val[4] = (init_value%10);						
						changed = 0;
					}
					else //이전 세팅번호로 이동
					{
						exit_flag = 1;
						changed = 1;
						func_value = SET_FORWARD;	// function -1
					}
					break;

				case KEY_ENTER:
					if(changed)
					{
						func_value = curr_val[3]*10 + curr_val[4];
						
						if(func_value > limit)
						{ 
							vputs("ERR27 "); Delay_ms(300);
							curr_val[3]=init_value/10;
							curr_val[4]=init_value%10;
						}
/*						
						else if(setting_number == 6 && func_value<1)// 설정번호 6(디지털필터 설정범위 1 ~ 49 인데 1 이하 에러 띄우기 위해 추가함.
						{
							vputs("ERR27 "); Delay_ms(300);
							curr_val[3]=init_value/10;
							curr_val[4]=init_value%10;
						}
*/						
						else {
							exit_flag = 1; 
							changed = 1;
///							func_value = SET_FORWARD;
						}
					}
					else
					{
						exit_flag = 1;
						changed = 1;
						func_value = SET_END;
					}
					break;
			}
			
			if( !exit_flag) func_value = curr_val[0]*10000 + curr_val[1]*1000 + curr_val[2]*100 + curr_val[3]*10 + curr_val[4];
		} //keypush
	} while(!exit_flag);
	
	if(changed)
	{
		return func_value;
	}
	else return init_value;
}  /* of lValue_set */


// IValueSetMode(설정 번호, 설정변수명, 최대값)
long lValueSetMode(char number ,long init_value, long limit)
{
	u8  v_key;
	u8  exit_flag =0;
	u8  changed = 0;
	u8  index = 0;	/* only HIDE mode, instead number to '-' */
	s8  func_num_digit_10, func_num_digit_1, func_value_digit_10, func_value_digit_1, func_value;
	s8	limitx100k, limitx10k, limitx1k, limitx100, limitx10, limitx1;
	u8 str[30];
	long curr_num = 0;
	long val;

	char digit=0;
	long blink = 0;
	
	limitx100k = (limit%1000000) / 100000;
	limitx10k  = limit / 10000;
	limitx1k   = limit / 1000;
	limitx100  = limit / 100;
	limitx10   = limit / 10;
	limitx1    = limit % 10;
	
	func_num_digit_10 = number/10;       //0
	func_num_digit_1 = number%10;  //1

	func_value_digit_10 = init_value/10;
	func_value_digit_1 = init_value%10;
	
	Display_buffer[3]=font_fnd[func_value_digit_10];
	Display_buffer[4]=font_fnd[func_value_digit_1];

	do
	{
		blink++;
//		vputnum( 0, 0, blink);

		Display_buffer[0]=font_fnd[func_num_digit_10];
		Display_buffer[1]=font_fnd[func_num_digit_1];
		Display_buffer[2] = CHAR_BAR;
//		if((TIM2_1ms < 40) || (80 < TIM2_1ms && TIM2_1ms < 120))
		if((blink == 35000))
		{
			if(digit == 1) Display_buffer[3] = 0;
			else Display_buffer[4] = 0;
		}
		else if(70000 < blink)
		{
			blink = 0;
			Display_buffer[3]=font_fnd[func_value_digit_10];
			Display_buffer[4]=font_fnd[func_value_digit_1];
		}

		if(keypush())
		{
			switch(KeyCode)
			{
				case KEY_ZERO:
					if(digit == 1)
					{
						func_value_digit_10 ++;
						//if(func_value_digit_10 >=10) func_value_digit_10=0;
						//func_value_digit_10 %= (limitx10+1); //최대값이 limit 값으로 제한
						func_value_digit_10 %= 10;
					}
					else if(digit == 0)
					{
						func_value_digit_1++;
						//if(func_value_digit_1>=10) func_value_digit_1=0;
						//if( !limitx10) func_value_digit_1 %= (limitx1+1);	//최대값이 10보다 작으면 limit 값으로 제한
						//else 
							func_value_digit_1 %= 10;
					}
					
					changed = 1;
					break;

				case KEY_TARE:
					if(digit==1)
					{
						if( !func_value_digit_10) func_value_digit_10 = 9;	//limitx10;
						else func_value_digit_10--;
						
						sprintf(str, "func_value_digit_10 : %d",func_value_digit_10);
					  	USART1_puts(str);
					}
					else if(digit==0)
					{
						if( !func_value_digit_1) func_value_digit_1 = 9;
						///{
						///	if( !limitx10) func_value_digit_1 = limitx1;	//최대값이 10보다 작으면 limit 값으로 설정
						///	else func_value_digit_1 = 9;
						///}
						else func_value_digit_1--;
						//func_value_digit_1 %= 10;
						
						sprintf(str, "func_value_digit_1 : %d", func_value_digit_1);
					  	USART1_puts(str);
					}
					
					changed = 1;
					break;
				
				case KEY_SETTING:
					key_count = 0;
					while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) == 0x00)
					{
							key_count++; Delay_ms(1);
							if(key_count >= 500) break;
					}
					
					if(key_count>=500) 
					{
							if(changed) //설정값 바꿨을 때 길게 누르면 해당 설정번호로 이동
							{
								number=func_value_digit_10*10 + func_value_digit_1;
								
								if(number>36 || number<0) 
								{
									vputs("ERR28 "); Delay_ms(300);
								}
								else //설정값 안바꼈을 때 길게 누르면 설정번호 뒤로 이동
								{ 
									setting_number =  number; 
									exit_flag = 1; 
									changed = 0;
								}
							}
							else
							{
								setting_number--;
								if(setting_number>30 || setting_number<1) setting_number = 30;
								exit_flag = 1;
								
								sprintf(str, "setting_number : %d", setting_number);
								USART1_puts(str);
							}
						key_count = 0;
					}
	
					else 
					{	//if(9 < limit) 
						digit++;
						//else retry_message();
						digit %= 2;
					}
					break;				
					/*
					if(changed) 
					{
						number=func_value_digit_10*10 + func_value_digit_1;
						
						if(number>36 || number<0) 
						{
							vputs("ERROR "); Delay_ms(300);
						}
						else 
						{ 
							setting_number =  number; 
							flag = 0; 
							changed = 0;
						}
					}
					else
					{
						setting_number++; 
						if(setting_number>30 || setting_number<0) setting_number = 1;
						flag = 0;
						
						sprintf(str, "setting_number : %d", setting_number);
						USART1_puts(str);
					}
					*/

				case KEY_CANCEL:
					if(changed)//세팅값 초기화
					{
						func_value_digit_10=init_value/10;
						func_value_digit_1=init_value%10;
						changed = 0;
					}
					else //이전 세팅번호로 이동
					{
						setting_number++; 
						if(setting_number>30 || setting_number<0) setting_number = 1;
						exit_flag = 1;
						
						sprintf(str, "setting_number : %d", setting_number);
						USART1_puts(str);
					}
					break;

				case KEY_ENTER:
					if(changed)
					{
						func_value = func_value_digit_10*10 + func_value_digit_1;
						
						if(func_value > limit)
						{ 
							vputs("ERR27 "); Delay_ms(300);
							func_value_digit_10=init_value/10;
							func_value_digit_1=init_value%10;
						}
						else if(setting_number == 6 && func_value<1)// 설정번호 6(디지털필터 설정범위 1 ~ 49 인데 1 이하 에러 띄우기 위해 추가함.
						{
							vputs("ERR27 "); Delay_ms(300);
							func_value_digit_10=init_value/10;
							func_value_digit_1=init_value%10;
						}
						else { setting_number++; if(setting_number>30) setting_number = 0; exit_flag = 1; }
						
						
						
					}
					else
					{
						setting_number = 0;
						exit_flag = 1;
					}
					break;
			}
		} //keypush
	} while(!exit_flag);
	
	if(changed)
	{
		return func_value;
	}
	else return init_value;
/*
	if (changed) return curr_num;
    else         return init_value;
	*/
//	else         return -1;
}  /* of lValue_set */


long lValueSetMode_(char number , char dp, long init_value, long limit)
{
	u8  v_key;
	u8  exit_flag =0;
	u8  changed = 0;
	u8  index = 0;	/* only HIDE mode, instead number to '-' */
	s8  func_num_digit_10, func_num_digit_1, func_value_digit_10, func_value_digit_1, func_value;
	s8 	func_value_digit_10k, func_value_digit_1k, func_value_digit_100;
	s8	limitx100k, limitx10k, limitx1k, limitx100, limitx10, limitx1;
	u8 str[30];
	long curr_num = 0;
	long val;

	char digit=0;
	char max_digit = 0;
	
	limitx100k = limit / 100000;
	limitx10k  = limit / 10000;
	limitx1k   = limit / 1000;
	limitx100  = limit / 100;
	limitx10   = limit / 10;
	limitx1    = limit % 10;
	if(limitx100k) max_digit = 6;
	else if(limitx10k) max_digit = 5;
	else if(limitx1k) max_digit = 4;
	else if(limitx100) max_digit = 3;
	else if(limitx10) max_digit = 2;
	else max_digit = 1;
	
	func_num_digit_10 = number/10;       //0
	func_num_digit_1 = number%10;  //1

	val = init_value;
	func_value_digit_10k = init_value/10000;
	func_value_digit_1k = init_value/1000;
	func_value_digit_100 = init_value/100;
	func_value_digit_10 = init_value/10;
	func_value_digit_1 = init_value%10;
	
	do
	{
		if(100 < limit) vputnum( 1, dp, val);
		else 
		{
			Display_buffer[0]=font_fnd[func_num_digit_10];
			Display_buffer[1]=font_fnd[func_num_digit_1];
			Display_buffer[2]=CHAR_BAR;
			Display_buffer[3]=font_fnd[func_value_digit_10];
			Display_buffer[4]=font_fnd[func_value_digit_1];
		}
		
		if(keypush())
		{
			switch(KeyCode)
			{
				case KEY_ZERO:
					val += power(10, digit);
					

					if(digit == 1)
					{
						func_value_digit_10 ++;
						//if(func_value_digit_10 >=10) func_value_digit_10=0;
						func_value_digit_10 %= (limitx10+1); //최대값이 limit 값으로 제한
					}
					else if(digit == 0)
					{
						func_value_digit_1++;
						//if(func_value_digit_1>=10) func_value_digit_1=0;
						if( !limitx10) func_value_digit_1 %= (limitx1+1);	//최대값이 10보다 작으면 limit 값으로 제한
						else func_value_digit_1 %= 10;
					}
					
					changed = 1;
					break;

				case KEY_TARE:
					if(digit==1)
					{
						if( !func_value_digit_10) func_value_digit_10 = limitx10;
						else func_value_digit_10--;
						
						sprintf(str, "func_value_digit_10 : %d",func_value_digit_10);
					  	USART1_puts(str);
					}
					else if(digit==0)
					{
						if( !func_value_digit_1)
						{
							if( !limitx10) func_value_digit_1 = limitx1;	//최대값이 10보다 작으면 limit 값으로 설정
							else func_value_digit_1 = 9;
						}
						else func_value_digit_1--;
						
						sprintf(str, "func_value_digit_1 : %d", func_value_digit_1);
					  	USART1_puts(str);
					}
					
					changed = 1;
					break;
				
				case KEY_SETTING:
					key_count = 0;
					while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) == 0x00)
					{
							key_count++; Delay_ms(1);
							if(key_count >= 500) break;
					}
					
					if(key_count>=500) 
					{
							if(changed) //설정값 바꿨을 때 길게 누르면 해당 설정번호로 이동
							{
								number=func_value_digit_10*10 + func_value_digit_1;
								
								if(number>36 || number<0) 
								{
									vputs("ERR28 "); Delay_ms(300);
								}
								else //설정값 안바꼈을 때 길게 누르면 설정번호 뒤로 이동
								{ 
									setting_number =  number; 
									exit_flag = 1; 
									changed = 0;
								}
							}
							else
							{
								setting_number--;
								if(setting_number>30 || setting_number<1) setting_number = 30;
								exit_flag = 1;
								
								sprintf(str, "setting_number : %d", setting_number);
								USART1_puts(str);
							}
						key_count = 0;
					}
	
					else 
					{	digit++;
						digit %= max_digit;
					}
					break;				
					/*
					if(changed) 
					{
						number=func_value_digit_10*10 + func_value_digit_1;
						
						if(number>36 || number<0) 
						{
							vputs("ERROR "); Delay_ms(300);
						}
						else 
						{ 
							setting_number =  number; 
							flag = 0; 
							changed = 0;
						}
					}
					else
					{
						setting_number++; 
						if(setting_number>30 || setting_number<0) setting_number = 1;
						flag = 0;
						
						sprintf(str, "setting_number : %d", setting_number);
						USART1_puts(str);
					}
					*/

				case KEY_CANCEL:
					if(changed)//세팅값 초기화
					{
						func_value_digit_10=init_value/10;
						func_value_digit_1=init_value%10;
						changed = 0;
					}
					else //이전 세팅번호로 이동
					{
						setting_number++; 
						if(setting_number>30 || setting_number<0) setting_number = 1;
						exit_flag = 1;
						
						sprintf(str, "setting_number : %d", setting_number);
						USART1_puts(str);
					}
					break;

				case KEY_ENTER:
					if(changed)
					{
						func_value = func_value_digit_10*10 + func_value_digit_1;
						
						if(func_value > limit)
						{
							vputs("ERR27 "); Delay_ms(300);
							func_value_digit_10=init_value/10;
							func_value_digit_1=init_value%10;
						}
						else if(setting_number == 6 && func_value<1)// 설정번호 6(디지털필터 설정범위 1 ~ 49 인데 1 이하 에러 띄우기 위해 추가함.
						{
							vputs("ERR27 "); Delay_ms(300);
							func_value_digit_10=init_value/10;
							func_value_digit_1=init_value%10;
						}
						else { setting_number++; if(setting_number>30) setting_number = 0; exit_flag = 1; }
						
						
						
					}
					else
					{
						setting_number = 0;
						exit_flag = 1;
					}
					break;
			}
		} //keypush
	} while(!exit_flag);
	
	if(changed)
	{
		return func_value;
	}
	else return init_value;
/*
	if (changed) return curr_num;
    else         return init_value;
	*/
//	else         return -1;
}  /* of lValue_set */

//long lValueSetMode(char number ,long init_value, long limit)
long lValueSetMode_5digit_with_dp(char f_num, long init_value, long limit, char dp)
{
	u8  exit_flag =0;
	u8  changed = 0;
	u32 func_value;
	u8 str[30];
	char digit=0;
	s16 value_buffer[5];
	u16 digit_buffer = 0;
	long blink = 0;

	value_buffer[0] = (init_value%100000)/10000;
	value_buffer[1] = (init_value%10000)/1000;
	value_buffer[2] = (init_value%1000)/100;
	value_buffer[3] = (init_value%100)/10;
	value_buffer[4] = (init_value%10);

	func_value = value_buffer[0]*10000 + value_buffer[1]*1000 + value_buffer[2]*100 + value_buffer[3]*10 + value_buffer[4];
	vputnum( 1, dp, func_value);
	
	sprintf(str, "v=%ld, l=%ld, f=%ld", init_value, limit, func_value);
	USART1_puts(str);

	do
	{
		blink++;

		if((blink == 35000))
		{
			Display_buffer[4-digit_buffer] = 0;
		}
		else if(70000 < blink)
		{
			blink = 0;
			vputnum( 1, dp, func_value);
		}
	
		if(keypush())
		{
			switch(KeyCode)
			{
				case KEY_ZERO:
					value_buffer[4-digit_buffer]++;
					//if(value_buffer[4-digit_buffer]>=10 ) value_buffer[4-digit_buffer] = 0;
					value_buffer[4-digit_buffer] %= 10;
					changed = 1;
					break;
				 
				case KEY_TARE:
					if( !value_buffer[4-digit_buffer]) value_buffer[4-digit_buffer] = 9;
					else value_buffer[4-digit_buffer]--;
//					if(value_buffer[4-digit_buffer]<0) value_buffer[4-digit_buffer]=9;
					changed = 1;
					break;
				 				 
				case KEY_SETTING:
					/*
					if(digit_buffer>=100000) digit_buffer = 1;
					else digit_buffer*=10;
					capa_weight += capa_weight + value_buffer;
					*/
					digit_buffer++;
					digit_buffer %= 5;
//					if(digit_buffer >=5) digit_buffer = 0;
					//value_buffer = Display_Digit[digit_buffer]-0x30;
//					changed = 1;
					break;
				 
			case KEY_CANCEL:
				if(changed)//세팅값 초기화
				{
					value_buffer[0] = (init_value%100000)/10000;
					value_buffer[1] = (init_value%10000)/1000;
					value_buffer[2] = (init_value%1000)/100;
					value_buffer[3] = (init_value%100)/10;
					value_buffer[4] = (init_value%10);
					changed = 0;
				}
				break;				 
			 
			case KEY_ENTER:
				if(func_value>limit) 
				{
					vputs("ERROR "); Delay_ms(300); 
					sprintf(str, "v=%ld, l=%ld", func_value, limit);
					USART1_puts(str);
				}
				else
				{
					exit_flag=1;
					setting_number = 1;
				}
				break;
			}
			func_value = value_buffer[0]*10000 + value_buffer[1]*1000 + value_buffer[2]*100 + value_buffer[3]*10 + value_buffer[4];
			vputnum( 1, dp, func_value);
		}
	} while(!exit_flag);

	if(changed)
	{
		return func_value;
	}
	else return init_value;
/*
	if (changed) return curr_num;
    else         return init_value;
	*/
//	else         return -1;
}  /* of lValue_set */


void set_mode(void)
{
	char	iz;
	int		idx;
	long	ret;
	int		numoffno;	//설정값 개수
	char	exit_flag = 0;
	
	vputs("    F ");    Delay_ms(230);
	vputs("   FU ");    Delay_ms(230);
	vputs("  FUN ");    Delay_ms(230);
	vputs(" FUNC ");    Delay_ms(230);
	vputs("FUNC  ");
	beep(50); Delay_ms(550);	
	
	//set 모드 설정값 개수에서 pushpull 이후는 자동증가 안하는 갯수를 뺀거
	numoffno = sizeof(set_list) / sizeof(SET_LIST) - 8;
	
//	for(idx=0; set_list[idx].f_no != -1; idx++)
	idx = 0;
	do
	{
		ret = lValueSetMode_dp( set_list[idx].f_no, *set_list[idx].val, set_list[idx].limit, set_list[idx].dp);

//vputnum(1,2, ret); Delay_ms(1000);		
		
		if( 0 <= ret)
		{	*set_list[idx].val = EE_Write_val( set_list[idx].adr, ret, 4);
			//팩터등 변경후에는 SET 1로 가게함. 아니면 다음 SET 번호로 감.
			if(idx < numoffno) { idx++; idx %= numoffno; }
			else idx = 0;
		}	
		else
		{
			switch(ret)
			{
				case SET_END: exit_flag = 1; break;
				case SET_FORWARD: idx++; idx %= numoffno; break;
				case SET_BACKWARD: idx--; if(idx < 0) idx = numoffno-1; break;
				default:
					ret = set_find_list(ret * -1) + 1; //index가 0부터 시작하고 표시는 1부터 시작하니까 +1
					if(0 <= ret) idx = ret;					
					break;
			}
		}
	} while( !exit_flag);
	
	vputs("STEND ");  Delay_ms(1000);
	//	cls(); vputs("      ");
} /* of set_menu() */

			
void setting_mode(void)
{
	union { u32 dw; u16 u[2]; u8 c[4];} temp;
	u8 which = 0;
	u8 order[37] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36};
	
	u32 ret;
	
	u8 str[30];
	
	
	// read_set_factors();
	
	vputs("    F ");    Delay_ms(30);	//Delay_ms(150);
	vputs("   FU ");    Delay_ms(30);	//Delay_ms(150);
	vputs("  FUN ");    Delay_ms(30);	//Delay_ms(150);
	vputs(" FUNC ");    Delay_ms(30);	//Delay_ms(150);
	vputs("FUNC  ");	Delay_ms(200);
	//	beep(50); Delay_ms(550);
	
	/*
	gxhWeightBackup  =1;
	gxhFilter	  =2;
	gxhMotionBand	 =3;
	gxuAutoZero		=4;
	
	*/
	
	setting_number = 1;
		
		
	do {
		switch (order[setting_number])
		{
			case 1: //소수점 위치설정 0~3// => IValueSetMode(설정 번호, 설정변수명, 최대값)
				ret = lValueSetMode(1,gxlDecimalPoint, MAX_gxlDecimalPoint) ;	  
				gxlDecimalPoint = EE_Write_val(ADR_gxlDecimalPoint, ret, 4);	//add 2016.2.23
				break;
	
		  	case 2:
		       	ret = lValueSetMode(2,gxhWeightBackup, MAX_gxhWeightBackup) ;    // 영점 기억모드 0~1
			   	gxhWeightBackup = EE_Write_val(ADR_gxhWeightBackup, ret, 4);	//add 2016.2.23
				break;
	     
		  	case 3:								 
		       	ret=lValueSetMode(3,gxhMotionBand, MAX_gxhMotionBand) ;	// 안정범위 일정 시간 내에 중량 변화폭이 설정 범위 이내일 때 안정 기능 작동 0~9
			 	gxhMotionBand = EE_Write_val(ADR_gxhMotionBand, ret, 4);	//add 2016.2.23
				break;

		  	case 4:	//gnlGrossTotal	_at_	0xF824;
		       	ret=lValueSetMode(4,gxhZTDigit, MAX_gxhZTDigit) ;	//   설정값 이하로 중량값이 표시되어 안정된다면 그 순간에 “0”kg으로 0~9
				gxhZTDigit = EE_Write_val(ADR_gxhZTDigit, ret, 4);	//add 2016.2.23
				break;
		 
		  	case 5:
				ret=lValueSetMode(5,gxuAutoZero, MAX_gxuAutoZero) ; // AutoZero 0~99
			    gxuAutoZero = EE_Write_val(ADR_gxuAutoZero, ret, 4);	//add 2016.2.23
				break;
	
	      	case 6:
				ret=lValueSetMode(6,gxhFilter, MAX_gxhFilter) ; // 디지털 필터 범위 : 01~49
				gxhFilter = EE_Write_val(ADR_gxhFilter, ret, 4);	//add 2016.2.23
		    	break;
			
			case 7:
				ret=lValueSetMode(7,gxhZTKStable, MAX_gxhZTKStable) ; // 영점, 용기 작동 모드
		    	gxhZTKStable = EE_Write_val(ADR_gxhZTKStable, ret, 4);	//add 2016.2.23
	    		break;
			
			case 8:
				ret=lValueSetMode(8,gxlZeroKeyRange, MAX_gxlZeroKeyRange) ; // 제로키 동작 범위
			    gxlZeroKeyRange = EE_Write_val(ADR_gxlZeroKeyRange, ret, 4);	//add 2016.2.23
			    break;
			
			case 9:
				ret=lValueSetMode(9,gxlTareKeyRange, MAX_gxlTareKeyRange) ; // 용기 키 동작 범위
			    gxlTareKeyRange = EE_Write_val(ADR_gxlTareKeyRange, ret, 4);	//add 2016.2.23
			    break;
			
			case 10:
				ret=lValueSetMode(10,gxhHoldType, MAX_gxhHoldType) ; // 홀드기능 설정
			    gxhHoldType = EE_Write_val(ADR_gxhHoldType, ret, 4);	//add 2016.2.23
		    	break;
			
			case 11:
				ret=lValueSetMode(11,gxhEIMode, MAX_gxhEIMode) ; // 외부엽럭설정
			    gxhEIMode = EE_Write_val(ADR_gxhEIMode, ret, 4);	//add 2016.2.23
		    	break;
			
			case 12:
				ret=lValueSetMode(12,gxlKeySet, MAX_gxlKeySet) ; // 외부엽럭설정
			    gxlKeySet = EE_Write_val(ADR_gxlKeySet, ret, 4);	//add 2016.2.23
		    	break;
			
			case 13:
				ret=lValueSetMode(13,gxlCodeNumSet, MAX_gxlCodeNumSet) ; // 코드번호지정
			    gxlCodeNumSet = EE_Write_val(ADR_gxlCodeNumSet, ret, 4);	//add 2016.2.23
		    	break;
			
			case 14:
				ret=lValueSetMode(14,gxlHoldOffTime, MAX_gxlHoldOffTime) ; // 홀드 off 시간 설정
			    gxlHoldOffTime = EE_Write_val(ADR_gxlHoldOffTime, ret, 4);	//add 2016.2.23
		    	break;
			
			case 15:
				ret=lValueSetMode(15,gxhRelayMode, MAX_gxhRelayMode) ; // 릴레이 출력모드
			    gxhRelayMode = EE_Write_val(ADR_gxhRelayMode, ret, 4);	//add 2016.2.23
		    	break;
			
			case 16:
				ret=lValueSetMode(16,gxlDataOutDelay, MAX_gxlDataOutDelay) ; // 
			    gxlDataOutDelay = EE_Write_val(ADR_gxlDataOutDelay, ret, 4);	//add 2016.2.23
		    	break;
			
			case 17:
				ret=lValueSetMode(17,gxlRelayOnDelay, MAX_gxlRelayOnDelay) ; // 계량 판정 Relay 출력 지연시간(checker mode 1)
			    gxlRelayOnDelay = EE_Write_val(ADR_gxlRelayOnDelay, ret, 4);	//add 2016.2.23
		    	break;
			
			case 18:
				ret=lValueSetMode(18,gxlRelayOnTime, MAX_gxlRelayOnTime) ; // 계량 판정 Relay 출력 On 시간시간(checker mode 1)
			    gxlRelayOnTime = EE_Write_val(ADR_gxlRelayOnTime, ret, 4);	//add 2016.2.23
		    	break;
			
			case 19:
				ret = lValueSetMode(19,gdhParity, MAX_gdhParity) ; // Serial Parity Bit 설정
				gdhParity = EE_Write_val(ADR_gdhParity, ret, 4);	//add 2016.2.23
				break;
			
			case 20:
				ret = lValueSetMode(20,gxhBaudRate, MAX_gxhBaudRate) ; // 통신속도 설정
				gxhBaudRate = EE_Write_val(ADR_gxhBaudRate, ret, 4);	//add 2016.2.23
			    break;
			
			case 21:
				ret = lValueSetMode(21,gxhSendMode, MAX_gxhSendMode) ; // Serial 통신 모드
				gxhSendMode = EE_Write_val(ADR_gxhSendMode, ret, 4);	//add 2016.2.23
			    break;
			
			case 22:
				ret = lValueSetMode(22,gxlSerialMethod, MAX_gxlSerialMethod) ; // Serial stream, command mode
				gxlSerialMethod = EE_Write_val(ADR_gxlSerialMethod, ret, 4);	//add 2016.2.23
			    break;

			case 23:
				ret = lValueSetMode(23,gxhMyAddress, MAX_gxhMyAddress) ; // Serial 통신 방식 //0 ~99
			    gxhMyAddress = EE_Write_val(ADR_gxhMyAddress, ret, 4);	//add 2016.2.23
			    break;
			
			case 24:
				ret = lValueSetMode(24,gxhRsFormat, MAX_gxhRsFormat) ; // 전송데이터 Foramt
			    gxhRsFormat = EE_Write_val(ADR_gxhRsFormat, ret, 4);	//add 2016.2.23
			    break;
			
			case 25:
				ret = lValueSetMode(25,gxlBCCSelect, MAX_gxlBCCSelect) ; // BCC 선택 모드
			    gxlBCCSelect = EE_Write_val(ADR_gxlBCCSelect, ret, 4);	//add 2016.2.23
			    break;
			
			case 26:
				ret = lValueSetMode(26,gxlTransPerSec, MAX_gxlTransPerSec) ; // 일반 송신횟수 설정
			    gxlTransPerSec = EE_Write_val(ADR_gxlTransPerSec, ret, 4);	//add 2016.2.23
			    break;
			
			case 27:
				ret=lValueSetMode(27,gxlWeightUnit, MAX_gxlWeightUnit) ; // 중량 단위 설정(통신관련)
			    gxlWeightUnit = EE_Write_val(ADR_gxlWeightUnit, ret, 4);	//add 2016.2.23
			    break;

			case 28:
				ret=lValueSetMode(28,gxlDisplaySpeed, MAX_gxlDisplaySpeed) ; // 평균 디스플레이 표시 설정 모드
			    gxlDisplaySpeed = EE_Write_val(ADR_gxlDisplaySpeed, ret, 4);	//add 2016.2.23
		    	break;
			
			case 29:
				ret=lValueSetMode(29,gxlStableOnDelay, MAX_gxlStableOnDelay) ; // 평균 디스플레이 표시 설정 모드
			    gxlStableOnDelay = EE_Write_val(ADR_gxlStableOnDelay, ret, 4);	//add 2016.2.23
		    	break;
			
			case 30:
				ret=lValueSetMode(30,gxlPushPull, MAX_gxlPushPull) ; // 인장 압축 기능 사용설정
			    gxlPushPull = EE_Write_val(ADR_gxlPushPull, ret, 4);	//add 2016.2.23
		    	break;
			
			case 31:	
				ret = lValueSetMode_5digit_with_dp(31,gxhAroundZero, MAX_gxhAroundZero, gxlDecimalPoint);	
				gxhAroundZero = EE_Write_val(ADR_gxhAroundZero, ret, 4);	//add 2016.2.23
				break;
			
			case 32 : 
				ret = lValueSetMode_5digit_with_dp(32,gxlZeroDisplayRange, MAX_gxlZeroDisplayRange, gxlDecimalPoint);	
				gxlZeroDisplayRange = EE_Write_val(ADR_gxlZeroDisplayRange, ret, 4);	//add 2016.2.23
				break;
			
			case 33 : 
				ret = lValueSetMode_5digit_with_dp(33,gxlZeroMinus, MAX_gxlZeroMinus, 0);	
				gxlZeroMinus = EE_Write_val(ADR_gxlZeroMinus, ret, 4);	//add 2016.2.23
				break;			
			
			case 34 : 
				ret = lValueSetMode_5digit_with_dp(34,gxlAnalogOutMax, MAX_gxlAnalogOutMax, gxlDecimalPoint);		
				gxlAnalogOutMax = EE_Write_val(ADR_gxlAnalogOutMax, ret, 4);	//add 2016.2.23
				break;
			
			case 35 : 
				ret = lValueSetMode_5digit_with_dp(35,gxlEquivalInput, MAX_gxlEquivalInput, 4);
				gxlEquivalInput = EE_Write_val(ADR_gxlEquivalInput, ret, 4);	//add 2016.2.23
				EE_Write_val(ADR_gnfFactor, gxlEquivalInput*10000, 4);	//add 2016.2.23
				break;
			
			case 36 : 
/*
				ret=lValueSetMode_2(36,gxhGain, MAX_gxhGain);	
				gxhGain = EE_Write_val(ADR_gxhGain, ret, 4);	//add 2016.2.23
*/
				ret = lValueSetMode_5digit_with_dp(36, gdwFactor/1000, MAX_gdwFactor, 0);		
				gdwFactor = EE_Write_val(ADR_gdwFactor, ret*1000, 4);	//add 2016.2.23
				gdwFactor = EE_Write_val(ADR_gnfFactor, ret*1000, 4);	//add 2016.2.23
			
				break;

	  		default:
	  			retry_message();
	   			break;
		}; /*switch (which) */
	//setting_number++;
	} while(order[setting_number] != 0);

	vputs("STEND ");  Delay_ms(1000);
	//	cls(); vputs("      ");
} /* of setting_menu() */


void default_set()
{
	union { u32 dw; u16 u[2]; u8 c[4];} temp;
	
	vputs("RESET "); Delay_ms(1000); 
	
	gxlDecimalPoint = EE_Write_val(ADR_gxlDecimalPoint, DEFAULT_gxlDecimalPoint, 4);	//add 2016.2.23
	gxhWeightBackup = EE_Write_val(ADR_gxhWeightBackup, DEFAULT_gxhWeightBackup, 4);	//add 2016.2.23
	gxhMotionBand = EE_Write_val(ADR_gxhMotionBand, DEFAULT_gxhMotionBand, 4);	//add 2016.2.23
	gxhZTDigit = EE_Write_val(ADR_gxhZTDigit, DEFAULT_gxhZTDigit, 4);	//add 2016.2.23
	gxuAutoZero = EE_Write_val(ADR_gxuAutoZero, DEFAULT_gxuAutoZero, 4);	//add 2016.2.23
	gxhFilter = EE_Write_val(ADR_gxhFilter, DEFAULT_gxhFilter, 4);	//add 2016.2.23
	gxhZTKStable = EE_Write_val(ADR_gxhZTKStable, DEFAULT_gxhZTKStable, 4);	//add 2016.2.23
	gxlZeroKeyRange = EE_Write_val(ADR_gxlZeroKeyRange, DEFAULT_gxlZeroKeyRange, 4);	//add 2016.2.23
	gxlTareKeyRange = EE_Write_val(ADR_gxlTareKeyRange, DEFAULT_gxlTareKeyRange, 4);	//add 2016.2.23
	gxhHoldType = EE_Write_val(ADR_gxhHoldType, DEFAULT_gxhHoldType, 4);	//add 2016.2.23
	gxhEIMode = EE_Write_val(ADR_gxhEIMode, DEFAULT_gxhEIMode, 4);	//add 2016.2.23
	gxlKeySet = EE_Write_val(ADR_gxlKeySet, DEFAULT_gxlKeySet, 4);	//add 2016.2.23
	gxlCodeNumSet = EE_Write_val(ADR_gxlCodeNumSet, DEFAULT_gxlCodeNumSet, 4);	//add 2016.2.23
	gxlHoldOffTime = EE_Write_val(ADR_gxlHoldOffTime, DEFAULT_gxlHoldOffTime, 4);	//add 2016.2.23
	gxhRelayMode = EE_Write_val(ADR_gxhRelayMode, DEFAULT_gxhRelayMode, 4);	//add 2016.2.23
	gxlDataOutDelay = EE_Write_val(ADR_gxlDataOutDelay, DEFAULT_gxlDataOutDelay, 4);	//add 2016.2.23
	gxlRelayOnDelay = EE_Write_val(ADR_gxlRelayOnDelay, DEFAULT_gxlRelayOnDelay, 4);	//add 2016.2.23
	gxlRelayOnTime = EE_Write_val(ADR_gxlRelayOnTime, DEFAULT_gxlRelayOnTime, 4);	//add 2016.2.23
	gdhParity = EE_Write_val(ADR_gdhParity, DEFAULT_gdhParity, 4);	//add 2016.2.23
	gxhBaudRate = EE_Write_val(ADR_gxhBaudRate, DEFAULT_gxhBaudRate, 4);	//add 2016.2.23
	gxhSendMode = EE_Write_val(ADR_gxhSendMode, DEFAULT_gxhSendMode, 4);	//add 2016.2.23
	gxlSerialMethod = EE_Write_val(ADR_gxlSerialMethod, DEFAULT_gxlSerialMethod, 4);	//add 2016.2.23
	gxhMyAddress = EE_Write_val(ADR_gxhMyAddress, DEFAULT_gxhMyAddress, 4);	//add 2016.2.23
	gxhRsFormat = EE_Write_val(ADR_gxhRsFormat, DEFAULT_gxhRsFormat, 4);	//add 2016.2.23
	gxlBCCSelect = EE_Write_val(ADR_gxlBCCSelect, DEFAULT_gxlBCCSelect, 4);	//add 2016.2.23
	gxlTransPerSec = EE_Write_val(ADR_gxlTransPerSec, DEFAULT_gxlTransPerSec, 4);	//add 2016.2.23
	gxlWeightUnit = EE_Write_val(ADR_gxlWeightUnit, DEFAULT_gxlWeightUnit, 4);	//add 2016.2.23
	gxlDisplaySpeed = EE_Write_val(ADR_gxlDisplaySpeed, DEFAULT_gxlDisplaySpeed, 4);	//add 2016.2.23
	gxlStableOnDelay = EE_Write_val(ADR_gxlStableOnDelay, DEFAULT_gxlStableOnDelay, 4);	//add 2016.2.23
	gxlPushPull = EE_Write_val(ADR_gxlPushPull, DEFAULT_gxlPushPull, 4);	//add 2016.2.23
	gxhAroundZero = EE_Write_val(ADR_gxhAroundZero, DEFAULT_gxhAroundZero, 4);	//add 2016.2.23
	gxlZeroDisplayRange = EE_Write_val(ADR_gxlZeroDisplayRange, DEFAULT_gxlZeroDisplayRange, 4);	//add 2016.2.23
	gxlZeroMinus = EE_Write_val(ADR_gxlZeroMinus, DEFAULT_gxlZeroMinus, 4);	//add 2016.2.23
	gxlAnalogOutMax = EE_Write_val(ADR_gxlAnalogOutMax, DEFAULT_gxlAnalogOutMax, 4);	//add 2016.2.23
	gxlEquivalInput = EE_Write_val(ADR_gxlEquivalInput, DEFAULT_gxlEquivalInput, 4);	//add 2016.2.23
//	gxhGain = EE_Write_val(ADR_gxhGain, DEFAULT_gxhGain, 4);	//add 2016.2.23
	
	vputs(" END  "); Delay_ms(2000);
	
	
}

