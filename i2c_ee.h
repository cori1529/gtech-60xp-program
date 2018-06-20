/*
	i2c_ee.h
*/

/* Define to prevent recursive inclusion ------------------------------------ */
#ifndef __I2C_EE_H
#define __I2C_EE_H

#include "stm32f10x_type.h"


#define ADR_v_zero			       0
#define ADR_v_capacity		     4	/* 58:59 */
#define ADR_v_division		     8
#define ADR_v_decimal_point	   12
#define ADR_gnfFactor		       16
#define ADR_gxhMotionBand	     20
#define ADR_gxhZTDigit		     24
#define ADR_gxhZTTime		       28

#define ADR_gxhDanSetType       32
#define ADR_gxhWeightGrapeType  36
#define ADR_gxhWeightJado1Type  40
#define ADR_gxhWeightJado2Type  44
#define ADR_gxhWeightPearType   48
#define ADR_gxhWeightAppleType  52
#define ADR_gxhWeightPeackType  56
#define	ADR_gxhCurrentType		  60
#define	ADR_gxhVoiceLevel		    64
#define	ADR_gxhWhichSorting     68
//extern long   gxhWhichSorting;




#define	ADR_i1danB1				72		//grape 포도 15단 메모리 
#define	ADR_i2danB1				74
#define	ADR_i3danB1				76
#define	ADR_i4danB1				78
#define	ADR_i5danB1				80
#define	ADR_i6danB1				82
#define	ADR_i7danB1				84
#define	ADR_i8danB1				86
#define	ADR_i9danB1				88
#define	ADR_i10danB1			90
#define	ADR_i11danB1			92
#define	ADR_i12danB1			94
#define	ADR_i13danB1			96
#define	ADR_i14danB1			98
#define	ADR_i15danB1			100



#define	ADR_i1danB2				102	// jadu 15단
#define	ADR_i2danB2				104
#define	ADR_i3danB2				106
#define	ADR_i4danB2				108
#define	ADR_i5danB2				110
#define	ADR_i6danB2				112
#define	ADR_i7danB2				114
#define	ADR_i8danB2				116
#define	ADR_i9danB2				118
#define	ADR_i10danB2			120
#define	ADR_i11danB2			122
#define	ADR_i12danB2			124
#define	ADR_i13danB2			126
#define	ADR_i14danB2			128
#define	ADR_i15danB2			130


#define	ADR_i1danB3				132		// peach 복숭아  기존 15단 -> 20단 메모리확보
#define	ADR_i2danB3				134
#define	ADR_i3danB3				136
#define	ADR_i4danB3				138
#define	ADR_i5danB3				140
#define	ADR_i6danB3				142
#define	ADR_i7danB3				144
#define	ADR_i8danB3				146
#define	ADR_i9danB3				148
#define	ADR_i10danB3			150
#define	ADR_i11danB3			152
#define	ADR_i12danB3			154
#define	ADR_i13danB3			156
#define	ADR_i14danB3			158
#define	ADR_i15danB3			160
#define	ADR_i16danB3			162
#define	ADR_i17danB3			164
#define	ADR_i18danB3			166
#define	ADR_i19danB3			168
#define	ADR_i20danB3			170






#define	ADR_i1danB4				172	  // apple 기존 15에서 20단으로 메모리 확장
#define	ADR_i2danB4				174
#define	ADR_i3danB4				176
#define	ADR_i4danB4				178
#define	ADR_i5danB4				180
#define	ADR_i6danB4				182
#define	ADR_i7danB4				184
#define	ADR_i8danB4				186
#define	ADR_i9danB4				188
#define	ADR_i10danB4			190
#define	ADR_i11danB4			192
#define	ADR_i12danB4			194
#define	ADR_i13danB4			196
#define	ADR_i14danB4			198
#define	ADR_i15danB4			200
#define	ADR_i16danB4			202
#define	ADR_i17danB4			204
#define	ADR_i18danB4			206
#define	ADR_i19danB4			208
#define	ADR_i20danB4			210



#define	ADR_i1danB5				212	 // gam 15단
#define	ADR_i2danB5				214
#define	ADR_i3danB5				216
#define	ADR_i4danB5				218
#define	ADR_i5danB5				220
#define	ADR_i6danB5				222
#define	ADR_i7danB5				224   ///// end of BLOCK 0 OF EEPROM



