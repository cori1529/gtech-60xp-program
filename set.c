/* 
	set.c
*/

#include "stdio.h"

#include "stm32f10x_type.h"
#include "stm32f10x_gpio.h"

#include "adc.h"
#include "buzzer.h"
#include "cal.h"
#include "delay.h"
#include "func.h"
#include "i2c_ee.h"
#include "key.h"
#include "lcd.h"	  
#include "var.h"

extern volatile u16	KeyCode;
extern u16 Display_buffer[];

vu32 gnlCapacity;		//저장용 소숫점 뺀 capacity
vu32 gxlDecimalPoint;	// gxlDecimalPoin 소수점 위치설정
vu32 gxhWeightBackup;
vu32 gxhMotionBand=10;
vu32 gxhZTDigit;
vs32 gxuAutoZero;
vu32 gxhFilter;
vu32 gxhZTKStable;
vu32 gxlZeroKeyRange;
vu32 gxlTareKeyRange;

vu32 gxlZeroKeyDisabled;
vu32 gxlTareKeyDisabled;

vu32 gxhHoldType;
vu32 gxhEIMode;
vu32 gxlKeySet;
vu32 gxlCodeNumSet;
vu32 gxlHoldOffTime;
vu32 gxhRelayMode;
vu32 gxlDataOutDelay;
vu32 gxlRelayOnDelay;
vu32 gxlRelayOnTime;

vu32 gdhParity;
vu32 gxhBaudRate;
vu32 gxhSendMode;
vu32 gxlSerialMethod;
vu32 gxhMyAddress;
vu32 gxhRsFormat;
vu32 gxlBCCSelect;
vu32 gxlTransPerSec;
vu32 gxlWeightUnit;

vu32 gxlDisplaySpeed;
vu32 gxlStableOnDelay;
vu32 gxlPushPull;
vu32 gxhAroundZero;
vu32 gxlZeroDisplayRange;
vu32 gxlZeroMinus;
vu32 gxlAnalogOutMax;

vu32 gxlEquivalInput;
vu32 gdwWhichCal;
vu32 gxhGain;
vu32 gxhCurrentLoopBaudRate;//2016.08.09 추가

/*********낙상침대 2016년11월 추가 *********************/

vu32 gxhZero_1;
vu32 gxhZero_2;
vu32 gxhZero_3;
vu32 gxhZero_4;






/*********낙상침대 2016년11월 추가 *********************/

long lValueSetMode_dp(char f_no ,long init_value, long limit, char dp);
// 설정번호로 있는지 없는지 찾기
int set_find_list(char index);
void set_mode(void);
long lValueSetMode_5digit_with_dp(char f_num, long init_value, long limit, char dp);


typedef struct set_list_struct {
	int f_no;	//set no
//	long *val;	//설정값의 포인터
	volatile unsigned long *val;	//설정값의 포인터
	long limit;	//설정값의 최대값
	int	 adr;	//설정값 저장할 eeprom address
	char dp;	//설정값 소숫점
} SET_LIST;


