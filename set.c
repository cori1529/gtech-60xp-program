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

vu32 gnlCapacity;		//����� �Ҽ��� �� capacity
vu32 gxlDecimalPoint;	// gxlDecimalPoin �Ҽ��� ��ġ����
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
vu32 gxhCurrentLoopBaudRate;//2016.08.09 �߰�

/*********����ħ�� 2016��11�� �߰� *********************/

vu32 gxhZero_1;
vu32 gxhZero_2;
vu32 gxhZero_3;
vu32 gxhZero_4;






/*********����ħ�� 2016��11�� �߰� *********************/

long lValueSetMode_dp(char f_no ,long init_value, long limit, char dp);
// ������ȣ�� �ִ��� ������ ã��
int set_find_list(char index);
void set_mode(void);
long lValueSetMode_5digit_with_dp(char f_num, long init_value, long limit, char dp);


typedef struct set_list_struct {
	int f_no;	//set no
//	long *val;	//�������� ������
	volatile unsigned long *val;	//�������� ������
	long limit;	//�������� �ִ밪
	int	 adr;	//������ ������ eeprom address
	char dp;	//������ �Ҽ���
} SET_LIST;


//��¥, �ð�, ....
static const SET_LIST set_list[] = {
	
	 #ifdef dpfpdf
//	{  1, &gxlDecimalPoint, MAX_gxlDecimalPoint, ADR_gxlDecimalPoint, 0 },	// �Ҽ��� ��ġ���� 0~3
	{  1, &gxhWeightBackup, MAX_gxhWeightBackup, ADR_gxhWeightBackup, 0 },	// ���� ����� 0~1
	{  2, &gxhMotionBand, MAX_gxhMotionBand, ADR_gxhMotionBand, 0 },	// �������� ���� �ð� ���� �߷� ��ȭ���� ���� ���� �̳��� �� ���� ��� �۵� 0~9
	{  3, &gxhZTDigit, MAX_gxhZTDigit, ADR_gxhZTDigit, 0 },	// ������ ���Ϸ� �߷����� ǥ�õǾ� �����ȴٸ� �� ������ ��0��kg���� 0~9
	{  4, &gxuAutoZero, MAX_gxuAutoZero, ADR_gxuAutoZero, 0 },	// AutoZero 0~99
	{  5, &gxhFilter, MAX_gxhFilter, ADR_gxhFilter, 0 },	// ������ ���� ���� : 01~49

	{ 10, &gxhZTKStable, MAX_gxhZTKStable, ADR_gxhZTKStable, 0 },	// ����, ��� �۵� ���
	{ 11, &gxlZeroKeyRange, MAX_gxlZeroKeyRange, ADR_gxlZeroKeyRange, 0 },	// ����Ű ���� ����
	{ 12, &gxlTareKeyRange, MAX_gxlTareKeyRange, ADR_gxlTareKeyRange, 0 },	// ��� Ű ���� ����
	{ 13, &gxlZeroKeyDisabled, MAX_gxlZeroKeyDisabled, ADR_gxlZeroKeyDisabled, 0 },	//���� ���� Ű ������
	{ 14, &gxlTareKeyDisabled, MAX_gxlTareKeyDisabled, ADR_gxlTareKeyDisabled, 0 },	//���� ��� Ű ������
	{ 15, &gxlKeySet, MAX_gxlKeySet, ADR_gxlKeySet, 0 },	// ���� Ű ��� ����
	{ 16, &gxhEIMode, MAX_gxhEIMode, ADR_gxhEIMode, 0 },	// �ܺ��Է¼���

//	{ 13, &gxlCodeNumSet, MAX_gxlCodeNumSet, ADR_gxlCodeNumSet, 0 },	// �ڵ��ȣ����
	
	{ 20, &gxhHoldType, MAX_gxhHoldType, ADR_gxhHoldType, 0 },	// Ȧ���� ����
	{ 21, &gxlHoldOffTime, MAX_gxlHoldOffTime, ADR_gxlHoldOffTime, 0 },	// Ȧ�� off �ð� ����
	
	{ 30, &gxhRelayMode, MAX_gxhRelayMode, ADR_gxhRelayMode, 0 },	// ������ ��¸��
	{ 31, &gxlRelayOnDelay, MAX_gxlRelayOnDelay, ADR_gxlRelayOnDelay, 1 },	// �跮 ���� Relay ��� �����ð�(checker mode 1)
	{ 32, &gxlRelayOnTime, MAX_gxlRelayOnTime, ADR_gxlRelayOnTime, 1 },	// �跮 ���� Relay ��� On �ð��ð�(checker mode 1)
	{ 33, &gxlDataOutDelay, MAX_gxlDataOutDelay, ADR_gxlDataOutDelay, 1 },	// ��Ŀ �Ϸ��� ��� ���� �����ð�
	
	{ 40, &gxhMyAddress, MAX_gxhMyAddress, ADR_gxhMyAddress, 0 },	// Serial ��� ��� //0 ~99
	{ 41, &gxhBaudRate, MAX_gxhBaudRate, ADR_gxhBaudRate, 0 },	// ��żӵ� ����
	{ 42, &gdhParity, MAX_gdhParity, ADR_gdhParity, 0 },	// Serial Parity Bit ����
	{ 43, &gxlSerialMethod, MAX_gxlSerialMethod, ADR_gxlSerialMethod, 0 },	// Serial stream, command mode
	{ 44, &gxhSendMode, MAX_gxhSendMode, ADR_gxhSendMode, 0 },	// Serial ��� ���, �Ϸ��, �Ź�
	{ 45, &gxhRsFormat, MAX_gxhRsFormat, ADR_gxhRsFormat, 0 },	// ���۵����� Foramt
	{ 46, &gxlWeightUnit, MAX_gxlWeightUnit, ADR_gxlWeightUnit, 0 },	// �߷� ���� ����(��Ű���)
	{ 47, &gxlBCCSelect, MAX_gxlBCCSelect, ADR_gxlBCCSelect, 0 },	// BCC ���� ���
	{ 48, &gxlTransPerSec, MAX_gxlTransPerSec, ADR_gxlTransPerSec, 0 },	// �Ϲ� �۽�Ƚ�� ����
	
	{ 49, &gxhCurrentLoopBaudRate, MAX_gxhCurrentLoopBaudRate, ADR_gxhCurrentLoopBaudRate, 0 },	// Ŀ��Ʈ���� baud rate
	
	
	{ 60, &gxlDisplaySpeed, MAX_gxlDisplaySpeed, ADR_gxlDisplaySpeed, 0 },	// ��� ���÷��� ǥ�� ���� ���
	{ 61, &gxlStableOnDelay, MAX_gxlStableOnDelay, ADR_gxlStableOnDelay, 1 },	// ���� �����ð�
	{ 62, &gxhAroundZero, MAX_gxhAroundZero, ADR_gxhAroundZero, 10 },	// ���� �α� �߷� ����
	{ 63, &gxlZeroDisplayRange, MAX_gxlZeroDisplayRange, ADR_gxlZeroDisplayRange, 10 },	// "0" ǥ�� �߷� ����
	{ 64, &gxlAnalogOutMax, MAX_gxlAnalogOutMax, ADR_gxlAnalogOutMax, 10 },	// �Ƴ��α� �ִ밪 �߷� ����

//	{ 50, &gxlPushPull, MAX_gxlPushPull, ADR_gxlPushPull, 0 },	// ���� ���� ��� ��뼳��
//	{ 53, &gxlZeroMinus, MAX_gxlZeroMinus, ADR_gxlZeroMinus, 0 },	// ���� A/D ���� ���
	
	{ 91, &gdwFactor, MAX_gdwFactor, ADR_gdwFactor, 0 },	// �е� Ķ ���� Ȯ�� �� ����
	{ 92, &gxlEquivalInput, MAX_gxlEquivalInput, ADR_gxlEquivalInput, 4 },	// ��Է°� Ȯ�� �� ����
	
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
	
	{ -1, NULL, NULL, NULL }	// ������ ������
};