#define	ADR_i8danB5				226   /// start of BLOCK 1 OF EEPROM
#define	ADR_i9danB5				228
#define	ADR_i10danB5			230
#define	ADR_i11danB5			232
#define	ADR_i12danB5			234
#define	ADR_i13danB5			236
#define	ADR_i14danB5			238
#define	ADR_i15danB5			240
#define	ADR_i1danB6				242	 // bae 기존 15에서  20으로 메모리 확장
#define	ADR_i2danB6				244
#define	ADR_i3danB6				246
#define	ADR_i4danB6 			248
#define	ADR_i5danB6				250
#define	ADR_i6danB6				252
#define	ADR_i7danB6				254
#define	ADR_i8danB6				256
#define	ADR_i9danB6				258
#define	ADR_i10danB6			260
#define	ADR_i11danB6			262
#define	ADR_i12danB6			264
#define	ADR_i13danB6			266
#define	ADR_i14danB6			268
#define	ADR_i15danB6			270
#define	ADR_i16danB6			272
#define	ADR_i17danB6			274
#define	ADR_i18danB6			276
#define	ADR_i19danB6			278
#define	ADR_i20danB6			280






#define	ADR_i1danB7				282	  // halrabong 25 
#define	ADR_i2danB7				284
#define	ADR_i3danB7				286
#define	ADR_i4danB7 			288
#define	ADR_i5danB7				290
#define	ADR_i6danB7				292
#define	ADR_i7danB7				294
#define	ADR_i8danB7				296
#define	ADR_i9danB7				298
#define	ADR_i10danB7			300
#define	ADR_i11danB7			302
#define	ADR_i12danB7			304
#define	ADR_i13danB7			306
#define	ADR_i14danB7			308
#define	ADR_i15danB7			310 
#define	ADR_i16danB7			312
#define	ADR_i17danB7			314
#define	ADR_i18danB7			316
#define	ADR_i19danB7			318
#define	ADR_i20danB7			320
#define	ADR_i21danB7			322
#define	ADR_i22danB7			324
#define	ADR_i23danB7			326
#define	ADR_i24danB7			328 
#define	ADR_i25danB7			330




#define	ADR_i1danB8				332	   //레드향  19개에서 20개로 
#define	ADR_i2danB8				334
#define	ADR_i3danB8				336
#define	ADR_i4danB8 			338
#define	ADR_i5danB8				340
#define	ADR_i6danB8				342	 
#define	ADR_i7danB8				344	 
#define	ADR_i8danB8				346
#define	ADR_i9danB8				348
#define	ADR_i10danB8			350
#define	ADR_i11danB8			352
#define	ADR_i12danB8			354
#define	ADR_i13danB8			356
#define	ADR_i14danB8			358
#define	ADR_i15danB8			360
#define	ADR_i16danB8			362
#define	ADR_i17danB8			364
#define	ADR_i18danB8			366
#define	ADR_i19danB8			368
#define	ADR_i20danB8			370




#define	ADR_i1danB9				372 // 특작 19에서 25단으로
#define	ADR_i2danB9				374
#define	ADR_i3danB9				376
#define	ADR_i4danB9 			378
#define	ADR_i5danB9				380
#define	ADR_i6danB9				382
#define	ADR_i7danB9				384
#define	ADR_i8danB9				386
#define	ADR_i9danB9				388
#define	ADR_i10danB9			390
#define	ADR_i11danB9			392
#define	ADR_i12danB9			394
#define	ADR_i13danB9			396
#define	ADR_i14danB9			398
#define	ADR_i15danB9			400
#define	ADR_i16danB9			402
#define	ADR_i17danB9			404
#define	ADR_i18danB9			406
#define	ADR_i19danB9			408
#define	ADR_i20danB9			410
#define	ADR_i21danB9			412
#define	ADR_i22danB9			414
#define	ADR_i23danB9			416
#define	ADR_i24danB9			418
#define	ADR_i25danB9			420




#define	ADR_i1danB10				422 // 신규 단호박 20
#define	ADR_i2danB10				424
#define	ADR_i3danB10				426
#define	ADR_i4danB10 			  428
#define	ADR_i5danB10				430
#define	ADR_i6danB10				432
#define	ADR_i7danB10				434
#define	ADR_i8danB10				436
#define	ADR_i9danB10				438
#define	ADR_i10danB10			  440
#define	ADR_i11danB10			  442
#define	ADR_i12danB10			  444
#define	ADR_i13danB10			  446
#define	ADR_i14danB10			  448
#define	ADR_i15danB10			  450
#define	ADR_i16danB10			  452
#define	ADR_i17danB10			  454
#define	ADR_i18danB10			  456
#define	ADR_i19danB10			  458
#define	ADR_i20danB10			  460


