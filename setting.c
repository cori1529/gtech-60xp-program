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

u32 gxlDecimalPoint;				// gxlDecimalPoin �Ҽ��� ��ġ����

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

#define	SET_FORWARD		-101	//���� ���ϰ� ���� ��ȣ��
#define	SET_BACKWARD	-102	//���� ���ϰ� ������
#define SET_END		 	-100	//���� ���ϰ� SET MODE ����


typedef struct set_list_struct {
	int f_no;	//set no
	long *val;	//�������� ������
	long limit;	//�������� �ִ밪
	int	 adr;	//������ ������ eeprom address
	char dp;	//������ �Ҽ���
} SET_LIST;

static const SET_LIST set_list[] = {
	{  1, &gxlDecimalPoint, MAX_gxlDecimalPoint, ADR_gxlDecimalPoint, 0 },	// �Ҽ��� ��ġ���� 0~3
	{  2, &gxhWeightBackup, MAX_gxhWeightBackup, ADR_gxhWeightBackup, 0 },	// ���� ����� 0~1
//Program Size: Code=25996 RO-data=612 RW-data=704 ZI-data=2408  
	{  3, &gxhMotionBand, MAX_gxhMotionBand, ADR_gxhMotionBand, 0 },	// �������� ���� �ð� ���� �߷� ��ȭ���� ���� ���� �̳��� �� ���� ��� �۵� 0~9
	{  4, &gxhZTDigit, MAX_gxhZTDigit, ADR_gxhZTDigit, 0 },	// ������ ���Ϸ� �߷����� ǥ�õǾ� �����ȴٸ� �� ������ ��0��kg���� 0~9
	{  5, &gxuAutoZero, MAX_gxuAutoZero, ADR_gxuAutoZero, 0 },	// AutoZero 0~99
	{  6, &gxhFilter, MAX_gxhFilter, ADR_gxhFilter, 0 },	// ������ ���� ���� : 01~49
	{  7, &gxhZTKStable, MAX_gxhZTKStable, ADR_gxhZTKStable, 0 },	// ����, ��� �۵� ���
	{  8, &gxlZeroKeyRange, MAX_gxlZeroKeyRange, ADR_gxlZeroKeyRange, 0 },	// ����Ű ���� ����
	{  9, &gxlTareKeyRange, MAX_gxlTareKeyRange, ADR_gxlTareKeyRange, 0 },	// ��� Ű ���� ����
	{ 10, &gxhHoldType, MAX_gxhHoldType, ADR_gxhHoldType, 0 },	// Ȧ���� ����
	{ 11, &gxhEIMode, MAX_gxhEIMode, ADR_gxhEIMode, 0 },	// �ܺ��Է¼���
	{ 12, &gxlKeySet, MAX_gxlKeySet, ADR_gxlKeySet, 0 },	// ���� Ű ��� ����
	{ 13, &gxlCodeNumSet, MAX_gxlCodeNumSet, ADR_gxlCodeNumSet, 0 },	// �ڵ��ȣ����
	{ 14, &gxlHoldOffTime, MAX_gxlHoldOffTime, ADR_gxlHoldOffTime, 0 },	// Ȧ�� off �ð� ����
	{ 15, &gxhRelayMode, MAX_gxhRelayMode, ADR_gxhRelayMode, 0 },	// ������ ��¸��
	{ 16, &gxlDataOutDelay, MAX_gxlDataOutDelay, ADR_gxlDataOutDelay, 1 },	// ��Ŀ �Ϸ��� ��� ���� �����ð�
	{ 17, &gxlRelayOnDelay, MAX_gxlRelayOnDelay, ADR_gxlRelayOnDelay, 1 },	// �跮 ���� Relay ��� �����ð�(checker mode 1)
	{ 18, &gxlRelayOnTime, MAX_gxlRelayOnTime, ADR_gxlRelayOnTime, 1 },	// �跮 ���� Relay ��� On �ð��ð�(checker mode 1)
	{ 19, &gdhParity, MAX_gdhParity, ADR_gdhParity, 0 },	// Serial Parity Bit ����
	{ 20, &gxhBaudRate, MAX_gxhBaudRate, ADR_gxhBaudRate, 0 },	// ��żӵ� ����
	{ 21, &gxhSendMode, MAX_gxhSendMode, ADR_gxhSendMode, 0 },	// Serial ��� ���
	{ 22, &gxlSerialMethod, MAX_gxlSerialMethod, ADR_gxlSerialMethod, 0 },	// Serial stream, command mode
	{ 23, &gxhMyAddress, MAX_gxhMyAddress, ADR_gxhMyAddress, 0 },	// Serial ��� ��� //0 ~99
	{ 24, &gxhRsFormat, MAX_gxhRsFormat, ADR_gxhRsFormat, 0 },	// ���۵����� Foramt
	{ 25, &gxlBCCSelect, MAX_gxlBCCSelect, ADR_gxlBCCSelect, 0 },	// BCC ���� ���
	{ 26, &gxlTransPerSec, MAX_gxlTransPerSec, ADR_gxlTransPerSec, 0 },	// �Ϲ� �۽�Ƚ�� ����
	{ 27, &gxlWeightUnit, MAX_gxlWeightUnit, ADR_gxlWeightUnit, 0 },	// �߷� ���� ����(��Ű���)
	{ 28, &gxlDisplaySpeed, MAX_gxlDisplaySpeed, ADR_gxlDisplaySpeed, 0 },	// ��� ���÷��� ǥ�� ���� ���
	{ 29, &gxlStableOnDelay, MAX_gxlStableOnDelay, ADR_gxlStableOnDelay, 1 },	// ���� �����ð�

	{ 30, &gxlPushPull, MAX_gxlPushPull, ADR_gxlPushPull, 0 },	// ���� ���� ��� ��뼳��
	{ 31, &gxhAroundZero, MAX_gxhAroundZero, ADR_gxhAroundZero, 10 },	// ���� �α� �߷� ����
	{ 32, &gxlZeroDisplayRange, MAX_gxlZeroDisplayRange, ADR_gxlZeroDisplayRange, 10 },	// "0" ǥ�� �߷� ����
	{ 33, &gxlZeroMinus, MAX_gxlZeroMinus, ADR_gxlZeroMinus, 0 },	// ���� A/D ���� ���
	{ 34, &gxlAnalogOutMax, MAX_gxlAnalogOutMax, ADR_gxlAnalogOutMax, 10 },	// �Ƴ��α� �ִ밪 �߷� ����
	{ 35, &gxlEquivalInput, MAX_gxlEquivalInput, ADR_gxlEquivalInput, 4 },	// ��Է°� Ȯ�� �� ����
	{ 36, &gdwFactor, MAX_gdwFactor, ADR_gdwFactor, 0 },	// �е� Ķ ���� Ȯ�� �� ����
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


// IValueSetMode(���� ��ȣ, ����������, �ִ밪)
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
	
	limitx100k = (limit%1000000) / 100000;
	limitx10k  = limit / 10000;
	limitx1k   = limit / 1000;
	limitx100  = limit / 100;
	limitx10   = limit / 10;
	limitx1    = limit % 10;	

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
								
								number = set_find_list(number);
								if (number == -1)
								{
									vputs("ERR28 "); Delay_ms(300);
								}
								else //������ �ȹٲ��� �� ��� ������ ������ȣ �ڷ� �̵�
								{ 
									exit_flag = 1; 
									changed = 1;
									func_value = -number;
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
						func_value = curr_val[3]*10 + curr_val[4];
						
						if(func_value > limit)
						{ 
							vputs("ERR27 "); Delay_ms(300);
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


// IValueSetMode(���� ��ȣ, ����������, �ִ밪)
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
						//func_value_digit_10 %= (limitx10+1); //�ִ밪�� limit ������ ����
						func_value_digit_10 %= 10;
					}
					else if(digit == 0)
					{
						func_value_digit_1++;
						//if(func_value_digit_1>=10) func_value_digit_1=0;
						//if( !limitx10) func_value_digit_1 %= (limitx1+1);	//�ִ밪�� 10���� ������ limit ������ ����
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
						///	if( !limitx10) func_value_digit_1 = limitx1;	//�ִ밪�� 10���� ������ limit ������ ����
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
							if(changed) //������ �ٲ��� �� ��� ������ �ش� ������ȣ�� �̵�
							{
								number=func_value_digit_10*10 + func_value_digit_1;
								
								if(number>36 || number<0) 
								{
									vputs("ERR28 "); Delay_ms(300);
								}
								else //������ �ȹٲ��� �� ��� ������ ������ȣ �ڷ� �̵�
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
					if(changed)//���ð� �ʱ�ȭ
					{
						func_value_digit_10=init_value/10;
						func_value_digit_1=init_value%10;
						changed = 0;
					}
					else //���� ���ù�ȣ�� �̵�
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
						else if(setting_number == 6 && func_value<1)// ������ȣ 6(���������� �������� 1 ~ 49 �ε� 1 ���� ���� ���� ���� �߰���.
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
						func_value_digit_10 %= (limitx10+1); //�ִ밪�� limit ������ ����
					}
					else if(digit == 0)
					{
						func_value_digit_1++;
						//if(func_value_digit_1>=10) func_value_digit_1=0;
						if( !limitx10) func_value_digit_1 %= (limitx1+1);	//�ִ밪�� 10���� ������ limit ������ ����
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
							if( !limitx10) func_value_digit_1 = limitx1;	//�ִ밪�� 10���� ������ limit ������ ����
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
							if(changed) //������ �ٲ��� �� ��� ������ �ش� ������ȣ�� �̵�
							{
								number=func_value_digit_10*10 + func_value_digit_1;
								
								if(number>36 || number<0) 
								{
									vputs("ERR28 "); Delay_ms(300);
								}
								else //������ �ȹٲ��� �� ��� ������ ������ȣ �ڷ� �̵�
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
					if(changed)//���ð� �ʱ�ȭ
					{
						func_value_digit_10=init_value/10;
						func_value_digit_1=init_value%10;
						changed = 0;
					}
					else //���� ���ù�ȣ�� �̵�
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
						else if(setting_number == 6 && func_value<1)// ������ȣ 6(���������� �������� 1 ~ 49 �ε� 1 ���� ���� ���� ���� �߰���.
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
				if(changed)//���ð� �ʱ�ȭ
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
	int		numoffno;	//������ ����
	char	exit_flag = 0;
	
	vputs("    F ");    Delay_ms(230);
	vputs("   FU ");    Delay_ms(230);
	vputs("  FUN ");    Delay_ms(230);
	vputs(" FUNC ");    Delay_ms(230);
	vputs("FUNC  ");
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
				default:
					ret = set_find_list(ret * -1) + 1; //index�� 0���� �����ϰ� ǥ�ô� 1���� �����ϴϱ� +1
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
			case 1: //�Ҽ��� ��ġ���� 0~3// => IValueSetMode(���� ��ȣ, ����������, �ִ밪)
				ret = lValueSetMode(1,gxlDecimalPoint, MAX_gxlDecimalPoint) ;	  
				gxlDecimalPoint = EE_Write_val(ADR_gxlDecimalPoint, ret, 4);	//add 2016.2.23
				break;
	
		  	case 2:
		       	ret = lValueSetMode(2,gxhWeightBackup, MAX_gxhWeightBackup) ;    // ���� ����� 0~1
			   	gxhWeightBackup = EE_Write_val(ADR_gxhWeightBackup, ret, 4);	//add 2016.2.23
				break;
	     
		  	case 3:								 
		       	ret=lValueSetMode(3,gxhMotionBand, MAX_gxhMotionBand) ;	// �������� ���� �ð� ���� �߷� ��ȭ���� ���� ���� �̳��� �� ���� ��� �۵� 0~9
			 	gxhMotionBand = EE_Write_val(ADR_gxhMotionBand, ret, 4);	//add 2016.2.23
				break;

		  	case 4:	//gnlGrossTotal	_at_	0xF824;
		       	ret=lValueSetMode(4,gxhZTDigit, MAX_gxhZTDigit) ;	//   ������ ���Ϸ� �߷����� ǥ�õǾ� �����ȴٸ� �� ������ ��0��kg���� 0~9
				gxhZTDigit = EE_Write_val(ADR_gxhZTDigit, ret, 4);	//add 2016.2.23
				break;
		 
		  	case 5:
				ret=lValueSetMode(5,gxuAutoZero, MAX_gxuAutoZero) ; // AutoZero 0~99
			    gxuAutoZero = EE_Write_val(ADR_gxuAutoZero, ret, 4);	//add 2016.2.23
				break;
	
	      	case 6:
				ret=lValueSetMode(6,gxhFilter, MAX_gxhFilter) ; // ������ ���� ���� : 01~49
				gxhFilter = EE_Write_val(ADR_gxhFilter, ret, 4);	//add 2016.2.23
		    	break;
			
			case 7:
				ret=lValueSetMode(7,gxhZTKStable, MAX_gxhZTKStable) ; // ����, ��� �۵� ���
		    	gxhZTKStable = EE_Write_val(ADR_gxhZTKStable, ret, 4);	//add 2016.2.23
	    		break;
			
			case 8:
				ret=lValueSetMode(8,gxlZeroKeyRange, MAX_gxlZeroKeyRange) ; // ����Ű ���� ����
			    gxlZeroKeyRange = EE_Write_val(ADR_gxlZeroKeyRange, ret, 4);	//add 2016.2.23
			    break;
			
			case 9:
				ret=lValueSetMode(9,gxlTareKeyRange, MAX_gxlTareKeyRange) ; // ��� Ű ���� ����
			    gxlTareKeyRange = EE_Write_val(ADR_gxlTareKeyRange, ret, 4);	//add 2016.2.23
			    break;
			
			case 10:
				ret=lValueSetMode(10,gxhHoldType, MAX_gxhHoldType) ; // Ȧ���� ����
			    gxhHoldType = EE_Write_val(ADR_gxhHoldType, ret, 4);	//add 2016.2.23
		    	break;
			
			case 11:
				ret=lValueSetMode(11,gxhEIMode, MAX_gxhEIMode) ; // �ܺο�������
			    gxhEIMode = EE_Write_val(ADR_gxhEIMode, ret, 4);	//add 2016.2.23
		    	break;
			
			case 12:
				ret=lValueSetMode(12,gxlKeySet, MAX_gxlKeySet) ; // �ܺο�������
			    gxlKeySet = EE_Write_val(ADR_gxlKeySet, ret, 4);	//add 2016.2.23
		    	break;
			
			case 13:
				ret=lValueSetMode(13,gxlCodeNumSet, MAX_gxlCodeNumSet) ; // �ڵ��ȣ����
			    gxlCodeNumSet = EE_Write_val(ADR_gxlCodeNumSet, ret, 4);	//add 2016.2.23
		    	break;
			
			case 14:
				ret=lValueSetMode(14,gxlHoldOffTime, MAX_gxlHoldOffTime) ; // Ȧ�� off �ð� ����
			    gxlHoldOffTime = EE_Write_val(ADR_gxlHoldOffTime, ret, 4);	//add 2016.2.23
		    	break;
			
			case 15:
				ret=lValueSetMode(15,gxhRelayMode, MAX_gxhRelayMode) ; // ������ ��¸��
			    gxhRelayMode = EE_Write_val(ADR_gxhRelayMode, ret, 4);	//add 2016.2.23
		    	break;
			
			case 16:
				ret=lValueSetMode(16,gxlDataOutDelay, MAX_gxlDataOutDelay) ; // 
			    gxlDataOutDelay = EE_Write_val(ADR_gxlDataOutDelay, ret, 4);	//add 2016.2.23
		    	break;
			
			case 17:
				ret=lValueSetMode(17,gxlRelayOnDelay, MAX_gxlRelayOnDelay) ; // �跮 ���� Relay ��� �����ð�(checker mode 1)
			    gxlRelayOnDelay = EE_Write_val(ADR_gxlRelayOnDelay, ret, 4);	//add 2016.2.23
		    	break;
			
			case 18:
				ret=lValueSetMode(18,gxlRelayOnTime, MAX_gxlRelayOnTime) ; // �跮 ���� Relay ��� On �ð��ð�(checker mode 1)
			    gxlRelayOnTime = EE_Write_val(ADR_gxlRelayOnTime, ret, 4);	//add 2016.2.23
		    	break;
			
			case 19:
				ret = lValueSetMode(19,gdhParity, MAX_gdhParity) ; // Serial Parity Bit ����
				gdhParity = EE_Write_val(ADR_gdhParity, ret, 4);	//add 2016.2.23
				break;
			
			case 20:
				ret = lValueSetMode(20,gxhBaudRate, MAX_gxhBaudRate) ; // ��żӵ� ����
				gxhBaudRate = EE_Write_val(ADR_gxhBaudRate, ret, 4);	//add 2016.2.23
			    break;
			
			case 21:
				ret = lValueSetMode(21,gxhSendMode, MAX_gxhSendMode) ; // Serial ��� ���
				gxhSendMode = EE_Write_val(ADR_gxhSendMode, ret, 4);	//add 2016.2.23
			    break;
			
			case 22:
				ret = lValueSetMode(22,gxlSerialMethod, MAX_gxlSerialMethod) ; // Serial stream, command mode
				gxlSerialMethod = EE_Write_val(ADR_gxlSerialMethod, ret, 4);	//add 2016.2.23
			    break;

			case 23:
				ret = lValueSetMode(23,gxhMyAddress, MAX_gxhMyAddress) ; // Serial ��� ��� //0 ~99
			    gxhMyAddress = EE_Write_val(ADR_gxhMyAddress, ret, 4);	//add 2016.2.23
			    break;
			
			case 24:
				ret = lValueSetMode(24,gxhRsFormat, MAX_gxhRsFormat) ; // ���۵����� Foramt
			    gxhRsFormat = EE_Write_val(ADR_gxhRsFormat, ret, 4);	//add 2016.2.23
			    break;
			
			case 25:
				ret = lValueSetMode(25,gxlBCCSelect, MAX_gxlBCCSelect) ; // BCC ���� ���
			    gxlBCCSelect = EE_Write_val(ADR_gxlBCCSelect, ret, 4);	//add 2016.2.23
			    break;
			
			case 26:
				ret = lValueSetMode(26,gxlTransPerSec, MAX_gxlTransPerSec) ; // �Ϲ� �۽�Ƚ�� ����
			    gxlTransPerSec = EE_Write_val(ADR_gxlTransPerSec, ret, 4);	//add 2016.2.23
			    break;
			
			case 27:
				ret=lValueSetMode(27,gxlWeightUnit, MAX_gxlWeightUnit) ; // �߷� ���� ����(��Ű���)
			    gxlWeightUnit = EE_Write_val(ADR_gxlWeightUnit, ret, 4);	//add 2016.2.23
			    break;

			case 28:
				ret=lValueSetMode(28,gxlDisplaySpeed, MAX_gxlDisplaySpeed) ; // ��� ���÷��� ǥ�� ���� ���
			    gxlDisplaySpeed = EE_Write_val(ADR_gxlDisplaySpeed, ret, 4);	//add 2016.2.23
		    	break;
			
			case 29:
				ret=lValueSetMode(29,gxlStableOnDelay, MAX_gxlStableOnDelay) ; // ��� ���÷��� ǥ�� ���� ���
			    gxlStableOnDelay = EE_Write_val(ADR_gxlStableOnDelay, ret, 4);	//add 2016.2.23
		    	break;
			
			case 30:
				ret=lValueSetMode(30,gxlPushPull, MAX_gxlPushPull) ; // ���� ���� ��� ��뼳��
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