// ������ȣ�� �ִ��� ������ ã��
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
	char func_or_val; // ���� ��ȣ ǥ���� ����, ���ڸ����� 0, 5�ڸ��� 1

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
	
	//���� �Ҽ������� ǥ����.
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
		// �ڸ��� ���ڰŸ���
		blink++;

		if( !func_or_val)
		{
			vputchar( 0, func_num_digit_10);
			vputchar( 1, func_num_digit_1);
			vputchar( 2, '-');
		}
		
		if((blink == 35000)) vputchar(4-curr_digit, ' '); //�ڸ��� ���ڰŸ���
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
					//if(curr_val[4-curr_digit]>=10 ) curr_val[4-curr_digit] = 0; //�ִ밪�� limit ������ ����
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
							if(changed) //������ �ٲ��� �� ��� ������ �ش� ������ȣ�� �̵�
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
								else //������ �ٲ��� �� ��� ������ ������ȣ �ڷ� �̵�
								{ 
									exit_flag = 1; 
									changed = 1;
									if(number) func_value = -number;
									else func_value = SET_FUNCNO_1;
								}
							}
							else //������ �ȹٲ��� �� ��� ������ ������ȣ ������ �̵�
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
					if(changed)//���ð� �ʱ�ȭ
					{
						curr_val[0] = (init_value%100000)/10000;
						curr_val[1] = (init_value%10000)/1000;
						curr_val[2] = (init_value%1000)/100;
						curr_val[3] = (init_value%100)/10;
						curr_val[4] = (init_value%10);						
						changed = 0;
					}
					else //���� ���ù�ȣ�� �̵�
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
						else if(setting_number == 6 && func_value<1)// ������ȣ 6(���������� �������� 1 ~ 49 �ε� 1 ���� ���� ���� ���� �߰���.
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
	int		numoffno;	//������ ����
	char	exit_flag = 0;
	
	vputs("    F ");    Delay_ms(230);
	vputs("   Fu ");    Delay_ms(230);
	vputs("  Fun ");    Delay_ms(230);
	vputs(" Func ");    Delay_ms(230);
	vputs("Func  ");
	beep(50); Delay_ms(550);	
	
	//set ��� ������ �������� pushpull ���Ĵ� �ڵ����� ���ϴ� ������ ����
	numoffno = sizeof(set_list) / sizeof(SET_LIST) - 8;
	
//	for(idx=0; set_list[idx].f_no != -1; idx++)
	idx = 0;
	do
	{
		ret = lValueSetMode_dp( set_list[idx].f_no, *set_list[idx].val, set_list[idx].limit, set_list[idx].dp);

//vputnum(1,2, ret); Delay_ms(1000);
		
		if( 0 <= ret)
		{	*set_list[idx].val = EE_Write_val( set_list[idx].adr, ret, 4);
			if(set_list[idx].f_no == 92) //��Է� �ε弿 ��°� �Է��̸�
			{
				calc_equ_factor();
				write_cal_factors();  //���͸� ����
			}
			//���͵� �����Ŀ��� SET 1�� ������. �ƴϸ� ���� SET ��ȣ�� ��.
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
//2016.4.1			ret = set_find_list(ret * -1) + 1; //index�� 0���� �����ϰ� ǥ�ô� 1���� �����ϴϱ� +1
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