#define ADR_gxhWeightSpecialType 464

#define ADR_gxhDisplaySelect   468










#ifdef dpfdpfp
#define ADR_gxlDecimalPoint			0		// gxlDecimalPoin 소수점 위치설정
#define ADR_gxhWeightBackup			4
#define ADR_gxhMotionBand			8
#define ADR_gxhZTDigit				12
#define ADR_gxuAutoZero				16
#define ADR_gxhFilter				20
#define ADR_gxhZTKStable			24
#define ADR_gxlZeroKeyRange			28
#define ADR_gxlTareKeyRange			32
#define ADR_gxlZeroKeyDisabled		152
#define ADR_gxlTareKeyDisabled		156

#define ADR_gxhHoldType				36
#define ADR_gxhEIMode				40
#define ADR_gxlKeySet				44
#define ADR_gxlCodeNumSet			48
#define ADR_gxlHoldOffTime			52
#define ADR_gxhRelayMode			56
#define ADR_gxlDataOutDelay			60
#define ADR_gxlRelayOnDelay			64
#define ADR_gxlRelayOnTime			68
#define ADR_gdhParity				72
#define ADR_gxhBaudRate				76
#define ADR_gxhSendMode				80
#define ADR_gxlSerialMethod			84
#define ADR_gxhMyAddress			88
#define ADR_gxhRsFormat				92
#define ADR_gxlBCCSelect			96
#define ADR_gxlTransPerSec			100
#define ADR_gxlWeightUnit			104
#define ADR_gxlDisplaySpeed			108
#define ADR_gxlStableOnDelay		112
#define ADR_gxlPushPull				116
#define ADR_gxhAroundZero			120
#define ADR_gxlZeroDisplayRange		124
#define ADR_gxlZeroMinus			128
#define ADR_gxlAnalogOutMax			132
#define ADR_gxlEquivalInput			136
#define ADR_gdwFactor				140
#define	ADR_gdwWhichCal				144	//0:분동캘, 1:등가입력
//#define ADR_gxhGain				140

#define ADR_gnlCapacity             160 
#define ADR_v_ei_multiply_factor	164
#define ADR_gnfEquFactor			172
#define ADR_gnfRefFactor			176

#define ADR_gnfFactor             	196
#define ADR_v_zero                	200 
#define ADR_v_adc_org             	204	 
#define ADR_v_i_resolution			208
#define ADR_v_division				212

#define ADR_SP1						216 // 하한값
#define ADR_SP2						220 // 상한값
#define	ADR_gxhCurrentLoopBaudRate	224

// eeprom = 2048 byte ( 2048/4=	512개)
/************  낙상침대용 ***********************/

#define ADR_ch1_zero_value                	300  // 각 채널의 영점값 기억하기  
#define ADR_ch2_zero_value               	304 
#define ADR_ch3_zero_value                	308 
#define ADR_ch4_zero_value                	312 


#define ADR_patient_value                	316 
#define ADR_alert_setting                	320 



/************  낙상침대용 ***********************/


//default value
#define DEFAULT_gxlDecimalPoint	3				// gxlDecimalPoin 소수점 위치설정
#define DEFAULT_gxhWeightBackup	0				//영점기억
#define DEFAULT_gxhMotionBand		5			//motion band
#define DEFAULT_gxhZTDigit			5			//zero tracking
#define DEFAULT_gxuAutoZero		0				//auto zero range
#define DEFAULT_gxhFilter			15			//digital filter range
#define DEFAULT_gxhZTKStable		0			//zero, tare 키 작동모드 : 중량 변화 안정되었을때만 zero, tare 키 사용할 수 있는지 아닌지
#define DEFAULT_gxlZeroKeyRange	3				//zero key 범위 설정
#define DEFAULT_gxlTareKeyRange	3				//tare key 범위 설정

#define DEFAULT_gxlZeroKeyDisabled	0			//전면 zero key 동작 금지
#define DEFAULT_gxlTareKeyDisabled	0			//전면 tare key 동작 금지

#define DEFAULT_gxhHoldType		0				//홀드 기능 설정
#define DEFAULT_gxhEIMode			3			//외부입력 설정
#define DEFAULT_gxlKeySet			0			//전면 키 동작 설정
#define DEFAULT_gxlCodeNumSet		0			//코드번호 지정
#define DEFAULT_gxlHoldOffTime		0			//홀드 off 시간 설정
#define DEFAULT_gxhRelayMode		1			//릴레이 출력 모드
#define DEFAULT_gxlDataOutDelay		10				//계량모드 1, 2 일 때 통신 출력 지연 시간 설정
#define DEFAULT_gxlRelayOnDelay		10				//계량 판정 Relay 출력 지연 시간(Checker mode1)
#define DEFAULT_gxlRelayOnTime		10			//계량 판정 Relay 출력 On 시간(Checker Mode1)