//날짜, 시간, ....
static const SET_LIST set_list[] = {
	
	 #ifdef dpfpdf
//	{  1, &gxlDecimalPoint, MAX_gxlDecimalPoint, ADR_gxlDecimalPoint, 0 },	// 소수점 위치설정 0~3
	{  1, &gxhWeightBackup, MAX_gxhWeightBackup, ADR_gxhWeightBackup, 0 },	// 영점 기억모드 0~1
	{  2, &gxhMotionBand, MAX_gxhMotionBand, ADR_gxhMotionBand, 0 },	// 안정범위 일정 시간 내에 중량 변화폭이 설정 범위 이내일 때 안정 기능 작동 0~9
	{  3, &gxhZTDigit, MAX_gxhZTDigit, ADR_gxhZTDigit, 0 },	// 설정값 이하로 중량값이 표시되어 안정된다면 그 순간에 “0”kg으로 0~9
	{  4, &gxuAutoZero, MAX_gxuAutoZero, ADR_gxuAutoZero, 0 },	// AutoZero 0~99
	{  5, &gxhFilter, MAX_gxhFilter, ADR_gxhFilter, 0 },	// 디지털 필터 범위 : 01~49

	{ 10, &gxhZTKStable, MAX_gxhZTKStable, ADR_gxhZTKStable, 0 },	// 영점, 용기 작동 모드
	{ 11, &gxlZeroKeyRange, MAX_gxlZeroKeyRange, ADR_gxlZeroKeyRange, 0 },	// 제로키 동작 범위
	{ 12, &gxlTareKeyRange, MAX_gxlTareKeyRange, ADR_gxlTareKeyRange, 0 },	// 용기 키 동작 범위
	{ 13, &gxlZeroKeyDisabled, MAX_gxlZeroKeyDisabled, ADR_gxlZeroKeyDisabled, 0 },	//전면 영점 키 사용금지
	{ 14, &gxlTareKeyDisabled, MAX_gxlTareKeyDisabled, ADR_gxlTareKeyDisabled, 0 },	//전면 용기 키 사용금지
	{ 15, &gxlKeySet, MAX_gxlKeySet, ADR_gxlKeySet, 0 },	// 전면 키 기능 설정
	{ 16, &gxhEIMode, MAX_gxhEIMode, ADR_gxhEIMode, 0 },	// 외부입력설정

//	{ 13, &gxlCodeNumSet, MAX_gxlCodeNumSet, ADR_gxlCodeNumSet, 0 },	// 코드번호지정
	
	{ 20, &gxhHoldType, MAX_gxhHoldType, ADR_gxhHoldType, 0 },	// 홀드기능 설정
	{ 21, &gxlHoldOffTime, MAX_gxlHoldOffTime, ADR_gxlHoldOffTime, 0 },	// 홀드 off 시간 설정
	
	{ 30, &gxhRelayMode, MAX_gxhRelayMode, ADR_gxhRelayMode, 0 },	// 릴레이 출력모드
	{ 31, &gxlRelayOnDelay, MAX_gxlRelayOnDelay, ADR_gxlRelayOnDelay, 1 },	// 계량 판정 Relay 출력 지연시간(checker mode 1)
	{ 32, &gxlRelayOnTime, MAX_gxlRelayOnTime, ADR_gxlRelayOnTime, 1 },	// 계량 판정 Relay 출력 On 시간시간(checker mode 1)
	{ 33, &gxlDataOutDelay, MAX_gxlDataOutDelay, ADR_gxlDataOutDelay, 1 },	// 패커 완료후 통신 전송 지연시간
	
	{ 40, &gxhMyAddress, MAX_gxhMyAddress, ADR_gxhMyAddress, 0 },	// Serial 통신 방식 //0 ~99
	{ 41, &gxhBaudRate, MAX_gxhBaudRate, ADR_gxhBaudRate, 0 },	// 통신속도 설정
	{ 42, &gdhParity, MAX_gdhParity, ADR_gdhParity, 0 },	// Serial Parity Bit 설정
	{ 43, &gxlSerialMethod, MAX_gxlSerialMethod, ADR_gxlSerialMethod, 0 },	// Serial stream, command mode
	{ 44, &gxhSendMode, MAX_gxhSendMode, ADR_gxhSendMode, 0 },	// Serial 통신 모드, 완료시, 매번
	{ 45, &gxhRsFormat, MAX_gxhRsFormat, ADR_gxhRsFormat, 0 },	// 전송데이터 Foramt
	{ 46, &gxlWeightUnit, MAX_gxlWeightUnit, ADR_gxlWeightUnit, 0 },	// 중량 단위 설정(통신관련)
	{ 47, &gxlBCCSelect, MAX_gxlBCCSelect, ADR_gxlBCCSelect, 0 },	// BCC 선택 모드
	{ 48, &gxlTransPerSec, MAX_gxlTransPerSec, ADR_gxlTransPerSec, 0 },	// 일반 송신횟수 설정
	
	{ 49, &gxhCurrentLoopBaudRate, MAX_gxhCurrentLoopBaudRate, ADR_gxhCurrentLoopBaudRate, 0 },	// 커런트루프 baud rate
	
	
	{ 60, &gxlDisplaySpeed, MAX_gxlDisplaySpeed, ADR_gxlDisplaySpeed, 0 },	// 평균 디스플레이 표시 설정 모드
	{ 61, &gxlStableOnDelay, MAX_gxlStableOnDelay, ADR_gxlStableOnDelay, 1 },	// 안정 지연시간
	{ 62, &gxhAroundZero, MAX_gxhAroundZero, ADR_gxhAroundZero, 10 },	// 영점 부근 중량 범위
	{ 63, &gxlZeroDisplayRange, MAX_gxlZeroDisplayRange, ADR_gxlZeroDisplayRange, 10 },	// "0" 표시 중량 범위
	{ 64, &gxlAnalogOutMax, MAX_gxlAnalogOutMax, ADR_gxlAnalogOutMax, 10 },	// 아날로그 최대값 중량 설정

//	{ 50, &gxlPushPull, MAX_gxlPushPull, ADR_gxlPushPull, 0 },	// 인장 압축 기능 사용설정
//	{ 53, &gxlZeroMinus, MAX_gxlZeroMinus, ADR_gxlZeroMinus, 0 },	// 영점 A/D 빼기 기능
	
	{ 91, &gdwFactor, MAX_gdwFactor, ADR_gdwFactor, 0 },	// 분동 캘 팩터 확인 및 변경
	{ 92, &gxlEquivalInput, MAX_gxlEquivalInput, ADR_gxlEquivalInput, 4 },	// 등가입력값 확인 및 변경
	
	#endif
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
int set_find_list(char index)
{
	char iz;
	int	 ret_val = -1;
	
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


long lValueSetMode_dp(char f_no ,long init_value, long limit, char dp)
{
	u8  exit_flag =0;
	char  changed = 0;
//	u8  index = 0;	/* only HIDE mode, instead number to '-' */
	s8  func_num_digit_10, func_num_digit_1;
//	s8  func_value_digit_10, func_value_digit_1;
	int func_value;
//	s8	limitx100k, limitx10k, limitx1k, limitx100, limitx10, limitx1;
//	u8 str[30];
//	long curr_num = 0;
//	long val;
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
/*	
	limitx100k = (limit%1000000) / 100000;
	limitx10k  = limit / 10000;
	limitx1k   = limit / 1000;
	limitx100  = limit / 100;
	limitx10   = limit / 10;
	limitx1    = limit % 10;	
*/
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
								
/*
								number = set_find_list(number);

vputnum(1,3,number); Delay_ms(3000);
vputnum(1,1,set_list[number].f_no); Delay_ms(3000);


								if (number == -1)
								{
									vputerr("ERR28 ");
								}
*/
								if (set_find_list(number) == -1)
								{
									vputerr("ERR28 ");
								}
								else //설정값 바꼈을 때 길게 누르면 설정번호 뒤로 이동
								{ 
									exit_flag = 1; 
									changed = 1;
									if(number) func_value = -number;
									else func_value = SET_FUNCNO_1;
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
						func_value = curr_val[0]*10000 + curr_val[1]*1000 + curr_val[2]*100 + curr_val[3]*10 + curr_val[4];
						
						if(func_value > limit)
						{ 
							vputerr("ERR27 ");
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
//							changed = 1;
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
	
	if(changed) return func_value;
	else return init_value;
}  /* of lValue_set */


void set_mode(void)
{
//	char	iz;
	int		idx;
	long	ret;
	int		numoffno;	//설정값 개수
	char	exit_flag = 0;
	
	vputs("    F ");    Delay_ms(230);
	vputs("   Fu ");    Delay_ms(230);
	vputs("  Fun ");    Delay_ms(230);
	vputs(" Func ");    Delay_ms(230);
	vputs("Func  ");
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
			if(set_list[idx].f_no == 92) //등가입력 로드셀 출력값 입력이면
			{
				calc_equ_factor();
				write_cal_factors();  //팩터만 저장
			}
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
				case SET_FUNCNO_1: ret = 0;
				default:
//2016.4.1			ret = set_find_list(ret * -1) + 1; //index가 0부터 시작하고 표시는 1부터 시작하니까 +1
					ret = set_find_list(ret * -1);
					if(0 <= ret) idx = ret;	
					break;
			}
		}
	} while( !exit_flag);
	
	vputs("STEND ");  Delay_ms(1000);
	//	cls(); vputs("      ");
} /* of set_menu() */

			
void default_set()
{
//	union { u32 dw; u16 u[2]; u8 c[4];} temp;
	
	vputs("RESET "); Delay_ms(1000); 
	#ifdef dpfdp
	gxlDecimalPoint = EE_Write_val(ADR_gxlDecimalPoint, DEFAULT_gxlDecimalPoint, 4);	//add 2016.2.23
	gxhWeightBackup = EE_Write_val(ADR_gxhWeightBackup, DEFAULT_gxhWeightBackup, 4);	//add 2016.2.23
	gxhMotionBand = EE_Write_val(ADR_gxhMotionBand, DEFAULT_gxhMotionBand, 4);	//add 2016.2.23
	gxhZTDigit = EE_Write_val(ADR_gxhZTDigit, DEFAULT_gxhZTDigit, 4);	//add 2016.2.23
	gxuAutoZero = EE_Write_val(ADR_gxuAutoZero, DEFAULT_gxuAutoZero, 4);	//add 2016.2.23
	gxhFilter = EE_Write_val(ADR_gxhFilter, DEFAULT_gxhFilter, 4);	//add 2016.2.23
	gxhZTKStable = EE_Write_val(ADR_gxhZTKStable, DEFAULT_gxhZTKStable, 4);	//add 2016.2.23
	gxlZeroKeyRange = EE_Write_val(ADR_gxlZeroKeyRange, DEFAULT_gxlZeroKeyRange, 4);	//add 2016.2.23
	gxlTareKeyRange = EE_Write_val(ADR_gxlTareKeyRange, DEFAULT_gxlTareKeyRange, 4);	//add 2016.2.23
	gxlZeroKeyDisabled = EE_Write_val(ADR_gxlZeroKeyDisabled, DEFAULT_gxlZeroKeyDisabled, 4);	//add 2016.3.30
	gxlTareKeyDisabled = EE_Write_val(ADR_gxlTareKeyDisabled, DEFAULT_gxlTareKeyDisabled, 4);	//add 2016.3.30

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

  gxhCurrentLoopBaudRate = EE_Write_val(ADR_gxhCurrentLoopBaudRate, DEFAULT_gxhCurrentLoopBaudRate, 4);
	
	vputs(" END  "); Delay_ms(2000);
	#endif
	
}

/* end of file */