#define DEFAULT_gdhParity			0		//serial 통신 parity bit 설정 모드
#define DEFAULT_gxhBaudRate			7		//통신속도 설정
#define DEFAULT_gxhSendMode			0		//serial 통신 모드 : stream mode, 완료시 mode
#define DEFAULT_gxlSerialMethod		1		//serial 통신 방식 : 일반송신 mode, command mode, lcd mode, 외부표시기 mode
#define DEFAULT_gxhMyAddress		1		//장비번호 설정
#define DEFAULT_gxhRsFormat			0		//전송 데이터 format : 기본 format, cas format
#define DEFAULT_gxlBCCSelect		0		//bcc 선택모드
#define DEFAULT_gxlTransPerSec		3		//일반 송신 횟수 설정
#define DEFAULT_gxlWeightUnit		0		//중량 단위 설정(통신 관련) : kg, g, ton

#define DEFAULT_gxlDisplaySpeed		0		//평균 디스플레이 표시 설정 모드
#define DEFAULT_gxlStableOnDelay	0		//안정 LED On되는 지연시간 설정
#define DEFAULT_gxlPushPull			0		//인장압축 기능
#define DEFAULT_gxhAroundZero		0		//영점 부근 범위 설정
#define DEFAULT_gxlZeroDisplayRange	0		//영점 표시 범위 설정
#define DEFAULT_gxlZeroMinus		0		//영점빼기 값 설정
#define DEFAULT_gxlAnalogOutMax		0		//아날로그 출력 최고 값 설정
#define DEFAULT_gxlEquivalInput		20000	//등가입력 값 확인 및 설정 (2mV/V)
#define DEFAULT_gdwFactor			99999	//calibration span 상수 확인
#define	DEFAULT_gdwWhichCal			0		//분동캘을 기본
//#define DEFAULT_gxhGain			0		//calibration span 상수 확인

#define DEFAULT_gnfEquFactor		99999
#define DEFAULT_gnfRefFactor		99999

#define DEFAULT_SP1				SP2
#define DEFAULT_SP2				v_capacity

#define DEFAULT_gxhCurrentLoopBaudRate	2


//max value
#define MAX_gxlDecimalPoint		3				// gxlDecimalPoin 소수점 위치설정
#define MAX_gxhWeightBackup		1
#define MAX_gxhMotionBand		9
#define MAX_gxhZTDigit			9
#define MAX_gxuAutoZero			99
#define MAX_gxhFilter			49
#define MAX_gxhZTKStable		1 
#define MAX_gxlZeroKeyRange		4
#define MAX_gxlTareKeyRange		3

#define MAX_gxlZeroKeyDisabled	1
#define MAX_gxlTareKeyDisabled	1

#define MAX_gxhHoldType			2
#define MAX_gxhEIMode			3
#define MAX_gxlKeySet			2
#define MAX_gxlCodeNumSet		2
#define MAX_gxlHoldOffTime		99
#define MAX_gxhRelayMode		4
#define MAX_gxlDataOutDelay		99
#define MAX_gxlRelayOnDelay		99
#define MAX_gxlRelayOnTime		99

#define MAX_gdhParity			2
#define MAX_gxhBaudRate			9
#define MAX_gxhSendMode			1
#define MAX_gxlSerialMethod		1
#define MAX_gxhMyAddress		99
#define MAX_gxhRsFormat			2 
#define MAX_gxlBCCSelect		1
#define MAX_gxlTransPerSec		6
#define MAX_gxlWeightUnit		2

#define MAX_gxlDisplaySpeed		99
#define MAX_gxlStableOnDelay	99
#define MAX_gxlPushPull			1
#define MAX_gxhAroundZero		99999
#define MAX_gxlZeroDisplayRange	99999
#define MAX_gxlZeroMinus		99999
#define MAX_gxlAnalogOutMax		99999
#define MAX_gxlEquivalInput		99999
#define MAX_gdwFactor			99999
#define MAX_gdwWhichCal			1
//#define MAX_gxhGain				99999

#define MAX_gnfEquFactor		99999
#define MAX_gnfRefFactor		99999



#define MAX_SP1					SP2
#define MAX_SP2					v_capacity

#define MAX_gxhCurrentLoopBaudRate 4
 #endif

/* The M24C08W contains 4 blocks (128byte each) with the adresses below: E2 = 0 */
/* EEPROM Addresses defines */
#define EEPROM_Block0_ADDRESS 0xA0   /* E2 = 0 */
#define EEPROM_Block1_ADDRESS 0xA2 /* E2 = 0 */
#define EEPROM_Block2_ADDRESS 0xA4 /* E2 = 0 */
#define EEPROM_Block3_ADDRESS 0xA6 /* E2 = 0 */
#define EEPROM_Block4_ADDRESS 0xA8   /* E2 = 0 */
#define EEPROM_Block5_ADDRESS 0xAA /* E2 = 0 */
#define EEPROM_Block6_ADDRESS 0xAC /* E2 = 0 */
#define EEPROM_Block7_ADDRESS 0xAE /* E2 = 0 */

void I2C_EE_Init(void);
void I2C_EE_ByteWrite(u8* pBuffer, u8 WriteAddr);
void I2C_EE_PageWrite(u8* pBuffer, u16 WriteAddr, u8 NumByteToWrite);
void I2C_EE_BufferWrite(u8* pBuffer, u16 WriteAddr, u16 NumByteToWrite);
void I2C_EE_BufferRead(u8* pBuffer, u16 ReadAddr, u16 NumByteToRead);
void I2C_EE_WaitEepromStandbyState(void);

u32 EE_Write_val(int adr, u32 val, u8 bytes);	//add 2016.2.23
u32 EE_Read_val(int adr, u8 bytes);	//add 2016.2.23

void write_cal_factors(void); //calibration 0:분동 캘, 1:등가입력
void read_cal_factors(void);
void cond_check();
void DefaultDanSetting(); //6월20일
//void recall_dan(
void  recall_dan(char dan);
void  SaveDan(char dan);
void read_set_factors(void);


/************낙상침대 ********************/

void write_zero_factors(void); // 각내널의 영점값을 기억 
void read_zero_factors(void); // 각내널의 영점값을 기억 
extern long ch1_zero_value; //  침대에서 1번의 영점값 
extern long ch2_zero_value; //  침대에서 2번의 영점값 
extern long ch3_zero_value; //  침대에서 3번의 영점값 
extern long ch4_zero_value; //  침대에서 4번의 영점값 

extern long ch_zero_value[4]; //

//extern long   patient_value; 
extern long   alert_setting;   

 extern int				i1dan;
extern	int				i2dan;
extern	int				i3dan;
extern	int				i4dan;
 extern int				i5dan;
extern	int				i6dan;
extern	int				i7dan;
extern	int				i8dan;
extern	int				i9dan;
extern	int				i10dan;
extern	int				i11dan;
extern	int				i12dan;
extern	int				i13dan;
extern	int				i14dan;
extern	int				i15dan;
extern	int				i16dan;
extern	int				i17dan;
extern	int				i18dan;
extern	int				i19dan;
extern	int				i20dan;
extern	int				i21dan;
extern	int				i22dan;
 extern int				i23dan;
extern	int				i24dan;
extern	int				i25dan;


extern  unsigned char gxhZTTime; 	   /* zero tracking time (sec) */
extern 	unsigned char	gxhCurrentType;
extern	char 			gxhWeightGrapeType;  // 1 2 3 type
extern	char			gxhWeightJado1Type;  // 1  type
extern	char 			gxhWeightJado2Type;  // 1  type
extern	char 			gxhWeightPearType;   // 1 2   type
extern	char 			gxhWeightAppleType;  // 1  type
extern	char 			gxhWeightPeackType;  // 1 2 3 4 type
extern	unsigned char	gxhDanSetType;


extern  long	lBoundry[20];
extern  long	lCount[25];		// dan counter
extern  long	lTotalCount;	// Total counter

extern  u8	dpoint_bak;
extern unsigned char byDan[100];   // ms => byDan[25];byte	byDan[30];   // ms => byDan[25];
extern	int				iMinimumWeight;
extern	int				gxiIndex_End;
extern  int 	DAN_MAX;
extern long   gxhVoiceLevel;
extern long   gxhWhichSorting;
extern long  gxhWeightSpecialType;
extern long gxhDisplaySelect;  //  0: 선별시 단수 표시, 1: 선별시 무게 표시 







/************낙상침대 ********************/







#endif /* __I2C_EE_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/


