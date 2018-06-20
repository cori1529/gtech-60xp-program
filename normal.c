/*
	
   낙상침대용 





	TIM2 clock enable for timer base - ADC clock 200*384
	TIM3 clock enable  for timer base - FND display timer
  

   ---------------------------------------------------------------
    TIM2 Configuration: Output Compare Timing Mode:
    TIM2CLK = 36 MHz, Prescaler = 4, TIM2 counter clock = 7.2 MHz
    CC1 update rate = TIM2 counter clock / CCR1_Val = 146.48 Hz
    CC2 update rate = TIM2 counter clock / CCR2_Val = 219.7 Hz
    CC3 update rate = TIM2 counter clock / CCR3_Val = 439.4 Hz
    CC4 update rate = TIM2 counter clock / CCR4_Val =  878.9 Hz
   ---------------------------------------------------------------

relay pulldown
digit 도 PULLDOWN 확인할 것.
압축인장 확인할 것.
로드셀 최대값 확인할 것

2016.05.26
 - v1.01로 업데이트
 - 계량모드에서 확인키 5초이상 입력시 모드선택화면으로 이동하도록 추가
 - TEST, CAL, Func 메뉴 진입한 후 서 이전메뉴로 복귀하거나 설정이 완료되면 시 진입했던 화면이 표시되도록 수정
 - CAL_1, CAL_2 선택 화면에서 취소키 입력시 이전화면으로 복귀하도록 수정(기존 소수점 설정으로 진입했었음)
- mode_select() 함수에서 static char k = 0으로 초기화
*/

/* Includes ------------------------------------------------------------------*/

#include "stm32f10x_lib.h"
#include "stm32f10x_it.h"
#include "stm32f10x_rcc.h"
//#include "core_cm3.h"

#include "adc.h"
#include "buzzer.h"
#include "cal.h"
#include "dac.h"
#include "delay.h"
#include "dma.h"
#include "i2c_ee.h"
#include "key.h"
#include "lcd.h"	  
#include "relay.h"
#include "set.h"
#include "usart.h"
#include "key.h"
#include "var.h"
#include "matrix.h"
#include "relay.h"


#define	WEIGHING_MODE   0x0300
#define	VOICEOFFDELAY	4

#define CHECK_KEY_TIMER 150

//#define	DAN_MAX		11
 //int 	DAN_MAX	= 0;

int  iDanCheck();
void DanDisplay(char dan);
void SaveDan(char dan);
void DanSetting();
void CalcMinimumWeight();
void ByDan_Funciton();









u8 TxBufferSize1 = 50;
// int AverageHoldTimer;
 char HoldFlag;
 long HoldStateTimer;
 int HoldTimerFlag;
 int AverageHoldTimer;
 int AverageHoldTimerFlag;


char TxBuf1[30];
char TxBuf2[30];
char TxBuf3[30];
char TxBuf4[25];

char USART1_DMA_send_enabled = 1;
char USART2_DMA_send_enabled = 1;
char USART3_DMA_send_enabled = 1;
char UART4_DMA_send_enabled = 1;


typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;

DMA_InitTypeDef DMA_InitStructure;
vu16  ADCConvertedValue;

ADC_InitTypeDef ADC_InitStructure;

/* Private define ------------------------------------------------------------*/

/* Init Structure definition */
//TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;

#define nop()  __asm__ __volatile__("nop")

/* Private macro -------------------------------------------------------------*/
#define countof(a)   (sizeof(a) / sizeof(*(a)))

/* Private variables ---------------------------------------------------------*/
ErrorStatus HSEStartUpStatus;
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;



/* ####################Reset Registor define : reset_system(); 에서 사용하기 위한 정의 #####################*/


#define SCB_AIRCR_VECTKEY_Pos              16                                             /*!< SCB AIRCR: VECTKEY Position */
#define SCB_AIRCR_VECTKEY_Msk              (0xFFFFUL << SCB_AIRCR_VECTKEY_Pos)            /*!< SCB AIRCR: VECTKEY Mask */

#define SCB_AIRCR_VECTKEYSTAT_Pos          16                                             /*!< SCB AIRCR: VECTKEYSTAT Position */
#define SCB_AIRCR_VECTKEYSTAT_Msk          (0xFFFFUL << SCB_AIRCR_VECTKEYSTAT_Pos)        /*!< SCB AIRCR: VECTKEYSTAT Mask */

#define SCB_AIRCR_ENDIANESS_Pos            15                                             /*!< SCB AIRCR: ENDIANESS Position */
#define SCB_AIRCR_ENDIANESS_Msk            (1UL << SCB_AIRCR_ENDIANESS_Pos)               /*!< SCB AIRCR: ENDIANESS Mask */

#define SCB_AIRCR_PRIGROUP_Pos              8                                             /*!< SCB AIRCR: PRIGROUP Position */
#define SCB_AIRCR_PRIGROUP_Msk             (7UL << SCB_AIRCR_PRIGROUP_Pos)                /*!< SCB AIRCR: PRIGROUP Mask */

#define SCB_AIRCR_SYSRESETREQ_Pos           2                                             /*!< SCB AIRCR: SYSRESETREQ Position */
#define SCB_AIRCR_SYSRESETREQ_Msk          (1UL << SCB_AIRCR_SYSRESETREQ_Pos)             /*!< SCB AIRCR: SYSRESETREQ Mask */

#define SCB_AIRCR_VECTCLRACTIVE_Pos         1                                             /*!< SCB AIRCR: VECTCLRACTIVE Position */
#define SCB_AIRCR_VECTCLRACTIVE_Msk        (1UL << SCB_AIRCR_VECTCLRACTIVE_Pos)           /*!< SCB AIRCR: VECTCLRACTIVE Mask */

#define SCB_AIRCR_VECTRESET_Pos             0                                             /*!< SCB AIRCR: VECTRESET Position */
#define SCB_AIRCR_VECTRESET_Msk            (1UL << SCB_AIRCR_VECTRESET_Pos)               /*!< SCB AIRCR: VECTRESET Mask */


/* ####################Reset Registor define : reset_system(); 에서 사용하기 위한 정의 #####################*/


extern volatile u16 ref_cal_mode_check_timer;	//ref cal 모드에서 "T" 키 연속입력 시간 제한 타이머 



//u16  T2Reroad_value; // 타이머 2의 reloat  value 
vu16 CCR1_Val;

vu16 guClockMs = 0;
vu8 ghClockSec = 0;
vu8 ghClockMin = 0;
vu8 ghClockHour = 0;
vu8 ghClockDate = 1;
vu8 ghClockMonth = 1;
vu16 ghClockYear = 2016;


vu8 Usart1_Interrupt_flag=0;

vu8 USARTx_Interrupt_flag = 0;	//command가 입력된 port 번호

vu8 RxBuffer2[31];
vu8 RxCounter2 = 0x00;
vu8 Usart2_Interrupt_flag=0;
vu8 Rx2Cmd[31];

vu8 RxBuffer3[250];
vu8 RxCounter3 = 0x00;
vu8 Usart3_Interrupt_flag=0;
vu8 Rx3Cmd[250];

vu8 RxBuffer4[250];
vu8 RxCounter4 = 0x00;
vu8 Usart4_Interrupt_flag=0;
vu8 Rx4Cmd[250];



// cal
float         gnfFactor;
unsigned long gdwFactor;
unsigned long gdwFactorFraction;	//가수부   sign(1) exponent(8) fraction(23)
unsigned long gdwFactorexponent;	//지수부
float		  gnfEquFactor;
float		  gnfRefFactor;
float 		  gnfVoltPerMv;

// display
u8	Fnd_rotation;
u16 Display_buffer[14];


char str[300];

const char model[] = "GTECH ";
//const char model[] = " G3   ";
//const char version[] = "V 100 ";
//const char version[] = "V 102 ";
const char version[] = "V 110 ";
/* V 1.01
	- 2016.05.18 
	- 캘리브레이션 완료 후 모드선택화면으로 복귀
	- 모드선택 화면에 리부트 선택화면 추가
	- 노말모드에서 확인키 5초이상 누르고 있으면 모드선택화면으로 이동
	
	* V 1.02
	  - 영점키 연속으로 누르면 무게 튀는 현상 수정
		- AUTOZERO는 안정상태가 2초이상 유지되었을 때 동작하도록 수정
		- 테스트모드, 캘리브레이션에서 A/D값 표시화면에 5자리 넘어가면 1의자리 잘리는 문제 수정
		

* 2016.07.28
    - AUTOZERO는 안정상태가 2초이상 유지되었을 때 동작하는 기능 삭제(영점되는 순간 동작하도록)(Normal.c)
		- 자동영점범위 0으로 설정시 영점 계속 먹는 문제 수정(Normal.c)
		
*V 1.10
    - 2016.07.28 수정건 적용
		- 인장/압축 위한 A/D 나누는값 수정(read_filtered_ad() return시 /15 => /12)
		- 커런트루프 기능 추가
		- 커런트루프 통신속도 설정 추가(F49)
		- Cal, Test mode 에서 A/D값 표시 /10 하여 표시되도록 수정
		
*/

/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void NVIC_Configuration(void);
void GPIO_Configuration(void);
//void Delay_ms_us(vu32 nCount);
//void Delay_ms(vu32 nCount);
 
void set_mode(void);
void test_mode(void);
void te_set(void);//
void normal_mode(void);
void mode_select(void);
//void cal_mode(void);																		 `
void set_initialization(void);
void initial_adc(void);

void display_model(void);
void display_version(void);
void reset_system(void);

//void Voice_Reset(void);
//void Voice_Data_Loading(int number);
//void Voice_Play(int number);
/*
void  Potential_Increment_Set(void); 
void  Potential_Decrement_Set(void); 
void  Potential_Voice_Adjustment(void);
*/

void  mode_check(void);

void register_Delay_ms(int time);
/*
{
	int	 duration;

	for(duration=0; duration<time; duration++) {}
}

*/




int state=0;
enum
{
	MODE_SELECT,
	NORMAL_MODE,
	TEST_MODE,
	CALI_MODE,
	SETTING_MODE,
	CAL_1,
	CAL_2,
	CAL_R,
	SET_INITIALIZATION,
	REBOOT_MODE
};
	
//extern volatile	u16	 KeyCode;

union { u32 dw; u16 u[2]; u8 c[4];} memory_test;


void Init_TIM2_For_Ad_Clock()
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* TIM2 clock enable for timer base - ADC clock 200*384 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	
	
	// Enable the TIM2 gloabal Interrupt for ADC clock
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQChannel;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);


#ifdef ___another__	
     // TIM2CLK = 72 MHz( APB1은 원래 36MHz 인데 분주를 해서 사용하므로 36*2 = 72MHz 가 된다. )
    // 시간의 기본단위 :S(초)-->nS.uS.mS.S.
    // 72000000/60000=1200 즉 1초에 1200번 클럭이 발생하므로 
    // ARR 레지스터를 1199+1 번에 한번 인터럽트가 
    // 발생하도록 설정하면 1초에 한번 인터럽트가 발생된다.
	
	// ad 200회 clock 76800hz*2=153600
	// 72000000/153600 = 468.75

    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = 469-1;	// ARR(Auto reload register)
    TIM_TimeBaseStructure.TIM_Prescaler = 153600-1; 
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    TIM_ARRPreloadConfig(TIM2, ENABLE);
    TIM_Cmd(TIM2, ENABLE);

    /* TIM IT enable */
    TIM_ITConfig(TIM2, TIM_IT_Update , ENABLE); 	
#endif	//another	
	
#ifdef ___using__	
	
	///////very critical value for weighting ////////////////////
	//58 => 320sampling
	//	CCR1_Val=93; //  => 76.8KHZ, 200SAMPLING
	CCR1_Val=469; //for no prescaler => 153518.123667377Hz(76.8KHZ), 200 SAMPLING
	// 50 sampling   19200		  7200000/19200=375
	// 100 sampling  38400  	  7200000/38400=187
	// CCR1_Val=1845; 
	// CCR1_Val=375;
  

	//TIMER2 for ADC 1251 CLOCK
	/* Time base configuration */
	//TIM_TimeBaseStructure.TIM_Period = 255;		// 16 bit timer 
	TIM_TimeBaseStructure.TIM_Period = 65535;		// 16 bit timer 
	TIM_TimeBaseStructure.TIM_Prescaler = 0;		// 분주를 하면 안되고  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	/* Prescaler configuration */
//    TIM_PrescalerConfig(TIM2, 4, TIM_PSCReloadMode_Immediate);
    TIM_PrescalerConfig(TIM2, 0, TIM_PSCReloadMode_Immediate);
	//	TIM_PrescalerConfig(TIM2, 9, TIM_PSCReloadMode_Immediate);

	/* Output Compare Timing Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Disable);

	/* TIM IT enable */
	//TIM_ITConfig(TIM2, TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4 , ENABLE);
	TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);

	/* TIM2 enable counter */
	TIM_Cmd(TIM2, ENABLE);	
#endif	//#ifdef ___using__	

	TIM_TimeBaseStructure.TIM_Period = 1000;	//7200;		// 16 bit timer 
	TIM_TimeBaseStructure.TIM_Prescaler = 71;	//1us
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	/* Prescaler configuration */
    //TIM_PrescalerConfig(TIM2, 0, TIM_PSCReloadMode_Immediate);
     
	TIM_ClearITPendingBit(TIM2, TIM_FLAG_Update);	//clear update interrupt bit
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); 	//enable update interrupt

	/* TIM2 enable counter */
	TIM_Cmd(TIM2, ENABLE);	

}	
	


void retry_message()
{
	beep(10); Delay_ms(20);
	beep(10); Delay_ms(20);
	beep(10); Delay_ms(20);
	beep(10); Delay_ms(20);
	beep(10); Delay_ms(20);
}

//SPI_InitTypeDef   SPI_InitStructure;
/* Private define ------------------------------------------------------------*/
#define BufferSize 32
#define SEND_LIMIT 1000

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
SPI_InitTypeDef   SPI_InitStructure;

u8 SPI2_Buffer_Tx[BufferSize] = {0x1, 0x2, 0x4, 0x54, 0x55, 0x56, 0x57,
                                      0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E,
                                      0x5F, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65,
                                      0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C,
                                      0x6D, 0x6E, 0x6F, 0x70};

u8 TxIdx = 0, RxIdx = 0, k = 0;
u16 spi_send_data=0;

/* Private functions ---------------------------------------------------------*/



//#define    ADC_DATA_READY         if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == 0x00)

#define MAX_CHANNEL 4

    long   count =0;
	long result=0;
	float NewValue,Difference,Average;
    float Filtered_NewValue,Filtered_Average[4];
	signed long Filtered_Average_int; //

	long  Zero_channel_1,Zero_channel_2,Zero_channel_3,Zero_channel_4;
	long data,data1;
	long Unfiltered_data[4];
    long Filtered_data[4];
	//float Factor_channel_1,Factor_channel_2,Factor_channel_3,Factor_channel_4;
	float Factor_channel[4];
	long zero_value[4];  //영점값 
    long law_value[4];  //보정안한 순수한 a/d 값  

	long patient_value[4]; //환자 있을때 a/d  값
	long total_patient_span;
	float patient_span[4]; //환자 span 값 
	
	float channel_ratio_1, channel_ratio_2, channel_ratio_3, channel_ratio_4,temp1,temp2,temp3,temp4,channel_sum; // 각채널의 출력이 가지고 있는 분담비율 구하기 
	float judge_ratio_1,judge_ratio_2,judge_ratio_3,judge_ratio_4;
	float Left_ratio, Right_ratio;
    int  Current_patient_status,Before_patient_status;// 현재 환자상태 와 그전 환자 상태 
	char patient_being; // 0   현재 사람이 없다 , 1 현재 사람이 있
	long channel_sum_int;

	long  current_sum_int, before_sum_int;
	float judge_ration_1,judge_ration_2,judge_ration_3,judge_ration_4;
	
	float patient_detect_value_float,temp_calculate; // 환자의 있고 없고를 판단하는 값
    long patient_detect_value_long; // 환자의 있고 없고를 판단하는 값

    float lower_toilet_value_float, upper_toilet_value_float; // 환자의 있고 없고를 판단하는 값
    long lower_toilet_value_long,  upper_toilet_value_long; // 환자의 있고 없고를 판단하는 값
    




   void Exponential_Filtering(void);
   void	Get_ration_respective_channel();//채널별 분담비율 구하기 

  #define RATIO_DIFFERENCE 0.95  // 좌우 분담비율이  0.7 : 0.3 이상 으로 될때 

  #define  NO_PATIENT                        0 // 환자없음 
  #define  CENTER_POSTURE                    1 // 가운데 방향에서 잘누워있기
  #define  ALLMOST_FALLING_LEFT              2	// 왼쪽 낙상위험
  #define  ALLMOST_FALLING_RIGHT             3	// 오늘쪽 낙상위험
  #define  SEAT_LEFT_TOP                     4	// 좌상 앉아있기 
  #define  SEAT_CENTER_TOP                   5	// 좌상중 
  #define  SEAT_RIGHT_TOP                    6	// 우상중 
  #define  SEAT_CENTER_LEFT                  7	// 중좌 
  #define  SEAT_CENTER_CENTER                8	// 중중 
  #define  SEAT_CENTER_RIGHT                 9 	// 중우 
  #define  SEAT_LEFT_BOTTOM                  10	// 좌하 
  #define  SEAT_CENTER_BOTTOM                11	// 중하 
  #define  SEAT_RIGHT_BOTTOM                 12	// 우하  
  #define  FALLING_PATIENT                   13	// 우하  
  #define  PATIENT_FOR_TOILET                14	// 화장실 가기전에 앉아서 준비   
  #define  SEAT_CENTER_BOTTOM                15	// 침대앞쪽    
  
  //2017년 낙상징후 및 낙상감지 패드  
  #define   PAD_NO_PATIENT                  0 //  환자 없음 
  #define   PAD_CENTER_PATEINT              1 //  환자 가운데 누워있음
  #define   PAD_SEAT_PATEINT                2 //  환자 앉아 있음 
  #define   PAD_FALL_PATEINT                3 //  환자 낙상 위험에 처해 있음  
   char  Current_patient_status_pad=0,Before_patient_status_pad=0; 
   

   char guide_flag1=0;
   char guide_flag2=0;
   char guide_status_flag=0;
   char guide_send_once_flag=0;



   //2017년 낙상징후 및 낙상감지 패드  
  



 
 u16 sending_interval_counter;
// u16 key_timer=0;
 u16 hold_key_timer=0;
 u16 start_key_timer=0;
 u16 save_key_timer=0;
 
 //u32 adc_timer=0;




 u8 lamp_falling='0';
 u8 lamp_fall_warning='0';
 u8 lamp_guide='0';
 u8 lamp_no_patient='0';
 u8 once_flag=0; // 
 u8 left_flag=0; // 1 : 왼쪽방향   0: 오른쪽 방향 
 u8 guide_flag=0; // 1 :침대 안전 가이드 on  0: 침대 안전 가이드 off 
 u16 left_head_count=0,left_foot_count=0,right_head_count=0,right_foot_count=0;
 u8  count_temp=0;
 u32 result_1,result_2,result_3,result_4;
 
 u8 zero_key_pressed=0;	 // 키가 눌렸냐 안눌렸나 ?
 u8 start_key_pressed=0;
 u8 hold_key_pressed=0;
 u8 reset_key_pressed=0;
 u8 save_key_pressed=0;
 u32 key_pressed_timer=0;			

///////////stable 
/*
s32	diff1,	
	diff2, diff21,	
	diff3, diff31,
	over_weight,
	zero_range,
	limit,
	prev_adc1 = 0;
*/



//s32 prev_adc1 = 0, diff21;

//s32 prev_adc1 = 0;
s32 prev_adc1_temp = 0;

s32 diff21;
s32 diff2_temp;



int  stable_range, stable_time;
long stable_pre_weight;

u8 	g_sign=0;
u8 stable_flag=0; // 1 : stable, 0: unstable
u8	hUnstableCount = 0; /* when unstable */
u8  StableCount=0;








//stable
/*
#define    ZERO_KEY         if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_7) == 0x00)
#define    START_KEY         if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_6) == 0x00)
#define    HOLD_KEY         if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_5) == 0x00)
#define    RESET_KEY         if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_4) == 0x00)
#define    SAVE_KEY         if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_3) == 0x00)

  */

void Key_processing(void);
void logo(void);
void Saftey_Input(void);
union { float f; u32 dw; u16 u[2]; u8 c[4];} temp;
float judge_ratio; // 좌우 비율 판단기준, alert setting 값에 따라서  1~7단계   1: 0.8 , 7: 0.95 

/*
void No_patient_signal(void);
void Safety_signal(void);
void Falling_Warning_signal(void);
void Falling_signal(void);

*/
char patient_alram_once_flag=0;
char guard_alram_once_flag=0;
char warning_alram_once_flag=0;
char falling_alram_once_flag=0;
char  Judge_once_flag=0;
int  Warning_count=0; 
char  power_flag=1;// 처음에 전원 켰을때 
char  safety_power_flag=1;// 처음에 전원 켰을때 
char zero_key_ratatiion=0;


#define WARNING_COUNTER       5
#define PATIENT_SEAT_COUNTER  5
/**********gi-1400(new)****************************/
void   test_mode();
void   display_test();
void   dip_sw_test();
void   adc_test();
int   battery_test();

void   wiress_test();
void Get_My_Address(); // getting my address when power on
void initilize();
void  SendDanValue(void);
void  AllFruitLed_On(void);
void  AllFruitLed_Off(void);
void  Initial_Fruit_Led(void);
void Pojang_Display(void);
void Pojang_Display_Voice(void);
void Current_Type_DanSetting(void);
void Init_Dan(void);
void Dansu_Led_Blink(void);
void Pojang_Led_Blink(void);
void Bluetooth_Saving(void);

u8 bluetooth_which_fruit; 
u8 bluetooth_which_pojang; 


//hold_key_timer





#define  STABLE_TEST



/**********gi-1400(new)****************************/


#define BATTERY_ERROR_VOLTAGE  900 // 9 volt
#define BATTERY_CHECK_INTERVAL_ONE_MINUTE    (1000*60)

long Battery_Voltage;
long Get_weight(void);

s32 v_i_value ;
	long raw_data;
	long gxlGrossWeight_1;
	long gxlGrossWeight;
	long Final_Weight;
	long gxlWeight;
	long v_adc_init;//  = temp_org;
	
//#define _80SPEED   1250	
	

//int main(void)
u16  key_number;

#define	LED_ON	1
#define	LED_OFF	0
char LAMPST;

//u8		qKEYIN=0;

#define BLINK_COUNTER  50
u8 fruit_select=0;
u32 final_pojang_weight=0;

u8 stable_number=0;

#define POJANG_OFFSET  64
	
void normal_mode()
{
/*    long   count =0;
	long result=0;
	float NewValue,Difference,Average;
	long data,data1;
	long Unfiltered_data[4];
    long Filtered_data[4];
 */
	
	u8 suljung_flag=0; //  설정키를 일정시간 이상눌러서 설정모드로 정상적으로 진입하면 1, 아니면 0
	u8 power_key_flag=0;//
	//u8 fruit_select=0;
	u32 gnxTareStatus = 0;
	u32 gnlTareWeight = 0;
	u16 iz;
	u8 send_spi_data=0;
	long sum=0;
	int  i;
	char key_code;
	char CS5532_channel_selection=1;
	char Fall_Warning_Counter=0; // 낙상징후 경보를 하기위한 최소 카운터수 
	char Patient_Seat_Counter=0; // 환자이동 경보를 하기위한 최소 카운터스 
	unsigned long cs5555_result;
	unsigned int key_value;
	u16 loc,loc1;
	/*
	s32 v_i_value ;
	long raw_data;
	long gxlGrossWeight_1;
	*/
	u16 key_pressed_timer=0;
	u8  key_pressed=0;
	//u16  key_number;
	char Adc_Controller_Address;
	long Max_Chicket_Weight=500; // default value  
	long Min_Chicket_Weight=80; //  default value
	long Motion_Count=10; // default value
	long gxhMotionBand_temp;
	long Chicken_Getting_Enalble_flag=0; // 메인으로부터 상,하한 값을 받아야지 무게를 체크 한다. 
	s32	 v_adc_org_original;
	s32 gxlGrossWeight_original;
	u8  Serial_count=0;
	u8 Battery_error=0;

//  long sum=0;
	long sum_count=0;
	long sum_average=0;
	
	
	
	
	u8	exit_flag = 0,
			WeighMode = 1,  /* when 0 key input mode else weigh mode */
			ch = 0;
//			net_sign     = 0,
//			gross_sign   = 0;
	u8	sec_now = 0;    /* time out counter with key wait */
	u8	hKeyTimeOutCounter = 0;    /* time out counter with key wait */
	u8	hAutoZeroCounter  = 0;	/* zero tracking counter */
	u8	az_ok;          /* time of zero tracking */
	u8	StableCount = 0,	/* stable processing counter */
			hUnstableCount = 0; /* Unstable processing counter */
	u32	timeb = 0;		/* set mode */
	long	diff1, diff11,	/* for ...... */
			diff2,			/* for stable */
			diff21,			/* for stable */
			over_weight,
			zero_range,
			limit,
			prev_adc1 = 0;
	long	lPreWeight = 0;
	u8	hUpdateCount = 0;

	long  raw_data;	  /* float raw_data; */
	long  s_weight = 0;
	long  s_weight1 = 0;
	long  s_weight2 = 0;
	int		iVoice = 0;
	u8	bWorking = 0; /* working between start and stop */

	u8  gross_flag = 0;    /* when gross flag */

	long  lxlInNum = 0;         /* key input number */
	long	lxlInLimit = 0;    /* key input limit  maximum input / 10 */

	u8	bGradeReady = 0;

	long	lOneWeight;
	long	lPreRef = 0;
	u8	bScaleMode = 1;
	u8	bGradeMode = 0;
	u8	bSetMode = 0;
	int		bSetMode1 = 0;   // sul jung mode - pum mok x
	int		bSetMode2 = 1;   // sul jung mode

	u8	hEndCount = 5;
	u8	end_flag = 0;    /* determine done */

	u8	byDanIndex = 0;
	u8	byVoiceOff = 0;
	u8	bBlink = 0;
	u16	byBlinkCounter = 0;
	u8	bPreSignPlus = 1;
	u8	bGrossSignPlus = 1;
	u8	bNetSignPlus = 1;
	u8	byDisplayUpdate = 5;	/* ver 0.03 */
	u8	byCountIndex = 0;
	u8	bModeCount = 0;
	u16	command = 0;
	u16	command1 = 0;
	u8  kwak;
	long	lKeyWait = 0;

	u8	byDisplayMode = 0;	// 0:counter, 1:dan total, 2:weigh
	u8	byGrade = 1;		//ver1.00 --> 0 // current grade
//	long	lGradeWeight = 0;	// current grade weigh
	int		iKey = 0;

	int		iSpeedMode = 0;		//botong speed

	
	dpoint_bak = v_decimal_point;

	lTotalCount = 0;
    for(i=0; i<26; i++) lCount[i] = 0;
	
	
/*	
	
		if (i1dan < 0 || i2dan < 0 || i3dan < 0 || i4dan < 0 || i5dan < 0 ||
		i6dan < 0 || i7dan < 0 || i8dan < 0 || i9dan < 0 || i10dan < 0 ||
		i11dan < 0 || i12dan < 0 || i13dan < 0 || i14dan < 0 || i15dan < 0 ||
		i16dan < 0 || i17dan < 0 || i18dan < 0 || i19dan < 0 || i20dan < 0 ||
		i21dan < 0 || i22dan < 0 || i23dan < 0 || i24dan < 0 )
	{
		vputs("RELOD");
    DefaultDanSetting();
		SaveDan(0);
		Delay_ms(1000);
	}

	*/
	
  for(i=0;i<5;i++)
	 {
	 RCC_Configuration();     // 0308
	 GPIO_Configuration();	//zig   0403
	 NVIC_Configuration();
	 }
	 I2C_EE_Init();
	 read_cal_factors();
	 cond_check();

	// Configure the DMA for usart
	DMA_Configuration();  
	USARTx_Initial();	//USART_USER_Initialization(); // at stm32f10x_usart.c
	TM_Delay_Init();	//for TM_Delay_msMicros(), TM_Delay_msMillis

	//key_init();


/***/

	/******************* systick***********************************************/
	/* SysTick end of count event each 1ms with input clock equal to 9MHz (HCLK/8, default) */
///	SysTick_SetReload(5);// 9/9MHZ = 1u sec
//	SysTick_SetReload(RCC_Clocks.SYSCLK_Frequency / 1000);
//	SysTick_SetReload(9000);	//Setup SysTick Timer for 1 msec interrupts
//	SysTick_SetReload(9000000/153600);	//153600 ad clock 76.8k (200회*384)

	SysTick_SetReload(58);	//153600 ad clock 76.8k (200회*384)
	
	SysTick_SetReload(9000000/100000);	//0.01 msec   100000 개 : 1
	
	SysTick_ITConfig(ENABLE);	//Enable SysTick interrupt
	SysTick_CounterCmd(SysTick_Counter_Enable);	//Enable the SysTick Counter

	//timer 2 init, AD CLOCK AND DISPLAY 
	Init_TIM2_For_Ad_Clock();
 

/************* adc configuration********************************/

#define ADC3_DR_Address    ((u32)0x40013C4C)
#define ADC1_DR_Address    ((u32)0x4001244C)

 //#define ADC1_DR_Address    ((u32)0x4001244C)
//#define ADC3_DR_Address    ((u32)0x40013C4C)



	/*********온도 센서 *************/
//  DMA_InitTypeDef DMA_InitStructure;
  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADCConvertedValue;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = 1;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);  
  DMA_Cmd(DMA1_Channel1, ENABLE);

   DMA_Configuration();
   ADC_TempSensorVrefintCmd(ENABLE);


    /* ADC1 configuration ------------------------------------------------------*/





  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  //ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(ADC1, &ADC_InitStructure);
 
  ADC_RegularChannelConfig(ADC1,ADC_Channel_8,1,ADC_SampleTime_239Cycles5);

  // Enable ADC1 DMA 
  ADC_DMACmd(ADC1, ENABLE);

   // Enable ADC1 
  ADC_Cmd(ADC1, ENABLE);

  //Enable ADC1 reset calibaration register   
  ADC_ResetCalibration(ADC1);
  // Check the end of ADC3 reset calibration register 
  while(ADC_GetResetCalibrationStatus(ADC1));

  // Start ADC3 calibaration 
  ADC_StartCalibration(ADC1);
  // Check the end of ADC3 calibration 
  while(ADC_GetCalibrationStatus(ADC1));


  /* Start ADC1 Software Conversion */ 
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
  /* Start ADC2 Software Conversion */ 
  ADC_SoftwareStartConvCmd(ADC2, ENABLE); // 이거 없으면 이상한 결과 나옴 
 																	/* Start ADC2 Software Conversion */ 
  ADC_SoftwareStartConvCmd(ADC3, ENABLE);
 
  /* Start ADC3 Software Conversion */ 
  ADC_SoftwareStartConvCmd(ADC3, ENABLE);

/************* adc configuration********************************/
 buzzer_off();
 INIT_CS5555();
 v_ei_multiply_factor=10;
 

/************ Getting initial zero value *************/ 
i=0;



//Voice_Reset(); //0323
//Voice_Play(0);
//Voice_Play(1);
v_decimal_point=3;

 do //0503
 {
 if(adc_timer>=_80SPEED)// 
	 {
		  i++;
	   adc_timer=0;
 		 v_zero=read_filtered_adc();// get weight and judge stable
		 sprintf(str, "zero_value : %d", v_zero);	USART1_puts(str);
	  }// of key timey  
  if(i>50)
		break;
 }while(1);	
 
 
 /*
 if ((v_zero < 300L) || (65535L <= v_zero))
	{  vputs("ERR 2"); beep(100); Delay_ms(500); return; }

 */
 
	limit = v_capacity * power(10, v_decimal_point);
//	over_weight = limit + limit/10;	/* set over load range */
  over_weight = limit + 9*v_division;	/* set over load range */

			 /*  +- for banolim */
//	zero_range = (limit + v_ei_multiply_factor/2) /v_ei_multiply_factor;    /* zero key range is 10% */
	zero_range = limit;

	az_ok = 20;
	if (9 < gxhMotionBand) gxhMotionBand = 1; /* DIGIT of stable condition */
	
	
 
		sprintf(str, "zero_adc_value : %d", v_zero);	USART1_puts(str);
	  v_adc_org = (s32)(gnfFactor * v_zero);
	  v_adc_init=v_adc_org;
/************ Getting initial zero value *************/ 
   buzzer_off();	
   initilize();
   Voice_Reset();
	/********************************************/
	 //0412
	 
	 gxhFilter=45; // for  선별 
	// gxhFilter=10; // for  선별 
	 
	 
  /********************************************/
/************	mode_check ********************/ 
 /*
 while(1)
 {	 
 
 key_number=keypush();
 if(key_number)
 { 
 	sprintf(str, "key_number : %d", key_number);	USART1_puts(str);
	
 } 
 
 };
 */
 
 gxhDisplaySelect=0;
 
 gxhDisplaySelect=0; // 선별시 선별등급 갯수를 표시
 key_number=keypush();
 if(key_number==ZERO_K)
	  test_mode();
 else if(key_number==COUNT_K) 
    cal_mode();
 else if(key_number==SULJUNG_K) // 단수 초기화
		 Init_Dan();
 else if(key_number==VOICEUP_K)
 {
	 	  gxhDisplaySelect=1;
	    vputs("D = 1");
		  Delay_ms(1500);
	 
	 
	 
	 /***** ble name change*************** 201806190619
	 ********/
//	 #define   BLE_ATCOMMAND      GPIO_SetBits(GPIOB, GPIO_Pin_15)
//#define   BLE_DATAMODE         GPIO_ResetBits(GPIOB, GPIO_Pin_15)

	 
	 //AT+MANUF=xxxxxxxx
	 
	USART1_puts("1111111111111"); 
 while(1)
	 {
		 	USART1_puts("222222222222222"); 

	// BLE_ATCOMMAND; Delay_ms(1500);
	 //BLE_DATAMODE; Delay_ms(1500);
	  
		GPIO_SetBits(GPIOB, GPIO_Pin_15); Delay_ms(1500);
    GPIO_ResetBits(GPIOB, GPIO_Pin_15); Delay_ms(1500);

	 };
	 	USART1_puts("3333333333333333"); 

	 /*
	 for(i=0;i<5;i++)
	 {
	 UART4_putchar('A');
	 UART4_putchar('T');
	 UART4_putchar('+');
	 UART4_putchar('M');
	 UART4_putchar('A');
	 UART4_putchar('N');
	 UART4_putchar('U');
	 UART4_putchar('F');
	 UART4_putchar('=');
	 UART4_putchar('G');
	 UART4_putchar('P');
	 UART4_putchar('-');
	 UART4_putchar('6');
	  UART4_putchar('0');
	 Delay_ms(500);
	 }
	 
	 
	 
	 
	 BLE_DATAMODE;
	 */
	 
	 /****************************/
	 
	 
	 
	  /*
	 if(!gxhDisplaySelect)
	 {
		  gxhDisplaySelect=1;
	    vputs("D = 1");
		  Delay_ms(1500);
	 }
		 else
		 {
       vputs("D = 0");
		  Delay_ms(1500);			 
	    gxhDisplaySelect=0;
	 
		 }
*/	 
 }
	 
 
 /*
 
  BLE_ATCOMMAND; 
//for(i=0;i<10;i++)
{
   Delay_ms(500);
	BLE_ATCOMMAND;
	  Delay_ms(500);
	
   UART4_putchar('A');
	 UART4_putchar('T');
	 UART4_putchar('+');
	 UART4_putchar('M');
	 UART4_putchar('A');
	 UART4_putchar('N');
	 UART4_putchar('U');
	 UART4_putchar('F');
	 UART4_putchar('=');
	 UART4_putchar('G');
	 UART4_putchar('P');
	 UART4_putchar('-');
	 UART4_putchar('6');
	  UART4_putchar('0');
	 Delay_ms(100);
}	
	BLE_DATAMODE; Delay_ms(500);
 
 */
 /*
 while(1)
 {
	BLE_ATCOMMAND;
	  Delay_ms(2000);
	 
 	BLE_DATAMODE; Delay_ms(2000);
	 
	 
	 
 };
 */
 for(i=0;i<1;i++)
 {
   BLE_ATCOMMAND;
	  Delay_ms(2000);
	
   UART4_putchar('A');
	 UART4_putchar('T');
	 UART4_putchar('+');
	 UART4_putchar('M');
	 UART4_putchar('A');
	 UART4_putchar('N');
	 UART4_putchar('U');
	 UART4_putchar('F');
	 UART4_putchar('=');
	 UART4_putchar('G');
	 UART4_putchar('P');
	 UART4_putchar('-');
	 UART4_putchar('6');
	 UART4_putchar('0');
   UART4_putchar('X');
	 UART4_putchar('P');
	 	 
	 UART4_putchar(0x0d);
	  UART4_putchar(0x0a);
 } 
	 Delay_ms(100);
	 
	/*
 
   UART4_putchar('A');
	 UART4_putchar('T');
	 UART4_putchar('+');
	 UART4_putchar('U');
	 UART4_putchar('A');
	 UART4_putchar('R');
	 UART4_putchar('T');
	 UART4_putchar('=');
	 UART4_putchar('1');
	 UART4_putchar('1');
	 UART4_putchar('5');
	 UART4_putchar('2');
	 UART4_putchar('0');
   UART4_putchar('0');
	 UART4_putchar(0x0d);
	  UART4_putchar(0x0a);
   Delay_ms(100);
	
 
 
  USAR4_115200();
 */
 




  BLE_DATAMODE; Delay_ms(500);
 
 
 
 
 
 /*
 while(1)
 {
	 
	 GPIO_SetBits(GPIOB, GPIO_Pin_15); USART1_puts("high");Delay_ms(5000);
	 GPIO_ResetBits(GPIOB, GPIO_Pin_15); USART1_puts("low");Delay_ms(5000);
	 
	 //BLE_ATCOMMAND; Delay_ms(1500);
	 //BLE_DATAMODE; Delay_ms(1500);
	 
	 
	 
 };
 */
 
 read_cal_factors();
	 cond_check();

 
 
 
 
 
 
/************	mode_check ********************/ 
 
 
 //DefaultDanSetting();
// recall_dan(gxhCurrentType);	  // 0327
 Display_buffer[12]=0; // ALL LED OFF
 Display_buffer[13]=0;
 
 if(!gxhWhichSorting)//  현재선별방법이 단수선별이면 
				{
					DANSU_LED_ON;
				}
  else
		    POJANG_LED_ON;
	

/********** initial voice***********************/
	
	
	Potential_ZeroSet(); // 음량 1단계
 Potential_Increment_Set();
 
 if(gxhVoiceLevel)
 Potential_Voice_Adjustment(gxhVoiceLevel);//0330
  
 Potential_ZeroSet(); // 음량 1단계
 Potential_Increment_Set();
 Potential_Voice_Adjustment(2);//0330
 

   Voice_Play(1);Delay_ms(2800); // 안녕하세요 지테크 음성식
	 
     Voice_Play(2);Delay_ms(1500);  //Voice_Reset();
/********** initial voice***********************/

	
	
	
 //Initial_Fruit_Led();
 //Pojang_Display();
 
 
 /*
 // gxhCurrentType  순서 :   포도 -> 자두  -> 복숭아 -> 사과 -> 감  -> 배  -> 한라봉 -> 레드향 -> 특작
 // 과일종류 고르기 순서 :   포도 -> 자두-  > 복숭아 ->한라봉->사과 ->감-  ->배-      >레드향   ->특작1
 	 fruit_select++;
           if(fruit_select>9) fruit_select=1;
            
				    if(fruit_select==1)
              goto grape_place;							
				    else if(fruit_select==2)
				      goto jadoo_place;
				    else if(fruit_select==3)
				      goto peach_place;
				 	  else if(fruit_select==4)
				      goto halabong_place;
				    else if(fruit_select==5)
				      goto apple_place;
				    else if(fruit_select==6)
				      goto gam_place;
				    else if(fruit_select==7)
				      goto pear_place;
				    else if(fruit_select==8)
				      goto redhyang_place;
				    else // (fruit_select==9)
				      goto special_place;
*/				
 
 
 
 
 
 
 
 
 if (i1dan >30000 || i2dan >30000 || i3dan >30000 || i4dan >30000 || i5dan >30000 ||
		i6dan >30000 || i7dan  >30000 || i8dan >30000 || i9dan >30000 || i10dan >30000 ||
		i11dan >30000 || i12dan >30000 || i13dan >30000 || i14dan >30000 || i15dan >30000 ||
		i16dan >30000 || i17dan >30000 || i18dan >30000 || i19dan >30000 || i20dan >30000 ||
		i21dan >30000 || i22dan >30000 || i23dan >30000 || i24dan >30000 )
 
 {
		vputs("RELOD");
		DefaultDanSetting();
		SaveDan(0);
		delay(1000);
	}
 cond_check();
 sprintf(str, "C-TYPE=%d %d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d",gxhCurrentType, i1dan,i2dan,i3dan,i4dan,i5dan,i6dan,i7dan,i8dan,i9dan
		                                                          ,i10dan,i11dan,i12dan,i13dan,i14dan,i15dan  
	                                                            ,i16dan,i17dan,i18dan,i19dan,i20dan,i21dan
                                                              ,i22dan,i23dan,i24dan,i25dan);	USART1_puts(str);
 
	sprintf(str, "grapetype=%d",gxhWeightGrapeType);USART1_puts(str);
 
	
	//gxhWeightGrapeType
 recall_dan(gxhCurrentType);	  // 0327
 Display_buffer[12]=0; // ALL LED OFF
 Display_buffer[13]=0;
	if(!gxhWhichSorting)//  현재선별방법이 포장갯수이면
				{
					DANSU_LED_ON;
				}
  else
		    POJANG_LED_ON;

 Initial_Fruit_Led();
 Pojang_Display();

	
	
	SendDanValue();
	
	
	
	
	
//	Display_buffer[12]=0; // ALL LED OFF
//	Display_buffer[13]=0;
 
	/*
	for(i=0;i<10;i++)
	{
	 APPLE_LED_ON; Delay_ms(1000);
	 GAM_LED_ON; Delay_ms(1000); 
	 PEAR_LED_ON; Delay_ms(1000);
	 HALABONG_LED_ON; Delay_ms(1000);
	 RED_LED_ON; Delay_ms(1000);
	 PUMPKIN_LED_ON; Delay_ms(1000);
	 SPECIAL_LED_ON; Delay_ms(1000);
		
	 APPLE_LED_OFF; Delay_ms(1000);
	 GAM_LED_OFF; Delay_ms(1000); 
	 PEAR_LED_OFF; Delay_ms(1000);
	 HALABONG_LED_OFF; Delay_ms(1000);
	 RED_LED_OFF; Delay_ms(1000);
	 PUMPKIN_LED_OFF; Delay_ms(1000);
	 SPECIAL_LED_OFF; Delay_ms(1000);
		}
	*/
	ByDan_Funciton();
	
  gxlDecimalPoint=3;
	
/*

Potential_ZeroSet(); // 음량 1단계
 Potential_Increment_Set();
 
 if(gxhVoiceLevel)
 Potential_Voice_Adjustment(gxhVoiceLevel);//0330
  
 Potential_ZeroSet(); // 음량 1단계
 Potential_Increment_Set();
 Potential_Voice_Adjustment(1);//0330
 

   Voice_Play(1);Delay_ms(2500);
	 Voice_Play(2);
 
 */
 
 
 Potential_ZeroSet(); // 음량 1단계
 Potential_Increment_Set();
 if(gxhVoiceLevel)
 Potential_Voice_Adjustment(gxhVoiceLevel);//0330
 
 
 
 
vcls();
 key_number=keypush();
 if(key_number==ZERO_K)
	  test_mode();
 else if(key_number==COUNT_K) 
    cal_mode();
 else if(key_number==SULJUNG_K) // 단수 초기화
		 Init_Dan();
 
 
 vcls();

  CalcMinimumWeight(); //// 1.01


/*
if(!gxhWhichSorting)//  현재선별방법이 포장갯수이면
				{
					DANSU_LED_ON;
				}
  else
		    POJANG_LED_ON;

*/

  Pojang_Display();

	
	Usart3_Interrupt_flag=0;
	 Usart4_Interrupt_flag=0;
		  RxCounter4=0;
		
	
 do	 
 {
	
	 
	 
	 
	 /*****************blue tooth processing ****************/  
	 if(Usart4_Interrupt_flag==1)
	 {
		// beep(300);
 	   Bluetooth_Saving();
 	 Usart4_Interrupt_flag=0;
		  RxCounter4=0;
	 } 
	 /*****************blue tooth processing ****************/  
	 
	 
	 key_number=keypush();
		
	 if(key_number)
		{
		
			switch(key_number)
	     {
	
        case WHICH_GRAED_K:    // 0503
					
				 if (bScaleMode)//  only 무게 확인 모드일때
				 {			
				if(gxhWhichSorting)//  현재선별방법이 포장갯수이면
				{
				  gxhWhichSorting=0;
					DANSU_LED_ON;
				  POJANG_LED_OFF;
					Voice_Play(6); // 단수선별입니다.
					
				}
				else
				{
				 DANSU_LED_OFF;
				 POJANG_LED_ON;
				
         gxhWhichSorting=1; //포장갯수 선별 
					Voice_Play(7); // 포장갯수 선별입니다.

        }  
				 gxhWhichSorting = EE_Write_val(ADR_gxhWhichSorting, gxhWhichSorting, 4);	  Delay_ms(10);
			}
      else
     retry_message();				
				
				
				 break;




				case VOICEUP_K://0329
					 gxhVoiceLevel++;
				   if(gxhVoiceLevel>6)
					 {
						 retry_message();
						 gxhVoiceLevel=6;
					 } 
					 Potential_ZeroSet(); // 음량 1단계
           Potential_Increment_Set();
           Potential_Voice_Adjustment(gxhVoiceLevel);
      		 gxhVoiceLevel  = EE_Write_val(ADR_gxhVoiceLevel , gxhVoiceLevel , 4);	 Delay_ms(10); 
					 Voice_Reset();
					 
         switch(gxhVoiceLevel)
         {	
           case 0: 					 
					 Voice_Play(165); 
					  break;
					 
          case 1: 					 
					 Voice_Play(166); 
					  break;
					 case 2: 					 
					 Voice_Play(167); 
					  break;
					 case 3: 					 
					 Voice_Play(168); 
					  break;
					 case 4: 					 
					 Voice_Play(169); 
					  break;
					 case 5: 					 
					 Voice_Play(170); 
					  break;
					 case 6: 					 
					 Voice_Play(171); 
					  break;
					 
					 default: break;
				 } 
					 
					 sprintf(str,"VOL=%d",(gxhVoiceLevel+1));
					 vputs(str);
				   Delay_ms(300);
          break;	
				 
         case VOICEDOWN_K:
					 gxhVoiceLevel--;
				   if(gxhVoiceLevel<0)
					 {
						 retry_message();
						 gxhVoiceLevel=0;
					 } 
					 Potential_ZeroSet(); // 음량 1단계
           Potential_Increment_Set();
				  if(gxhVoiceLevel)// 1이면 음량0 단계
            Potential_Voice_Adjustment(gxhVoiceLevel);
					  //sprintf(str,"VOL=%d",gxhVoiceLevel);
					  gxhVoiceLevel  = EE_Write_val(ADR_gxhVoiceLevel , gxhVoiceLevel , 4);	 Delay_ms(10); 
					 
					 //vputs(str);
					  Voice_Reset();
            //Voice_Play(3);
					switch(gxhVoiceLevel)
         {	
           case 0: 					 
					 Voice_Play(165); 
					  break;
					 
          case 1: 					 
					 Voice_Play(166); 
					  break;
					 case 2: 					 
					 Voice_Play(167); 
					  break;
					 case 3: 					 
					 Voice_Play(168); 
					  break;
					 case 4: 					 
					 Voice_Play(169); 
					  break;
					 case 5: 					 
					 Voice_Play(170); 
					  break;
					 case 6: 					 
					 Voice_Play(171); 
					  break;
					 
					 default: break;
				 } 
					
					sprintf(str,"VOL=%d",(gxhVoiceLevel+1));
					 vputs(str);
				   Delay_ms(300);
          
				  break;				 
				 
				case HUNDRED_K:	//M1
					if (bSetMode2 == 1) { retry_message(); break;}	  // default =1	   // 1026 0328
					command = 0;
				//	if(gxhCurrentType == 0 || gxhCurrentType == 3) // 포도  감 
				 //	if(gxhCurrentType == 0 || gxhCurrentType == 4) // 포도  감   //1.10
					
				//	DanDisplay(byDanIndex+1);while(1);
					
					
       if(gxhCurrentType == 0 || gxhCurrentType == 5|| gxhCurrentType == 4) // 포도  배   //1.11   , 포도 와 배는 

					{
						byDan[byDanIndex*3]++;
						 byDan[byDanIndex*3] %= 30;
					//	if (v_decimal_point == 3) byDan[byDanIndex*3] %= 20;
					//	else byDan[byDanIndex*3] %= 10;
					}
					else
					{
						byDan[byDanIndex*3]++;
						byDan[byDanIndex*3] %= 30;
					}
					i1dan = byDan[ 0] * 100 + byDan[ 1] * 10 + byDan[ 2];
					i2dan = byDan[ 3] * 100 + byDan[ 4] * 10 + byDan[ 5];
					i3dan = byDan[ 6] * 100 + byDan[ 7] * 10 + byDan[ 8];
					i4dan = byDan[ 9] * 100 + byDan[10] * 10 + byDan[11];
					i5dan = byDan[12] * 100 + byDan[13] * 10 + byDan[14];
					i6dan = byDan[15] * 100 + byDan[16] * 10 + byDan[17];
					i7dan = byDan[18] * 100 + byDan[19] * 10 + byDan[20];
					i8dan = byDan[21] * 100 + byDan[22] * 10 + byDan[23];
					i9dan = byDan[24] * 100 + byDan[25] * 10 + byDan[26];
					i10dan = byDan[27] * 100 + byDan[28] * 10 + byDan[29];
					i11dan = byDan[30] * 100 + byDan[31] * 10 + byDan[32];
					i12dan = byDan[33] * 100 + byDan[34] * 10 + byDan[35];
					i13dan = byDan[36] * 100 + byDan[37] * 10 + byDan[38];
					i14dan = byDan[39] * 100 + byDan[40] * 10 + byDan[41];
					i15dan = byDan[42] * 100 + byDan[43] * 10 + byDan[44];
					i16dan = byDan[45] * 100 + byDan[46] * 10 + byDan[47];
					i17dan = byDan[48] * 100 + byDan[49] * 10 + byDan[50];
					i18dan = byDan[51] * 100 + byDan[52] * 10 + byDan[53];
					i19dan = byDan[54] * 100 + byDan[55] * 10 + byDan[56];

				   	//ver1.01
				  i20dan = byDan[57] * 100 + byDan[58] * 10 + byDan[59];
					i21dan = byDan[60] * 100 + byDan[61] * 10 + byDan[62];
					i22dan = byDan[63] * 100 + byDan[64] * 10 + byDan[65];
					i23dan = byDan[66] * 100 + byDan[67] * 10 + byDan[68];
					i24dan = byDan[69] * 100 + byDan[70] * 10 + byDan[71];
					i25dan = byDan[72] * 100 + byDan[73] * 10 + byDan[74];



	 
//					SaveDan(0);
					DanDisplay(byDanIndex+1);
//v1.10 20150715					CalcMinimumWeight();// ver 1.01
					break;
				case TEN_K:	//M2
					if (bSetMode2 == 1) { retry_message(); break;}
					command = 0;
					byDan[byDanIndex*3+1]++;
					byDan[byDanIndex*3+1] %= 10;
					i1dan  = byDan[ 0] * 100 + byDan[ 1] * 10 + byDan[ 2];
					i2dan  = byDan[ 3] * 100 + byDan[ 4] * 10 + byDan[ 5];
					i3dan  = byDan[ 6] * 100 + byDan[ 7] * 10 + byDan[ 8];
					i4dan  = byDan[ 9] * 100 + byDan[10] * 10 + byDan[11];
					i5dan  = byDan[12] * 100 + byDan[13] * 10 + byDan[14];
					i6dan  = byDan[15] * 100 + byDan[16] * 10 + byDan[17];
					i7dan  = byDan[18] * 100 + byDan[19] * 10 + byDan[20];
					i8dan  = byDan[21] * 100 + byDan[22] * 10 + byDan[23];
					i9dan  = byDan[24] * 100 + byDan[25] * 10 + byDan[26];
					i10dan = byDan[27] * 100 + byDan[28] * 10 + byDan[29];
					i11dan = byDan[30] * 100 + byDan[31] * 10 + byDan[32];
					i12dan = byDan[33] * 100 + byDan[34] * 10 + byDan[35];
					i13dan = byDan[36] * 100 + byDan[37] * 10 + byDan[38];
					i14dan = byDan[39] * 100 + byDan[40] * 10 + byDan[41];
					i15dan = byDan[42] * 100 + byDan[43] * 10 + byDan[44];
					i16dan = byDan[45] * 100 + byDan[46] * 10 + byDan[47];
					i17dan = byDan[48] * 100 + byDan[49] * 10 + byDan[50];
					i18dan = byDan[51] * 100 + byDan[52] * 10 + byDan[53];
					i19dan = byDan[54] * 100 + byDan[55] * 10 + byDan[56];


				   	//ver1.01
				    i20dan = byDan[57] * 100 + byDan[58] * 10 + byDan[59];
					i21dan = byDan[60] * 100 + byDan[61] * 10 + byDan[62];
					i22dan = byDan[63] * 100 + byDan[64] * 10 + byDan[65];
					i23dan = byDan[66] * 100 + byDan[67] * 10 + byDan[68];
					i24dan = byDan[69] * 100 + byDan[70] * 10 + byDan[71];
					i25dan = byDan[72] * 100 + byDan[73] * 10 + byDan[74];





//					SaveDan(0);
					DanDisplay(byDanIndex+1);
//v1.10 20150715					CalcMinimumWeight();
					break;
				case ONE_K: //M3
					if (bSetMode2 == 1) { retry_message(); break;}
					command = 0;
					byDan[byDanIndex*3+2]++;
					byDan[byDanIndex*3+2] %= 10;
					i1dan = byDan[ 0] * 100 + byDan[ 1] * 10 + byDan[ 2];
					i2dan = byDan[ 3] * 100 + byDan[ 4] * 10 + byDan[ 5];
					i3dan = byDan[ 6] * 100 + byDan[ 7] * 10 + byDan[ 8];
					i4dan = byDan[ 9] * 100 + byDan[10] * 10 + byDan[11];
					i5dan = byDan[12] * 100 + byDan[13] * 10 + byDan[14];
					i6dan = byDan[15] * 100 + byDan[16] * 10 + byDan[17];
					i7dan = byDan[18] * 100 + byDan[19] * 10 + byDan[20];
					i8dan = byDan[21] * 100 + byDan[22] * 10 + byDan[23];
					i9dan = byDan[24] * 100 + byDan[25] * 10 + byDan[26];
					i10dan = byDan[27] * 100 + byDan[28] * 10 + byDan[29];
					i11dan = byDan[30] * 100 + byDan[31] * 10 + byDan[32];
					i12dan = byDan[33] * 100 + byDan[34] * 10 + byDan[35];
					i13dan = byDan[36] * 100 + byDan[37] * 10 + byDan[38];
					i14dan = byDan[39] * 100 + byDan[40] * 10 + byDan[41];
					i15dan = byDan[42] * 100 + byDan[43] * 10 + byDan[44];
					i16dan = byDan[45] * 100 + byDan[46] * 10 + byDan[47];
					i17dan = byDan[48] * 100 + byDan[49] * 10 + byDan[50];
					i18dan = byDan[51] * 100 + byDan[52] * 10 + byDan[53];
					i19dan = byDan[54] * 100 + byDan[55] * 10 + byDan[56];

				  	//ver1.01
				    i20dan = byDan[57] * 100 + byDan[58] * 10 + byDan[59];
					i21dan = byDan[60] * 100 + byDan[61] * 10 + byDan[62];
					i22dan = byDan[63] * 100 + byDan[64] * 10 + byDan[65];
					i23dan = byDan[66] * 100 + byDan[67] * 10 + byDan[68];
					i24dan = byDan[69] * 100 + byDan[70] * 10 + byDan[71];
					i25dan = byDan[72] * 100 + byDan[73] * 10 + byDan[74];


//					SaveDan(0);
					DanDisplay(byDanIndex+1);
//v1.10 20150715					CalcMinimumWeight();
					break;

				case ZERO_K:  /* zero grader */   //  단수 갯수 지우기
					if (!bScaleMode) { retry_message(); break; }
					command = command1 = ZERO_K;
					lKeyWait = 0;
					if (bModeCount)	// 카운트확인 모드 
					{
						switch (byCountIndex)
						{
							case 0:
								lTotalCount = 0;
								for(i=0; i<26; i++) lCount[i] = 0;
								////vputnum_5digit (lTotalCount);
								break;
							case 1:
								lCount[1] = 0;
								////vputnum_5digit(lCount[1]);
								break;
							case 2:
								lCount[2] = 0;
								////vputnum_5digit(lCount[2]);
								break;
							case 3:
								lCount[3] = 0;
								////vputnum_5digit(lCount[3]);
								break;
							case 4:
								lCount[4] = 0;
								////vputnum_5digit(lCount[4]);
								break;
							case 5:
								lCount[5] = 0;
								////vputnum_5digit(lCount[5]);
								break;
							case 6:
								lCount[6] = 0;
								/////vputnum_5digit(lCount[6]);
								break;
							case 7:
								lCount[7] = 0;
								///vputnum_5digit(lCount[7]);
								break;
							case 8:
								lCount[8] = 0;
								////vputnum_5digit(lCount[8]);
								break;
							case 9:
								lCount[9] = 0;
								////vputnum_5digit(lCount[9]);
								break;
							case 10:
								lCount[10] = 0;
								////vputnum_5digit(lCount[10]);
								break;
							case 11:
								lCount[11] = 0;
								////vputnum_5digit(lCount[11]);
								break;
							case 12:
								lCount[12] = 0;
								////vputnum_5digit(lCount[12]);
								break;
							case 13:
								lCount[13] = 0;
								////vputnum_5digit(lCount[13]);
								break;
							case 14:
								lCount[14] = 0;
								/////vputnum_5digit(lCount[14]);
								break;
							case 15:
								lCount[15] = 0;
								////vputnum_5digit(lCount[15]);
								break;
							case 16:
								lCount[16] = 0;
								////vputnum_5digit(lCount[16]);
								break;
							case 17:
								lCount[17] = 0;
								/////vputnum_5digit(lCount[17]);
								break;
							case 18:
								lCount[18] = 0;
								////vputnum_5digit(lCount[18]);
								break;
							case 19:
								lCount[19] = 0;
								////vputnum_5digit(lCount[19]);
								break;
						
						//VER 1.01
                           	case 20:
								lCount[20] = 0;
								////vputnum_5digit(lCount[20]);
								break;
							case 21:
								lCount[21] = 0;
								////vputnum_5digit(lCount[21]);
								break;
							case 22:
								lCount[22] = 0;
								////vputnum_5digit(lCount[22]);
								break;
							case 23:
								lCount[23] = 0;
								////vputnum_5digit(lCount[23]);
								break;
							case 24:
								lCount[24] = 0;
								////vputnum_5digit(lCount[24]);
								break;
						    case 25:
								lCount[25] = 0;
								////vputnum_5digit(lCount[25]);
								break;



						//VER 1.01

						
						
						
							default:
								break;
						}
						break;
					} // 카운트 확인 모드 
//					if (!bScaleMode) { retry_message(); break; }
			//		if (gnxTareStatus) { retry_message(); break; }
//kim				if (!LAMPST)
//kim					{ vputs("ERR 8"); retry_message(); delay(400); break;}

//ms/at				if (gxlGrossWeight <= zero_range )

      


					if ((((v_adc1_buf - v_adc_init + v_ei_multiply_factor/2) / v_ei_multiply_factor) * v_division) < limit)
					{
						v_adc_org =  read_filtered_adc();
						v_adc_org = (long) (((float) v_adc_org) * gnfFactor);
					}
					else { vputs("ERR 9"); retry_message(); delay(400); }
				
    

					break;
//2014.3.4
/*
				case STORE_K:
					if (bBlink) { retry_message(); break; }
					if (command == HUNDRED_K || command == TEN_K || command == ONE_K)
					{ retry_message(); WeighMode = 1; command = 0; break; }
					command = command1 = STORE_K;
					vputs("STORE");
					lKeyWait = 100;
					WeighMode = 0;
					break;
*/
				case COUNT_K: // 등급키    VER1.01
					lKeyWait = 0;
					bSetMode1 = 1;
					if(bSetMode == 1) bSetMode2 = 0;
					if (bBlink)
					{
						if (!byDanIndex) byDanIndex = DAN_MAX-1;
						else --byDanIndex;

///60ST					else	VOICE = 43;
						bBlink = 1;
						byBlinkCounter = (BLINK_COUNTER*2);	//2014.3.6 10;
						bModeCount = 0;
						WeighMode = 0;
						bScaleMode = 0;
						bGradeMode = 0;
						bSetMode = 1;
//						VOICE = byDanIndex+1;
						v_decimal_point = dpoint_bak;
						DanDisplay(byDanIndex+1);//7월25일 
//v1.10 20150715		CalcMinimumWeight();// ver 1.01
						WeighMode = 1;	//MTS
						break;
					}

//					if (bScaleMode) { retry_message(); break; }
					lKeyWait = 0;
					command = command1 = COUNT_K;
					if (!bModeCount)
						{	bModeCount = 1;
							byCountIndex = 0;
						}
					byCountIndex++;
//					byCountIndex %= (DAN_MAX+2);	//gxiIndex_End;
					switch (gxhCurrentType)
						{
							case 0:  byCountIndex %= (15); break; //  포도    15
							case 1:  byCountIndex %= (15); break; //  자두    15
							case 2:  byCountIndex %= (15); break; //  복숭아  15 -> 20단
							case 3:  byCountIndex %= (15); break; //  사과    15 -> 20
							case 4:  byCountIndex %= (15); break; //  감 : 15
							case 5:  byCountIndex %= (15); break;  // 배 : 15 ->20
						  case 6:  byCountIndex %= (25); break; //ver1.01,한라봉 
							case 7:  byCountIndex %= (20); break;    // 레드향 19 -> 20
							case 8:  byCountIndex %= (20); break ;    // 특작  19 -> 25
							case 9:  byCountIndex %= (20); break ;    // 단호박 20
	
							default: break;
						}
					bScaleMode = 0;
					v_decimal_point = 0;
					SIGNFLAG = 0;

					switch (byCountIndex)
					{
						case 0:
							lTotalCount = lCount[1] + lCount[2];
							gxiIndex_End = 26; //20
							switch(gxiIndex_End)
							{
								
								
								
								case 26:lTotalCount += lCount[25];
								case 25:lTotalCount += lCount[24];
								case 24:lTotalCount += lCount[23];
								case 23:lTotalCount += lCount[22];
								case 22:lTotalCount += lCount[21];
								case 21:lTotalCount += lCount[20];
								
								
								
								
								
								case 20:lTotalCount += lCount[19];
								case 19:lTotalCount += lCount[18];
								case 18:lTotalCount += lCount[17];
								case 17:lTotalCount += lCount[16];
								case 16:lTotalCount += lCount[15];
								case 15:lTotalCount += lCount[14];
								case 14:lTotalCount += lCount[13];
								case 13:lTotalCount += lCount[12];
								case 12:lTotalCount += lCount[11];
								case 11:lTotalCount += lCount[10];
								case 10:lTotalCount += lCount[9];
								case 9:	lTotalCount += lCount[8];
								case 8:	lTotalCount += lCount[7];
								case 7:	lTotalCount += lCount[6];
								case 6:	lTotalCount += lCount[5];
								case 5:	lTotalCount += lCount[4];
								case 4:	lTotalCount += lCount[3];
								case 3:
								default:  ;
							}


		//				vputnum_5digit (lTotalCount);  vputgrade2digit(0);  ///// DISPBUF[5]=0X00;  DISPBUF[6]=0X78;   //vputgrade2digit('T');  //vputgrade2digit('T'); //KIM
							break;
						case 1: vputnum_5digit(lCount[1]); vputgrade2digit(1);
							break;
						case 2: vputnum_5digit(lCount[2]); vputgrade2digit(2);
							break;
						case 3: vputnum_5digit(lCount[3]); vputgrade2digit(3);
							break;
						case 4: vputnum_5digit(lCount[4]); vputgrade2digit(4);
							break;
						case 5: vputnum_5digit(lCount[5]); vputgrade2digit(5);
							break;
						case 6: vputnum_5digit(lCount[6]); vputgrade2digit(6);
							break;
						case 7:	vputnum_5digit(lCount[7]); vputgrade2digit(7);
							break;
						case 8:	vputnum_5digit(lCount[8]); vputgrade2digit(8);
							break;
						case 9:	vputnum_5digit(lCount[9]); vputgrade2digit(9);
							break;
						case 10: vputnum_5digit(lCount[10]); vputgrade2digit(10);
							break;
						case 11: vputnum_5digit(lCount[11]); vputgrade2digit(11);
							break;
						case 12: vputnum_5digit(lCount[12]); vputgrade2digit(12);
							break;
						case 13: vputnum_5digit(lCount[13]); vputgrade2digit(13);
							break;
						case 14: vputnum_5digit(lCount[14]); vputgrade2digit(14);
							break;
						case 15: vputnum_5digit(lCount[15]); vputgrade2digit(15);
							break;
						case 16: vputnum_5digit(lCount[16]); vputgrade2digit(16);
							break;
						case 17: vputnum_5digit(lCount[17]); vputgrade2digit(17);
							break;
						case 18: vputnum_5digit(lCount[18]); vputgrade2digit(18);
							break;
						case 19: vputnum_5digit(lCount[19]); vputgrade2digit(19);
							break;
					   
					    case 20: vputnum_5digit(lCount[20]); vputgrade2digit(20);
							break;
					   
					  	case 21: vputnum_5digit(lCount[21]); vputgrade2digit(21);
							break;
					   
					    case 22: vputnum_5digit(lCount[22]); vputgrade2digit(22);
							break;
					    case 23: vputnum_5digit(lCount[23]); vputgrade2digit(23);
							break;
					   
					    case 24: vputnum_5digit(lCount[24]); vputgrade2digit(24);
							break;
					   case 25: vputnum_5digit(lCount[25]); vputgrade2digit(25);
							break;
					   
					
						default:
							break;
					}
					break;
				case WEIGH_K:  /* batch stop - number 9 */ //0329
					if (command == GRAPE_SELECT) recall_dan(0);
					if (command == JADOO_SELECT) recall_dan(1);
//					if (command == JADO2_K) recall_dan(2);
					if (command == PEACH_SELECT) recall_dan(2);
					if (command == APPLE_SELECT) recall_dan(3);
					if (command == GAM_SELECT) recall_dan(4);
					if (command == PEAR_SELECT) recall_dan(5);

					if (command == HALABONG_SELECT) recall_dan(6);
					if (command == REDHAYNG_SELECT) recall_dan(7);
					if (command == TEUKJAK_SELECT) recall_dan(8);
				  if(command == PUMKIN_SELECT) recall_dan(9);

					if (   command == GRAPE_SELECT || command == JADOO_SELECT && PEACH_SELECT
						|| command == APPLE_SELECT || command == GAM_SELECT || command == PEAR_SELECT
						|| command == HALABONG_SELECT || command == REDHAYNG_SELECT || command == TEUKJAK_SELECT || command == PUMKIN_SELECT)
					{
					 /*
						byDan[ 0]=i1dan/100;byDan[ 1]=i1dan%100;byDan[ 1]/=10;byDan[ 2]=i1dan%10;
						byDan[ 3]=i2dan/100;byDan[ 4]=i2dan%100;byDan[ 4]/=10;byDan[ 5]=i2dan%10;
						byDan[ 6]=i3dan/100;byDan[ 7]=i3dan%100;byDan[ 7]/=10;byDan[ 8]=i3dan%10;
						byDan[ 9]=i4dan/100;byDan[10]=i4dan%100;byDan[10]/=10;byDan[11]=i4dan%10;
						byDan[12]=i5dan/100;byDan[13]=i5dan%100;byDan[13]/=10;byDan[14]=i5dan%10;
						byDan[15]=i6dan/100;byDan[16]=i6dan%100;byDan[16]/=10;byDan[17]=i6dan%10;
						byDan[18]=i7dan/100;byDan[19]=i7dan%100;byDan[19]/=10;byDan[20]=i7dan%10;
						byDan[21]=i8dan/100;byDan[22]=i8dan%100;byDan[22]/=10;byDan[23]=i8dan%10;
						byDan[24]=i9dan/100;byDan[25]=i9dan%100;byDan[25]/=10;byDan[26]=i9dan%10;
						byDan[27]=i10dan/100;byDan[28]=i10dan%100;byDan[28]/=10;byDan[29]=i10dan%10;
						byDan[30]=i11dan/100;byDan[31]=i11dan%100;byDan[31]/=10;byDan[ 2]=i11dan%10;
						byDan[33]=i12dan/100;byDan[34]=i12dan%100;byDan[34]/=10;byDan[ 5]=i12dan%10;
						byDan[36]=i13dan/100;byDan[37]=i13dan%100;byDan[37]/=10;byDan[ 8]=i13dan%10;
						byDan[39]=i14dan/100;byDan[40]=i14dan%100;byDan[40]/=10;byDan[11]=i14dan%10;
						byDan[42]=i15dan/100;byDan[43]=i15dan%100;byDan[43]/=10;byDan[14]=i15dan%10;
						byDan[45]=i16dan/100;byDan[46]=i16dan%100;byDan[46]/=10;byDan[17]=i16dan%10;
						byDan[48]=i17dan/100;byDan[49]=i17dan%100;byDan[49]/=10;byDan[50]=i17dan%10;
						byDan[51]=i18dan/100;byDan[52]=i18dan%100;byDan[52]/=10;byDan[53]=i18dan%10;
						byDan[54]=i19dan/100;byDan[55]=i19dan%100;byDan[55]/=10;byDan[56]=i19dan%10;


							//ver1.01
                     	byDan[57]=i20dan/100;byDan[58]=i20dan%100;byDan[58]/=10;byDan[59]=i20dan%10;
                        byDan[60]=i21dan/100;byDan[61]=i21dan%100;byDan[61]/=10;byDan[62]=i21dan%10;
	                    byDan[63]=i22dan/100;byDan[64]=i22dan%100;byDan[64]/=10;byDan[65]=i22dan%10;
                        byDan[66]=i23dan/100;byDan[67]=i23dan%100;byDan[67]/=10;byDan[68]=i23dan%10;
                        byDan[69]=i24dan/100;byDan[70]=i24dan%100;byDan[70]/=10;byDan[71]=i24dan%10;
                        byDan[72]=i25dan/100;byDan[73]=i25dan%100;byDan[73]/=10;byDan[74]=i25dan%10;

					  */
					  ByDan_Funciton();



					}
					command = command1 = WEIGH_K;
					lKeyWait = 0;
///					DISPBUF[6] = DISPBUF[7] = vfd_font['-'];
					if (!bScaleMode)
					{
				   		
						if(gxhWhichSorting)// 단수 선별이면
						{
							DANSU_LED_ON;
							POJANG_LED_OFF;
						}	
						else
					 	{
							DANSU_LED_OFF;
							POJANG_LED_ON;
						}	
							
						
/*
						VOICE(0);
						VOICE_SBT = 1;
//						delay(6);	//ver 2.00 add because aP8942A
						VoiceStop();
						VOICE_SBT = 0;

						VOICE(VMODEWEIGH); byVoiceOff = VOICEOFFDELAY;
*/
						Voice_Play(VMODEWEIGH); byVoiceOff = VOICEOFFDELAY;
						
					//	gxhFilter=10;
					}
					
					gxhFilter=45;
					v_decimal_point = dpoint_bak;
					end_flag = 0;
					bWorking = 0;
					bScaleMode = 1;
					bGradeMode = 0;
					bSetMode = 0;
					bSetMode1 = 0;
					bSetMode2 = 1;
					WeighMode = 1;
					bBlink = 0;
					bModeCount = 0;
					byCountIndex = 0;

				////	iSpeedMode = 0;		//botong speed

					break;
				case GRADE_K: /* batch start - number 6 */ // "선별"키 
//					vputnum_5digit(iMinimumWeight);
         //           vputs("TTTTT");
			//		delay(1000);
//vputs("     ");
					if (bSetMode1 == 1) { retry_message(); break;}
					
				//	gxhFilter=10;//  
					lKeyWait = 0;
					recall_dan(gxhCurrentType);		//ver 1.02
					CalcMinimumWeight();
					//vputs("     ");
					////vputs("CCCCC"); Delay_ms(1000);
					hold_key_timer=0;
					sprintf(str, "C-TYPE=%d %d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d",gxhCurrentType, i1dan,i2dan,i3dan,i4dan,i5dan,i6dan,i7dan,i8dan,i9dan
		                                                          ,i10dan,i11dan,i12dan,i13dan,i14dan,i15dan  
	                                                            ,i16dan,i17dan,i18dan,i19dan,i20dan,i21dan
                                                              ,i22dan,i23dan,i24dan,i25dan);	USART1_puts(str);
 
					
				   /*
					if(gxhCurrentType==6)// 한라봉  ver 1.01
				 	DAN_MAX = 24;	/// kkkk 
					else if(gxhCurrentType==7 || gxhCurrentType==8)
					DAN_MAX = 18;	/// kkkk 
					else
					DAN_MAX = 14;	/// kkkk 
				*/	
				// 0326	 
				
					gxhDanSetType =  2;

					if ((i = iDanCheck()) != 0)//  작은단이 큰단보다 크게 입력 되어 있으면 에러다 
					{	
						
						//20180502
						
						
						
						retry_message();
						Voice_Play(173);//에러가 발생했습니다.
            Delay_ms(3000);
	         Voice_Play(i+21);//몇단
           Delay_ms(1000);
          Voice_Play(174);//등급설정을 다시해주세요
         Delay_ms(3000);
	
					//	Delay_ms(1000);
						vputnum_5digit(i);Delay_ms(3000); // 
						
						byDanIndex = i - 10 + 1;
						bScaleMode = 0;
						bBlink = 1;
						switch(gxhCurrentType)
						{
							case 0:	command = command1 = GRAPE_SELECT; break;
							case 1:	command = command1 = JADOO_SELECT; break;
							case 2:	command = command1 = TEUKJAK_SELECT; break;
							case 3:	command = command1 = PEAR_SELECT; break;
							case 4:	command = command1 = APPLE_SELECT; break;
							case 5:	command = command1 = PEACH_SELECT; break;
							case 6:	command = command1 = HALABONG_SELECT; break;
							case 7:	command = command1 = REDHAYNG_SELECT; break;
							case 8:	command = command1 = GAM_SELECT; break;
							case 9:	command = command1 = PUMKIN_SELECT; break;
							default:
									break;
						}
						break;
					}
					command = command1 = GRADE_K;

					if (bScaleMode || bSetMode)
					{
//						VOICE = 0;
//						T1 = 1;
//						delay(6);	//ver 2.00 add because aP8942A
//						T1 = 0;
//						VOICE = VMODEGRADE ;
//						byVoiceOff = VOICEOFFDELAY;
						dpoint_bak = v_decimal_point;
					}
					v_decimal_point = 0;
					bBlink = 0;
					lPreRef = gxlWeight;
					//lPreRef =gxlGrossWeight_1;
					bPreSignPlus = bNetSignPlus;
					bScaleMode = 0;
					lOneWeight = 0;
					bModeCount = 0;
					bSetMode = 0;
					bSetMode1 = 1;
					bSetMode2 = 1;
					bGradeMode = 1;
					WeighMode = 1;		//Ver 1.01
					SIGNFLAG = 0;		//ver 1.00
					vputnum_5digit(lOneWeight);

/**
					VOICE(0);
					VOICE_SBT = 1;
//					delay(6);	//ver 2.00 add because aP8942A
					VoiceStop();
					VOICE_SBT = 0;
					byVoiceOff = VOICEOFFDELAY;
***/
					switch(gxhCurrentType)// 0325
					{
						case 0:
//								VOICE(VOICE_TYPE1);   delay(400); VOICE(0);delay(200);
//								VoiceStop();	//add ver 2.01-2
								Voice_Play(VOICE_TYPE1); //delay(400); delay(200); delay(400); delay(200); delay(400); delay(200);
								Delay_ms(1800); 

								if     (gxhWeightGrapeType == 1)
								{	Voice_Play(VOICE_P4); /*VOICE(VOICE_P4);*/      Delay_ms(2000); /*VOICE(0);*/ }
								else if(gxhWeightGrapeType == 2)
								{	Voice_Play(VOICE_P5); /*VOICE(VOICE_P5);*/      Delay_ms(2000); /*VOICE(0);*/ }
								else if(gxhWeightGrapeType == 3)
								{	Voice_Play(VOICE_P10); /*VOICE(VOICE_P10);*/      Delay_ms(2000); /*VOICE(0);*/ }
								
								if(gxhWhichSorting)//  포장갯수
									Voice_Play(7);
                 else
									 Voice_Play(6);
								
								break;
						case 1:
/**
								VOICE(VOICE_TYPE2);   delay(400); VOICE(0);delay(200);
								VoiceStop();	//add ver 2.01-2
								VOICE(VOICE_P10);     delay(200); VOICE(0);
**/
								Voice_Play(VOICE_TYPE2); Delay_ms(1800);  // delay(400); delay(200); delay(400); delay(200); delay(400); delay(200);
								Voice_Play(VOICE_P10);     Delay_ms(2000);
								if(gxhWhichSorting)//  포장갯수
									Voice_Play(7);
                 else
									 Voice_Play(6);
	
								break;
						case 3:
						    	Voice_Play(VOICE_TYPE4);  Delay_ms(1800);  //delay(400); delay(200); delay(400); delay(200); delay(400); delay(200);
									if     (gxhWeightAppleType == 1)
									  {	Voice_Play(VOICE_P5);      Delay_ms(2000); }
									else if(gxhWeightAppleType == 2)
									  {	Voice_Play(VOICE_P10);      Delay_ms(2000); }
									else if(gxhWeightAppleType == 3)
									  {	Voice_Play(VOICE_P15);     Delay_ms(2000); }
								if(gxhWhichSorting)//  포장갯수
									Voice_Play(7);
                 else
									 Voice_Play(6);
	
						     /*
						      	Voice_Play(VOICE_TYPE3);   delay(500); delay(200); delay(400); delay(200); delay(400); delay(200);
									if     (gxhWeightPeackType == 1)
									  {	Voice_Play(VOICE_P45);      delay(200); }
									else if(gxhWeightPeackType == 2)
									  {	Voice_Play(VOICE_P5);      delay(200); }
									else if(gxhWeightPeackType == 3)
									  {	Voice_Play(VOICE_P10);     delay(200); }
									else if(gxhWeightPeackType == 4)
									  {	Voice_Play(VOICE_P15);     delay(200); }
							   */
/**
								VOICE(VOICE_TYPE3);   delay(300); VOICE(0);delay(200);
								VoiceStop();	//add ver 2.01-2
								VOICE(VOICE_P10);     delay(200); VOICE(0);
**/								/*
								Voice_Play(VOICE_TYPE5);   delay(400); delay(200); delay(400); delay(200); delay(400); delay(200);
								Voice_Play(VOICE_P10);     delay(200);
								*/
								break;
						case 2:
/**
								VOICE(VOICE_TYPE4);   delay(100); VOICE(0);delay(200);
								VoiceStop();	//add ver 2.01-2
**/								/*
								Voice_Play(VOICE_TYPE6);   delay(400); delay(200); delay(400); delay(200); delay(400); delay(200);
								if     (gxhWeightPearType == 1)
//2014.3.5 ?????				{	Voice_Play(VOICE_P7);      delay(200);}
								{	Voice_Play(VOICE_P75);      delay(200);}
								else if(gxhWeightPearType == 2)
								{	Voice_Play(VOICE_P15);     delay(200);}
								*/
								 
								Voice_Play(VOICE_TYPE3);  Delay_ms(1900);  //delay(500); delay(200); delay(400); delay(200); delay(400); delay(200);
									if     (gxhWeightPeackType == 1)
									  {	Voice_Play(VOICE_P45);      Delay_ms(2000); }
									else if(gxhWeightPeackType == 2)
									  {	Voice_Play(VOICE_P5);      Delay_ms(2000); }
									else if(gxhWeightPeackType == 3)
									  {	Voice_Play(VOICE_P10);     Delay_ms(2000); }
									else if(gxhWeightPeackType == 4)
									  {	Voice_Play(VOICE_P15);     Delay_ms(2000); }
										if(gxhWhichSorting)//  포장갯수
									Voice_Play(7);
                 else
									 Voice_Play(6);
	
								
								

							//		Voice_Play(VOICE_TYPE5);   delay(400); delay(200); delay(400); delay(200); delay(400); delay(200);
							//	Voice_Play(VOICE_P10);     delay(200);
							
								break;
						case 4:

						   	    Voice_Play(VOICE_TYPE5); Delay_ms(1900);   //delay(400); delay(200); delay(400); delay(200); delay(400); delay(200);
								Voice_Play(VOICE_P10);     Delay_ms(2000);
						if(gxhWhichSorting)//  포장갯수
									Voice_Play(7);
                 else
									 Voice_Play(6);
	
					
/**
                             			Voice_Play(VOICE_TYPE2);   delay(400); delay(200); delay(400); delay(200); delay(400); delay(200);
								Voice_Play(VOICE_P10);     delay(200);
					
								VOICE(VOICE_TYPE5);   delay(400); VOICE(0);delay(200);
								VoiceStop();	//add ver 2.01-2
									if     (gxhWeightAppleType == 1)
									  {	VOICE(VOICE_P5);      delay(200); VOICE(0); }
									else if(gxhWeightAppleType == 2)
									  {	VOICE(VOICE_P10);      delay(200); VOICE(0); }
									else if(gxhWeightAppleType == 3)
									  {	VOICE(VOICE_P15);     delay(200); VOICE(0); }
***/							/*
								Voice_Play(VOICE_TYPE4);   delay(400); delay(200); delay(400); delay(200); delay(400); delay(200);
									if     (gxhWeightAppleType == 1)
									  {	Voice_Play(VOICE_P5);      delay(200); }
									else if(gxhWeightAppleType == 2)
									  {	Voice_Play(VOICE_P10);      delay(200); }
									else if(gxhWeightAppleType == 3)
									  {	Voice_Play(VOICE_P15);     delay(200); }
									  */
								break;
						case 5:

						     	Voice_Play(VOICE_TYPE6);  Delay_ms(1900);  //delay(400); delay(200); delay(400); delay(200); delay(400); delay(200);
								if     (gxhWeightPearType == 1)
//2014.3.5 ?????				{	Voice_Play(VOICE_P7);      delay(2000);}
								{	Voice_Play(VOICE_P75);      Delay_ms(2000);}
								else if(gxhWeightPearType == 2)
								{	Voice_Play(VOICE_P15);     Delay_ms(2000);}
								if(gxhWhichSorting)//  포장갯수
									Voice_Play(7);
                 else
									 Voice_Play(6);
	

/**
								VOICE(VOICE_TYPE6);   delay(500); VOICE(0);delay(200);
								VoiceStop();	//add ver 2.01-2
									if     (gxhWeightPeackType == 1)
									  {	VOICE(VOICE_P45);      delay(200); VOICE(0); }
									else if(gxhWeightPeackType == 2)
									  {	VOICE(VOICE_P5);      delay(200); VOICE(0); }
									else if(gxhWeightPeackType == 3)
									  {	VOICE(VOICE_P10);     delay(200); VOICE(0); }
									else if(gxhWeightPeackType == 4)
									  {	VOICE(VOICE_P15);     delay(200); VOICE(0); }
***/
//								Voice_Play(VOICE_TYPE6);   delay(500); delay(200); delay(400); delay(200); delay(400); delay(200);
							 /*
							
								Voice_Play(VOICE_TYPE3);   delay(500); delay(200); delay(400); delay(200); delay(400); delay(200);
									if     (gxhWeightPeackType == 1)
									  {	Voice_Play(VOICE_P45);      delay(200); }
									else if(gxhWeightPeackType == 2)
									  {	Voice_Play(VOICE_P5);      delay(200); }
									else if(gxhWeightPeackType == 3)
									  {	Voice_Play(VOICE_P10);     delay(200); }
									else if(gxhWeightPeackType == 4)
									  {	Voice_Play(VOICE_P15);     delay(200); }
							   */
							
								break;

						case 6:
								Voice_Play(VOICE_TYPE7); Delay_ms(1800);   //delay(400); delay(200); delay(400); delay(200); delay(400); delay(200);delay(400); delay(200);
								Voice_Play(VOICE_P3);     Delay_ms(2000);
						if(gxhWhichSorting)//  포장갯수
									Voice_Play(7);
                 else
									 Voice_Play(6);
	
								break;

						case 7:
								Voice_Play(VOICE_TYPE8); Delay_ms(1800);  // delay(400); delay(200); delay(400); delay(200); delay(400); delay(200); delay(400); delay(200);
								Voice_Play(VOICE_P3);     Delay_ms(2000);
						if(gxhWhichSorting)//  포장갯수
									Voice_Play(7);
                 else
									 Voice_Play(6);
	
								break;

						case 8:  // 특작
						
           // 	gxhWeightSpecialType=1;  //  1 :  특작 3 kg , 2: 특작 melon   3: 특작 대봉,  4: 특작 파프니카
	            if(gxhWeightSpecialType==1) //특작
							{
								Voice_Play(20); Delay_ms(1800); //특작
								Voice_Play(23); Delay_ms(1800); //3  kg  v포장
								
							}	
							
							 if(gxhWeightSpecialType==2) //특작 메론
							{
								Voice_Play(175); Delay_ms(1800); //특작메론
								Voice_Play(23); Delay_ms(1800); //3  kg  v포장
								
							}
							if(gxhWeightSpecialType==3) //특작 대봉
							{
								Voice_Play(176); Delay_ms(1800); //특작대봉
								Voice_Play(28); Delay_ms(1800); //10  kg  v포장
								
							}
							if(gxhWeightSpecialType==4) //특작 파프리카
							{
								Voice_Play(177); Delay_ms(1800); //특작파프리카
								Voice_Play(29); Delay_ms(1800); //15  kg  v포장
								
							}
							
						//Voice_Play(VOICE_TYPE9); Delay_ms(1800);  // delay(400); delay(200); delay(400); delay(200); delay(400); delay(200); delay(400); delay(200);
								//Voice_Play(VOICE_P3);     Delay_ms(2000);
						
						
						if(gxhWhichSorting)//  포장갯수
									Voice_Play(7);
                 else
									 Voice_Play(6);
	
								break;

						case 9://단호박 
								//Voice_Play(VOICE_TYPE9); Delay_ms(1800);  // delay(400); delay(200); delay(400); delay(200); delay(400); delay(200); delay(400); delay(200);
						
            Voice_Play(16);     Delay_ms(2000); //단호박
						Voice_Play(VOICE_P3);     Delay_ms(2000);
						   
						
						if(gxhWhichSorting)//  포장갯수
									Voice_Play(7);
                 else
									 Voice_Play(6);
	
								break;

						
						
						default: break;
					}
//					vputs("TYPE ");
//					vputgrade2digit(gxhCurrentType+1);
//					delay(1500);
//					delay(500);
//					delay(100);
//					vputs("     ");
//					delay(100);
					switch (byDisplayMode)
					{
						case 0: v_decimal_point = 0; break;
						case 1:
						case 2: v_decimal_point = dpoint_bak; break;
					}
					vputnum_5digit(0);
//					vputnum_5digit((long)iMinimumWeight);
					break;
   	
#ifdef dpdpe 20180327

						case RIGHT_K:
//					if (bSetMode1 == 1) { retry_message(); break;}
					lKeyWait = 0;
					command = command1 = RIGHT_K;
					if (bBlink)
					{
						if (!bScaleMode)
						{
/**
							VOICE(0);
							VOICE_SBT = 1;
							delay(6);	//ver 2.00 add because aP8942A
							VOICE_SBT = 0;
							VOICE(VMODEWEIGH);
**/
							Voice_Play(VMODEWEIGH);

							byVoiceOff = VOICEOFFDELAY;
							command = command1 = 0;
						}
						v_decimal_point = dpoint_bak;
						end_flag = 0;
						bWorking = 0;
						bScaleMode = 1;
						bGradeMode = 0;
						bSetMode = 0;
						bSetMode1 = 0;
						WeighMode = 1;
						bBlink = 0;
						bModeCount = 0;
						byCountIndex = 0;
						DanSetting();			//ver 1.02
						SaveDan(gxhCurrentType);
						CalcMinimumWeight();
					}
					break;

	#endif				
				
				case	SULJUNG_K: //0327
				  if (command == WEIGHING_MODE)
			//	if (bScaleMode)//  only 무게 확인 모드일때
					{
						timeb = 0;
						do
						{
							CHKEYIN();
							if (timeb > (CHECK_KEY_TIMER*15000)) 
							{
               suljung_flag=1; // 설정모드로 정상적으로 진입								
							break;
							}
								else timeb++;
						} while (!qKEYIN);
						
						if(suljung_flag)// 정상적으로 설정모드로 진입했으면 
						{	
						 vputs("CCCCC");
						 suljung_flag=0;
						
						 switch(gxhCurrentType)
						 {
							 case 0:
								 goto grape_suljung;
							   break;
						
							 case 1:
								 goto jadoo_suljung;
							   break;
							 case 2:
								 goto peach_suljung;
							   break;
							 case 3:
								 goto apple_suljung;
							   break;
							 case 4:
								 goto gam_suljung;
							   break;
							 case 5:
								 goto pear_suljung;
							   break;
							 case 6:
								 goto halrabong_suljung;
							   break;
							 case 7:
								 goto red_suljung;
							   break;
						   case 8:
								 goto special_suljung;
							   break;
							 case 9:
								 goto pumpkin_suljung;
							   break;
							 
							 
							 
							 default: break;
						 } //of switch
						}//if(suljung_flag)// 정상적으로 설정모드로 진입했으면 
						
				//		else // 키를 잠까 눌렀다 
						{
				if (bBlink)
					{
						if (!bScaleMode)
						{
/**
							VOICE(0);
							VOICE_SBT = 1;
							delay(6);	//ver 2.00 add because aP8942A
							VOICE_SBT = 0;
							VOICE(VMODEWEIGH);
**/
							Voice_Play(VMODEWEIGH);

							byVoiceOff = VOICEOFFDELAY;
							command = command1 = 0;
						}
						v_decimal_point = dpoint_bak;
						end_flag = 0;
						bWorking = 0;
						bScaleMode = 1;
						bGradeMode = 0;
						bSetMode = 0;
						bSetMode1 = 0;
						WeighMode = 1;
						bBlink = 0;
						bModeCount = 0;
						byCountIndex = 0;
						DanSetting();			//ver 1.02
						SaveDan(gxhCurrentType);
						CalcMinimumWeight();
					}
							
							
							
						}	
						
						
						
						
					}	
					else
						retry_message();
				 break;
					
					




					
			case PUMKIN_TYPE://0328

				pumpkin_suljung:
				  DAN_MAX = 16;
					Voice_Play(VOICE_SET);
					beep(30); Delay_ms(20);

					gxhCurrentType = 9;
					recall_dan(gxhCurrentType);
				  ByDan_Funciton();
					i17dan = 0;
					command = command1 = PUMKIN_TYPE;
					bBlink = 1;
					byBlinkCounter = (BLINK_COUNTER*2);	//2014.3.6 10;
					bModeCount = 0;
					WeighMode = 0;
					bScaleMode = 0;
					bGradeMode = 0;
					bSetMode = 1;
					bSetMode1 = 0;
					bSetMode2 = 0;
					v_decimal_point = dpoint_bak;

					SIGNFLAG = 0;	//blank when minus
					byDanIndex = DAN_MAX-1;
					
					
					
					
					DanDisplay(byDanIndex+1);
					//while(1);
					CalcMinimumWeight();//ver 1.01

					DAN_MAX = 16;	//v1.10 20150715
					WeighMode = 1;	//MTS
					break;
				
					
					
					
					
				case GRAPE_TYPE://0328

				grape_suljung:
				  DAN_MAX = 14;
					Voice_Play(VOICE_SET);
					beep(30); Delay_ms(20);

					gxhCurrentType = 0;
					recall_dan(gxhCurrentType);
				  ByDan_Funciton();
					i15dan = 0;
					command = command1 = GRAPE_TYPE;
					bBlink = 1;
					byBlinkCounter = (BLINK_COUNTER*2);	//2014.3.6 10;
					bModeCount = 0;
					WeighMode = 0;
					bScaleMode = 0;
					bGradeMode = 0;
					bSetMode = 1;
					bSetMode1 = 0;
					bSetMode2 = 0;
					v_decimal_point = dpoint_bak;

					SIGNFLAG = 0;	//blank when minus
					byDanIndex = DAN_MAX-1;
					
					
					
					
					DanDisplay(byDanIndex+1);
					//while(1);
					CalcMinimumWeight();//ver 1.01

					DAN_MAX = 14;	//v1.10 20150715
					WeighMode = 1;	//MTS
					break;
				case JADOO_TYPE:
					jadoo_suljung:
					DAN_MAX = 14;
/**
					VoiceStop();	//add ver 2.01-2
					VOICE(VOICE_SET);
					beep(30); delay(20);
					VOICE(0);
**/
					Voice_Play(VOICE_SET);
					beep(30); delay(20);

					gxhCurrentType = 1;
					recall_dan(gxhCurrentType);
				  ByDan_Funciton();
					command = command1 = JADOO_TYPE;
					i15dan = 0;
					bBlink = 1;
						byBlinkCounter = BLINK_COUNTER*2;	//2014.3.6 10;
					bModeCount = 0;
					WeighMode = 0;
					bScaleMode = 0;
					bGradeMode = 0;
					bSetMode = 1;
					bSetMode1 = 0;
					bSetMode2 = 0;
					v_decimal_point = dpoint_bak;

					SIGNFLAG = 0;	//blank when minus
					byDanIndex = DAN_MAX-1;
					DanDisplay(byDanIndex+1);
					CalcMinimumWeight();//ver1.01
					
					DAN_MAX = 14;	//v1.10 20150715
					WeighMode = 1;	//MTS
					break;
				case TEUKJAK_TYPE:   //특작 
					special_suljung:
					DAN_MAX = 18;
/**
//2014.2.5					VoiceStop();	//add ver 2.01-2
					VOICE(VOICE_SET);
					beep(30); delay(20);
					VOICE(0);
**/
					Voice_Play(VOICE_SET);
					beep(30); delay(20);

					gxhCurrentType = 8;
					recall_dan(gxhCurrentType);
				  ByDan_Funciton();
					i19dan = 0;
					command = command1 = TEUKJAK_TYPE;
					bBlink = 1;
						byBlinkCounter = BLINK_COUNTER*2;	//2014.3.6 10;
					bModeCount = 0;
					WeighMode = 0;
					bScaleMode = 0;
					bGradeMode = 0;
					bSetMode = 1;
					bSetMode1 = 0;
					bSetMode2 = 0;

					v_decimal_point = dpoint_bak;

					SIGNFLAG = 0;	//blank when minus
					byDanIndex = DAN_MAX-1;
					DanDisplay(byDanIndex+1);
					CalcMinimumWeight();//ver1.01
					
					DAN_MAX = 18;	//v1.10 20150715
					WeighMode = 1;	//MTS
					break;
				case PEAR_TYPE:
					pear_suljung:
					DAN_MAX = 14;
/**
//2014.2.5					VoiceStop();	//add ver 2.01-2
					VOICE(VOICE_SET);
					beep(30); delay(20);
					VOICE(0);
**/
					Voice_Play(VOICE_SET);
					beep(30); delay(20);

					gxhCurrentType = 5;
					recall_dan(gxhCurrentType);
				  ByDan_Funciton();
					command = command1 = PEAR_TYPE;
					i15dan = 0;
					bBlink = 1;
						byBlinkCounter = BLINK_COUNTER*2;	//2014.3.6 10;
					bModeCount = 0;
					WeighMode = 0;
					bScaleMode = 0;
					bGradeMode = 0;
					bSetMode = 1;
					bSetMode1 = 0;
					bSetMode2 = 0;
					v_decimal_point = dpoint_bak;

					SIGNFLAG = 0;	//blank when minus
					byDanIndex = DAN_MAX-1;
					DanDisplay(byDanIndex+1);
					CalcMinimumWeight();//ver1.01

					DAN_MAX = 14;	//v1.10 20150715
					WeighMode = 1;	//MTS
					break;
				case APPLE_TYPE:
					apple_suljung:
					DAN_MAX = 14;
/**
//2014.2.5					VoiceStop();	//add ver 2.01-2
					VOICE(VOICE_SET);
					beep(30); delay(20);
					VOICE(0);
**/
					Voice_Play(VOICE_SET);
					beep(30); delay(20);

					gxhCurrentType = 3;
					recall_dan(gxhCurrentType);
				  ByDan_Funciton();
					command = command1 = APPLE_TYPE;
					i15dan = 0;
					bBlink = 1;
						byBlinkCounter = BLINK_COUNTER*2;	//2014.3.6 10;
					bModeCount = 0;
					WeighMode = 0;
					bScaleMode = 0;
					bGradeMode = 0;
					bSetMode = 1;
					bSetMode1 = 0;
					bSetMode2 = 0;
					v_decimal_point = dpoint_bak;

					SIGNFLAG = 0;	//blank when minus
					byDanIndex = DAN_MAX-1;
					DanDisplay(byDanIndex+1);
					CalcMinimumWeight();//ver1.01

					DAN_MAX = 14;	//v1.10 20150715
					WeighMode = 1;	//MTS
					break;

				case PEACH_TYPE:
					peach_suljung:
					DAN_MAX = 14;
/**
//2014.2.5					VoiceStop();	//add ver 2.01-2
					VOICE(VOICE_SET);
					beep(30); delay(20);
					VOICE(0);
**/
					Voice_Play(VOICE_SET);
					beep(30); delay(20);

					gxhCurrentType = 2;
					recall_dan(gxhCurrentType);
				  ByDan_Funciton();
					command = command1 = PEACH_TYPE;
					i15dan = 0;
					bBlink = 1;
				    byBlinkCounter = BLINK_COUNTER*2;	//2014.3.6 10;
					bModeCount = 0;
					WeighMode = 0;
					bScaleMode = 0;
					bGradeMode = 0;
					bSetMode = 1;
					bSetMode1 = 0;
					bSetMode2 = 0;
					v_decimal_point = dpoint_bak;

					SIGNFLAG = 0;	//blank when minus
					byDanIndex = DAN_MAX-1;
				//	DanDisplay(byDanIndex+1);
					CalcMinimumWeight();//add ver1.10 20150715

					DAN_MAX = 14;	//v1.10 20150715
					WeighMode = 1;	//MTS
					break;
				case HALABONG_TYPE:   //한라봉 
					halrabong_suljung:
				   // DAN_MAX = 18;
				     DAN_MAX = 24;	//ver1.01
					 
					 
					 Voice_Play(VOICE_SET);
					 beep(30); delay(20);

					gxhCurrentType = 6;
					recall_dan(gxhCurrentType);
				  ByDan_Funciton();
					command = command1 = HALABONG_TYPE;
				//	i15dan = 0;
				//	i26dan = 0;
				
					bBlink = 1;
					byBlinkCounter = BLINK_COUNTER*2;
					bModeCount = 0;
					WeighMode = 0;
					bScaleMode = 0;
					bGradeMode = 0;
					bSetMode = 1;
					bSetMode1 = 0;
					bSetMode2 = 0;
					v_decimal_point = dpoint_bak;

					SIGNFLAG = 0;	//blank when minus
					byDanIndex = DAN_MAX-1;
					DanDisplay(byDanIndex+1);
					CalcMinimumWeight();//ver1.01

					DAN_MAX = 24;	//add v1.10 20150715
					WeighMode = 1;	//MTS
					break;
				case REDHAYNG_TYPE:  //레드향 
					red_suljung:
					DAN_MAX = 18;
					Voice_Play(VOICE_SET);
					beep(30); delay(20);

					gxhCurrentType = 7;
					recall_dan(gxhCurrentType);
				  ByDan_Funciton();
					command = command1 = REDHAYNG_TYPE;
				//	i15dan = 0;
			    	i19dan = 0;
				
					bBlink = 1;
						byBlinkCounter = BLINK_COUNTER*2;	//2014.3.6 10;
					bModeCount = 0;
					WeighMode = 0;
					bScaleMode = 0;
					bGradeMode = 0;
					bSetMode = 1;
					bSetMode1 = 0;
					bSetMode2 = 0;
					v_decimal_point = dpoint_bak;

					SIGNFLAG = 0;	//blank when minus
					byDanIndex = DAN_MAX-1;
					DanDisplay(byDanIndex+1);
					CalcMinimumWeight();//ver1.01

					DAN_MAX = 18;	//add v1.10 20150715
					WeighMode = 1;	//MTS
					break;
				case GAM_TYPE:	  //0725
					gam_suljung:
					DAN_MAX = 14;
					Voice_Play(VOICE_SET);
					beep(30); delay(20);

					gxhCurrentType = 4;
					recall_dan(gxhCurrentType);
				  ByDan_Funciton();
					command = command1 = GAM_TYPE;
					i15dan = 0;
					bBlink = 1;
						byBlinkCounter = BLINK_COUNTER*2;	//2014.3.6 10;
					bModeCount = 0;
					WeighMode = 0;
					bScaleMode = 0;
					bGradeMode = 0;
					bSetMode = 1;
					bSetMode1 = 0;
					bSetMode2 = 0;
					v_decimal_point = dpoint_bak;

					SIGNFLAG = 0;	//blank when minus
					byDanIndex = DAN_MAX-1;
					DanDisplay(byDanIndex+1);
					CalcMinimumWeight();//ver1.01

					DAN_MAX = 14;	//add v1.10 20150715
					WeighMode = 1;	//MTS
					break;


			   case  POWER_K: //0329
					 
				 timeb = 0;
						do
						{
							CHKEYIN();
							if (timeb > (CHECK_KEY_TIMER*15000)) 
							{
							 power_key_flag=1; // 키가 정상적으로 일정이상 눌려야지 꺼진다.	
            	break;
							}
								else timeb++;
						} while (!qKEYIN);
				 
					 if(!power_key_flag)
						  goto POWER_FAIL; 
						
         Voice_Play(VOICE_POWEROFF); Delay_ms(2500);		//전원이 꺼집니다.
				 Voice_Play(61); Delay_ms(2500);		//이용해 주셔서 감사합니다.
					 
					 
				 for(i=0;i<14;i++)
					{
					  Display_buffer[i] = 0x00;
					}
					for(;;)
					{
						 key_value=keypush();
						if (key_value)
						{
							if (key_value == POWER_K )
							{   beep(50);
								v_decimal_point = dpoint_bak;
								power_key_flag=0;
								return;
							}
						}
					}
					
					POWER_FAIL:
					break;


        case WHICH_FRUIT_K:  // 0326  과일종류선택키  
					 //순선   포도 -> 자두-> 복숭아->한라봉->사과->감->배->레드향->단호박-> 특작1
				 		 if (bScaleMode)//  only 무게 확인 모드일때
						 {
					 fruit_select++;
           if(fruit_select>10) fruit_select=1;
            
				    if(fruit_select==1)
              goto grape_place;							
				    else if(fruit_select==2)
				      goto jadoo_place;
				    else if(fruit_select==3)
				      goto peach_place;
				 	  else if(fruit_select==4)
				      goto halabong_place;
				    else if(fruit_select==5)
				      goto apple_place;
				    else if(fruit_select==6)
				      goto gam_place;
				    else if(fruit_select==7)
				      goto pear_place;
				    else if(fruit_select==8)
				      goto redhyang_place;
				    else if (fruit_select==9)
				      goto special_place;
				    else //if(fruit_select==10)
				       goto pumpkin_place;
				  }
						 else
							 retry_message();
						
						
						//goto kwakwonho;
				
				     break;
				 
						
				case PUMKIN_SELECT://0329
					pumpkin_place:
				if (bGradeMode == 1 || bSetMode1 == 1) { retry_message(); break;}

							lTotalCount = 0;
							lCount[1] = lCount[2] = lCount[3] = 0;
							lCount[4] = lCount[5] = lCount[6] = 0;
							lCount[7] = lCount[8] = lCount[9] = 0;
							lCount[10] = lCount[11] = lCount[12] = 0;
							lCount[13] = lCount[14] = lCount[15] = 0;
							AllFruitLed_Off();
							PUMPKIN_LED_ON;

							DAN_MAX = 16;
							gxhDanSetType =  2;
/***
							VoiceStop();	//add ver 2.01-2
							VOICE(VOICE_TYPE1); delay(10); VOICE(0);
***/
							//Voice_Play(VOICE_TYPE1); delay(10);
				      Voice_Reset();
							Voice_Play(16); //단호박
							gxhCurrentType = 9;
							recall_dan(gxhCurrentType);
							Pojang_Display();
							CalcMinimumWeight();
							vputs("TYPE ");
							vputgrade2digit(gxhCurrentType+1);
							
							gxhCurrentType = EE_Write_val(ADR_gxhCurrentType, gxhCurrentType, 4);	//add 2016.2.23
							beep(50); Delay_ms(100);

             break;				
						
						

			   case  GRAPE_SELECT: //0326
					if (command == WEIGHING_MODE)
					{
						timeb = 0;
						do
						{
							CHKEYIN();
							if (timeb > CHECK_KEY_TIMER) break;
							else timeb++;
						} while (!qKEYIN);
						if (timeb > CHECK_KEY_TIMER)
						                                                                                                                                                                                  {
              grape_place: 
							
							if (bGradeMode == 1 || bSetMode1 == 1) { retry_message(); break;}

							lTotalCount = 0;
							lCount[1] = lCount[2] = lCount[3] = 0;
							lCount[4] = lCount[5] = lCount[6] = 0;
							lCount[7] = lCount[8] = lCount[9] = 0;
							lCount[10] = lCount[11] = lCount[12] = 0;
							lCount[13] = lCount[14] = lCount[15] = 0;
							AllFruitLed_Off();
							GRAPE_LED_ON;

							DAN_MAX = 14;
							gxhDanSetType =  2;
/***
							VoiceStop();	//add ver 2.01-2
							VOICE(VOICE_TYPE1); delay(10); VOICE(0);
***/          Voice_Reset();
							Voice_Play(VOICE_TYPE1); delay(10);
							gxhCurrentType = 0;
							recall_dan(gxhCurrentType);
							Pojang_Display();
							CalcMinimumWeight();
							vputs("TYPE ");
							vputgrade2digit(gxhCurrentType+1);
							
							//GRAPE_LED_ON;
							gxhCurrentType = EE_Write_val(ADR_gxhCurrentType, gxhCurrentType, 4);	//add 2016.2.23
							beep(50); Delay_ms(100);
							break;
						}
						else if (command1 != GRAPE_SELECT &&   command1 != JADOO_SELECT &&
								 command1 != TEUKJAK_SELECT &&   command1 != PEAR_SELECT &&
								 command1 != APPLE_SELECT &&   command1 != PEACH_SELECT &&
								 command1 != HALABONG_SELECT &&   command1 != REDHAYNG_SELECT &&
								 command1 != GAM_SELECT)
						{	retry_message();
							break;
						}
					}
			   case  JADOO_SELECT:
					if (command == WEIGHING_MODE)
					{
						timeb = 0;
						do
						{
							CHKEYIN();
							if (timeb > CHECK_KEY_TIMER) break;
							else timeb++;
						} while (!qKEYIN);
						if (timeb > CHECK_KEY_TIMER)
						{
							jadoo_place:
							if (bGradeMode == 1 || bSetMode1 == 1) { retry_message(); break;}

							lTotalCount = 0;
							lCount[1] = lCount[2] = lCount[3] = 0;
							lCount[4] = lCount[5] = lCount[6] = 0;
							lCount[7] = lCount[8] = lCount[9] = 0;
							lCount[10] = lCount[11] = lCount[12] = 0;
							lCount[13] = lCount[14] = lCount[15] = 0;

							DAN_MAX = 14;
							gxhDanSetType =  2;
							AllFruitLed_Off();
							JADOO_LED_ON;

						//	CALL_FN = FN_write_eep;
						//	write_eep(ADR_gxhDanSetType, u16gxhDanSetType);
						//	CALL_FN = FN_read_eep;
/***
							VoiceStop();	//add ver 2.01-2
							VOICE(VOICE_TYPE2); delay(10); VOICE(0);
***/          Voice_Reset();
							Voice_Play(VOICE_TYPE2); delay(10);
							gxhCurrentType = 1;
							recall_dan(gxhCurrentType);
							Pojang_Display();
							CalcMinimumWeight();
							vputs("TYPE ");
							vputgrade2digit(gxhCurrentType+1);
							//CALL_FN = FN_write_eep;
						//	write_eep(ADR_gxhCurrentType, u16gxhCurrentType);
							//CALL_FN = FN_read_eep;
							gxhCurrentType = EE_Write_val(ADR_gxhCurrentType, gxhCurrentType, 4);	//add 2016.2.23
							beep(50); Delay_ms(100);
							break;
						}
						else if 
							   (command1 != GRAPE_SELECT &&   command1 != JADOO_SELECT &&
								 command1 != TEUKJAK_SELECT &&   command1 != PEAR_SELECT &&
								 command1 != APPLE_SELECT &&   command1 != PEACH_SELECT &&
								 command1 != HALABONG_SELECT &&   command1 != REDHAYNG_SELECT &&
								 command1 != GAM_SELECT)
							   /*(command1 != GRAPE_K &&   command1 != JADOO_K &&
								 command1 != TEUKJAK_K &&   command1 != PEAK_K &&
								 command1 != APPLE_K &&   command1 != PEACH_K &&
								 command1 != HALABONG_K &&   command1 != REDHAYNG_K &&
								 command1 != GAM_K) */
						{	retry_message();
							break;
						}
					}
/*
			   case  JADO2_K:
					if (command == WEIGHING_MODE)
					{
						timeb = 0;
						do
						{
							CHKEYIN();
							if (timeb > 800) break;
							else timeb++;
						} while (!qKEYIN);
						if (timeb > 800)
						{
							if (bGradeMode == 1 || bSetMode1 == 1) { retry_message(); break;}

							lTotalCount = 0;
							lCount[1] = lCount[2] = lCount[3] = 0;
							lCount[4] = lCount[5] = lCount[6] = 0;
							lCount[7] = lCount[8] = lCount[9] = 0;
							lCount[10] = lCount[11] = lCount[12] = 0;
							lCount[13] = lCount[14] = lCount[15] = 0;

							DAN_MAX = 14;
							gxhDanSetType =  2;
							CALL_FN = FN_write_eep;
							write_eep(ADR_gxhDanSetType, u16gxhDanSetType);
							CALL_FN = FN_read_eep;
							VoiceStop();	//add ver 2.01-2
							VOICE = VOICE_TYPE3; delay(10); VOICE = 0;
							gxhCurrentType = 2;
							recall_dan(gxhCurrentType);
							CalcMinimumWeight();
							vputs("TYPE ");
							vputgrade2digit(gxhCurrentType+1);
							CALL_FN = FN_write_eep;
							write_eep(ADR_gxhCurrentType, u16gxhCurrentType);
							CALL_FN = FN_read_eep;
							beep(300); delay(1500);
							break;
						}
						else if (command1 != GRAPE_K &&   command1 != JADOO_K &&
								 command1 != TEUKJAK_K &&   command1 != PEAK_K &&
								 command1 != APPLE_K &&   command1 != PEACH_K)
						{	retry_message();
							break;
						}
					}
*/
			   case  PEAR_SELECT:
					if (command == WEIGHING_MODE)
					{
						timeb = 0;
						do
						{
							CHKEYIN();
							if (timeb >CHECK_KEY_TIMER) break;
							else timeb++;
						} while (!qKEYIN);
						if (timeb > CHECK_KEY_TIMER)
						{
							pear_place:
							if (bGradeMode == 1 || bSetMode1 == 1) { retry_message(); break;}

							lTotalCount = 0;
							lCount[1] = lCount[2] = lCount[3] = 0;
							lCount[4] = lCount[5] = lCount[6] = 0;
							lCount[7] = lCount[8] = lCount[9] = 0;
							lCount[10] = lCount[11] = lCount[12] = 0;
							lCount[13] = lCount[14] = lCount[15] = 0;

							DAN_MAX = 14;
							gxhDanSetType =  2;
							AllFruitLed_Off();
							PEAR_LED_ON;

							//CALL_FN = FN_write_eep;
						//	write_eep(ADR_gxhDanSetType, u16gxhDanSetType);
							//CALL_FN = FN_read_eep;
/***
							VoiceStop();	//add ver 2.01-2
							VOICE(VOICE_TYPE4); delay(10); VOICE(0);
***/
              Voice_Reset();
							Voice_Play(VOICE_TYPE6); delay(10);

//							gxhCurrentType = 3;
							gxhCurrentType = 5;
							recall_dan(gxhCurrentType);
							Pojang_Display();
							CalcMinimumWeight();
							vputs("TYPE ");
							vputgrade2digit(gxhCurrentType+1);
							//CALL_FN = FN_write_eep;
						//	write_eep(ADR_gxhCurrentType, u16gxhCurrentType);
							//CALL_FN = FN_read_eep;
							gxhCurrentType = EE_Write_val(ADR_gxhCurrentType, gxhCurrentType, 4);	//add 2016.2.23
							beep(50); Delay_ms(100);
							break;
						}
						else if
							(command1 != GRAPE_SELECT &&   command1 != JADOO_SELECT &&
								 command1 != TEUKJAK_SELECT &&   command1 != PEAR_SELECT &&
								 command1 != APPLE_SELECT &&   command1 != PEACH_SELECT &&
								 command1 != HALABONG_SELECT &&   command1 != REDHAYNG_SELECT &&
								 command1 != GAM_SELECT)
		             /*					
							   (command1 != GRAPE_K &&   command1 != JADOO_K &&
								 command1 != TEUKJAK_K &&   command1 != PEAK_K &&
								 command1 != APPLE_K &&   command1 != PEACH_K &&
								 command1 != HALABONG_K &&   command1 != REDHAYNG_K &&
								 command1 != GAM_K)*/
						{	retry_message();
							break;
						}
					}

			   case  APPLE_SELECT:
					if (command == WEIGHING_MODE)
					{
						timeb = 0;
						do
						{
							CHKEYIN();
							if (timeb > CHECK_KEY_TIMER) break;
							else timeb++;
						} while (!qKEYIN);
						if (timeb > CHECK_KEY_TIMER)
						{
							apple_place:
							if (bGradeMode == 1 || bSetMode1 == 1) { retry_message(); break;}

							lTotalCount = 0;
							lCount[1] = lCount[2] = lCount[3] = 0;
							lCount[4] = lCount[5] = lCount[6] = 0;
							lCount[7] = lCount[8] = lCount[9] = 0;
							lCount[10] = lCount[11] = lCount[12] = 0;
							lCount[13] = lCount[14] = lCount[15] = 0;

							DAN_MAX = 14;
							gxhDanSetType = 2;
							AllFruitLed_Off();
							APPLE_LED_ON;

//							CALL_FN = FN_write_eep;
						//	write_eep(ADR_gxhDanSetType, u16gxhDanSetType);
//							CALL_FN = FN_read_eep;
/***
							VoiceStop();	//add ver 2.01-2
							VOICE(VOICE_TYPE5); delay(10); VOICE(0);
***/
              Voice_Reset();
							Voice_Play(VOICE_TYPE4); delay(10);

//							gxhCurrentType = 4;
							gxhCurrentType = 3;
							recall_dan(gxhCurrentType);
							CalcMinimumWeight();
							vputs("TYPE ");
							vputgrade2digit(gxhCurrentType+1);
							Pojang_Display();
							gxhCurrentType = EE_Write_val(ADR_gxhCurrentType, gxhCurrentType, 4);	//add 2016.2.23
							
						//	CALL_FN = FN_write_eep;
						//	write_eep(ADR_gxhCurrentType, u16gxhCurrentType);
						//	CALL_FN = FN_read_eep;
							beep(50); Delay_ms(100);
							break;
						}
						else if 
							(command1 != GRAPE_SELECT &&   command1 != JADOO_SELECT &&
								 command1 != TEUKJAK_SELECT &&   command1 != PEAR_SELECT &&
								 command1 != APPLE_SELECT &&   command1 != PEACH_SELECT &&
								 command1 != HALABONG_SELECT &&   command1 != REDHAYNG_SELECT &&
								 command1 != GAM_SELECT)
		           
							/*						
						 (command1 != GRAPE_K &&   command1 != JADOO_K &&
								 command1 != TEUKJAK_K &&   command1 != PEAK_K &&
								 command1 != APPLE_K &&   command1 != PEACH_K &&
								 command1 != HALABONG_K &&   command1 != REDHAYNG_K &&
								 command1 != GAM_K) */
						{	retry_message();
							break;
						}

					}

			   	case  PEACH_SELECT:
					if (command == WEIGHING_MODE)
					{
						timeb = 0;
						do
						{
							CHKEYIN();
							if (timeb > CHECK_KEY_TIMER) break;
							else timeb++;
						} while (!qKEYIN);
						if (timeb > CHECK_KEY_TIMER)
						{
							peach_place:
							if (bGradeMode == 1 || bSetMode1 == 1) { retry_message(); break;}

							lTotalCount = 0;
							lCount[1] = lCount[2] = lCount[3] = 0;
							lCount[4] = lCount[5] = lCount[6] = 0;
							lCount[7] = lCount[8] = lCount[9] = 0;
							lCount[10] = lCount[11] = lCount[12] = 0;
							lCount[13] = lCount[14] = lCount[15] = 0;

							DAN_MAX = 14;
							gxhDanSetType =  2;
							AllFruitLed_Off();
							PEACH_LED_ON;

							//CALL_FN = FN_write_eep;
						//	write_eep(ADR_gxhDanSetType, u16gxhDanSetType);
							//CALL_FN = FN_read_eep;
/***
							VoiceStop();	//add ver 2.01-2
							VOICE(VOICE_TYPE6); delay(10); VOICE(0);
***/
              Voice_Reset();
							Voice_Play(VOICE_TYPE3); delay(10);
//							gxhCurrentType = 5;
							gxhCurrentType = 2;

							recall_dan(gxhCurrentType);
							CalcMinimumWeight();
							vputs("TYPE ");
							vputgrade2digit(gxhCurrentType+1);
							Pojang_Display();
							//CALL_FN = FN_write_eep;
						//	write_eep(ADR_gxhCurrentType, u16gxhCurrentType);
							//CALL_FN = FN_read_eep;
							gxhCurrentType = EE_Write_val(ADR_gxhCurrentType, gxhCurrentType, 4);	//add 2016.2.23
							beep(50); Delay_ms(100);
							break;
						}
						else if (command1 != GRAPE_SELECT &&   command1 != JADOO_SELECT &&
								 command1 != TEUKJAK_SELECT &&   command1 != PEAR_SELECT &&
								 command1 != APPLE_SELECT &&   command1 != PEACH_SELECT &&
								 command1 != HALABONG_SELECT &&   command1 != REDHAYNG_SELECT &&
								 command1 != GAM_SELECT)
		          
						{	retry_message();
							break;
						}
					}

				case HALABONG_SELECT:
					if (command == WEIGHING_MODE)
					{
						timeb = 0;
						do
						{
							CHKEYIN();
							if (timeb > CHECK_KEY_TIMER) break;
							else timeb++;
						} while (!qKEYIN);
						if (timeb > CHECK_KEY_TIMER)
						{
							halabong_place:
							if (bGradeMode == 1 || bSetMode1 == 1) { retry_message(); break;}

							lTotalCount = 0;
							lCount[1] = lCount[2] = lCount[3] = 0;
							lCount[4] = lCount[5] = lCount[6] = 0;
							lCount[7] = lCount[8] = lCount[9] = 0;
							lCount[10] = lCount[11] = lCount[12] = 0;
							lCount[13] = lCount[14] = lCount[15] = 0;
							lCount[16] = lCount[17] = lCount[18] = 0;
							lCount[19] = lCount[20] = lCount[21] = 0;
							lCount[22] = lCount[23] = lCount[24] = lCount[25]= 0;

							//DAN_MAX = 19;
							DAN_MAX = 25;
							AllFruitLed_Off();
							HALABONG_LED_ON;

						
						
						
							gxhDanSetType =  2;
							//CALL_FN = FN_write_eep;
						//	write_eep(ADR_gxhDanSetType, u16gxhDanSetType);
							//CALL_FN = FN_read_eep;
/***
							VoiceStop();	//add ver 2.01-2
							VOICE(VOICE_TYPE6); delay(10); VOICE(0);
***/
              Voice_Reset();
							Voice_Play(VOICE_TYPE7); delay(10);
//							gxhCurrentType = 7;
							gxhCurrentType = 6;

							recall_dan(gxhCurrentType);
							CalcMinimumWeight();
							vputs("TYPE ");
							vputgrade2digit(gxhCurrentType+1);
							Pojang_Display();
							//CALL_FN = FN_write_eep;
						//	write_eep(ADR_gxhCurrentType, u16 gxhCurrentType);
							//CALL_FN = FN_read_eep;
							gxhCurrentType = EE_Write_val(ADR_gxhCurrentType, gxhCurrentType, 4);	//add 2016.2.23
							beep(50); Delay_ms(100);
							break;
						}
						else if (command1 != GRAPE_SELECT &&   command1 != JADOO_SELECT &&
								 command1 != TEUKJAK_SELECT &&   command1 != PEAR_SELECT &&
								 command1 != APPLE_SELECT &&   command1 != PEACH_SELECT &&
								 command1 != HALABONG_SELECT &&   command1 != REDHAYNG_SELECT &&
								 command1 != GAM_SELECT)
		          
						{	retry_message();
							break;
						}
					}

				case GAM_SELECT:
					if (command == WEIGHING_MODE)
					{
						timeb = 0;
						do
						{
							CHKEYIN();
							if (timeb > CHECK_KEY_TIMER) break;
							else timeb++;
						} while (!qKEYIN);
						if (timeb > CHECK_KEY_TIMER)
						{
							gam_place:
							if (bGradeMode == 1 || bSetMode1 == 1) { retry_message(); break;}

							lTotalCount = 0;
							lCount[1] = lCount[2] = lCount[3] = 0;
							lCount[4] = lCount[5] = lCount[6] = 0;
							lCount[7] = lCount[8] = lCount[9] = 0;
							lCount[10] = lCount[11] = lCount[12] = 0;
							lCount[13] = lCount[14] = lCount[15] = 0;

							DAN_MAX = 14;
							gxhDanSetType =  2;
							AllFruitLed_Off();
							GAM_LED_ON;

							//CALL_FN = FN_write_eep;
						//	write_eep(ADR_gxhDanSetType, (u16)gxhDanSetType);
							//CALL_FN = FN_read_eep;
/***
							VoiceStop();	//add ver 2.01-2
							VOICE(VOICE_TYPE6); delay(10); VOICE(0);
***/
              Voice_Reset();
							Voice_Play(VOICE_TYPE5); delay(10);
//							gxhCurrentType = 5;
							gxhCurrentType =4;

							recall_dan(gxhCurrentType);
							CalcMinimumWeight();
							vputs("TYPE ");
							vputgrade2digit(gxhCurrentType+1);
							Pojang_Display();
							//CALL_FN = FN_write_eep;
						//	write_eep(ADR_gxhCurrentType, (u16)gxhCurrentType);
							//CALL_FN = FN_read_eep;
							gxhCurrentType = EE_Write_val(ADR_gxhCurrentType, gxhCurrentType, 4);	//add 2016.2.23
							beep(50); Delay_ms(100);
							break;
						}
						else if (command1 != GRAPE_SELECT &&   command1 != JADOO_SELECT &&
								 command1 != TEUKJAK_SELECT &&   command1 != PEAR_SELECT &&
								 command1 != APPLE_SELECT &&   command1 != PEACH_SELECT &&
								 command1 != HALABONG_SELECT &&   command1 != REDHAYNG_SELECT &&
								 command1 != GAM_SELECT)
		          
						{	retry_message();
							break;
						}
					}

				case REDHAYNG_SELECT:
					if (command == WEIGHING_MODE)
					{
						timeb = 0;
						do
						{
							CHKEYIN();
							if (timeb > CHECK_KEY_TIMER) break;
							else timeb++;
						} while (!qKEYIN);
						if (timeb > CHECK_KEY_TIMER)
						{ 
							redhyang_place:
							if (bGradeMode == 1 || bSetMode1 == 1) { retry_message(); break;}

							lTotalCount = 0;
							lCount[1] = lCount[2] = lCount[3] = 0;
							lCount[4] = lCount[5] = lCount[6] = 0;
							lCount[7] = lCount[8] = lCount[9] = 0;
							lCount[10] = lCount[11] = lCount[12] = 0;
							lCount[13] = lCount[14] = lCount[15] = 0;
							lCount[16] = lCount[17] = lCount[18] = lCount[19]= 0;

							DAN_MAX = 19;
							gxhDanSetType =  2;
							AllFruitLed_Off();
							RED_LED_ON;

							//CALL_FN = FN_write_eep;
						//	write_eep(ADR_gxhDanSetType, (u16)gxhDanSetType);
							//CALL_FN = FN_read_eep;
/***
							VoiceStop();	//add ver 2.01-2
							VOICE(VOICE_TYPE6); delay(10); VOICE(0);
***/
              Voice_Reset();
							Voice_Play(VOICE_TYPE8); delay(10);
//							gxhCurrentType = 8;
							gxhCurrentType = 7;

							recall_dan(gxhCurrentType);
							CalcMinimumWeight();
							vputs("TYPE ");
							vputgrade2digit(gxhCurrentType+1);
							Pojang_Display();
							gxhCurrentType = EE_Write_val(ADR_gxhCurrentType, gxhCurrentType, 4);	//add 2016.2.23
							
							//CALL_FN = FN_write_eep;
						//	write_eep(ADR_gxhCurrentType, (u16)gxhCurrentType);
							//CALL_FN = FN_read_eep;
							beep(50); Delay_ms(100);
							break;
						}
						else if (command1 != GRAPE_SELECT &&   command1 != JADOO_SELECT &&
								 command1 != TEUKJAK_SELECT &&   command1 != PEAR_SELECT &&
								 command1 != APPLE_SELECT &&   command1 != PEACH_SELECT &&
								 command1 != HALABONG_SELECT &&   command1 != REDHAYNG_SELECT &&
								 command1 != GAM_SELECT)
		          
						{	retry_message();
							break;
						}
					}

				case TEUKJAK_SELECT:
					if (command == WEIGHING_MODE)
					{
						timeb = 0;
						do
						{
							CHKEYIN();
							if (timeb > CHECK_KEY_TIMER) break;
							else timeb++;
						} while (!qKEYIN);
						if (timeb > CHECK_KEY_TIMER)
						{
							special_place:
							if (bGradeMode == 1 || bSetMode1 == 1) { retry_message(); break;}

							lTotalCount = 0;
							lCount[1] = lCount[2] = lCount[3] = 0;
							lCount[4] = lCount[5] = lCount[6] = 0;
							lCount[7] = lCount[8] = lCount[9] = 0;
							lCount[10] = lCount[11] = lCount[12] = 0;
							lCount[13] = lCount[14] = lCount[15] = 0;
							lCount[16] = lCount[17] = lCount[18] =lCount[19] = 0;

							DAN_MAX = 19;
							gxhDanSetType =  2;
							AllFruitLed_Off();
							SPECIAL_LED_ON;

							//CALL_FN = FN_write_eep;
						////	write_eep(ADR_gxhDanSetType, u16 gxhDanSetType);
							//CALL_FN = FN_read_eep;
/***
							VoiceStop();	//add ver 2.01-2
							VOICE(VOICE_TYPE6); delay(10); VOICE(0);
***/
              Voice_Reset();
							Voice_Play(VOICE_TYPE9); delay(10);
//							gxhCurrentType = 9;
							gxhCurrentType = 8;

							recall_dan(gxhCurrentType);
							CalcMinimumWeight();
							vputs("TYPE ");
							vputgrade2digit(gxhCurrentType+1);
							Pojang_Display();
							//CALL_FN = FN_write_eep;
						//	write_eep(ADR_gxhCurrentType, (u16gxhCurrentType);
							//CALL_FN = FN_read_eep;
							gxhCurrentType = EE_Write_val(ADR_gxhCurrentType, gxhCurrentType, 4);	//add 2016.2.23
							
							beep(50); Delay_ms(100);
							break;
						}
						else if (command1 != GRAPE_SELECT &&   command1 != JADOO_SELECT &&
								 command1 != TEUKJAK_SELECT &&   command1 != PEAR_SELECT &&
								 command1 != APPLE_SELECT &&   command1 != PEACH_SELECT &&
								 command1 != HALABONG_SELECT &&   command1 != REDHAYNG_SELECT &&
								 command1 != GAM_SELECT)
		          
						{	retry_message();
							break;
						}
					}

				case POJANG_K:	//0328
						if(bScaleMode)
						{							
					
					  switch(gxhCurrentType)
						{
							case 0: //  포도
							gxhWeightGrapeType++;
							if(gxhWeightGrapeType>3)
								gxhWeightGrapeType=1;
							gxhWeightGrapeType = EE_Write_val(ADR_gxhWeightGrapeType, gxhWeightGrapeType, 4);	  Delay_ms(10);
							break;
							
							case 2: //  복숭아
							gxhWeightPeackType++;
							if(gxhWeightPeackType>4)
								gxhWeightPeackType=1;
							gxhWeightGrapeType = EE_Write_val(ADR_gxhWeightGrapeType, gxhWeightGrapeType, 4);	  Delay_ms(10);
							
							break;
						  case 3: //  사과
							gxhWeightAppleType++;
							if(gxhWeightAppleType>3)
								gxhWeightAppleType=1;
							gxhWeightAppleType = EE_Write_val(ADR_gxhWeightAppleType, gxhWeightAppleType, 4);	  Delay_ms(10);
							
							break;
							
							case 5: //  배
							gxhWeightPearType++;
							if(gxhWeightPearType>2)
								gxhWeightPearType=1;
							gxhWeightPearType = EE_Write_val(ADR_gxhWeightPearType, gxhWeightPearType, 4);	  Delay_ms(10);
							
							break;
							
							case 8: //  특작
							gxhWeightSpecialType++;
							if(gxhWeightSpecialType>4)
								gxhWeightSpecialType=1;
							gxhWeightSpecialType = EE_Write_val(ADR_gxhWeightSpecialType, gxhWeightSpecialType, 4);	  Delay_ms(10);
                switch(gxhWeightSpecialType)
                {
									case 1:  //20180425
										Voice_Play(VOICE_TYPE9);// 특작
				            Delay_ms(2000);
				       	break;
									case 2:
										Voice_Play(175);// 특작 메론
				            Delay_ms(2000);
									break;
								 
									case 3:
										Voice_Play(176);// 특작 대봉
				            Delay_ms(2000);
									break;
									case 4:
										Voice_Play(177);// 특작 파프리카
				            Delay_ms(2000);
									break;
									
									default: break;



                }									
						


							break;
							//gxhWeightSpecialType
							
							
							case 1: case 4: case 6: case 7: case 9:
								retry_message();
							default: break;
						}	
						Current_Type_DanSetting();
						Pojang_Display_Voice();
					}
          else
          retry_message();						
  						
						break;
				case GRAED_SPEED_K:
					if (command == WEIGHING_MODE)
					{
						timeb = 0;
						do
						{
							CHKEYIN();
							if (timeb > CHECK_KEY_TIMER) break;
							else timeb++;
						} while (!qKEYIN);

						if (timeb > CHECK_KEY_TIMER)
						{
							if(iSpeedMode == 0)
							{
								iSpeedMode = 1;	//gosok
								Voice_Play(63); //delay(10);
								SPECIAL_LED_ON;
							////	LAMPSPEED = LED_ON;
							}
							else
							{
								iSpeedMode = 0;	//botong
								Voice_Play(64); //delay(10);
								SPECIAL_LED_OFF;
							 //   Voice_Play(BOJUNG_RELEASE); delay(10);
							////	LAMPSPEED = LED_OFF;
							}
						}
						else  retry_message();
						break;
					}

				default:
					command = 0;
					lKeyWait = 0;
					retry_message();
					break;

			}
			sprintf(str, "key_number=%2d",(int)key_number); 
  ////    USART1_puts(str);
		
		}	//of keypush
	


  /*
		
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
		  Display_Weight(gxlGrossWeight_1);	  //50.00
		  sprintf(str, "%9u",(long)gxlGrossWeight_1); 
		  USART1_puts(str);
	 }// of key timey  
	 
		*/
	
/************** weight processing*******************************************/

 if(adc_timer>=_80SPEED)// 0325
	 {
	     adc_timer=0;
 	     raw_data = read_filtered_adc();		// ad 값으 읽어온다ㅣ
	

		v_adc1_buf = (long)(gnfFactor * raw_data);
		diff1 = v_adc1_buf - v_adc_org;
		diff2 = v_adc1_buf - prev_adc1;	/* for stable */ 
		 
		 
		 
			/*---------- autozero processing ----------*/	 
	 if (!gnxTareStatus && gxhZTDigit)	/* ver 1.13 */
		 {
// ver 1.20	diff11 = (diff1+10)/20;
			diff11 = (diff1+v_ei_multiply_factor/2)/v_ei_multiply_factor;
			if (-(long)gxhZTDigit <= diff11 && diff11 <= (long)gxhZTDigit)
			{
				++hAutoZeroCounter;
				if (hAutoZeroCounter > az_ok)
				{
					hAutoZeroCounter = 0;
					v_adc_org = v_adc1_buf;
				}
			}
			else  hAutoZeroCounter = 0;
		}
		 
		gxlGrossWeight = ((diff1 + v_ei_multiply_factor/2) / v_ei_multiply_factor) * v_division;

		if (gxlGrossWeight < 0)
		{	gxlGrossWeight = gxlGrossWeight - v_division;
			bGrossSignPlus = 0;
		}
		else	bGrossSignPlus = 1; 
		
		
		if (bScaleMode)
		{
			/* +- 1 digit zeterring */
					  /* -1.5d */                          /* +1.5d */
//			if ((lPreWeight-30) <= diff1 && diff1 <= (lPreWeight+30))
			if ((lPreWeight-15) <= diff1 && diff1 <= (lPreWeight+15))
			{
				hUpdateCount++;
//				if (20 < hUpdateCount)
				if (2 < hUpdateCount)
				{
					if (gnxTareStatus)  gxlWeight = gxlGrossWeight - gnlTareWeight;
					else  gxlWeight = gxlGrossWeight;
					bNetSignPlus = 1;
					if (gxlWeight < 0)
					{	bNetSignPlus = 0;
						gxlWeight = -1 * gxlWeight;
					}
					lPreWeight = diff1;
					hUpdateCount = 0;
				}
			}
			else
			{	hUpdateCount = 0;
				if (!gross_flag)  gxlWeight = gxlGrossWeight - gnlTareWeight;
				else  gxlWeight = gxlGrossWeight;
				bNetSignPlus = 1;
				lPreWeight = diff1;
				if (gxlWeight < 0)
				{	bNetSignPlus = 0;
					gxlWeight = -1 * gxlWeight;
				}
			}
		}
		else	// 0413
		{
			if (gnxTareStatus)  gxlWeight = gxlGrossWeight - gnlTareWeight;
			else  gxlWeight = gxlGrossWeight;
			bNetSignPlus = 1;
			if (gxlWeight < 0)
			{	bNetSignPlus = 0;
				gxlWeight = -1 * gxlWeight;
			}
			lPreWeight = diff1;
		}

		
		
		
		/*--------------- Stable Lamp ON/OFF ------------------*/
		diff21 = (diff2 + v_ei_multiply_factor/2) / v_ei_multiply_factor;

  /*
	//은정->여기는 건들지마!
    if(!iSpeedMode) //보정  기능이 없을때 
	{ 
	 
	if (gxhCurrentType == 3||gxhCurrentType == 4 || gxhCurrentType == 5 || gxhCurrentType == 6 || gxhCurrentType == 7) //사과, 감,배,한라봉,레드항 
		{
			if (0 <= diff21 && diff21 <= 5)	 // 안정범위 
			{
				StableCount++;
				hUnstableCount = 0;
			}
			else  // stable LAMP, RELAY OFF 
			{
				LAMPST = LED_OFF;
				StableCount = 0;
				prev_adc1 = v_adc1_buf;
			}
		}
		else //JADO, GREAP, PEACK
		{
		//	if (diff21 == 0)
		 if (0 <= diff21 && diff21 <= 2) //안정범위  
			{
				StableCount++;
				hUnstableCount = 0;
			}
			else  //stable LAMP, RELAY OFF 
			{
				LAMPST = LED_OFF;
				StableCount = 0;
				prev_adc1 = v_adc1_buf;
			}
		}

		
	 } // of if 보정  기능이 없을때  , 그전하고 같음 
	  
	//은정
	 else // 보정 기능일때    
	  {
	    if (0 <= diff21 && diff21 <= 7)
	
			{
				StableCount++;
				hUnstableCount = 0;
			}
			else  // stable LAMP, RELAY OFF 
			{
				LAMPST = LED_OFF;
				StableCount = 0;
				prev_adc1 = v_adc1_buf;
			}
	   }

	 */

 //0329
/*
 if(!gxhWhichSorting)//  현재선별방법이 포장갯수이면
				{
					DANSU_LED_ON;
					POJANG_LED_OFF;
				}
  else
	    {
	     POJANG_LED_ON;
				DANSU_LED_OFF;

      }
*/


//0412

//diff21


sprintf(str, "diff21 =%d   ",diff21);////USART1_puts(str);

    if(!iSpeedMode) //보정  기능이 없을때  0327
	{ 
		SPEED_LED_OFF;
	//	#define  STABLE_NUMBER  10
		 stable_number=15;
		
		if(!bScaleMode) // 선별시
			 gxhFilter=25;
	 
		 if (0 <= diff21 && diff21 <= 1) //안정범위    original : 2
			{
				StableCount++;
				hUnstableCount = 0;
			}
			else  //stable LAMP, RELAY OFF 
			{
				LAMPST = LED_OFF;
				//DANSU_LED_OFF;
	//			SPEED_LED_OFF;
				StableCount = 0;
				prev_adc1 = v_adc1_buf;
			}
	} // of if 보정  기능이 없을때  , 그전하고 같음 
	  
	//은정
	 else // 보정 기능일때    
	  {
			SPEED_LED_ON;
			//#define  STABLE_NUMBER  7
			if(!bScaleMode) // 선별시
			 gxhFilter=25;

			
			stable_number=10;
	    if (0 <= diff21 && diff21 <= 2)
	
			{
				StableCount++;
				hUnstableCount = 0;
			}
			else  // stable LAMP, RELAY OFF 
			{
				LAMPST = LED_OFF;
		//		SPEED_LED_OFF;
				StableCount = 0;
				prev_adc1 = v_adc1_buf;
			}
	   }
		
		
//	#define  STABLE_NUMBER  10
		 
	//	if (1 < StableCount)	/* stable LAMP, RELAY ON */
	//	if (2 < StableCount)	/* stable LAMP, RELAY ON */
 	//	if (4 < StableCount)	/* stable LAMP, RELAY ON */
   if (StableCount>=stable_number)	/* stable LAMP, RELAY ON *///fil 0413
		
		{  
			//SPEED_LED_ON; 
			LAMPST = LED_ON;
			
			prev_adc1 = v_adc1_buf;
				
			//if (StableCount == 4) prev_adc1 = v_adc1_buf;
			//if (10 < StableCount) StableCount = 6;
		//	StableCount = 0;
		  	
			hUnstableCount = 0;
		}

		/*---------- Zero lamp ON/OFF ----------*/
		//LAMPZR = LED_OFF;
	//	ZERO_LED_OFF;
		if(gxlWeight>=0 && gxlWeight<5 )//201804
		{ 	//LAMPZR = LED_ON;
			ZERO_LED_ON;
			hAutoZeroCounter = 0; /* initialize auto zero timer for exect time */
		}
   else
		 ZERO_LED_OFF;
		 
		//ms/at	if (gxlGrossWeight > over_weight) { vputs(" OVER  "); beep(100); }
		if ((((v_adc1_buf - v_adc_init + v_ei_multiply_factor/2) / v_ei_multiply_factor) * v_division) > over_weight)  //0508
		{
//jadue		vputs(" OVER"); beep(1000);
		 }
		if (bScaleMode)
		{	
			
			if(!gxhWhichSorting)//  현재선별방법이 포장갯수이면
				{DANSU_LED_ON; POJANG_LED_OFF;}
  else
	     {POJANG_LED_ON;DANSU_LED_OFF;}
      
			
			
			SIGNFLAG = !bNetSignPlus;	/* weight sign */
			byDisplayUpdate--;

			s_weight1 += gxlWeight;
			s_weight++;
			if(s_weight == 7)
//			if (!byDisplayUpdate)
			{
				s_weight1 /= 8;
				if(v_division == 2)
				{
					s_weight2 = s_weight1;
					s_weight2 %= 2;
					if(s_weight2 == 1) s_weight1 += 1;
				}

				if(v_division == 5)
				{
					s_weight2 = s_weight1;
					s_weight2 %= 5;
					if(s_weight2 == 1) s_weight1 -= 1;
					if(s_weight2 == 2) s_weight1 -= 2;
					if(s_weight2 == 3) s_weight1 += 2;
					if(s_weight2 == 4) s_weight1 += 1;
				}
				//vputnum_5digit(s_weight1);
				//0503
			
        if(s_weight1>over_weight)
				{
					retry_message();
					vputs("OVER ");
				}				
					else
				 {
				Display_Weight(s_weight1);	  //50.00
				if(!bNetSignPlus)// if current weight is negative 
					Display_buffer[5]=CH_BAR;
			   }
				
				//	vputchar (5, CH_BAR);
				//vputchar (5, str[0]);
			//	vputnum_5digit(gxlWeight);
            //	vputnum_5digit(diff21);


				byDisplayUpdate = 5;
		//		DISPBUF[5] &= 0x80;
		//		DISPBUF[6] = 0;
				s_weight = 0;
			}
		} //저울모드

		else // not scale mode   선별모드
		{
			if(bGradeMode) // only    선별상태에서만 ..
			{	
			if(!gxhWhichSorting)// 단수 선별이면
			Dansu_Led_Blink();
		  else   // 포장선별이면 
			Pojang_Led_Blink();
		 }
			
		  if(Usart4_Interrupt_flag==1)
	 {
		// beep(300);
 	   Bluetooth_Saving();
 	 Usart4_Interrupt_flag=0;
		  RxCounter4=0;
	 }
		 
		 //0413
		 
	////	 sprintf(str, "bPreSignPlus=%d -bNetSignPlus=%d -lPreRef=%d -gxlWeight=%d ",bPreSignPlus,bNetSignPlus,lPreRef,gxlWeight);USART1_puts(str);
 
			if (bPreSignPlus)
			{
				if (bNetSignPlus)
				{	if (lPreRef < gxlWeight) lOneWeight = 0;
					else	lOneWeight = lPreRef - gxlWeight;
				}
				else
				{	if (lPreRef < gxlWeight) lOneWeight = 0;
					else lOneWeight = lPreRef + gxlWeight;
				}
			}
			else
			{
				if (bNetSignPlus)
				{	//lOneWeight = lPreRef + gxlWeight;
					lOneWeight = 0;
				}
				else
				{
					if (gxlWeight < lPreRef) lOneWeight = 0;
					else if (gxlWeight > lPreRef && !bPreSignPlus) lOneWeight = 0;
					else if (bPreSignPlus) lOneWeight = gxlWeight - lPreRef;
				}
			}

			//0325
			
		//	sprintf(str, "LAMPAS=%d -end_flag=%d -bGradeReady=%d -bSetMode2=%d ",LAMPST,end_flag,bGradeReady,bSetMode2);USART1_puts(str);
 
		//	sprintf(str, "lOneWeight =%d  iMinimumWeight=%d ",lOneWeight,iMinimumWeight);USART1_puts(str);
 
			
		//	gxhDisplaySelect=0;
			
			if (lOneWeight > iMinimumWeight/3) end_flag = 1;  // 0325
			else end_flag = 0;
			if (!bGrossSignPlus) end_flag = 0;

			if (LAMPST && end_flag && bGradeReady && bSetMode2 == 1)    //0420
			{
				 sprintf(str, "lOneWeight =%d   ",lOneWeight);USART1_puts(str);
 
				{
					////LAMPZR = LED_ON; //delay(100);	//kim

					if (i1dan <= (int)lOneWeight) // lOneWeight : 현재선별한 무게가 
					{
						
						if(gxhWhichSorting==1 && gxhCurrentType !=1) // 자두일때는 포장갯수 선별을 하지 않는다.

		 				Voice_Play((final_pojang_weight/i1dan)+POJANG_OFFSET);
			      else 		
						Voice_Play(VOICE_1);
					

						lCount[1]++; lTotalCount++;
						if(gxhDisplaySelect)
						Display_Weight(lOneWeight);
						else
						vputnum_5digit(lCount[1]);
						
						vputgrade2digit(1);
						iVoice=1;
						
						vputgwa3digit(final_pojang_weight/i1dan);
						 sprintf(str, "final_pojang_weight=%5d, 1dan=%4d, result=%3d",(int)final_pojang_weight,(int)i1dan, (int)(final_pojang_weight/i1dan)); 
           	 USART1_puts(str);
					
						
					}
					else if (i2dan <= (int)lOneWeight)
					{
						if(gxhWhichSorting==1&& gxhCurrentType !=1)
						Voice_Play((final_pojang_weight/i2dan)+POJANG_OFFSET);
			      else 		
						Voice_Play(VOICE_2);
			
						lCount[2]++; lTotalCount++;
						if(gxhDisplaySelect)
						Display_Weight(lOneWeight);
						else
						vputnum_5digit(lCount[2]);
						vputgrade2digit(2);
						iVoice=2;
						vputgwa3digit(final_pojang_weight/i2dan);
	           sprintf(str, "final_pojang_weight=%5d, 2dan=%4d, result=%3d",(int)final_pojang_weight,(int)i2dan, (int)(final_pojang_weight/i2dan)); 
           	 USART1_puts(str);
					
					}
					else if (i3dan <= (int)lOneWeight && (3 <= DAN_MAX+1))
					{
						if(gxhWhichSorting==1&& gxhCurrentType !=1)
						Voice_Play((final_pojang_weight/i3dan)+POJANG_OFFSET);
			      else 		
						Voice_Play(VOICE_3);

						lCount[3]++; lTotalCount++;
							if(gxhDisplaySelect)
						Display_Weight(lOneWeight);
						else
						vputnum_5digit(lCount[3]);
						vputgrade2digit(3);
						iVoice=3;
						vputgwa3digit(final_pojang_weight/i3dan);
						 sprintf(str, "final_pojang_weight=%5d, 3dan=%4d, result=%3d",(int)final_pojang_weight,(int)i3dan, (int)(final_pojang_weight/i3dan)); 
           	 USART1_puts(str);
					
					}
					else if (i4dan <= (int)lOneWeight && (4 <= DAN_MAX+1))
					{
						if(gxhWhichSorting==1&& gxhCurrentType !=1)
						Voice_Play((final_pojang_weight/i4dan)+POJANG_OFFSET);
			      else 		
						Voice_Play(VOICE_4);
						
						
						lCount[4]++; lTotalCount++;
							if(gxhDisplaySelect)
						Display_Weight(lOneWeight);
						else
						vputnum_5digit(lCount[4]);
						vputgrade2digit(4);
						iVoice=4;
						vputgwa3digit(final_pojang_weight/i4dan);
						 sprintf(str, "final_pojang_weight=%5d, 4dan=%4d, result=%3d",(int)final_pojang_weight,(int)i4dan, (int)(final_pojang_weight/i4dan)); 
           	 USART1_puts(str);
					
					}
					else if (i5dan <= (int)lOneWeight && (5 <= DAN_MAX+1))
					{
						if(gxhWhichSorting==1&& gxhCurrentType !=1)
						 Voice_Play((final_pojang_weight/i5dan)+POJANG_OFFSET);
			      else 		
						Voice_Play(VOICE_5);

						lCount[5]++; lTotalCount++;
							if(gxhDisplaySelect)
						Display_Weight(lOneWeight);
						else
						vputnum_5digit(lCount[5]);
						vputgrade2digit(5);
						iVoice=5;
						vputgwa3digit(final_pojang_weight/i5dan);
						 sprintf(str, "final_pojang_weight=%5d, 5dan=%4d, result=%3d",(int)final_pojang_weight,(int)i5dan, (int)(final_pojang_weight/i5dan)); 
           	 USART1_puts(str);
					
					}
					else if((gxhDanSetType == 2))
					{
						if (i6dan <= (int)lOneWeight && (6 <= DAN_MAX+1))
						{
							if(gxhWhichSorting==1&& gxhCurrentType !=1)
						 Voice_Play((final_pojang_weight/i6dan)+POJANG_OFFSET);
			       else
							Voice_Play(VOICE_6);
							lCount[6]++; lTotalCount++;
						 	if(gxhDisplaySelect)
						Display_Weight(lOneWeight);
						else
							vputnum_5digit(lCount[6]);
							vputgrade2digit(6);
							iVoice=6;
							vputgwa3digit(final_pojang_weight/i6dan);
						  sprintf(str, "final_pojang_weight=%5d, 6dan=%4d, result=%3d",(int)final_pojang_weight,(int)i6dan, (int)(final_pojang_weight/i6dan)); 
           	 USART1_puts(str);
					
						}
						else if (i7dan <= (int)lOneWeight && (7 <= DAN_MAX+1))
						{
							if(gxhWhichSorting==1&& gxhCurrentType !=1)
						 Voice_Play((final_pojang_weight/i7dan)+POJANG_OFFSET);
			       else
							Voice_Play(VOICE_7);
							lCount[7]++; lTotalCount++;
						 	if(gxhDisplaySelect)
						Display_Weight(lOneWeight);
						else
							vputnum_5digit(lCount[7]);
							vputgrade2digit(7);
							iVoice=7;
							vputgwa3digit(final_pojang_weight/i7dan);
						 sprintf(str, "final_pojang_weight=%5d, 7dan=%4d, result=%3d",(int)final_pojang_weight,(int)i7dan, (int)(final_pojang_weight/i7dan)); 
           	 USART1_puts(str);
					
						}
						else if (i8dan <= (int)lOneWeight && (8 <= DAN_MAX+1))
						{
							if(gxhWhichSorting==1&& gxhCurrentType !=1)
						 Voice_Play((final_pojang_weight/i8dan)+POJANG_OFFSET);
			       else
							Voice_Play(VOICE_8);
							lCount[8]++; lTotalCount++;
						 	if(gxhDisplaySelect)
						Display_Weight(lOneWeight);
						else
							vputnum_5digit(lCount[8]);
							vputgrade2digit(8);
							iVoice=8;
							vputgwa3digit(final_pojang_weight/i8dan);
						 sprintf(str, "final_pojang_weight=%5d, 8dan=%4d, result=%3d",(int)final_pojang_weight,(int)i8dan, (int)(final_pojang_weight/i8dan)); 
           	 USART1_puts(str);
					
						}
						else if (i9dan <= (int)lOneWeight && (9 <= DAN_MAX+1))
						{
							if(gxhWhichSorting==1&& gxhCurrentType !=1)
						 Voice_Play((final_pojang_weight/i9dan)+POJANG_OFFSET);
			       else
							Voice_Play(VOICE_9);
							lCount[9]++; lTotalCount++;
						 	if(gxhDisplaySelect)
						Display_Weight(lOneWeight);
						else
							vputnum_5digit(lCount[9]);
							vputgrade2digit(9);
							iVoice=9;
							vputgwa3digit(final_pojang_weight/i9dan);
						 
						 sprintf(str, "final_pojang_weight=%5d, 9dan=%4d, result=%3d",(int)final_pojang_weight,(int)i9dan, (int)(final_pojang_weight/i9dan)); 
           	 USART1_puts(str);
					
						 
						 
						 //0420
						 
						 
						}
						else if (i10dan <= (int)lOneWeight && (10 <= DAN_MAX+1))
						{
							if(gxhWhichSorting==1&& gxhCurrentType !=1)
						 Voice_Play((final_pojang_weight/i10dan)+POJANG_OFFSET);
			       else
							Voice_Play(VOICE_10);
							lCount[10]++; lTotalCount++;
						 	if(gxhDisplaySelect)
						Display_Weight(lOneWeight);
						else
							vputnum_5digit(lCount[10]);
							vputgrade2digit(10);
							iVoice=10;
							vputgwa3digit(final_pojang_weight/i10dan);
						 sprintf(str, "final_pojang_weight=%5d, 10dan=%4d, result=%3d",(int)final_pojang_weight,(int)i10dan, (int)(final_pojang_weight/i10dan)); 
           	 USART1_puts(str);
					
						}
						else if (i11dan <= (int)lOneWeight && (11 <= DAN_MAX+1))
						{
							if(gxhWhichSorting==1&& gxhCurrentType !=1)
						 Voice_Play((final_pojang_weight/i11dan)+POJANG_OFFSET);
			       else
							Voice_Play(VOICE_11);
							lCount[11]++; lTotalCount++;
						 	if(gxhDisplaySelect)
						Display_Weight(lOneWeight);
						else
							vputnum_5digit(lCount[11]);
							vputgrade2digit(11);
							iVoice=11;
							vputgwa3digit(final_pojang_weight/i11dan);
						 sprintf(str, "final_pojang_weight=%5d, 11dan=%4d, result=%3d",(int)final_pojang_weight,(int)i11dan, (int)(final_pojang_weight/i11dan)); 
           	 USART1_puts(str);
					
						}
						else if (i12dan <= (int)lOneWeight && (12 <= DAN_MAX+1))
						{
							if(gxhWhichSorting==1&& gxhCurrentType !=1)
						 Voice_Play((final_pojang_weight/i12dan)+POJANG_OFFSET);
			       else
							Voice_Play(VOICE_12);
							lCount[12]++; lTotalCount++;
						 	if(gxhDisplaySelect)
						Display_Weight(lOneWeight);
						else
							vputnum_5digit(lCount[12]);
							vputgrade2digit(12);
							iVoice=12;
							vputgwa3digit(final_pojang_weight/i12dan);
						  
						 	 sprintf(str, "final_pojang_weight=%5d, 12dan=%4d, result=%3d",(int)final_pojang_weight,(int)i12dan, (int)(final_pojang_weight/i12dan)); 
           	   USART1_puts(str);
					
						 
						}
						else if (i13dan <= (int)lOneWeight && (13 <= DAN_MAX+1))
						{
							if(gxhWhichSorting==1&& gxhCurrentType !=1)
						 Voice_Play((final_pojang_weight/i13dan)+POJANG_OFFSET);
			       else
							Voice_Play(VOICE_13);
							lCount[13]++; lTotalCount++;
						 	if(gxhDisplaySelect)
						Display_Weight(lOneWeight);
						else
							vputnum_5digit(lCount[13]);
						 	vputgrade2digit(13);
							vputgwa3digit(final_pojang_weight/i13dan);
						  sprintf(str, "final_pojang_weight=%5d, 13dan=%4d, result=%3d",(int)final_pojang_weight,(int)i13dan, (int)(final_pojang_weight/i13dan)); 
           	   USART1_puts(str);
					
						//	vputgrade2digit();

							iVoice=13;
						}
						else if (i14dan <= (int)lOneWeight && (14 <= DAN_MAX+1))
						{
							if(gxhWhichSorting==1&& gxhCurrentType !=1)
						 Voice_Play((final_pojang_weight/i14dan)+POJANG_OFFSET);
			       else
							Voice_Play(VOICE_14);
							lCount[14]++; lTotalCount++;
						 	if(gxhDisplaySelect)
						Display_Weight(lOneWeight);
						else
							vputnum_5digit(lCount[14]);
							vputgrade2digit(14);
							iVoice=14;
							vputgwa3digit(final_pojang_weight/i14dan);
						  sprintf(str, "final_pojang_weight=%5d, 14dan=%4d, result=%3d",(int)final_pojang_weight,(int)i14dan, (int)(final_pojang_weight/i14dan)); 
           	   USART1_puts(str);
					
						}
					
					
					
				     else if (i15dan <= (int)lOneWeight && (15 <= DAN_MAX+1))
					  
					  //ver 1.01
				//	  else if (i15dan <= (int)lOneWeight && (15 <= DAN_MAX+1) && (gxhCurrentType==6|| gxhCurrentType==7||gxhCurrentType==8))//한라봉, 레드향,특장 
					
						{
							if(gxhWhichSorting==1&& gxhCurrentType !=1)
						 Voice_Play((final_pojang_weight/i15dan)+POJANG_OFFSET);
			       else
							Voice_Play(VOICE_15);
							lCount[15]++; lTotalCount++;
						 	if(gxhDisplaySelect)
						Display_Weight(lOneWeight);
						else
							vputnum_5digit(lCount[15]);
							vputgrade2digit(15);
							iVoice=15;
							vputgwa3digit(final_pojang_weight/i15dan);
						  sprintf(str, "final_pojang_weight=%5d, 15dan=%4d, result=%3d",(int)final_pojang_weight,(int)i15dan, (int)(final_pojang_weight/i15dan)); 
           	   USART1_puts(str);
					
						}
					
					

                      	else if (i16dan <= (int)lOneWeight && (16 <= DAN_MAX+1))
						{
							if(gxhWhichSorting==1&& gxhCurrentType !=1)
						 Voice_Play((final_pojang_weight/i16dan)+POJANG_OFFSET);
			       else
							Voice_Play(VOICE_16);
							lCount[16]++; lTotalCount++;
						 	if(gxhDisplaySelect)
						Display_Weight(lOneWeight);
						else
							vputnum_5digit(lCount[16]);
							vputgrade2digit(16);
							iVoice=16;
							vputgwa3digit(final_pojang_weight/i16dan);
						  sprintf(str, "final_pojang_weight=%5d, 16dan=%4d, result=%3d",(int)final_pojang_weight,(int)i16dan, (int)(final_pojang_weight/i16dan)); 
           	   USART1_puts(str);
					
						}
					
					   	else if (i17dan <= (int)lOneWeight && (17 <= DAN_MAX+1))
						{
							if(gxhWhichSorting==1&& gxhCurrentType !=1)
						 Voice_Play((final_pojang_weight/i17dan)+POJANG_OFFSET);
			       else
							Voice_Play(VOICE_17);
							lCount[17]++; lTotalCount++;
						 	if(gxhDisplaySelect)
						Display_Weight(lOneWeight);
						else
							vputnum_5digit(lCount[17]);
							vputgrade2digit(17);
							iVoice=17;
							vputgwa3digit(final_pojang_weight/i17dan);
						  sprintf(str, "final_pojang_weight=%5d, 17dan=%4d, result=%3d",(int)final_pojang_weight,(int)i17dan, (int)(final_pojang_weight/i17dan)); 
           	   USART1_puts(str);
					
						}

					   	else if (i18dan <= (int)lOneWeight && (18 <= DAN_MAX+1))
						{
							if(gxhWhichSorting==1&& gxhCurrentType !=1)
						 Voice_Play((final_pojang_weight/i18dan)+POJANG_OFFSET);
			       else
							Voice_Play(VOICE_18);
							lCount[18]++; lTotalCount++;
						 	if(gxhDisplaySelect)
						Display_Weight(lOneWeight);
						else
							vputnum_5digit(lCount[18]);
							vputgrade2digit(18);
							iVoice=18;
							vputgwa3digit(final_pojang_weight/i18dan);
						  sprintf(str, "final_pojang_weight=%5d, 18dan=%4d, result=%3d",(int)final_pojang_weight,(int)i18dan, (int)(final_pojang_weight/i18dan)); 
           	   USART1_puts(str);
					
						}

				//      else if (i19dan <= (int)lOneWeight && (19 <= DAN_MAX+1) && (gxhCurrentType==6))  // ver1.01 .한라봉일때만 
				         else if (i19dan <= (int)lOneWeight && (19 <= DAN_MAX+1))  // ver1.01 .한라봉일때만 
				
						{
							if(gxhWhichSorting==1&& gxhCurrentType !=1)
						 Voice_Play((final_pojang_weight/i19dan)+POJANG_OFFSET);
			       else
	             Voice_Play(VOICE_19);
							 lCount[19]++; lTotalCount++;
						 	if(gxhDisplaySelect)
						Display_Weight(lOneWeight);
						else
							 vputnum_5digit(lCount[19]);
							 vputgrade2digit(19);
							 iVoice=19;
							vputgwa3digit(final_pojang_weight/i19dan);
						  sprintf(str, "final_pojang_weight=%5d, 19dan=%4d, result=%3d",(int)final_pojang_weight,(int)i19dan, (int)(final_pojang_weight/i19dan)); 
           	   USART1_puts(str);
					
                       
						}

					   else if (i20dan <= (int)lOneWeight && (20 <= DAN_MAX+1))
						{
							if(gxhWhichSorting==1&& gxhCurrentType !=1)
						 Voice_Play((final_pojang_weight/i20dan)+POJANG_OFFSET);
			       else
							Voice_Play(VOICE_20);
							lCount[20]++; lTotalCount++;
						 	if(gxhDisplaySelect)
						Display_Weight(lOneWeight);
						else
							vputnum_5digit(lCount[20]);
							vputgrade2digit(20);
							iVoice=20;
							vputgwa3digit(final_pojang_weight/i20dan);
						  sprintf(str, "final_pojang_weight=%5d, 20dan=%4d, result=%3d",(int)final_pojang_weight,(int)i20dan, (int)(final_pojang_weight/i20dan)); 
           	   USART1_puts(str);
					
						}

				      else if (i21dan <= (int)lOneWeight && (21 <= DAN_MAX+1))
						{
							if(gxhWhichSorting==1&& gxhCurrentType !=1)
						 Voice_Play((final_pojang_weight/i21dan)+POJANG_OFFSET);
			       else
							Voice_Play(VOICE_21);
							lCount[21]++; lTotalCount++;
						 	if(gxhDisplaySelect)
						Display_Weight(lOneWeight);
						else
							vputnum_5digit(lCount[21]);
							vputgrade2digit(21);
							iVoice=21;
							vputgwa3digit(final_pojang_weight/i21dan);
						  sprintf(str, "final_pojang_weight=%5d, 21dan=%4d, result=%3d",(int)final_pojang_weight,(int)i21dan, (int)(final_pojang_weight/i21dan)); 
           	   USART1_puts(str);
					
						}
                     
           else if (i22dan <= (int)lOneWeight && (22 <= DAN_MAX+1))
						{
							if(gxhWhichSorting==1&& gxhCurrentType !=1)
						 Voice_Play((final_pojang_weight/i22dan)+POJANG_OFFSET);
			       else
							Voice_Play(VOICE_22);
							lCount[22]++; lTotalCount++;
						 	if(gxhDisplaySelect)
						Display_Weight(lOneWeight);
						else
							vputnum_5digit(lCount[22]);
							vputgrade2digit(22);
							iVoice=22;
							vputgwa3digit(final_pojang_weight/i22dan);
						  sprintf(str, "final_pojang_weight=%5d, 22dan=%4d, result=%3d",(int)final_pojang_weight,(int)i22dan, (int)(final_pojang_weight/i22dan)); 
           	   USART1_puts(str);
					
						}
					 else if (i23dan <= (int)lOneWeight && (23 <= DAN_MAX+1))
						{
							if(gxhWhichSorting==1&& gxhCurrentType !=1)
						 Voice_Play((final_pojang_weight/i23dan)+POJANG_OFFSET);
			       else
							Voice_Play(VOICE_23);
							lCount[23]++; lTotalCount++;
						 	if(gxhDisplaySelect)
						Display_Weight(lOneWeight);
						else
							vputnum_5digit(lCount[23]);
							vputgrade2digit(23);
							iVoice=23;
							vputgwa3digit(final_pojang_weight/i23dan);
						  sprintf(str, "final_pojang_weight=%5d, 23dan=%4d, result=%3d",(int)final_pojang_weight,(int)i23dan, (int)(final_pojang_weight/i23dan)); 
           	   USART1_puts(str);
					
						}

					 else if (i24dan <= (int)lOneWeight && (24 <= DAN_MAX+1))
						{
							if(gxhWhichSorting==1&& gxhCurrentType !=1)
						 Voice_Play((final_pojang_weight/i24dan)+POJANG_OFFSET);
			       else
							Voice_Play(VOICE_24);
							lCount[24]++; lTotalCount++;
						 	if(gxhDisplaySelect)
						Display_Weight(lOneWeight);
						else
							vputnum_5digit(lCount[24]);
							vputgrade2digit(24);
							iVoice=24;
							vputgwa3digit(final_pojang_weight/i24dan);
						  sprintf(str, "final_pojang_weight=%5d, 24dan=%4d, result=%3d",(int)final_pojang_weight,(int)i24dan, (int)(final_pojang_weight/i24dan)); 
           	   USART1_puts(str);
					
						}
 						
						else if((int)lOneWeight > iMinimumWeight)	  // 포도, 자두, 복숭아, 사과,감,배  : 15단,    한라봉: 25단, 특작,레드향: 19단,  단호박: 17단
						{
						  	
						if(gxhWhichSorting==1&& gxhCurrentType !=1) // 포장갯수 선별이고,자두가 아닐때
						{ 
							 Voice_Play((final_pojang_weight/(lOneWeight))+POJANG_OFFSET);//64
							/*
						   if(gxhCurrentType==0 || gxhCurrentType==2 || gxhCurrentType ==3 ||gxhCurrentType==4 ||gxhCurrentType) 
							
							  Voice_Play((final_pojang_weight/(i15dan))+POJANG_OFFSET);//64
			        else if(gxhCurrentType==6) //한라봉 
							 Voice_Play((final_pojang_weight/(i25dan))+POJANG_OFFSET);//64
              else if(gxhCurrentType==7 || gxhCurrentType==8 ) //레드향, 특작,
								 Voice_Play((final_pojang_weight/(i19dan))+POJANG_OFFSET);//64
							 else if( gxhCurrentType==9) //단호박
								  Voice_Play((final_pojang_weight/(i15dan))+POJANG_OFFSET);//64
							*/
            }

						else
							Voice_Play(DAN_MAX+31);
							
						 
						 
						  lCount[DAN_MAX+2]++; lTotalCount++;
						 	if(gxhDisplaySelect)
						Display_Weight(lOneWeight);
						else
							vputnum_5digit(lCount[DAN_MAX+2]);
							vputgrade2digit(DAN_MAX+2);
							///iVoice=24;
						 
						 
							 vputgwa3digit(final_pojang_weight/(lOneWeight));
						   sprintf(str, "final_pojang_weight=%5d, 24dan=%4d, result=%3d",(int)final_pojang_weight,(int)(DAN_MAX+2), (int)(final_pojang_weight/(DAN_MAX+2))); 
           	   USART1_puts(str);
					

//20180424
							
							/*
							
							
							
							
							if(DAN_MAX<20)
							Voice_Play(DAN_MAX+21);
							else
							//Voice_Play(DAN_MAX+48);
							Voice_Play(DAN_MAX+29);
				
							lCount[DAN_MAX+2]++; lTotalCount++;
							vputnum_5digit(lCount[DAN_MAX+2]);
							vputgrade2digit(DAN_MAX+2);
							iVoice=(DAN_MAX+2);
						 */
						
						
						}
						//else if((int)lOneWeight <= iMinimumWeight && (int)lOneWeight > iMinimumWeight/3)
						
						
						else if((int)lOneWeight <= iMinimumWeight && (int)lOneWeight > iMinimumWeight/2) // 낙과 
						
						{
							Voice_Play(VOICE_TING);
						}
					  

					
					
					
					}

					bPreSignPlus = bNetSignPlus;
					lPreRef = gxlWeight;
					byVoiceOff = VOICEOFFDELAY;
					bGradeReady = 0;
					end_flag = 0;
				}
				
				/*
				//0413
						{	lPreRef = gxlWeight;		//MTS
			        bPreSignPlus = bNetSignPlus;
		        }

			 */	
				
			}//  of if 	if (LAMPST && end_flag && bGradeReady && bSetMode2 == 1)



		//	bPreSignPlus = bNetSignPlus;
		if (StableCount==stable_number)//0413
    {		
		  lPreRef = gxlWeight;		//MTS
			StableCount = 0;
		
		}	
		
		}// grade mode

		if (byVoiceOff) byVoiceOff--;
		else  {	bGradeReady = 1; }

		
	//	 bPreSignPlus = bNetSignPlus;
//		if (byVoiceOff <= 1) VOICE(0);
/*
		if (6 == StableCount)//0413
		{	lPreRef = gxlWeight;		//MTS
			bPreSignPlus = bNetSignPlus;
		}
*/		
		
		
		
		
		
		
		
		
		
		 
   }//if(adc_timer>=_80SPEED)// 0325	 



/************** weight processing*******************************************/



  if(Usart4_Interrupt_flag==1)
	 {
		// beep(300);
 	   Bluetooth_Saving();
 	 Usart4_Interrupt_flag=0;
		  RxCounter4=0;
	 }






//0328


	 if (bBlink) // 0325
		{ 
			
			if(save_key_timer>500)
			{
					Display_buffer[5] = 0;
					Display_buffer[6] = 0;
					Display_buffer[7] = 0;
					Display_buffer[8] = 0;
					Display_buffer[9] = 0;
			}
     if(save_key_timer>1000)
		 {
      save_key_timer=0;
			 DanDisplay(byDanIndex+1);

     }			 
		}

	

		if (lKeyWait)
		{
			lKeyWait--;
			if (!lKeyWait)
			{	retry_message();
				command = 0;
			}
		}
		else  WeighMode = 1;

		if (!WeighMode)  continue;      /* when key input mode */
		if (command != RIGHT_K) command = WEIGHING_MODE;

	 
	 
	 
	 
	 
	 
	 
	 
	 
	 
	 
	 
	}while(1);// of while
} //of main




long Get_weight(void)
{	
			
			raw_data = read_filtered_adc();
       v_adc1_buf = (s32)(gnfFactor * raw_data);
		   v_i_value  =  v_adc1_buf - v_adc_org;  // 카운트값  카운트값  카운트값 
	if (0 <= v_i_value) 
		{
			gxlGrossWeight_1 = (v_i_value + (v_ei_multiply_factor/2));
			gxlGrossWeight_1=(gxlGrossWeight_1 / v_ei_multiply_factor);
			gxlGrossWeight_1=gxlGrossWeight_1 * 1;
			g_sign=0;
		}
		else  
		{
			g_sign=1;
			v_i_value=v_i_value*-1;
			gxlGrossWeight_1 = (v_i_value + (v_ei_multiply_factor/2));
			gxlGrossWeight_1=(gxlGrossWeight_1 / v_ei_multiply_factor);
			gxlGrossWeight_1=gxlGrossWeight_1 * 1;
		}
	
		if (prev_adc1_temp < v_adc1_buf) diff2_temp = v_adc1_buf - prev_adc1_temp;
			else  diff2_temp = prev_adc1_temp - v_adc1_buf;
			diff21 = (diff2_temp + (v_ei_multiply_factor/2)) / v_ei_multiply_factor;

		 sprintf(str, "dif=%05d",(long)diff21); 
		 if (StableCount <= 5)// 부등호가 들어간것이 굉장히 중요
  		{
				//if ((diff21 <= (long)(5)))	 // 현재 무게가 범위내에 있으면 
        if ((diff21 <= 3))			
				{	StableCount++;
					hUnstableCount = 0;
				}
				else  /* stable LAMP, RELAY OFF */
				{
					hUnstableCount++;
					if (hUnstableCount>=1)//
					{
						if(stable_flag)
						{	
						// USART1_puts("unstable");	
						//	USART1_puts(str);
						 stable_flag=0;
							//Delay_ms(200);
					  }	
							StableCount = 0;
						prev_adc1_temp = v_adc1_buf;
					}
				}
			}
			else /* stable LAMP, RELAY ON */
			{
						if(!stable_flag)
						{	
						 //USART1_puts("stable !!!");	
							//USART1_puts(str);
						 ////v_adc_org = (s32)(gnfFactor * raw_data);// 안정만 되면 영점을 만든다. 
						  sprintf(str, "%5d",(int)gxlGrossWeight_1); 
           	// USART1_puts(str);
						 stable_flag=1;
					//		Delay_ms(20);
					  }	
				StableCount = 0;
				hUnstableCount = 0;
				prev_adc1_temp = v_adc1_buf;
			}
		//	 sprintf(str, "%5d",(int)gxlGrossWeight_1); 
     //  USART1_puts(str);
		
  return gxlGrossWeight_1;
 // return raw_data;
}		






void Get_ration_respective_channel(void)//채널별 분담비율 구하기 
	{
			 if((Filtered_data[0]+Filtered_data[1]+Filtered_data[2]+Filtered_data[3])>15000) //12 kg 짐판에서 5 kg 올렸을때 span이 약 30000 정도 
			 { 
			  channel_sum=Filtered_data[0]+Filtered_data[1]+Filtered_data[2]+Filtered_data[3];
			  channel_ratio_1=(float)(Filtered_data[0]/channel_sum);
			  channel_ratio_2=(float)(Filtered_data[1]/channel_sum);
			  channel_ratio_3=(float)(Filtered_data[2]/channel_sum);
			  channel_ratio_4=(float)(Filtered_data[3]/channel_sum);
			}
		    else
			 {
			   channel_ratio_1=0.0; channel_ratio_2=0.0; channel_ratio_3=0.0; channel_ratio_4=0.0;
			 } 
	
    }      




void Exponential_Filtering(void)
{
 
		 Difference = abs ( NewValue-Average );
		 if ( Difference > 256 ) // No software average for large steps 
		   {
             Average = NewValue;	// USART1_putchar('a'); 
  		   }
		else if ( Difference > 64 ) // Exponential average of 8 for medium step size 
          {
		   (float)Average = (float)(Average * 0.875 + NewValue * 0.125); // USART1_putchar('b');
	      }
		 else // Exp. ave. of 64 for steady state noise reduction 
          {
		  (float)Average = (float)(Average * 0.984375 + NewValue * 0.015625);// USART1_putchar('c');
		    
		  }
		  
		  


}


void reset_system(void)
{
	
	vputs("      "); Delay_ms(500);

	Display_buffer[5] += SEGMENT_A; Delay_ms(100);
	Display_buffer[5] += SEGMENT_B; Delay_ms(100);
	Display_buffer[5] += SEGMENT_C; Delay_ms(100);
	Display_buffer[5] += SEGMENT_D; Delay_ms(100);
	Display_buffer[5] += SEGMENT_E; Delay_ms(100);
	Display_buffer[5] += SEGMENT_F; Delay_ms(100);
	Display_buffer[5] += SEGMENT_G; Delay_ms(800);

	/*        reset process : core_cm3.h 에서 가져옴.               */
	
	 __DSB();                                                     /* Ensure all outstanding memory accesses included
                                                                  buffered write are completed before reset */
  SCB->AIRCR  = ((0x5FA << SCB_AIRCR_VECTKEY_Pos)      |
                 (SCB->AIRCR & SCB_AIRCR_PRIGROUP_Msk) |
                 SCB_AIRCR_SYSRESETREQ_Msk);                   /* Keep priority group unchanged */
  __DSB();                                                     /* Ensure completion of memory access */
  while(1);                                                    /* wait until reset */;
}



void initial_adc()
{
 	int	iz, count =0;
	long  temp, temp1_org=0, temp_org;
	long  diff;

	for (iz=0; iz < MAX_FILTER*2; iz++) /* for (i=0; i < gxhFilter+2; i++) */
  	{	
		temp1_org = read_filtered_adc(); Delay_ms(50);	   // 기냥 읽어버린다. 
	}

	//weight backup 이면 
	if (gxhWeightBackup) return;

	//안정인지 흔들리고 있는지 기준잡고
	temp_org = (long) ((float)temp1_org * gnfFactor);
	temp = temp_org;
	
	do
	{
		temp1_org = read_filtered_adc();
		temp_org = (long)((float)temp1_org * gnfFactor);
		
		diff = temp -  temp_org;
		if (diff < -80 && 80 < diff) { vputs("ERR 1 "); count = 0; }
		temp = temp_org;
	} while (++count < 4);

//	 pass_range = v_capacity * power(10, v_decimal_point);
//	 pass_range = pass_range / v_division * v_ei_multiply_factor;
//	 pass_range = pass_range / 5;
//	 zero_org = (long)((float)v_zero * gnfFactor);
//	 for (i=0; i<7; i++) { Display_buffer[i] = 0x9C; Delay_ms(150); Display_buffer[i] = 0xFF; }
//	 for (i=0; i<7; i++) { Display_buffer[6-i] = 0xA3; Delay_ms(150); Display_buffer[6-i] = 0xFF; }
/*
	diff = temp_org - zero_org;
	if (diff < -pass_range || pass_range < diff)
	{
	}
*/
	 
	for(iz=0;iz<200;iz++)
	{
		temp_org = read_filtered_adc();
		Delay_ms(1);
	}	
	display_version(); 
	for(iz=0;iz<400;iz++)
	{
		temp_org = read_filtered_adc();
		Delay_ms(1);
	}	

	// new zero adc value
	v_zero = (long)((float)temp_org * gnfFactor);

	v_zero = EE_Write_val(ADR_v_zero, v_zero, 4);	////영점(zero) a/d 값 저장
} /* void initial_adc() */

 
void cali_mode(void)
{
	char exit_flag = 0;
	static char k = 0;
	int	ref_cal_mode_count = 2;
	
	ref_cal_mode_check_timer = 1000;	
	
	switch(k)
	{
		case 0 : vputs("CAL_1 "); state = CAL_1; break;
		case 1 : vputs("CAL_2 "); state = CAL_2; break;
		case 2 : vputs("CAL_R "); state = CAL_R; break;
	}
	
	do	
	{
//vputnum(1,0,ref_cal_mode_count);	
		if ( !ref_cal_mode_check_timer) ref_cal_mode_count = 3;

		if(keypush())
		{
			switch(KeyCode)
			{
				case KEY_ZERO:
					k++;
					k %= 2; //0, 1
					
					switch(k)
					{
						case 0 : vputs("CAL_1 "); state = CAL_1; break;
						case 1 : vputs("CAL_2 "); state = CAL_2; break;
						case 2 : vputs("CAL_R "); state = CAL_R; break;
					}
					break;
					
				case KEY_TARE:
					if ((state == CAL_2) && ref_cal_mode_check_timer && ref_cal_mode_count) ref_cal_mode_count--;
					else ref_cal_mode_count = 2;
					ref_cal_mode_check_timer = 1000;	
					break;

				case KEY_SETTING:
					if ( ref_cal_mode_check_timer && !ref_cal_mode_count)
					{
						exit_flag = 1;
						state = CAL_R;
					}
					else  retry_message();
					break;
					
				case KEY_CANCEL: state = MODE_SELECT; exit_flag = 1; break;
					
				case KEY_ENTER:
					exit_flag = 1;
					break;

				default:
					retry_message();
					break;
			}
		} //keypush		 
	} while(!exit_flag); 	 
}

void mode_select(void)
{
	//char k=0;
	char exit_flag = 0;
	static char k=0;
	
	
	switch(k)
	{
		case 0: vputs("TEST  "); state = TEST_MODE; break;
		case 1: vputs("CAL   "); state = CALI_MODE; break;
		case 2: vputs("Func  "); state = SETTING_MODE; break;
	}

	do
	{
		if(keypush())
		{
			switch(KeyCode)
			{
				case KEY_ZERO:
					k++;
					k %= 3;	//0, 1, 2
					switch(k)
					{
						case 0: vputs("TEST  "); state = TEST_MODE; break;
						case 1: vputs("CAL   "); state = CALI_MODE; break;
						case 2: vputs("Func  "); state = SETTING_MODE; break;
					}
					break;
					
						case KEY_CANCEL : state = REBOOT_MODE; exit_flag = 1; break;
			 
				case KEY_ENTER:
					exit_flag = 1;
					break;

				default:
					retry_message();
					break;
			}
		} //keypush
	}while(!exit_flag);
}
 

/*******************************************************************************
* Function Name  : RCC_Configuration
* Description    : Configures the different system clocks.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RCC_Configuration(void)
{
	/* RCC system reset(for debug purpose) */
	RCC_DeInit();
	
	/* Enable HSE */
	RCC_HSEConfig(RCC_HSE_ON);
	
	/* Wait till HSE is ready */
	HSEStartUpStatus = RCC_WaitForHSEStartUp();

	if(HSEStartUpStatus == SUCCESS)
	{
		/* Enable Prefetch Buffer */
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
		
		/* Flash 2 wait state */
		FLASH_SetLatency(FLASH_Latency_2);
		
		/* HCLK = SYSCLK */
		RCC_HCLKConfig(RCC_SYSCLK_Div1); 
		
		/* PCLK2 = HCLK */
		RCC_PCLK2Config(RCC_HCLK_Div1); 
		
		/* PCLK1 = HCLK/2 */
		RCC_PCLK1Config(RCC_HCLK_Div2);
		
		/* PLLCLK = 8MHz * 9 = 72 MHz */
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
		
		/* Enable PLL */ 
		RCC_PLLCmd(ENABLE);
		
		/* Wait till PLL is ready */
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
		{
		}
		
		/* Select PLL as system clock source */
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		
		/* Wait till PLL is used as system clock source */
		while(RCC_GetSYSCLKSource() != 0x08)
		{
		}
	}

	/* Enable GPIOA, GPIOx clocks */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);
	
	/* TIM2 clock enable for timer base - ADC clock 200*384 */
///	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
//#ifdef ___moved_usart_c___
	/* Enable USART1 clocks */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	/* Enable USART2 clock */
////	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	/* Enable USART3 clock */
////	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	/* Enable UART4 clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
//#endif	//#ifdef ___moved_usart.c___	
	/* I2C1 Periph clock enable   EEPROM */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	
	
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);		  // SPI2 CLOCK ENABLE 
	
	#ifdef dpfpd
	RCC_APB2PeriphClockCmd(RCC_APB1Periph_USB,DISABLE);


  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);		  // SPI2 CLOCK ENABLE 
	 
	 
	 
	    /* Enable ADC1 */
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1|RCC_APB2Periph_ADC3, ENABLE);

      /* Enable DMA1 clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1|RCC_AHBPeriph_DMA2, ENABLE);

	#endif



}

/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures Vector Table base location.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

#ifdef  VECT_TAB_RAM  
	/* Set the Vector Table base location at 0x20000000 */ 
	NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
#else  /* VECT_TAB_FLASH  */
	/* Set the Vector Table base location at 0x08000000 */ 
	// NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x3000); // 2013년 6월 3일 
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);  
#endif

	/* Enable the USART1 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	
	/* Enable the USART5 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	
	
#ifdef dpfdpf	
	/* Enable the USART2 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	// Enable the USART3 Interrupt 
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif

/*
	// Enable the TIM2 gloabal Interrupt for ADC clock
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
*/	
}

/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	

	

	
/***********************GPI-8510****************************************/

//adc 

  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_13;  //  DOUT 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_14  ;  // sclk
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

//adc


  /********************eeprom ****************************/
  	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
  /********************eeprom ****************************/






// 7-SEGMENT DIGIT SELECTION
  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_8 | GPIO_Pin_9 ;  //  LED_1, LED_2
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);


  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_11 | GPIO_Pin_12|GPIO_Pin_13 | GPIO_Pin_14 |GPIO_Pin_15 ;  // SEGMENT DIGIT 1~5
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0 | GPIO_Pin_1|GPIO_Pin_2 | GPIO_Pin_3 ;  // SEGMENT DIGIT 6~9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

 
  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_3 | GPIO_Pin_4|GPIO_Pin_5  ;  // SEGMENT DIGIT 10,11,12
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

// 7-SEGMENT DIGIT SELECTION



// 7-SEGMENT DATA
  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0 | GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_3| GPIO_Pin_4| GPIO_Pin_5| GPIO_Pin_6| GPIO_Pin_7;  //  PIN 1
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
// 7-SEGMENT DATA

//buzzer 
  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_14;  //     buzzer
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
//buzzer



// key input

  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;   
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//GPIO_Mode_IPD
	GPIO_Init(GPIOA, &GPIO_InitStructure);
// key input



//voice

  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_7 | GPIO_Pin_8;  // voice_s1, voice_s2
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);


  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_8 | GPIO_Pin_9| GPIO_Pin_10| GPIO_Pin_11| GPIO_Pin_12;  // voice_s3~7
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_13;  // voice_8
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);



  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_12;  // voice_sbt
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_2;  // voice_rst
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

//voice

//potentialmeter

  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0 | GPIO_Pin_1;  // 0: chip select,1: up/down
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);


//potentialmeter

  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_13 | GPIO_Pin_14| GPIO_Pin_15 ;  // 0: chip select,1: up/down
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);


//ble mode select pin 20180619

 GPIO_InitStructure.GPIO_Pin =GPIO_Pin_15 ;  // ble  AT COMMAND : HIGH    ,DATA_MODE : LOW
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);









//	  _Remap_SWJ_JTAGDisable


  GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);	 // JTAG 사용하지 않는 Remapping
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);	 // JTAG 사용하지 않는 Remapping


/**********************************************************/

}

void display_model(void)
{
	vputs((char *)model);
}

void display_version(void)
{
	vputs((char *)version);	Display_buffer[2] += SEGMENT_DOT;
//	Delay_ms(1000); 
}

/***********new gi-1400*************************/

	//raw_data=ADCConvertedValue;
 int battery_test(void)
	 {
		 int battery_value;
		 battery_value=ADCConvertedValue;
		 return battery_value;
	 }


void logo()//1102
{	
	#define TIMER   2000
	
	//1126
	
	Display_buffer[0]=CH_V ;
	Display_buffer[1]=CH_R ;
	Delay_ms(TIMER);
	
	
	Display_buffer[0]=NUM_1+SEGMENT_DOT ;
	Display_buffer[1]=NUM_0 ;
	Delay_ms(TIMER);
	
	PUSH_BUTTON_ON
{
 {	
	USART1_puts("CAL_MODE");
	Delay_ms(300);

 }
 
  cal_mode();
}
	
	
/*
	Display_buffer[0]=NUM_2 ;
	Display_buffer[1]=NUM_2 ;
	Delay_ms(TIMER);
 
	Display_buffer[0]=NUM_3 ;
	Display_buffer[1]=NUM_3 ;
	Delay_ms(TIMER);
  
*/	
	
}	 


void Get_My_Address(void)//1122
{	
 
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
	 //Display_buffer[0]=dip_switch[device_address/10];
	 Display_buffer[0]=0;	
	 Display_buffer[1]=dip_switch[device_address%10];
		//Delay_ms(2000);
		
	
		sprintf(str, "My address=%d", device_address);	USART1_puts(str);
 
		
		

}
#ifdef dpfdpfdp
	//voice reset
void Voice_Reset(void) //0312
{	
 GPIO_ResetBits(GPIOD, GPIO_Pin_2);// low
 GPIO_SetBits(GPIOD, GPIO_Pin_2); Delay_ms(100);
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
	
    if(digit[7]) GPIO_SetBits(GPIOC, GPIO_Pin_10); 
		else         GPIO_ResetBits(GPIOC, GPIO_Pin_10); 
		
		
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
 
 GPIO_SetBits(GPIOC, GPIO_Pin_12); // HIGH SBT
 Delay_ms(10);
 Delay_ms(10);
 Delay_ms(10);
 GPIO_ResetBits(GPIOC, GPIO_Pin_12); // LOW SBT
} 
	

#endif 
/*	
void  Potential_Increment_Set(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_0); // set to higt chip select
	GPIO_ResetBits(GPIOB, GPIO_Pin_1); // set to higt chip select
	//Delay_ms(1);
	register_Delay_ms(30);
	GPIO_SetBits(GPIOB, GPIO_Pin_1); // set to higt chip select
  //Delay_ms(1);
	register_Delay_ms(30);
 	GPIO_ResetBits(GPIOB, GPIO_Pin_0); // set to higt chip select
	
}	
	
void  Potential_Decrement_Set(void) 
	
{
 GPIO_SetBits(GPIOB, GPIO_Pin_0); // set to higt chip select
 GPIO_SetBits(GPIOB, GPIO_Pin_1); // set to higt chip select
 //Delay_ms(1);
register_Delay_ms(30); 	
 GPIO_ResetBits(GPIOB, GPIO_Pin_1); // set to higt chip select
 //Delay_ms(1);
	register_Delay_ms(30);
 GPIO_ResetBits(GPIOB, GPIO_Pin_0); // set to higt chip select
	
}	



void  Potential_Voice_Adjustment(void)
{
 GPIO_ResetBits(GPIOB, GPIO_Pin_1); // set to higt chip select
// Delay_ms(1);
 register_Delay_ms(30);
 GPIO_SetBits(GPIOB, GPIO_Pin_1); // set to higt chip select
	
 register_Delay_ms(30);	
 GPIO_ResetBits(GPIOB, GPIO_Pin_1); // set to higt chip select
	
	
}	

void register_Delay_ms(int time)

{
	int	 duration;

	for(duration=0; duration<time; duration++) {}
}

*/
void mode_check()
{
//	u8 key_number; 
	u8 bPEACH = FALSE;
	u8 bHUNDRED = FALSE;
	u8 bTEN = FALSE;
	u8 bONE = FALSE;
	int	 i;
/*
	for(i=0; i<10; i++)
	{
		if (keypush()) break;
	}
	if(10 <= i) return;
*/
	
	//vputs("11111");
	//Delay_ms(3000);
	//key_number=0;
	key_number=0;
  for(i=0;i<30;i++)
	key_number=keypush();
  if(key_number==ZERO_K)
  test_mode();		
	#ifdef dpfdpf
if(key_number)	
	{
		switch (key_number)
		{
			case ZERO_K:		    	/* TEST ROUTINE */
				test_mode();
				break;
			case COUNT_K:         	/* CALIBRATION MODE */
				cal_mode();
				break;
			case TEST_K:
				//cal_test_set();
			   test_mode();
				break;
			case WEIGH_K:
				set_mode();
				break;
			
			/*
			case RIGHT_K:	//set_k
				vputs("INIT ");
				do
				{
					if (keypush())
					{
						switch (GetKeyCode())
						{
							case SET_K: break;
							case PEACH_K:
								if (bPEACH == FALSE && bHUNDRED == FALSE && bTEN == FALSE && bONE == FALSE)
								{
									bPEACH = TRUE;
									vputs("    -");
								}
								else bPEACH = bHUNDRED = bTEN = bONE = FALSE;
								break;
							case HUNDRED_K:
								if (bPEACH == TRUE && bHUNDRED == FALSE && bTEN == FALSE && bONE == FALSE)
								{
									bHUNDRED = TRUE;
									vputs("   --");
								}
								else bPEACH = bHUNDRED = bTEN = bONE = FALSE;
								break;
							case TEN_K:
								if (bPEACH == TRUE && bHUNDRED == TRUE && bTEN == FALSE && bONE == FALSE)
								{
									bTEN = TRUE;
									vputs("  ---");
								}
								else bPEACH = bHUNDRED = bTEN = bONE = FALSE;
								break;
							case ONE_K:
								if (bPEACH == TRUE && bHUNDRED == TRUE && bTEN == TRUE && bONE == FALSE)
								{
									bONE = TRUE;
									vputs(" ----");

									gxhWeightGrapeType = 2;
									CALL_FN = FN_write_eep;
									write_eep(ADR_gxhWeightGrapeType, u16gxhWeightGrapeType);
									CALL_FN = FN_read_eep;
									gxhWeightJado1Type = 1;
									CALL_FN = FN_write_eep;
									write_eep(ADR_gxhWeightJado1Type, u16gxhWeightJado1Type);
									CALL_FN = FN_read_eep;
									gxhWeightJado2Type = 1;
									CALL_FN = FN_write_eep;
									write_eep(ADR_gxhWeightJado2Type, u16gxhWeightJado2Type);
									CALL_FN = FN_read_eep;
									gxhWeightPearType  = 2;
									CALL_FN = FN_write_eep;
									write_eep(ADR_gxhWeightPearType, u16gxhWeightPearType);
									CALL_FN = FN_read_eep;
									gxhWeightAppleType = 3;
									CALL_FN = FN_write_eep;
									write_eep(ADR_gxhWeightAppleType, u16gxhWeightAppleType);
									CALL_FN = FN_read_eep;
									gxhWeightPeackType = 1;
									CALL_FN = FN_write_eep;
									write_eep(ADR_gxhWeightPeackType, u16gxhWeightPeackType);
									CALL_FN = FN_read_eep;

									vputs("     ");

									DefaultDanSetting();
									vputs(" ----");

								}
								else bPEACH = bHUNDRED = bTEN = bONE = FALSE;
								break;
							default:
								break;
						}
					}
				} while(gxiKey != SET_K);
				break;
				
				*/
			default:
				break;
		} /* of switch */
	} /* of if (keypush()) */
	
	#endif
} /* of void mode_check() */


void initilize()
{
	u8	by;

	read_cal_factors();
	
	switch (v_capacity)
	{
		case 15: // VfdMovingDisplay("ST3-15  V @00", 100); break;
				/* .gfe dcba - 0:off 1:on */
				Display_buffer[5] = 0x08;  /* 0000 1000 */
				Display_buffer[6] = 0x00;  /* 0000 0000 */
				Display_buffer[7] = 0x08;  /* 0000 1000 */
				Display_buffer[8] = 0x00;  /* 0000 0000 */
				Display_buffer[9] = 0x00;  /* 0000 0000 */
				Delay_ms(500);
				Delay_ms(500);
				Delay_ms(500);
				buzzer_off();
				Display_buffer[5] = 0x58;  /* 0101 1000 */
				Display_buffer[6] = 0x18;  /* 0001 1000 */
				Display_buffer[7] = 0x58;	//4C;  /* 0100 1100 */
				Display_buffer[8] = 0x08;  /* 0000 1000 */
				Display_buffer[9] = 0x04;  /* 0000 0100 */
				Delay_ms(500);
				Delay_ms(500);
				Delay_ms(500);
				Display_buffer[0] = 0x6D;  /* 0110 1101 */
				Display_buffer[1] = 0x78;  /* 0111 1000 */
				Display_buffer[2] = 0x6D;	//4F;  /* 1111 0010 */
				Display_buffer[3] = 0x40;  /* 0100 0000 */
				Display_buffer[4] = 0x06;  /* 0000 0110 */
				Delay_ms(500);
				Delay_ms(500);
				Delay_ms(500);
				break;
		case 20: // VfdMovingDisplay("ST3-20  V @00", 100); break;
				/* .gfe dcba - 0:off 1:on */
				Display_buffer[0] = 0x08;  /* 0000 1000 */
				Display_buffer[1] = 0x00;  /* 0000 0000 */
				Display_buffer[2] = 0x08;  /* 0000 1000 */
				Display_buffer[3] = 0x00;  /* 0000 0000 */
				Display_buffer[4] = 0x08;  /* 0000 1000 */
				Delay_ms(500);
				buzzer_off();
				Display_buffer[0] = 0x58;  /* 0101 1000 */
				Display_buffer[1] = 0x18;  /* 0001 1000 */
				Display_buffer[2] = 0x58;	//4C;  /* 0100 1100 */
				Display_buffer[3] = 0x08;  /* 0100 0100 */
				Display_buffer[4] = 0x4C;  /* 0010 1100 */
				Delay_ms(500);
				Display_buffer[0] = 0x6D;  /* 0110 1101 */
				Display_buffer[1] = 0x78;  /* 0111 1000 */
				Display_buffer[2] = 0x6D;	//4F;  /* 1111 0010 */
				Display_buffer[3] = 0x40;  /* 0100 1111 */
				Display_buffer[4] = 0x5B;  /* 0011 1011 */
				Delay_ms(500);
				break;
		case 30: // VfdMovingDisplay("ST3-30  V @00", 100); break;
				/* .gfe dcba - 0:off 1:on */
				Display_buffer[0] = 0x08;  /* 0000 1000 */
				Display_buffer[1] = 0x00;  /* 0000 0000 */
				Display_buffer[2] = 0x08;  /* 0000 1000 */
				Display_buffer[3] = 0x00;  /* 0000 0000 */
				Display_buffer[4] = 0x08;  /* 0000 1000 */
				Delay_ms(500);
				buzzer_off();
				Display_buffer[0] = 0x58;  /* 0101 1000 */
				Display_buffer[1] = 0x18;  /* 0001 1000 */
				Display_buffer[2] = 0x58;	//4C;  /* 0100 1100 */
				Display_buffer[3] = 0x08;  /* 0000 1000 */
				Display_buffer[4] = 0x4C;  /* 0100 1100 */
				Delay_ms(500);
				Display_buffer[0] = 0x6D;  /* 0110 1101 */
				Display_buffer[1] = 0x78;  /* 0111 1000 */
				Display_buffer[2] = 0x6D;	//4F;  /* 1111 0010 */
				Display_buffer[3] = 0x40;  /* 0100 0000 */
				Display_buffer[4] = 0x4F;  /* 1111 0010 */
				Delay_ms(500);
				break;	   
		case 60: // VfdMovingDisplay("ST3-60  V @00", 100); break;
 #ifdef dpfdp		
		
				Display_buffer[5] = 0x08;  /* 0000 1000 */
				Display_buffer[6] = 0x00;  /* 0000 0000 */
				Display_buffer[7] = 0x08;  /* 0000 1000 */
		  	Display_buffer[8] = 0x00;  /* 0000 0000 */		//0X00
				Display_buffer[9] = 0x08;  /* 0000 1000 */
				Delay_ms(500);
				buzzer_off();
				Display_buffer[5] = 0x58;  /* 0101 1000 */
				Display_buffer[6] = 0x18;  /* 0001 1000 */
				Display_buffer[7] = 0x58;	//4C;  /* 0100 1100 */
				Display_buffer[8] = 0x08;  /* 0100 0100 */
				Display_buffer[9] = 0x58;  /* 0101 0100 */
				Delay_ms(500);
				//Delay_ms(500);
				//Delay_ms(500);
				Display_buffer[5] = 0x6D;  /* 0110 1101 */
				Display_buffer[6] = 0x78;  /* 0111 1000 */
				Display_buffer[7] = 0x6D;  /* 1111 0010 */
				Display_buffer[8] = 0x40;  /* 0100 1111 */
				Display_buffer[9] = 0x7D;  /* 0011 1111 */
   #endif
    vputs_3("GP-");
		vputs("60 HP");
		Delay_ms(2000);



    Delay_ms(500);
			//	Delay_ms(500);
			//	Delay_ms(500);
				break;
		case 150: // VfdMovingDisplay("ST3150  V @00", 100); break;
				/* .gfe dcba - 0:off 1:on */
				Display_buffer[0] = 0x08;  /* 0000 1000 */
				Display_buffer[1] = 0x00;  /* 0000 0000 */
				Display_buffer[2] = 0x08;  /* 0000 1000 */
				Display_buffer[3] = 0x00;  /* 0000 0000 */
				Display_buffer[4] = 0x08;  /* 0000 1000 */
				Delay_ms(500);
				buzzer_off();
				Display_buffer[0] = 0x58;  /* 0101 1000 */
				Display_buffer[1] = 0x18;  /* 0001 1000 */
				Display_buffer[2] = 0x58;	//4C;  /* 0100 1100 */
				Display_buffer[3] = 0x08;  /* 0000 0100 */
				Display_buffer[4] = 0x58;  /* 0101 0100 */
				Delay_ms(500);
				Display_buffer[0] = 0x6D;  /* 0110 1101 */
				Display_buffer[1] = 0x78;  /* 0111 1000 */
				Display_buffer[2] = 0x6D;	//4F;  /* 1111 0010 */
				Display_buffer[3] = 0x40;  /* 0100 1111 */
				Display_buffer[4] = 0x6D;  /* 0000 0110 */
				Delay_ms(500);
				break;
		case 300: // VfdMovingDisplay("GF300MT  V @00", 100); break;
				/* .gfe dcba - 0:off 1:on */
				Display_buffer[0] = 0x08;  /* 0000 1000 */
				Display_buffer[1] = 0x00;  /* 0000 0000 */
				Display_buffer[2] = 0x08;  /* 0000 1000 */
				Display_buffer[3] = 0x00;  /* 0000 1000 */
				Display_buffer[4] = 0x08;  /* 0000 1000 */
				Delay_ms(500);
				buzzer_off();
				Display_buffer[0] = 0x58;  /* 0101 1000 */
				Display_buffer[1] = 0x18;  /* 0001 1000 */
				Display_buffer[2] = 0x58;	//4C;  /* 0100 1100 */
				Display_buffer[3] = 0x08;  /* 0100 1100 */
				Display_buffer[4] = 0x54;  /* 0101 0100 */
				Delay_ms(500);
				Display_buffer[0] = 0x6D;  /* 0110 1101 */
				Display_buffer[1] = 0x78;  /* 0111 1000 */
				Display_buffer[2] = 0x6D;	//4F;  /* 1111 0010 */
				Display_buffer[3] = 0x40;  /* 0100 1111 */
				Display_buffer[4] = 0x3F;  /* 0011 1111 */
				Delay_ms(500);
				break;
		case 600: // VfdMovingDisplay("GF600MT  V @00", 100); break;
				/* .gfe dcba - 0:off 1:on */
				Display_buffer[0] = 0x08;  /* 0000 1000 */
				Display_buffer[1] = 0x00;  /* 0000 0000 */
				Display_buffer[2] = 0x08;  /* 0000 1000 */
				Display_buffer[3] = 0x00;  /* 0000 1000 */
				Display_buffer[4] = 0x08;  /* 0000 1000 */
				Delay_ms(500);
				buzzer_off();
				Display_buffer[0] = 0x58;  /* 0101 1000 */
				Display_buffer[1] = 0x18;  /* 0001 1000 */
				Display_buffer[2] = 0x58;	//4C;  /* 0100 1100 */
				Display_buffer[3] = 0x08;  /* 0101 1000 */
				Display_buffer[4] = 0x54;  /* 0101 0100 */
				Delay_ms(500);
				Display_buffer[0] = 0x6D;  /* 0110 1101 */
				Display_buffer[1] = 0x78;  /* 0111 1000 */
				Display_buffer[2] = 0x6D;	//4F;  /* 1111 0010 */
				Display_buffer[3] = 0x40;  /* 0111 1101 */
				Display_buffer[4] = 0x3F;  /* 0011 1111 */
				Delay_ms(500);
				break;
		default: // VfdMovingDisplay("ST3---  V @00", 100); break;
				/* .gfe dcba - 0:off 1:on */
				Display_buffer[0] = 0x08;  /* 0000 1000 */
				Display_buffer[1] = 0x00;  /* 0000 0000 */
				Display_buffer[2] = 0x08;  /* 0000 1000 */
				Display_buffer[3] = 0x00;  /* 0000 1000 */
				Display_buffer[4] = 0x00;  /* 0000 1000 */
				Delay_ms(500);
				buzzer_off();
				Display_buffer[0] = 0x58;  /* 0101 1000 */
				Display_buffer[1] = 0x18;  /* 0001 1000 */
				Display_buffer[2] = 0x58;	//4C;  /* 0100 1100 */
				Display_buffer[3] = 0x08;  /* 0100 0100 */
				Display_buffer[4] = 0x08;  /* 0101 0100 */
				Delay_ms(500);
				Display_buffer[0] = 0x6D;  /* 0110 1101 */
				Display_buffer[1] = 0x78;  /* 0111 1000 */
				Display_buffer[2] = 0x6D;	//4F;  /* 1111 0010 */
				Display_buffer[3] = 0x40;  /* 0100 1111 */
				Display_buffer[4] = 0x40;  /* 0011 1111 */
				Delay_ms(500);
				break;
	}
	

	i1dan=0;
	i2dan=0;
	i3dan=0;
	i4dan=0;
	i5dan=0;
	i6dan=0;
	i7dan=0;
	i8dan=0;
	i9dan=0;
	i10dan=0;
	i11dan=0;
	i12dan=0;
	i13dan=0;
	i14dan=0;
	i15dan=0;
	i16dan=0;
	i17dan=0;
	i18dan=0;
	i19dan=0;
	i20dan=0;
	i21dan=0;
	i22dan=0;
	i23dan=0;
	i24dan=0;
	i25dan=0;
	
	
	Delay_ms(1000);
	
	
	vputs("VER11");
	Display_buffer[8]+=SEGMENT_DOT;
//	Delay_ms(2000);
	
	
	//Delay_ms(1500);
	//Delay_ms(1500);

//	Delay_ms(2000);
	//for (by=0; by < 30; by++) DISPDAN[by] = 0;

	cond_check();

} /* of void initilize() */


void  SendDanValue(void)
  {
	recall_dan(0);
  sprintf(str, "%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d", i1dan,i2dan,i3dan,i4dan,i5dan,i6dan,i7dan,i8dan,i9dan
		                                                          ,i10dan,i11dan,i12dan,i13dan,i14dan,i15dan   );	USART1_puts(str);
	USART1_puts("*********************");	
		
	

recall_dan(1);
  sprintf(str, "%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d", i1dan,i2dan,i3dan,i4dan,i5dan,i6dan,i7dan,i8dan,i9dan
		                                                          ,i10dan,i11dan,i12dan,i13dan,i14dan,i15dan   );	USART1_puts(str);
	USART1_puts("*********************");	
			
 recall_dan(2);
  sprintf(str, "%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d", i1dan,i2dan,i3dan,i4dan,i5dan,i6dan,i7dan,i8dan,i9dan
		                                                          ,i10dan,i11dan,i12dan,i13dan,i14dan,i15dan   );	USART1_puts(str);
	USART1_puts("*********************");	
	
recall_dan(3);
  sprintf(str, "%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d", i1dan,i2dan,i3dan,i4dan,i5dan,i6dan,i7dan,i8dan,i9dan
		                                                          ,i10dan,i11dan,i12dan,i13dan,i14dan,i15dan   );	USART1_puts(str);
	USART1_puts("*********************");	
	
recall_dan(4);
  sprintf(str, "%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d", i1dan,i2dan,i3dan,i4dan,i5dan,i6dan,i7dan,i8dan,i9dan
		                                                          ,i10dan,i11dan,i12dan,i13dan,i14dan,i15dan   );	USART1_puts(str);
	USART1_puts("*********************");	
	
recall_dan(5);
  sprintf(str, "%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d", i1dan,i2dan,i3dan,i4dan,i5dan,i6dan,i7dan,i8dan,i9dan
		                                                          ,i10dan,i11dan,i12dan,i13dan,i14dan,i15dan   );	USART1_puts(str);
	USART1_puts("*********************");	
	
recall_dan(6);
  sprintf(str, "%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d", i1dan,i2dan,i3dan,i4dan,i5dan,i6dan,i7dan,i8dan,i9dan
		                                                          ,i10dan,i11dan,i12dan,i13dan,i14dan,i15dan   );	USART1_puts(str);
	USART1_puts("*********************");	
	
recall_dan(7);
  sprintf(str, "%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d", i1dan,i2dan,i3dan,i4dan,i5dan,i6dan,i7dan,i8dan,i9dan
		                                                          ,i10dan,i11dan,i12dan,i13dan,i14dan,i15dan   );	USART1_puts(str);
	USART1_puts("*********************");	
recall_dan(8);
  sprintf(str, "%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d", i1dan,i2dan,i3dan,i4dan,i5dan,i6dan,i7dan,i8dan,i9dan
		                                                          ,i10dan,i11dan,i12dan,i13dan,i14dan,i15dan   );	USART1_puts(str);
	USART1_puts("*********************");	
		
recall_dan(9);
  sprintf(str, "%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d", i1dan,i2dan,i3dan,i4dan,i5dan,i6dan,i7dan,i8dan,i9dan
		                                                          ,i10dan,i11dan,i12dan,i13dan,i14dan,i15dan   );	USART1_puts(str);
	USART1_puts("*********************");	
  
}

int iDanCheck()  // 0323
{
	if (i1dan < i2dan)
	{
		vputs("ERR 4"); Delay_ms(500);
		return 10;
	}
	if (i2dan < i3dan)
	{
		vputs("ERR 4");Delay_ms(500);
		return 11;
	}
	if (i3dan < i4dan)
	{

		vputs("ERR 4");Delay_ms(500);
		return 12;
	}
	if (i4dan < i5dan)
	{
		vputs("ERR 4");Delay_ms(500);
		return 13;
	}
	if (i5dan < i6dan)
	{
		vputs("ERR 4");Delay_ms(500);
		return 14;
	}
	if (i6dan < i7dan)
	{
		vputs("ERR 4");Delay_ms(500);
		return 15;
	}
	if (i7dan < i8dan)
	{
		vputs("ERR 4");Delay_ms(500);
		return 16;
	}
	if (i8dan < i9dan)
	{
		vputs("ERR 4");Delay_ms(500);
		return 17;
	}
	if (i9dan < i10dan)
	{
		vputs("ERR 4");Delay_ms(500);
		return 18;
	}
	if (i10dan < i11dan)
	{
		vputs("ERR 4");Delay_ms(500);
		return 19;
	}
	if (i11dan < i12dan)
	{
		vputs("ERR 4");Delay_ms(500);
		return 20;
	}
	if (i12dan < i13dan)
	{
		vputs("ERR 4");Delay_ms(500);
		return 21;
	}
	if (i13dan < i14dan)
	{
		vputs("ERR 4");Delay_ms(500);
		return 22;
	}
	if (i14dan < i15dan)
	{
		vputs("ERR 4");Delay_ms(500);
		return 23;
	}
	if (i15dan < i16dan)
	{
		vputs("ERR 4");Delay_ms(500);
		return 24;
	}
	if (i16dan < i17dan)
	{
		vputs("ERR 4");Delay_ms(500);
		return 25;
	}
	if (i17dan < i18dan)
	{
		vputs("ERR 4");Delay_ms(500);
		return 26;
	}
	if (i18dan < i19dan)
	{
		vputs("ERR 4");Delay_ms(500);
		return 27;
	}

//ver 1.01
   	if (i19dan < i20dan)
	{
		vputs("ERR 4");Delay_ms(500);
		return 28;
	}

	if (i20dan < i21dan)
	{
		vputs("ERR 4");Delay_ms(500);
		return 29;
	}

   if (i21dan < i22dan)//Delay_ms(500);
	{
		vputs("ERR 4");Delay_ms(500);
		return 30;
	}

   if (i22dan < i23dan)
	{
		vputs("ERR 4");Delay_ms(500);
		return 31;
	}

   if (i23dan < i24dan)
	{
		vputs("ERR 4");Delay_ms(500);
		return 32;
	}

   if (i24dan < i25dan)
	{
		vputs("ERR 4");Delay_ms(500);
		return 33;
	}

	return	0;
}

void DanDisplay(char dan)  // VER 1.01
{
	int		iVoicef = 0;

//	gxiIndex_End=16;

	switch(dan)
	{
		case 1:
			if (i1dan == 0)
			{
				vputs("-----");
				i1dan = 0;
			}
			else
			{
				//vputnum_5digit 
				vputnum_5digit((long)i1dan);
				gxiIndex_End=3;
				iMinimumWeight = (i1dan*2)/3;
			}
			vputgrade2digit(1);
			break;

		case 2:
			if (i1dan&&i2dan)
			{
				vputnum_5digit((long)i2dan);
				gxiIndex_End=4;
				iMinimumWeight = (i2dan*2)/3;
			}
			else
			{
				vputs("-----");
				i2dan = 0;
			}
			vputgrade2digit(2);
			break;
		case 3:
			if (i1dan&&i2dan&&i3dan)
			{
				vputnum_5digit((long)i3dan);
				gxiIndex_End=5;
				iMinimumWeight = (i3dan*2)/3;
			}
			else
			{
				vputs("-----");
				i3dan = 0;
			}
			vputgrade2digit(3);
			break;
		case 4:
			if (i1dan&&i2dan&&i3dan&&i4dan)
			{
				vputnum_5digit((long)i4dan);
				gxiIndex_End=6;
				iMinimumWeight = (i4dan*2)/3;
			}
			else
			{
				vputs("-----");
				i4dan = 0;
			}
			vputgrade2digit(4);
		//	vputgrade2digit(4);
			break;
		case 5:
			if (i1dan&&i2dan&&i3dan&&i4dan&&i5dan)
			{
				vputnum_5digit((long)i5dan);
				gxiIndex_End=7;
				iMinimumWeight = (i5dan*2)/3;
			}
			else
			{
				vputs("-----");
				i5dan = 0;
			}
			vputgrade2digit(5);
	//		vputgrade2digit(5);
			break;
		case 6:
			if (i1dan&&i2dan&&i3dan&&i4dan&&i5dan&&i6dan)
			{
				vputnum_5digit((long)i6dan);
				gxiIndex_End=8;
				iMinimumWeight = (i6dan*2)/3;
			}
			else
			{
				vputs("-----");
				i6dan = 0;
			}
			vputgrade2digit(6);
		//	vputgrade2digit(6);
			break;
		case 7:
			if (i1dan&&i2dan&&i3dan&&i4dan&&i5dan&&i6dan&&i7dan)
			{
				vputnum_5digit((long)i7dan);
				gxiIndex_End=9;
				iMinimumWeight = (i7dan*2)/3;
			}
			else
			{
				vputs("-----");
				i7dan = 0;
			}
			vputgrade2digit(7);
			//vputgrade2digit(7);
			break;
		case 8:
			if (i1dan&&i2dan&&i3dan&&i4dan&&i5dan&&i6dan&&i7dan&&i8dan)
			{
				vputnum_5digit((long)i8dan);
				gxiIndex_End=10;
				iMinimumWeight = (i8dan*2)/3;
			}
			else
			{
				vputs("-----");
				i8dan = 0;
			}
			vputgrade2digit(8);
		//	vputgrade2digit(8);
			break;
		case 9:
			if (i1dan&&i2dan&&i3dan&&i4dan&&i5dan&&i6dan&&i7dan&&i8dan&&i9dan)
			{
				vputnum_5digit((long)i9dan);
				gxiIndex_End=11;
				iMinimumWeight = (i9dan*2)/3;
			}
			else
			{
				vputs("-----");
				i9dan = 0;
			}
			vputgrade2digit(9);
			//vputgrade2digit(9);
			break;
		case 10:
			if (i1dan&&i2dan&&i3dan&&i4dan&&i5dan&&i6dan&&i7dan&&i8dan&&i9dan&&i10dan)
			{
				vputnum_5digit((long)i10dan);
				gxiIndex_End=12;
				iMinimumWeight = (i10dan*2)/3;
			}
			else
			{
				vputs("-----");
				i10dan = 0;
			}
			vputgrade2digit(10);
		//	vputgrade2digit(10);
			break;
		case 11:
			if (i1dan&&i2dan&&i3dan&&i4dan&&i5dan&&i6dan&&i7dan&&i8dan&&i9dan&&i10dan&&i11dan)
			{
				vputnum_5digit((long)i11dan);
				gxiIndex_End=13;
				iMinimumWeight = (i11dan*2)/3;
			}
			else
			{
				vputs("-----");
				i11dan = 0;
			}
			vputgrade2digit(11);
			//vputgrade2digit(11);
			break;
		case 12:
			if (i1dan&&i2dan&&i3dan&&i4dan&&i5dan&&i6dan&&i7dan&&i8dan&&i9dan&&i10dan&&i11dan&&i12dan)
			{
				vputnum_5digit((long)i12dan);
				gxiIndex_End=14;
				iMinimumWeight = (i12dan*2)/3;
			}
			else
			{
				vputs("-----");
				i12dan = 0;
			}
			vputgrade2digit(12);
		//	vputgrade2digit(12);
			break;
		case 13:
			if (i1dan&&i2dan&&i3dan&&i4dan&&i5dan&&i6dan&&i7dan&&i8dan&&i9dan&&i10dan&&i11dan&&i12dan&&i13dan)
			{
				vputnum_5digit((long)i13dan);
				gxiIndex_End=15;
				iMinimumWeight = (i13dan*2)/3;
			}
			else
			{
				vputs("-----");
				i13dan = 0;
			}
			vputgrade2digit(13);
			//vputgrade2digit(13);
			break;
		case 14:
			if (i1dan&&i2dan&&i3dan&&i4dan&&i5dan&&i6dan&&i7dan&&i8dan&&i9dan&&i10dan&&i11dan&&i12dan&&i13dan&&i14dan)
			{
				vputnum_5digit((long)i14dan);
				gxiIndex_End=16;
				iMinimumWeight = (i14dan*2)/3;
			}
			else
			{
				vputs("-----");
				i14dan = 0;
			}
			vputgrade2digit(14);
		//	vputgrade2digit(14);
			break;

		case 15:
			if (i1dan&&i2dan&&i3dan&&i4dan&&i5dan&&i6dan&&i7dan&&i8dan&&i9dan&&i10dan&&i11dan&&i12dan&&i13dan&&i14dan&&i15dan)
			{
				vputnum_5digit((long)i15dan);
				gxiIndex_End=17;
				iMinimumWeight = (i15dan*2)/3;
			}
			else
			{
				vputs("-----");
				i15dan = 0;
			}
			vputgrade2digit(15);
			//vputgrade2digit(15);
			break;

		case 16:
			if (i1dan&&i2dan&&i3dan&&i4dan&&i5dan&&i6dan&&i7dan&&i8dan&&i9dan&&i10dan&&i11dan&&i12dan&&i13dan&&i14dan&&i15dan&&i16dan)
			{
				vputnum_5digit((long)i16dan);
				gxiIndex_End=18;
				iMinimumWeight = (i16dan*2)/3;
			}
			else
			{
				vputs("-----");
				i16dan = 0;
			}
			vputgrade2digit(16);
		//	vputgrade2digit(16);
			break;

		case 17:
			if (i1dan&&i2dan&&i3dan&&i4dan&&i5dan&&i6dan&&i7dan&&i8dan&&i9dan&&i10dan&&i11dan&&i12dan&&i13dan&&i14dan&&i15dan&&i16dan&&i17dan)
			{
				vputnum_5digit((long)i17dan);
				gxiIndex_End=19;
				iMinimumWeight = (i17dan*2)/3;
			}
			else
			{
				vputs("-----");
				i17dan = 0;
			}
			vputgrade2digit(17);
			//vputgrade2digit(17);
			break;

		case 18:
		  vputnum_5digit((long)i18dan);
			 	gxiIndex_End=20;
				iMinimumWeight = (i18dan*2)/3;
		   
			if (i1dan&&i2dan&&i3dan&&i4dan&&i5dan&&i6dan&&i7dan&&i8dan&&i9dan&&i10dan&&i11dan&&i12dan&&i13dan&&i14dan&&i15dan&&i16dan&&i17dan&&i18dan)
			{
			  // i18dan=123;
				//vputnum_5digit((long)i18dan);
				gxiIndex_End=20;
				iMinimumWeight = (i18dan*2)/3;
			}
		else
			{
				vputs("-----");
				i18dan = 0;
			}
			 
			vputgrade2digit(18);
			//vputgrade2digit(18);
			break;

		case 19:
			if (i1dan&&i2dan&&i3dan&&i4dan&&i5dan&&i6dan&&i7dan&&i8dan&&i9dan&&i10dan&&i11dan&&i12dan&&i13dan&&i14dan&&i15dan&&i16dan&&i17dan&&i18dan&&i19dan)
			{
				vputnum_5digit((long)i19dan);
				gxiIndex_End=21;
				iMinimumWeight = (i19dan*2)/3;
			}
			else
			{
				vputs("-----");
				i19dan = 0;
			}
			vputgrade2digit(19);
			//vputgrade2digit(19);
			break;


// ver 1.01


       		case 20:
			if (i1dan&&i2dan&&i3dan&&i4dan&&i5dan&&i6dan&&i7dan&&i8dan&&i9dan&&i10dan&&i11dan&&i12dan&&i13dan&&i14dan&&i15dan&&i16dan&&i17dan&&i18dan&&i19dan
			    &&i20dan)
			{
				vputnum_5digit((long)i20dan);
				gxiIndex_End=22;
				iMinimumWeight = (i20dan*2)/3;
			}
			else
			{
				vputs("-----");
				i20dan = 0;
			}
			vputgrade2digit(20);
			//vputgrade2digit(20);
			break;

		  case 21:
			if (i1dan&&i2dan&&i3dan&&i4dan&&i5dan&&i6dan&&i7dan&&i8dan&&i9dan&&i10dan&&i11dan&&i12dan&&i13dan&&i14dan&&i15dan&&i16dan&&i17dan&&i18dan&&i19dan
			    &&i20dan&&i21dan)
			{
				vputnum_5digit((long)i21dan);
				gxiIndex_End=23;
				iMinimumWeight = (i21dan*2)/3;
			}
			else
			{
				vputs("-----");
				i21dan = 0;
			}
			vputgrade2digit(21);
			//vputgrade2digit(21);
			break;

         case 22:
			if (i1dan&&i2dan&&i3dan&&i4dan&&i5dan&&i6dan&&i7dan&&i8dan&&i9dan&&i10dan&&i11dan&&i12dan&&i13dan&&i14dan&&i15dan&&i16dan&&i17dan&&i18dan&&i19dan
			    &&i20dan&&i21dan&&i22dan)
			{
				vputnum_5digit((long)i22dan);
				gxiIndex_End=24;
				iMinimumWeight = (i22dan*2)/3;
			}
			else
			{
				vputs("-----");
				i22dan = 0;
			}
			vputgrade2digit(22);
		//	vputgrade2digit(22);
			break;

	    case 23:
			if (i1dan&&i2dan&&i3dan&&i4dan&&i5dan&&i6dan&&i7dan&&i8dan&&i9dan&&i10dan&&i11dan&&i12dan&&i13dan&&i14dan&&i15dan&&i16dan&&i17dan&&i18dan&&i19dan
			    &&i20dan&&i21dan&&i22dan&&i23dan)
			{
				vputnum_5digit((long)i23dan);
				gxiIndex_End=25;
				iMinimumWeight = (i23dan*2)/3;
			}
			else
			{
				vputs("-----");
				i23dan = 0;
			}
			vputgrade2digit(23);
			//vputgrade2digit(23);
			break;

		case 24:
			if (i1dan&&i2dan&&i3dan&&i4dan&&i5dan&&i6dan&&i7dan&&i8dan&&i9dan&&i10dan&&i11dan&&i12dan&&i13dan&&i14dan&&i15dan&&i16dan&&i17dan&&i18dan&&i19dan
			    &&i20dan&&i21dan&&i22dan&&i23dan&&i24dan)
			{
				vputnum_5digit((long)i24dan);
				gxiIndex_End=26;
				iMinimumWeight = (i24dan*2)/3;
			}
			else
			{
				vputs("-----");
				i24dan = 0;
			}
			vputgrade2digit(24);
		//	vputgrade2digit(24);
			break;
        /*
		 case 25:	//ver 1.01
			if (i1dan&&i2dan&&i3dan&&i4dan&&i5dan&&i6dan&&i7dan&&i8dan&&i9dan&&i10dan&&i11dan&&i12dan&&i13dan&&i14dan&&i15dan&&i16dan&&i17dan&&i18dan&&i19dan
			    &&i20dan&&i21dan&&i22dan&&i23dan&&i24dan&&i25dan)
			{
				vputnum_5digit((long)i25dan);
				gxiIndex_End=27;
				iMinimumWeight = (i25dan*2)/3;
			}
			else
			{
				vputs("-----");
				i25dan = 0;
			}
			vputgrade2digit(25);
			break;
		*/
   		  



//ver1.01



		default:
				break;
	}
}

void DanSetting()
{
	if (i1dan == 0)
		i2dan=i3dan=i4dan=i5dan=i6dan=i7dan=i8dan=i9dan=i10dan=i11dan=i12dan=i13dan=i14dan=i15dan=i16dan=i17dan=i18dan=i19dan=i20dan=i21dan=i22dan=i23dan=i24dan=i25dan=0;
	if (i2dan == 0)
	    i3dan=i4dan=i5dan=i6dan=i7dan=i8dan=i9dan=i10dan=i11dan=i12dan=i13dan=i14dan=i15dan=i16dan=i17dan=i18dan=i19dan=i20dan=i21dan=i22dan=i23dan=i24dan=i25dan=0;
	if (i3dan == 0)
	   	i4dan=i5dan=i6dan=i7dan=i8dan=i9dan=i10dan=i11dan=i12dan=i13dan=i14dan=i15dan=i16dan=i17dan=i18dan=i19dan=i20dan=i21dan=i22dan=i23dan=i24dan=i25dan=0;
    if (i4dan == 0)
        i5dan=i6dan=i7dan=i8dan=i9dan=i10dan=i11dan=i12dan=i13dan=i14dan=i15dan=i16dan=i17dan=i18dan=i19dan=i20dan=i21dan=i22dan=i23dan=i24dan=i25dan=0;
 	if (i5dan == 0)
	    i6dan=i7dan=i8dan=i9dan=i10dan=i11dan=i12dan=i13dan=i14dan=i15dan=i16dan=i17dan=i18dan=i19dan=i20dan=i21dan=i22dan=i23dan=i24dan=i25dan=0;
	if (i6dan == 0)
	    i7dan=i8dan=i9dan=i10dan=i11dan=i12dan=i13dan=i14dan=i15dan=i16dan=i17dan=i18dan=i19dan=i20dan=i21dan=i22dan=i23dan=i24dan=i25dan=0;
	if (i7dan == 0)
 	    i8dan=i9dan=i10dan=i11dan=i12dan=i13dan=i14dan=i15dan=i16dan=i17dan=i18dan=i19dan=i20dan=i21dan=i22dan=i23dan=i24dan=i25dan=0;
	if (i8dan == 0)
	    i9dan=i10dan=i11dan=i12dan=i13dan=i14dan=i15dan=i16dan=i17dan=i18dan=i19dan=i20dan=i21dan=i22dan=i23dan=i24dan=i25dan=0;
	if (i9dan == 0)
	    i10dan=i11dan=i12dan=i13dan=i14dan=i15dan=i16dan=i17dan=i18dan=i19dan=i20dan=i21dan=i22dan=i23dan=i24dan=i25dan=0;
	if (i10dan == 0)
	    i11dan=i12dan=i13dan=i14dan=i15dan=i16dan=i17dan=i18dan=i19dan=i20dan=i21dan=i22dan=i23dan=i24dan=i25dan=0;
	if (i11dan == 0)
	    i12dan=i13dan=i14dan=i15dan=i16dan=i17dan=i18dan=i19dan=i20dan=i21dan=i22dan=i23dan=i24dan=i25dan=0;
	if (i12dan == 0)
	    i13dan=i14dan=i15dan=i16dan=i17dan=i18dan=i19dan=i20dan=i21dan=i22dan=i23dan=i24dan=i25dan=0;
	if (i13dan == 0)
	    i14dan=i15dan=i16dan=i17dan=i18dan=i19dan=i20dan=i21dan=i22dan=i23dan=i24dan=i25dan=0;
	if (i14dan == 0)
	     i15dan=i16dan=i17dan=i18dan=i19dan=i20dan=i21dan=i22dan=i23dan=i24dan=i25dan=0;
	if (i15dan == 0)
	    i16dan=i17dan=i18dan=i19dan=i20dan=i21dan=i22dan=i23dan=i24dan=i25dan=0;
	if (i16dan == 0)
	    i17dan=i18dan=i19dan=i20dan=i21dan=i22dan=i23dan=i24dan=i25dan;
	if (i17dan == 0)
	   i18dan=i19dan=i20dan=i21dan=i22dan=i23dan=i24dan=i25dan=0;
	if (i18dan == 0)
	 i19dan=i20dan=i21dan=i22dan=i23dan=i24dan=i25dan=0;
	
   
     if (i19dan == 0)
	  i20dan=i21dan=i22dan=i23dan=i24dan=i25dan=0;

	if (i20dan == 0)
	  i21dan=i22dan=i23dan=i24dan=i25dan=0;

	if (i21dan == 0)
	  i22dan=i23dan=i24dan=i25dan=0;
	if (i22dan == 0)
	  i23dan=i24dan=i25dan=0;
	
	if (i23dan == 0)
	  i24dan=i25dan=0;
	if (i24dan == 0)
	  i25dan=0;


	/*
	byDan[ 0]=i1dan/100;byDan[ 1]=i1dan%100;byDan[ 1]/=10;byDan[ 2]=i1dan%10;
	byDan[ 3]=i2dan/100;byDan[ 4]=i2dan%100;byDan[ 4]/=10;byDan[ 5]=i2dan%10;
	byDan[ 6]=i3dan/100;byDan[ 7]=i3dan%100;byDan[ 7]/=10;byDan[ 8]=i3dan%10;
	byDan[ 9]=i4dan/100;byDan[10]=i4dan%100;byDan[10]/=10;byDan[11]=i4dan%10;
	byDan[12]=i5dan/100;byDan[13]=i5dan%100;byDan[13]/=10;byDan[14]=i5dan%10;
	byDan[15]=i6dan/100;byDan[16]=i6dan%100;byDan[16]/=10;byDan[17]=i6dan%10;
	byDan[18]=i7dan/100;byDan[19]=i7dan%100;byDan[19]/=10;byDan[20]=i7dan%10;
	byDan[21]=i8dan/100;byDan[22]=i8dan%100;byDan[22]/=10;byDan[23]=i8dan%10;
	byDan[24]=i9dan/100;byDan[25]=i9dan%100;byDan[25]/=10;byDan[26]=i9dan%10;
	byDan[27]=i10dan/100;byDan[28]=i10dan%100;byDan[28]/=10;byDan[29]=i10dan%10;
	byDan[30]=i11dan/100;byDan[31]=i11dan%100;byDan[31]/=10;byDan[32]=i11dan%10;
	byDan[33]=i12dan/100;byDan[34]=i12dan%100;byDan[34]/=10;byDan[35]=i12dan%10;
	byDan[36]=i13dan/100;byDan[37]=i13dan%100;byDan[37]/=10;byDan[38]=i13dan%10;
	byDan[39]=i14dan/100;byDan[40]=i14dan%100;byDan[40]/=10;byDan[41]=i14dan%10;
	byDan[42]=i15dan/100;byDan[43]=i15dan%100;byDan[43]/=10;byDan[44]=i15dan%10;
	byDan[45]=i16dan/100;byDan[46]=i16dan%100;byDan[46]/=10;byDan[47]=i16dan%10;
	byDan[48]=i17dan/100;byDan[49]=i17dan%100;byDan[49]/=10;byDan[50]=i17dan%10;
	byDan[51]=i18dan/100;byDan[52]=i18dan%100;byDan[52]/=10;byDan[53]=i18dan%10;
	byDan[54]=i19dan/100;byDan[55]=i19dan%100;byDan[55]/=10;byDan[56]=i19dan%10;



   	//ver1.01
	byDan[57]=i20dan/100;byDan[58]=i20dan%100;byDan[58]/=10;byDan[59]=i20dan%10;
    byDan[60]=i21dan/100;byDan[61]=i21dan%100;byDan[61]/=10;byDan[62]=i21dan%10;
	byDan[63]=i22dan/100;byDan[64]=i22dan%100;byDan[64]/=10;byDan[65]=i22dan%10;
    byDan[66]=i23dan/100;byDan[67]=i23dan%100;byDan[67]/=10;byDan[68]=i23dan%10;
    byDan[69]=i24dan/100;byDan[70]=i24dan%100;byDan[70]/=10;byDan[71]=i24dan%10;
    byDan[72]=i25dan/100;byDan[73]=i25dan%100;byDan[73]/=10;byDan[74]=i25dan%10;
	 */

	 ByDan_Funciton();





}



void CalcMinimumWeight()
{
	switch(gxiIndex_End)
	{
		case 18:
			iMinimumWeight = (i16dan*2)/3;
			if (!i16dan) iMinimumWeight = (i15dan*2)/3;
			break;
		case 19:
			iMinimumWeight = (i17dan*2)/3;
			if (!i17dan) iMinimumWeight = (i16dan*2)/3;
			if (!i16dan) iMinimumWeight = (i15dan*2)/3;
			break;
		case 20:
			iMinimumWeight = (i18dan*2)/3;
			if (!i18dan) iMinimumWeight = (i17dan*2)/3;
			if (!i17dan) iMinimumWeight = (i16dan*2)/3;
			if (!i16dan) iMinimumWeight = (i15dan*2)/3;
			break;
		case 21:
			iMinimumWeight = (i19dan*2)/3;
			if (!i19dan) iMinimumWeight = (i18dan*2)/3;
			if (!i18dan) iMinimumWeight = (i17dan*2)/3;
			if (!i17dan) iMinimumWeight = (i16dan*2)/3;
			if (!i16dan) iMinimumWeight = (i15dan*2)/3;
			break;

	  
       //ver 1.01

       case 22:
			iMinimumWeight = (i20dan*2)/3;
			if (!i20dan) iMinimumWeight = (i19dan*2)/3;
			if (!i19dan) iMinimumWeight = (i18dan*2)/3;
			if (!i18dan) iMinimumWeight = (i17dan*2)/3;
			if (!i17dan) iMinimumWeight = (i16dan*2)/3;
			if (!i16dan) iMinimumWeight = (i15dan*2)/3;
			break;
  
       case 23:
			iMinimumWeight = (i21dan*2)/3;
			if (!i21dan) iMinimumWeight = (i20dan*2)/3;
			if (!i20dan) iMinimumWeight = (i19dan*2)/3;
			if (!i19dan) iMinimumWeight = (i18dan*2)/3;
			if (!i18dan) iMinimumWeight = (i17dan*2)/3;
			if (!i17dan) iMinimumWeight = (i16dan*2)/3;
			if (!i16dan) iMinimumWeight = (i15dan*2)/3;
			break;
	    
		case 24:
			iMinimumWeight = (i22dan*2)/3;
			
			if (!i22dan) iMinimumWeight = (i21dan*2)/3;
			if (!i21dan) iMinimumWeight = (i20dan*2)/3;
			if (!i20dan) iMinimumWeight = (i19dan*2)/3;
			if (!i19dan) iMinimumWeight = (i18dan*2)/3;
			if (!i18dan) iMinimumWeight = (i17dan*2)/3;
			if (!i17dan) iMinimumWeight = (i16dan*2)/3;
			if (!i16dan) iMinimumWeight = (i15dan*2)/3;
			break;
	    
		case 25:
			iMinimumWeight = (i23dan*2)/3;
			
			if (!i23dan) iMinimumWeight = (i22dan*2)/3;
			if (!i22dan) iMinimumWeight = (i21dan*2)/3;
			if (!i21dan) iMinimumWeight = (i20dan*2)/3;
			if (!i20dan) iMinimumWeight = (i19dan*2)/3;
			if (!i19dan) iMinimumWeight = (i18dan*2)/3;
			if (!i18dan) iMinimumWeight = (i17dan*2)/3;
			if (!i17dan) iMinimumWeight = (i16dan*2)/3;
			if (!i16dan) iMinimumWeight = (i15dan*2)/3;
			break;

	   

	  //ver 1.01







		default:
			iMinimumWeight = (i15dan*2)/3;
			break;
	}
 
 //20180424
 
   //VER 1.01
    if (!i25dan) {iMinimumWeight = (i24dan*2)/3;DAN_MAX=23;}//  DAN_MAX=25;
	if (!i24dan) {iMinimumWeight = (i23dan*2)/3;DAN_MAX=22;}
	if (!i23dan) {iMinimumWeight = (i22dan*2)/3;DAN_MAX=21;}
    if (!i22dan) {iMinimumWeight = (i21dan*2)/3;DAN_MAX=20;}
	if (!i21dan) {iMinimumWeight = (i20dan*2)/3;DAN_MAX=19;}
	if (!i20dan) {iMinimumWeight = (i19dan*2)/3;DAN_MAX=18;}
	if (!i19dan) {iMinimumWeight = (i18dan*2)/3;DAN_MAX=17;}
	if (!i18dan) {iMinimumWeight = (i17dan*2)/3;DAN_MAX=16;}
	if (!i17dan) {iMinimumWeight = (i16dan*2)/3;DAN_MAX=15;}
	if (!i16dan) {iMinimumWeight = (i15dan*2)/3;DAN_MAX=14;}
  //VER 1.01
  

	if (!i15dan) {iMinimumWeight = (i14dan*2)/3;DAN_MAX=13;}
	if (!i14dan) {iMinimumWeight = (i13dan*2)/3;DAN_MAX=12;}
	if (!i13dan) {iMinimumWeight = (i12dan*2)/3; DAN_MAX=11;}
	if (!i12dan) {iMinimumWeight = (i11dan*2)/3;DAN_MAX=10;}
	if (!i11dan) {iMinimumWeight = (i10dan*2)/3;DAN_MAX=9;}
	if (!i10dan) {iMinimumWeight = (i9dan*2)/3;DAN_MAX=8;}
	if (!i9dan)  {iMinimumWeight = (i8dan*2)/3;DAN_MAX=7;}
	if (!i8dan)  {iMinimumWeight = (i7dan*2)/3;DAN_MAX=6;}
	if (!i7dan)  {iMinimumWeight = (i6dan*2)/3;DAN_MAX=5;}
	if (!i6dan)  {iMinimumWeight = (i5dan*2)/3;DAN_MAX=4;}
	if (!i5dan)  {iMinimumWeight = (i4dan*2)/3;DAN_MAX=3;}
	if (!i4dan)  {iMinimumWeight = (i3dan*2)/3;DAN_MAX=2;}
	if (!i3dan)  iMinimumWeight = (i2dan*2)/3;
	if (!i2dan)  iMinimumWeight = (i1dan*2)/3;
	if (!i1dan)  iMinimumWeight = (i1dan*2)/3;
}

void ByDan_Funciton(void)
 {
 	byDan[ 0]=i1dan/100;byDan[ 1]=i1dan%100;byDan[ 1]/=10;byDan[ 2]=i1dan%10;
	byDan[ 3]=i2dan/100;byDan[ 4]=i2dan%100;byDan[ 4]/=10;byDan[ 5]=i2dan%10;
	byDan[ 6]=i3dan/100;byDan[ 7]=i3dan%100;byDan[ 7]/=10;byDan[ 8]=i3dan%10;
	byDan[ 9]=i4dan/100;byDan[10]=i4dan%100;byDan[10]/=10;byDan[11]=i4dan%10;
	byDan[12]=i5dan/100;byDan[13]=i5dan%100;byDan[13]/=10;byDan[14]=i5dan%10;
	byDan[15]=i6dan/100;byDan[16]=i6dan%100;byDan[16]/=10;byDan[17]=i6dan%10;
	byDan[18]=i7dan/100;byDan[19]=i7dan%100;byDan[19]/=10;byDan[20]=i7dan%10;
	byDan[21]=i8dan/100;byDan[22]=i8dan%100;byDan[22]/=10;byDan[23]=i8dan%10;
	byDan[24]=i9dan/100;byDan[25]=i9dan%100;byDan[25]/=10;byDan[26]=i9dan%10;
	byDan[27]=i10dan/100;byDan[28]=i10dan%100;byDan[28]/=10;byDan[29]=i10dan%10;
	byDan[30]=i11dan/100;byDan[31]=i11dan%100;byDan[31]/=10;byDan[32]=i11dan%10;
	byDan[33]=i12dan/100;byDan[34]=i12dan%100;byDan[34]/=10;byDan[35]=i12dan%10;
	byDan[36]=i13dan/100;byDan[37]=i13dan%100;byDan[37]/=10;byDan[38]=i13dan%10;
	byDan[39]=i14dan/100;byDan[40]=i14dan%100;byDan[40]/=10;byDan[41]=i14dan%10;
	byDan[42]=i15dan/100;byDan[43]=i15dan%100;byDan[43]/=10;byDan[44]=i15dan%10;
	byDan[45]=i16dan/100;byDan[46]=i16dan%100;byDan[46]/=10;byDan[47]=i16dan%10;
	byDan[48]=i17dan/100;byDan[49]=i17dan%100;byDan[49]/=10;byDan[50]=i17dan%10;
	byDan[51]=i18dan/100;byDan[52]=i18dan%100;byDan[52]/=10;byDan[53]=i18dan%10;
	byDan[54]=i19dan/100;byDan[55]=i19dan%100;byDan[55]/=10;byDan[56]=i19dan%10;
	byDan[57]=i20dan/100;byDan[58]=i20dan%100;byDan[58]/=10;byDan[59]=i20dan%10;
  byDan[60]=i21dan/100;byDan[61]=i21dan%100;byDan[61]/=10;byDan[62]=i21dan%10;
	byDan[63]=i22dan/100;byDan[64]=i22dan%100;byDan[64]/=10;byDan[65]=i22dan%10;
  byDan[66]=i23dan/100;byDan[67]=i23dan%100;byDan[67]/=10;byDan[68]=i23dan%10;
  byDan[69]=i24dan/100;byDan[70]=i24dan%100;byDan[70]/=10;byDan[71]=i24dan%10;
  byDan[72]=i25dan/100;byDan[73]=i25dan%100;byDan[73]/=10;byDan[74]=i25dan%10;
 }


  void  AllFruitLed_On(void)
	{
		GRAPE_LED_ON; 
		JADOO_LED_ON;
		PEACH_LED_ON;
		APPLE_LED_ON;
		GAM_LED_ON;
		PEAR_LED_ON;
		HALABONG_LED_ON;
		RED_LED_ON;
		PUMPKIN_LED_ON;
		SPECIAL_LED_ON;
	}	

  void  AllFruitLed_Off(void)
	{
		GRAPE_LED_OFF; 
		JADOO_LED_OFF;
		PEACH_LED_OFF;
		APPLE_LED_OFF;
		GAM_LED_OFF;
		PEAR_LED_OFF;
		HALABONG_LED_OFF;
		RED_LED_OFF;
		PUMPKIN_LED_OFF;
		SPECIAL_LED_OFF;
	}	

 void  Initial_Fruit_Led(void)
 {
 switch(gxhCurrentType)
 {
	 case 0: //포도
		 fruit_select=1;
	   GRAPE_LED_ON;
		 break;
	 case 1://자두
		 fruit_select=2;
      JADOO_LED_ON;	
	 break;
	 case 2://복숭아
		 fruit_select=3;
		 PEACH_LED_ON;
	   break;
	 case 3://사과
		 fruit_select=5;
	   APPLE_LED_ON;
		 break;
	 case 4://감
		 fruit_select=6;
		 GAM_LED_ON; 
	 break;
	 case 5://배
		 PEAR_LED_ON; 
	  fruit_select=7;
		 break;
	 case 6://한라봉
		HALABONG_LED_ON; 
	 fruit_select=4;
		 break;
	 case 7://레드향
		RED_LED_ON;  
	 fruit_select=8;
		 break;
	 case 8://특작
		SPECIAL_LED_ON; 
	 fruit_select=9;
		 break;
	 case 9://단호박
		PUMPKIN_LED_ON; 
	 fruit_select=10;
		 break;
 } 

 }
 
 void Pojang_Display(void)//0328
 {
	 switch(gxhCurrentType)
	 {
		 case 0:
      if(gxhWeightGrapeType==1) //4 kg  포장
			{
			  vputpojang2digit(4);	
				final_pojang_weight=4000;
			//	Voice_Play(VOICE_P4);// 4 kg   
		  }
		  else if(gxhWeightGrapeType==2) //5 kg  포장
				{	
			  vputpojang2digit(5);
					final_pojang_weight=5000;
				//Voice_Play(VOICE_P5);
        }
		  else  // 10 kg  포장
			{
			vputpojang2digit(10);
				final_pojang_weight=10000;
      //Voice_Play(VOICE_P10);				
      
			}
				break;
		 
		 case 1: //   자두
    	 vputpojang2digit(10);	
		 final_pojang_weight=10000;
         break; 
		 
		 case 2: //  복숭아
    	 if(gxhWeightPeackType==1) //4.5 kg  포장
			 {
			  vputpojang2digit(45);// 4.5  kg	
        Display_buffer[10]+=SEGMENT_DOT;
        final_pojang_weight=4500;				 
			 }
			else if(gxhWeightPeackType==2) //5 kg  포장
			{
			  vputpojang2digit(5);
        final_pojang_weight=5000;
			}
			else if(gxhWeightPeackType==3) //10 kg  포장
				{
        final_pojang_weight=10000;					
			  vputpojang2digit(10);
    	  }
					
			else  // 15 kg  포장
			{
 			 vputpojang2digit(15);	
       final_pojang_weight=15000;
			}
				break;
		 
		 case 3: //  사과
    	 if(gxhWeightAppleType==1) //5 kg  포장
			 {
			  vputpojang2digit(5);// 4.5  kg	
				 final_pojang_weight=5000;
       }
			else if(gxhWeightAppleType==2) //10 kg  포장
			{
			  vputpojang2digit(10);
        final_pojang_weight=10000;
			}
		 else
        {					
			  vputpojang2digit(15);
        final_pojang_weight=15000;
				}
					break;
			
		 case 4: //감
			 vputpojang2digit(10);
		 final_pojang_weight=10000;
		   break;
		 
		 case 5: //  gxhWeightPearType  , 감 
			 
			if(gxhWeightPearType==1) //7.5 kg  포장
			 {
			  vputpojang2digit(75);// 7.5  kg	
        Display_buffer[10]+=SEGMENT_DOT;
        final_pojang_weight=7500;				 
			 }
			else if(gxhWeightPearType==2) //15 kg  포장
			{
			  vputpojang2digit(15);
        final_pojang_weight=15000;
			} 
				break;
			
		case 6: //한라봉
			 vputpojang2digit(3);
		   final_pojang_weight=3000;
		   break;
		
		case 7: //레드향
			 vputpojang2digit(3);
		   final_pojang_weight=3000;   
		break;
		case 8: //특작
			 vputpojang2digit(3);
		final_pojang_weight=3000;
		   break;
		case 9: //단호박
			 vputpojang2digit(3);
		final_pojang_weight=3000;
		   break;
		
		
		 default: break; 
	 }	//  of switch  
 } 

void Pojang_Display_Voice(void)
 {
	  Voice_Reset();
	 switch(gxhCurrentType)
	 {
		 case 0:
      if(gxhWeightGrapeType==1) //4 kg  포장
			{
			  vputpojang2digit(4);	
			  Voice_Play(VOICE_P4);// 4 kg   
				final_pojang_weight=4000;
		  }
		  else if(gxhWeightGrapeType==2) //5 kg  포장
				{	
			   vputpojang2digit(5);
				 Voice_Play(VOICE_P5);
					final_pojang_weight=5000;
        }
		  else  // 10 kg  포장
			{
			  vputpojang2digit(10);
        Voice_Play(VOICE_P10);
        final_pojang_weight=10000;				
      
			}
				break;
		 
		 case 1: //   자두
    	 vputpojang2digit(10);	
		  Voice_Play(VOICE_P10);
		   final_pojang_weight=10000;
         break; 
		 
		 case 2: //  복숭아
    	 if(gxhWeightPeackType==1) //4.5 kg  포장
			 {
			  vputpojang2digit(45);// 4.5  kg	
        Display_buffer[10]+=SEGMENT_DOT;
			  Voice_Play(VOICE_P45);
        final_pojang_weight=4500;				 
			 }
			else if(gxhWeightPeackType==2) //5 kg  포장
			{ 			
			vputpojang2digit(5);
      Voice_Play(VOICE_P5);
      final_pojang_weight=5000;				
			}
			else if(gxhWeightPeackType==3) //10 kg  포장
			{
			  vputpojang2digit(10);
    	  Voice_Play(VOICE_P10);
        final_pojang_weight=10000;				
			}
			else  // 15 kg  포장
			{	
 			 vputpojang2digit(15);	
       Voice_Play(VOICE_P15);
       final_pojang_weight=15000;				

			}
				break;
		 
		 case 3: //  사과
    	 if(gxhWeightAppleType==1) //5 kg  포장
			 {
			  vputpojang2digit(5);// 5  kg	
				Voice_Play(VOICE_P5);	
        final_pojang_weight=5000;				 
       }
			else if(gxhWeightAppleType==2) //10 kg  포장
			{
			  vputpojang2digit(10);
        Voice_Play(VOICE_P10);	
        final_pojang_weight=10000;				
 
			}
			else 
			{		
			  vputpojang2digit(15);
        Voice_Play(VOICE_P15);	
        final_pojang_weight=15000;				

			}
				break;
			
		 case 4: //감
			  Voice_Play(VOICE_P10);
			 vputpojang2digit(10);
		 final_pojang_weight=10000;
		   break;
		 
		 case 5: //  gxhWeightPearType  , 감 
			 
			if(gxhWeightPearType==1) //7.5 kg  포장
			 {
			  vputpojang2digit(75);// 7.5  kg	
        Display_buffer[10]+=SEGMENT_DOT;		
        Voice_Play(VOICE_P75);
        final_pojang_weight=7500;				 
				 
			 }
			else if(gxhWeightPearType==2) //15 kg  포장
			{
			  vputpojang2digit(15);
        Voice_Play(VOICE_P15);	
        final_pojang_weight=15000;				

			} 
				break;
			
		case 6: //한라봉
			Voice_Play(VOICE_P3);
			 vputpojang2digit(3);
		final_pojang_weight=3000;
		   break;
		
		case 7: //레드향
			Voice_Play(VOICE_P3);
			 vputpojang2digit(3);
		final_pojang_weight=3000;
		   break;
		case 8: //특작
		 switch(gxhWeightSpecialType)	
		 {
	     case 1:
				 Voice_Play(VOICE_P3);
			   vputpojang2digit(3);
		     final_pojang_weight=3000;
		    break;
			 case 2:
				 Voice_Play(VOICE_P3);
			  vputpojang2digit(3);
		    final_pojang_weight=3000;
		    break;
			 case 3:
				 Voice_Play(VOICE_P10);
			  vputpojang2digit(10);
		    final_pojang_weight=10000;
		    break;
			 case 4:
				 Voice_Play(VOICE_P15);
			  vputpojang2digit(15);
		    final_pojang_weight=15000;
		    break;
			 default: break;

		 }
		break;
	  
			 case 9: //단호박
				Voice_Play(VOICE_P3); 
			 vputpojang2digit(3);
		   final_pojang_weight=3000;
		   break;	
		 default: break; 
	 }	//  of switch  
 } 
 
 
void Current_Type_DanSetting(void)	   // 현재 선택되어진 것만 값을 읽어오는거야 0212
 {
 
 if(gxhCurrentType==0)
  {
  if(gxhWeightGrapeType == 1)	//4kg
	{
		i1dan  = 1224;
		i2dan  = 944;
		i3dan  = 770;
		i4dan  = 650;
		i5dan  = 562;
		i6dan  = 494;
		i7dan  = 442;
		i8dan  = 400;
		i9dan  = 364;
		i10dan = 336;
		i11dan = 310;
		i12dan = 290;
		i13dan = 270;
		i14dan = 254;
		i15dan = 0;
		SaveDan(0);
	}
	if(gxhWeightGrapeType == 2)		//5kg
	{
		i1dan  = 804;
		i2dan  = 696;
		i3dan  = 612;
		i4dan  = 548;
		i5dan  = 496;
		i6dan  = 452;
		i7dan  = 416;
		i8dan  = 384;
		i9dan  = 358;
		i10dan = 334;
		i11dan = 314;
		i12dan = 296;
		i13dan = 280;
		i14dan = 266;
		i15dan = 0;
		SaveDan(0);
	}
	if(gxhWeightGrapeType == 3)	//10kg
	{
		i1dan  = 788;
		i2dan  = 682;
		i3dan  = 602;
		i4dan  = 538;
		i5dan  = 486;
		i6dan  = 444;
		i7dan  = 408;
		i8dan  = 376;
		i9dan  = 352;
		i10dan = 328;
		i11dan = 308;
		i12dan = 290;
		i13dan = 274;
		i14dan = 260;
		i15dan = 0;
		SaveDan(0);
	}
   }
	//jado - daiseok 10kg
//자두   
  if(gxhCurrentType==1)
  {

 	if(gxhWeightJado1Type == 1 || gxhWeightJado1Type == 2)	//10kg
	{
		i1dan  = 260;
		i2dan  = 240;
		i3dan  = 220;
		i4dan  = 200;
		i5dan  = 180;
		i6dan  = 160;
		i7dan  = 140;
		i8dan  = 120;
		i9dan  = 100;
		i10dan = 80;
		i11dan = 65;
		i12dan = 50;
		i13dan = 0;
		i14dan = 0;
		i15dan = 0;
		SaveDan(1);
	}
   } //	현재 타입이 자두이면 


 // 복숭아
if(gxhCurrentType==2)
{
 if(gxhWeightPeackType == 1)	//4.5kg
	{
//		i1dan  = 862;		//5ea
//		i2dan  = 727;		//6ea
		i1dan  = 629;		//7ea
		i2dan  = 555;
		i3dan  = 496;
		i4dan  = 449;
		i5dan  = 409;
		i6dan  = 377;
		i7dan  = 349;
		i8dan  = 325;
		i9dan  = 304;
		i10dan = 285;
		i11dan = 269;
		i12dan = 254;		//18ea
		i13dan = 236;		//19ea
		i14dan = 225;		//20ea
		i15dan = 0;
		SaveDan(2);
	}
	if(gxhWeightPeackType == 2)	//5kg
	{
//		i1dan  = 804;		//6ea
//		i2dan  = 696;		//7ea
		i1dan  = 612;		//8ea
		i2dan  = 548;
		i3dan  = 496;
		i4dan  = 452;
		i5dan  = 416;
		i6dan  = 384;
		i7dan  = 358;
		i8dan = 334;
		i9dan = 314;
		i10dan = 296;
		i11dan = 280;
		i12dan = 266;		//19ea
		i13dan = 250;		//20ea
		i14dan = 214;		//21ea
		i15dan = 0;
		SaveDan(2);
	}
	else if(gxhWeightPeackType == 3)	//10kg
	{
//		i1dan  = 732;		//13ea
//		i2dan  = 640;		//15ea
		i1dan  = 568;		//17ea
		i2dan  = 510;
		i3dan  = 464;
		i4dan  = 424;
		i5dan  = 392;
		i6dan  = 364;
		i7dan  = 340;
		i8dan  = 318;
		i9dan  = 300;
		i10dan = 282;
		i11dan = 268;		//37ea
		i12dan = 254;		//39ea
		i13dan = 243;		//41ea
		i14dan = 235;		//43ea
		i15dan = 0;
		SaveDan(2);
	}
	else if(gxhWeightPeackType == 4)	//15kg
	{
//		i1dan  = 784;		//18ea
//		i2dan  = 678;		//21ea
		i1dan  = 598;		//24ea
		i2dan  = 534;
		i3dan  = 482;
		i4dan  = 440;
		i5dan  = 406;
		i6dan  = 374;
		i7dan  = 348;
		i8dan  = 326;
		i9dan  = 306;
		i10dan = 288;
		i11dan = 274;
		i12dan = 260;		//57ea
		i13dan = 250;		//60ea
		i14dan = 238;		//63ea
		i15dan = 0;
		SaveDan(2);
	}
  } // current type=2
  // 복 숭아 



// 사과 
  if(gxhCurrentType==3)
  {

   if(gxhWeightAppleType == 1)	//5kg
	{
		i1dan  = 612;
		i2dan  = 548;
		i3dan  = 496;
		i4dan  = 452;
		i5dan  = 416;
		i6dan  = 384;
		i7dan  = 358;
		i8dan  = 334;
		i9dan  = 314;
		i10dan = 296;
		i11dan = 280;
		i12dan = 266;
		i13dan = 252;
		i14dan = 242;
		i15dan = 0;
		SaveDan(3);
	}
	if(gxhWeightAppleType == 2)	//10kg
	{
		i1dan  = 538;
		i2dan  = 486;
		i3dan  = 444;
		i4dan  = 408;
		i5dan  = 378;
		i6dan  = 352;
		i7dan  = 328;
		i8dan  = 308;
		i9dan  = 290;
		i10dan = 274;
		i11dan = 260;
		i12dan = 248;
		i13dan = 236;
		i14dan = 226;
		i15dan = 0;
		SaveDan(3);
	}
	if(gxhWeightAppleType == 3)	//15kg
	{
		i1dan  = 574;
		i2dan  = 508;
		i3dan  = 454;
		i4dan  = 416;
		i5dan  = 376;
		i6dan  = 334;
		i7dan  = 300;
		i8dan  = 274;
		i9dan  = 248;
		i10dan = 228;
		i11dan = 218;
		i12dan = 208;
		i13dan = 196;
		i14dan = 184;
		i15dan = 0;
		SaveDan(3);
	}
 } // current type=3
//사과 

   // 감  
   if(gxhCurrentType==4)
     {
    	i1dan  = 284;
		i2dan  = 254;
		i3dan  = 232;
		i4dan  = 212;
		i5dan  = 196;
		i6dan  = 182;
		i7dan  = 170;
		i8dan  = 158;
		i9dan  = 150;
		i10dan = 142;
		i11dan = 134;
		i12dan = 126;
		i13dan = 120;
		i14dan = 116;
		i15dan = 0;
		SaveDan(4);
 	  } // current type=4
 //감 
 
 
  //배 
 if(gxhCurrentType==5)
  {

    if(gxhWeightPearType == 1)	//7.5kg
	{
		i1dan  = 1410;
		i2dan  = 1190;
		i3dan  = 1030;
		i4dan  = 908;
		i5dan  = 812;
		i6dan  = 734;
		i7dan  = 670;
		i8dan  = 616;
		i9dan  = 570;
		i10dan = 530;
		i11dan = 496;
		i12dan = 466;
		i13dan = 440;
		i14dan = 416;
		i15dan = 0;
		SaveDan(5);
	}
	if(gxhWeightPearType == 2)	//15kg
	{
		i1dan  = 1274;
		i2dan  = 1090;
		i3dan  = 952;
		i4dan  = 846;
		i5dan  = 744;
		i6dan  = 662;
		i7dan  = 598;
		i8dan  = 542;
		i9dan  = 498;
		i10dan = 454;
		i11dan = 416;
		i12dan = 384;
		i13dan = 358;
		i14dan = 334;
		i15dan = 0;
		SaveDan(5);
	}
  } // current type ==5
 //배 
//>>

//한라봉 
	if(gxhCurrentType==6)	//0212
      {
	  i1dan  = 952;
		i2dan  = 872;
		i3dan  = 792;
		i4dan  = 712;
		i5dan  = 560;
		i6dan  = 480;
		i7dan  = 415;
		i8dan  = 365;
		i9dan  = 325;
		i10dan = 290;
		i11dan = 265;
		i12dan = 245;
		i13dan = 220;
		i14dan = 210;
		i15dan = 198;
		i16dan = 186;
		i17dan = 175;
		i18dan = 165;
		i19dan = 155;
		i20dan = 145;
		i21dan = 135;
		i22dan = 125;
		i23dan = 115;
		i24dan = 105;
		i25dan = 0;
		SaveDan(6);
	   /*
		i1dan  = 952;
		i2dan  = 872;
		i3dan  = 792;
		i4dan  = 712;
		i5dan  = 560;
		i6dan  = 480;
		i7dan  = 415;
		i8dan  = 365;
		i9dan  = 325;
		i10dan = 290;
		i11dan = 265;
		i12dan = 245;
		i13dan = 220;
		i14dan = 210;
		i15dan = 198;
		i16dan = 186;
		i17dan = 175;
		i18dan = 165;
		i19dan = 0;
	   SaveDan(6);
	   */
	  
	  } // current type==6
//한라봉 

//레드향
   if(gxhCurrentType==7)
     {
 
			 
		i1dan  = 952;
		i2dan  = 872;
		i3dan  = 792;
		i4dan  = 712;
		i5dan  = 560;
		i6dan  = 480;
		i7dan  = 415;
		i8dan  = 365;
		i9dan  = 325;
		i10dan = 290;
		i11dan = 265;
		i12dan = 245;
		i13dan = 220;
		i14dan = 210;
		i15dan = 198;
		i16dan = 186;
		i17dan = 175;
		i18dan = 165;
		i19dan = 0;
		SaveDan(7);
	 }
//특작
	 
	 // 특장 초기등급값(18개)
//    String special_default_3             ="9A/00280/00270/00260/00250/00240/00230/00220/00210/00200/00190/00180/00170/00160/00150/00140/00130/00120/00110";    //   1 : jadoo       A : POJANG  10 kg
//    String special_melon_default_3      ="9B/03000/02500/01900/01450/01200/01000/00000/00000/00000/00000/00000/00000/00000/00000/00000/00000/00000/00000";    //   1 : jadoo       A : POJANG  10 kg
//    String special_daebong_default_10   ="9C/00428/00358/00333/00305/00273/00255/00230/00217/00200/00189/00175/00168/00157/00151/00145/00100/00000/00000";    //   1 : jadoo       A : POJANG  10 kg
 //   String special_papurica_default_15  ="9D/00301/00251/00180/00145/00120/00100/00000/00000/00000/00000/00000/00000/00000/00000/00000/00000/00000/00000";    //   1 : jadoo       A : POJANG  10 kg

    if(gxhCurrentType==8)
     {
			if(gxhWeightSpecialType==1) // 특작 3 kg 
      {				
		  	i1dan  = 280;
		    i2dan  = 270;
		    i3dan  = 260;
		    i4dan  = 250;
		   i5dan  = 240;
		   i6dan  = 230;
		   i7dan  = 220;
		   i8dan  = 210;
		   i9dan  = 200;
		   i10dan = 190;
		   i11dan = 180;
		  i12dan = 170;
		  i13dan = 160;
		  i14dan = 150;
		  i15dan = 140;
		  i16dan = 130;
		  i17dan = 120;
		  i18dan = 110;
		  i19dan = 0;
	 	  SaveDan(8);
	    }
		//9B/03000/02500/01900/01450/01200/01000/00000/00000/00000/00000/00000/00000/00000/00000/00000/00000/00000/00000";	
  else if(gxhWeightSpecialType==2) // 특작  메론 3 kg 
      {				
		  	i1dan  = 3000;
		    i2dan  = 2500;
		    i3dan  = 1900;
		    i4dan  = 1450;
		   i5dan  =  1200;
		   i6dan  =  1000;
		   i7dan  =  0;
		   i8dan  =  0;
		   i9dan  =  0;
		   i10dan =  0;
		   i11dan =  0;
		  i12dan =  0;
		  i13dan =  0;
		  i14dan =  0;
		  i15dan =  0;
		  i16dan =  0;
		  i17dan =  0;
		  i18dan =  0;
		  i19dan = 0;
	 	  SaveDan(8);
	    }
	//		9C/00428/00358/00333/00305/00273/00255/00230/00217/00200/00189/00175/00168/00157/00151/00145/00100/00000/00000";
	else if(gxhWeightSpecialType==3) // 특작  대봉  10 kg 
      {				
		  	i1dan  = 428;
		    i2dan  = 358;
		    i3dan  = 333;
		    i4dan  = 305;
		   i5dan  =  273;
		   i6dan  =  255;
		   i7dan  =  230;
		   i8dan  =  217;
		   i9dan  =  200;
		   i10dan =  189;
		   i11dan =  175;
		  i12dan =  168;
		  i13dan =  157;
		  i14dan =  151;
		  i15dan =  145;
		  i16dan =  100;
		  i17dan =  0;
		  i18dan =  0;
		  i19dan = 0;
	 	  SaveDan(8);
	    }
	else if(gxhWeightSpecialType==4) // 특작  파프리카   15 kg 
      {
	      i1dan  = 301;
		    i2dan  = 251;
		    i3dan  = 180;
		    i4dan  = 145;
		   i5dan  =  120;
		   i6dan  =  100;
		   i7dan  =  0;
		   i8dan  =  0;
		   i9dan  =  0;
		   i10dan =  0;
		   i11dan =  0;
		  i12dan =  0;
		  i13dan =  0;
		  i14dan =  0;
		  i15dan =  0;
		  i16dan =  0;
		  i17dan =  0;
		  i18dan =  0;
		  i19dan = 0;
	 	  SaveDan(8);
	    				
		  }
	
			}
  // 특작 
	//"AA/03400/03200/02916/02250/01833/01547/01339/01180/01055/00945/00871/00801/00741/00690/00645/00100";	
	//레드향
   if(gxhCurrentType==9) // 단호박 
     {
 
			 
		i1dan  =3400;
		i2dan  =3200;
		i3dan  =2916;
		i4dan  =2250;
		i5dan  = 1833;
		i6dan  = 1547;
		i7dan  = 1339;
		i8dan  = 1180;
		i9dan  = 1055;
		i10dan = 945;
		i11dan = 871;
		i12dan = 801;
		i13dan = 741;
		i14dan = 690;
		i15dan = 645;
		i16dan = 100;
		i17dan = 0;
		SaveDan(7);
	 }	
		



}
 
 
void Init_Dan(void)

{
	u8 bPEACH = TRUE;
	u8 bHUNDRED = FALSE;
	u8 bTEN = FALSE;
	u8 bONE = FALSE;
	int	 i;
  u8 key_value;
	//	if (keypush())  //0329
	{
		
				vputs("INIT ");
				do
				{
					 key_value=keypush();
					if (key_value)
					{
						switch (key_value)
						{
							case SET_K: break; //등급키
							
							case HUNDRED_K:
								if (bPEACH == TRUE && bHUNDRED == FALSE && bTEN == FALSE && bONE == FALSE)
								{
									bHUNDRED = TRUE;
									vputs("   - ");
								}
								else bPEACH = bHUNDRED = bTEN = bONE = FALSE;
								break;
							case TEN_K:
								if (bPEACH == TRUE && bHUNDRED == TRUE && bTEN == FALSE && bONE == FALSE)
								{
									bTEN = TRUE;
									vputs("  -- ");
								}
								else bPEACH = bHUNDRED = bTEN = bONE = FALSE;
								break;
							case ONE_K:
								if (bPEACH == TRUE && bHUNDRED == TRUE && bTEN == TRUE && bONE == FALSE)
								{
									bONE = TRUE;
									vputs(" --- "); //0329

									gxhWeightGrapeType = 2;
									gxhWeightGrapeType=EE_Write_val(ADR_gxhWeightGrapeType , gxhWeightGrapeType , 4);
									gxhWeightJado1Type = 1;
									gxhWeightJado1Type=EE_Write_val(ADR_gxhWeightJado1Type , gxhWeightJado1Type , 4);
								
									gxhWeightJado2Type = 1;
									gxhWeightJado2Type=EE_Write_val(ADR_gxhWeightJado2Type , gxhWeightJado2Type , 4);
								
									gxhWeightPearType  = 2;
									gxhWeightPearType=EE_Write_val(ADR_gxhWeightPearType , gxhWeightPearType , 4);
								
									gxhWeightAppleType = 3;
									gxhWeightAppleType=EE_Write_val(ADR_gxhWeightAppleType , gxhWeightAppleType , 4);
								
									gxhWeightPeackType = 1;
									gxhWeightPeackType=EE_Write_val(ADR_gxhWeightPeackType , gxhWeightPeackType , 4);
								
									
									vputs("     ");

									DefaultDanSetting();
									vputs("GOOD ");

								}
								else bPEACH = bHUNDRED = bTEN = bONE = FALSE;
								break;
							default:
								break;
						}
					}
				} while(key_value != SET_K);//등급키
					
	} /* of if (keypush()) */
} /* of void mode_check() */


void Dansu_Led_Blink(void)
{
/*	
	DANSU_LED_ON;
	Delay_ms(500);
	DANSU_LED_OFF;
	Delay_ms(500);
	
 */
	if(hold_key_timer>500)
	  DANSU_LED_ON;
	if(hold_key_timer>1000)
	  {
	   DANSU_LED_OFF;
	  hold_key_timer=0;  
	  }
		
}	
void Pojang_Led_Blink(void)
{
	if(hold_key_timer>500)
	  POJANG_LED_ON;
	if(hold_key_timer>1000)
	  {
	  POJANG_LED_OFF;
	  hold_key_timer=0;  
	  }
}	


 void Bluetooth_Saving(void)    //20180607
 {
  #define  MAX_DAN 25
	  int i, j;
	  u8 temp[200];
	  u8 str[300];
	 u8 Input_wrong=0;
	//  u8 bluetooth_which_fruit; 
  //u8 bluetooth_which_pojang; 
	 
	// #ifdef dpfdpf
   bluetooth_which_fruit=RxBuffer4[0]; // which_fruit
	 bluetooth_which_pojang=RxBuffer4[1];// which_pojang
	 
	 
	 for(i=0;i<RxCounter4;i++)
		  USART1_putchar(RxBuffer4[i]); 
	 
	 
	 sprintf(str, "counter = %d", RxCounter4);	USART1_puts(str);
	 
	 //i1dan  RxBuffer4
//	  for(i=0;i< (3+MAX_DAN*5);i++)
// 포도,사과,배,감,등등 : 73
// 한라봉: 123
//레드량,특작: 93
//단호박: 83

 if(RxCounter4==73 || RxCounter4==123 ||RxCounter4==93||RxCounter4==83) // 날아온게 갯수가 맞지 않으면 
 {
	 RxCounter4=0;
    beep(200);
 for(i=0;i< (MAX_DAN*5);i++)
	 temp[i]=RxBuffer4[i+2]; // discarding 1A
	
	 i1dan=(temp[0]-0x30)*10000 +(temp[1]-0x30)*1000+(temp[2]-0x30)*100+(temp[3]-0x30)*10+(temp[4]-0x30);
	 i2dan=(temp[5]-0x30)*10000 +(temp[6]-0x30)*1000+(temp[7]-0x30)*100+(temp[8]-0x30)*10+(temp[9]-0x30);
	 i3dan=(temp[10]-0x30)*10000 +(temp[11]-0x30)*1000+(temp[12]-0x30)*100+(temp[13]-0x30)*10+(temp[14]-0x30);
	 i4dan=(temp[15]-0x30)*10000 +(temp[16]-0x30)*1000+(temp[17]-0x30)*100+(temp[18]-0x30)*10+(temp[19]-0x30);
	 i5dan=(temp[20]-0x30)*10000 +(temp[21]-0x30)*1000+(temp[22]-0x30)*100+(temp[23]-0x30)*10+(temp[24]-0x30);
	 i6dan=(temp[25]-0x30)*10000 +(temp[26]-0x30)*1000+(temp[27]-0x30)*100+(temp[28]-0x30)*10+(temp[29]-0x30);
	 i7dan=(temp[30]-0x30)*10000 +(temp[31]-0x30)*1000+(temp[32]-0x30)*100+(temp[33]-0x30)*10+(temp[34]-0x30);
	 i8dan=(temp[35]-0x30)*10000 +(temp[36]-0x30)*1000+(temp[37]-0x30)*100+(temp[38]-0x30)*10+(temp[39]-0x30);
	 i9dan=(temp[40]-0x30)*10000 +(temp[41]-0x30)*1000+(temp[42]-0x30)*100+(temp[43]-0x30)*10+(temp[44]-0x30);
	 i10dan=(temp[45]-0x30)*10000 +(temp[46]-0x30)*1000+(temp[47]-0x30)*100+(temp[48]-0x30)*10+(temp[49]-0x30);
	 i11dan=(temp[50]-0x30)*10000 +(temp[51]-0x30)*1000+(temp[52]-0x30)*100+(temp[53]-0x30)*10+(temp[54]-0x30);
	 i12dan=(temp[55]-0x30)*10000 +(temp[56]-0x30)*1000+(temp[57]-0x30)*100+(temp[58]-0x30)*10+(temp[59]-0x30);
	 i13dan=(temp[60]-0x30)*10000 +(temp[61]-0x30)*1000+(temp[62]-0x30)*100+(temp[63]-0x30)*10+(temp[64]-0x30);
	 i14dan=(temp[65]-0x30)*10000 +(temp[66]-0x30)*1000+(temp[67]-0x30)*100+(temp[68]-0x30)*10+(temp[69]-0x30);
	
   i15dan=(temp[70]-0x30)*10000 +(temp[71]-0x30)*1000+(temp[72]-0x30)*100+(temp[73]-0x30)*10+(temp[74]-0x30);
	 i16dan=(temp[75]-0x30)*10000 +(temp[76]-0x30)*1000+(temp[77]-0x30)*100+(temp[78]-0x30)*10+(temp[79]-0x30);
	 i17dan=(temp[80]-0x30)*10000 +(temp[81]-0x30)*1000+(temp[82]-0x30)*100+(temp[83]-0x30)*10+(temp[84]-0x30);
	 i18dan=(temp[85]-0x30)*10000 +(temp[86]-0x30)*1000+(temp[87]-0x30)*100+(temp[88]-0x30)*10+(temp[89]-0x30);
	 i19dan=(temp[90]-0x30)*10000 +(temp[91]-0x30)*1000+(temp[92]-0x30)*100+(temp[93]-0x30)*10+(temp[94]-0x30);
	 i20dan=(temp[95]-0x30)*10000 +(temp[96]-0x30)*1000+(temp[97]-0x30)*100+(temp[98]-0x30)*10+(temp[99]-0x30);
	 i21dan=(temp[100]-0x30)*10000 +(temp[101]-0x30)*1000+(temp[102]-0x30)*100+(temp[103]-0x30)*10+(temp[104]-0x30);
	 
	 i22dan=(temp[105]-0x30)*10000 +(temp[106]-0x30)*1000+(temp[107]-0x30)*100+(temp[108]-0x30)*10+(temp[109]-0x30);
	 
	 i23dan=(temp[110]-0x30)*10000 +(temp[111]-0x30)*1000+(temp[112]-0x30)*100+(temp[113]-0x30)*10+(temp[114]-0x30);
	 
	 i24dan=(temp[115]-0x30)*10000 +(temp[116]-0x30)*1000+(temp[117]-0x30)*100+(temp[118]-0x30)*10+(temp[119]-0x30);
	 
	 i25dan=(temp[120]-0x30)*10000 +(temp[121]-0x30)*1000+(temp[122]-0x30)*100+(temp[123]-0x30)*10+(temp[124]-0x30);
	 
	 
	 switch(bluetooth_which_fruit)
	 {
     case '1': 
	    gxhCurrentType=0;
		 break;
		 
		 case '2': 
	    gxhCurrentType=1;
		 break;
		 case '3': 
	    gxhCurrentType=2;
		 break;
		 case '4': 
	    gxhCurrentType=3;
		 break;
		 case '5': 
	    gxhCurrentType=4;
		 break;
		 case '6': 
	    gxhCurrentType=5;
		 break;
		 case '7': 
	    gxhCurrentType=6;
		 break;
		 case '8': 
	    gxhCurrentType=7;
		 break;
		 case '9': 
	    gxhCurrentType=8;
		 break;
		 
		 case 'A': 
	    gxhCurrentType=9;
		 break;
		 
		 
		 default: break;
	 }
	 
	if(gxhCurrentType==0 || gxhCurrentType==1||gxhCurrentType==2 ||gxhCurrentType==3||gxhCurrentType==4 ||gxhCurrentType==5)
	{
		/*
		if(  (i14dan>i13dan) || (i13dan>i12dan)|| (i12dan>i11dan)|| (i11dan>i10dan)|| (i10dan>i9dan)|| (i9dan>i8dan)|| (i8dan>i7dan)
			|| (i7dan>i6dan)  || (i6dan>i5dan)|| (i5dan>i4dan)|| (i4dan>i3dan)|| (i3dan>i2dan)|| (i2dan>i1dan)  ) 
		
		  Input_wrong=1;
	  */
		if(i2dan>i1dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=2;}
    if(i3dan>i2dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=3;}
    if(i4dan>i3dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=4;}
	  if(i5dan>i4dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=5;}
	  if(i6dan>i5dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=6;}
	  if(i7dan>i6dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=7;}
	  if(i8dan>i7dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=8;}
	  if(i9dan>i8dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=9;}
	  if(i10dan>i9dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=10;}
	  if(i11dan>i10dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=11;}
	  if(i12dan>i11dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=12;}
	  if(i13dan>i12dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=13;}
	  if(i14dan>i13dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=14;}
	 
		
	
	}	
		
	else if(gxhCurrentType==6)// 한라봉
	{
		if(i2dan>i1dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=2;}
    if(i3dan>i2dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=3;}
    if(i4dan>i3dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=4;}
	  if(i5dan>i4dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=5;}
	  if(i6dan>i5dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=6;}
	  if(i7dan>i6dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=7;}
	  if(i8dan>i7dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=8;}
	  if(i9dan>i8dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=9;}
	  if(i10dan>i9dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=10;}
	  if(i11dan>i10dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=11;}
	  if(i12dan>i11dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=12;}
	  if(i13dan>i12dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=13;}
	  if(i14dan>i13dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=14;}
	  if(i15dan>i13dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=15;}
	  if(i16dan>i13dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=16;}
	  if(i17dan>i13dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=17;}
	  if(i18dan>i13dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=18;}
	  if(i19dan>i13dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=19;}
	  if(i20dan>i13dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=20;}
	  if(i21dan>i13dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=21;}
	  if(i22dan>i13dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=22;}
	  if(i23dan>i13dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=23;}
	  if(i24dan>i13dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=24;}
	 
		
		
		/*
		if( (i24dan>i23dan) ||(i23dan>i22dan) ||(i22dan>i21dan) ||(i21dan>i20dan) ||(i20dan>i19dan) ||(i19dan>i18dan)||(i18dan>i17dan)||(i17dan>i16dan)||(i16dan>i15dan) 
		   || (i14dan>i13dan) || (i13dan>i12dan)|| (i12dan>i11dan)|| (i11dan>i10dan)|| (i10dan>i9dan)|| (i9dan>i8dan)|| (i8dan>i7dan)
			|| (i7dan>i6dan)  || (i6dan>i5dan)|| (i5dan>i4dan)|| (i4dan>i3dan)|| (i3dan>i2dan)|| (i2dan>i1dan)  ) 
		   Input_wrong=1;
	  */
	}
	else if(gxhCurrentType==7 || gxhCurrentType==8)// 레드향 , 특작
	{
		if(i2dan>i1dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=2;}
    if(i3dan>i2dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=3;}
    if(i4dan>i3dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=4;}
	  if(i5dan>i4dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=5;}
	  if(i6dan>i5dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=6;}
	  if(i7dan>i6dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=7;}
	  if(i8dan>i7dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=8;}
	  if(i9dan>i8dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=9;}
	  if(i10dan>i9dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=10;}
	  if(i11dan>i10dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=11;}
	  if(i12dan>i11dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=12;}
	  if(i13dan>i12dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=13;}
	  if(i14dan>i13dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=14;}
	  if(i15dan>i13dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=15;}
	  if(i16dan>i13dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=16;}
	  if(i17dan>i13dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=17;}
	  if(i18dan>i13dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=18;}
	  
		/*
		if( (i18dan>i17dan)||(i17dan>i16dan)||(i16dan>i15dan) 
		   || (i14dan>i13dan) || (i13dan>i12dan)|| (i12dan>i11dan)|| (i11dan>i10dan)|| (i10dan>i9dan)|| (i9dan>i8dan)|| (i8dan>i7dan)
			|| (i7dan>i6dan)  || (i6dan>i5dan)|| (i5dan>i4dan)|| (i4dan>i3dan)|| (i3dan>i2dan)|| (i2dan>i1dan)  ) 
		   Input_wrong=1;
    */
		}

else if(gxhCurrentType==9)// 단호박
	{
		if(i2dan>i1dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=2;}
    if(i3dan>i2dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=3;}
    if(i4dan>i3dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=4;}
	  if(i5dan>i4dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=5;}
	  if(i6dan>i5dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=6;}
	  if(i7dan>i6dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=7;}
	  if(i8dan>i7dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=8;}
	  if(i9dan>i8dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=9;}
	  if(i10dan>i9dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=10;}
	  if(i11dan>i10dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=11;}
	  if(i12dan>i11dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=12;}
	  if(i13dan>i12dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=13;}
	  if(i14dan>i13dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=14;}
	  if(i15dan>i13dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=15;}
	  if(i16dan>i13dan){vputs("ERR 4"); Delay_ms(500); Input_wrong=16;}
	  /*
		if( (i16dan>i15dan) 
		   || (i14dan>i13dan) || (i13dan>i12dan)|| (i12dan>i11dan)|| (i11dan>i10dan)|| (i10dan>i9dan)|| (i9dan>i8dan)|| (i8dan>i7dan)
			|| (i7dan>i6dan)  || (i6dan>i5dan)|| (i5dan>i4dan)|| (i4dan>i3dan)|| (i3dan>i2dan)|| (i2dan>i1dan)  ) 
		   Input_wrong=1;
	*/
		}

	
		
	if( Input_wrong) //0502
  {
  vputs("ERROR");
  Voice_Play(173);//에러가 발생했습니다.
  Delay_ms(3000);
	Voice_Play(Input_wrong+29);//몇단
  Delay_ms(1000);
		
		
		
  Voice_Play(174);//등급설정을 다시해주세요
  Delay_ms(3000);
	//	Input_wrong=0;

	}		
	 
	 
	 
	 //20180424
	 
	 sprintf(str, "C-TYPE=%d %d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d-%d",gxhCurrentType, i1dan,i2dan,i3dan,i4dan,i5dan,i6dan,i7dan,i8dan,i9dan
		                                                          ,i10dan,i11dan,i12dan,i13dan,i14dan,i15dan  
	                                                            ,i16dan,i17dan,i18dan,i19dan,i20dan,i21dan
                                                              ,i22dan,i23dan,i24dan,i25dan);	USART1_puts(str);
 
	 
	 
// 0419
	if(!Input_wrong)// 입력된데이타가 정상일때
	{
	 switch(bluetooth_which_fruit)
	 {
     case '1': // grape
			 switch(bluetooth_which_pojang)
			 {
				 case 'A': // 4 kg pojang 
				     			 gxhWeightGrapeType=1; // pojang 4 kg 
						       gxhWeightGrapeType = EE_Write_val(ADR_gxhWeightGrapeType, gxhWeightGrapeType, 4);	  Delay_ms(10);
						        gxhCurrentType=0;
				            AllFruitLed_Off();
        				   GRAPE_LED_ON;
							    gxhCurrentType = EE_Write_val(ADR_gxhCurrentType, gxhCurrentType, 4);	//add 2016.2.23
				 
				          Voice_Play(VOICE_TYPE1);// 포도 
				          Delay_ms(2000);
				          Pojang_Display_Voice();  //0420  4 KG 포장 
				          Delay_ms(2000);
				         Voice_Play(178);//등급설정이 완료되었습니다.
				          i15dan=0;
				          SaveDan(0); // 
				          USART1_puts("grape 4 kg pojang saved !");
					  break;

				 case 'B': // 5 kg pojang 
              	
             	      gxhWeightGrapeType=2; // pojang 5 kg 
						        gxhWeightGrapeType = EE_Write_val(ADR_gxhWeightGrapeType, gxhWeightGrapeType, 4);	  Delay_ms(10);
						        gxhCurrentType=0;
				            AllFruitLed_Off();
        				   GRAPE_LED_ON;
							     gxhCurrentType = EE_Write_val(ADR_gxhCurrentType, gxhCurrentType, 4);	//add 2016.2.23
				           Voice_Play(VOICE_TYPE1);// 포도 
				           Delay_ms(2000);
				           Pojang_Display_Voice();  //0420  5 KG 포장 
				           Delay_ms(2000);
				          Voice_Play(178);//등급설정이 완료되었습니다.
				          i15dan=0;
				          SaveDan(0); // 
				          USART1_puts("grape 5 kg pojang !");
					  break;
        
				 case 'C': // 10 kg pojang 
                  	 gxhWeightGrapeType=3; // pojang 10 kg 
						        gxhWeightGrapeType = EE_Write_val(ADR_gxhWeightGrapeType, gxhWeightGrapeType, 4);	  Delay_ms(10);
						        gxhCurrentType=0;
				            AllFruitLed_Off();
        				   GRAPE_LED_ON;
							     gxhCurrentType = EE_Write_val(ADR_gxhCurrentType, gxhCurrentType, 4);	//add 2016.2.23
				           Voice_Play(VOICE_TYPE1);// 포도 
				           Delay_ms(2000);
				           Pojang_Display_Voice();  //0420  10 KG 포장 
				           Delay_ms(2000);
				          Voice_Play(178);//등급설정이 완료되었습니다.
				          i15dan=0;
				          SaveDan(0); // 
				          USART1_puts("grape 10 kg pojang !");
								
					  break;

	         defalut: break;
		 }				 
       break;
				 case '2': // 자두
                    gxhCurrentType=1;
				            AllFruitLed_Off();
        				   JADOO_LED_ON;
							     gxhCurrentType = EE_Write_val(ADR_gxhCurrentType, gxhCurrentType, 4);	//add 2016.2.23
				           Voice_Play(VOICE_TYPE2);// 자두
				           Delay_ms(2000);
				           Pojang_Display_Voice();  //0420  10 KG 포장 
				           Delay_ms(2000);
				          Voice_Play(178);//등급설정이 완료되었습니다.
				         i15dan=0; 
				         SaveDan(1); // 
				          USART1_puts("jadoo 10 kg pojang !");
		         break;

				 case'3': //복숭아
					 
				 switch(bluetooth_which_pojang)
			 {
				 case 'A': // 4.5 kg pojang 
				     			 gxhWeightPeackType=1; // pojang 4.5 kg 
						       gxhWeightPeackType = EE_Write_val(ADR_gxhWeightPeackType, gxhWeightPeackType, 4);	  Delay_ms(10);
						        gxhCurrentType=2;
				            AllFruitLed_Off();
        				   PEACH_LED_ON;
							    gxhCurrentType = EE_Write_val(ADR_gxhCurrentType, gxhCurrentType, 4);	//add 2016.2.23
				 
				          Voice_Play(VOICE_TYPE3);// 복숭아 
				          Delay_ms(2000);
				          Pojang_Display_Voice();  //0420  4.5 KG 포장 
				          Delay_ms(2000);
				         Voice_Play(178);//등급설정이 완료되었습니다.
				          i15dan=0;
				          SaveDan(2); // 
				          USART1_puts("peach  4.5 kg pojang saved !");
					  break;

				 case 'B': // 5 kg pojang

                 gxhWeightPeackType=2; // pojang 5 kg 
						       gxhWeightPeackType = EE_Write_val(ADR_gxhWeightPeackType, gxhWeightPeackType, 4);	  Delay_ms(10);
						        gxhCurrentType=2;
				            AllFruitLed_Off();
        				   PEACH_LED_ON;
							    gxhCurrentType = EE_Write_val(ADR_gxhCurrentType, gxhCurrentType, 4);	//add 2016.2.23
				 
				          Voice_Play(VOICE_TYPE3);// 복숭아 
				          Delay_ms(2000);
				          Pojang_Display_Voice();  //0420  5 KG 포장 
				          Delay_ms(2000);
				         Voice_Play(178);//등급설정이 완료되었습니다.
				          i15dan=0;
				          SaveDan(2); // 
				          USART1_puts("peach  5 kg pojang saved !");

				 break;
        
				 case 'C': // 10 kg pojang 
                    gxhWeightPeackType=3; // pojang 10 kg 
						       gxhWeightPeackType = EE_Write_val(ADR_gxhWeightPeackType, gxhWeightPeackType, 4);	  Delay_ms(10);
						        gxhCurrentType=2;
				            AllFruitLed_Off();
        				   PEACH_LED_ON;
							    gxhCurrentType = EE_Write_val(ADR_gxhCurrentType, gxhCurrentType, 4);	//add 2016.2.23
				 
				          Voice_Play(VOICE_TYPE3);// 복숭아 
				          Delay_ms(2000);
				          Pojang_Display_Voice();  //0420  10 KG 포장 
				          Delay_ms(2000);
				         Voice_Play(178);//등급설정이 완료되었습니다.
				          i15dan=0;
				          SaveDan(2); // 
				          USART1_puts("peach  10 kg pojang saved !");
				 
					  break;

				 case 'D': // 15 kg pojang 
					        gxhWeightPeackType=4; // pojang 5 kg 
						       gxhWeightPeackType = EE_Write_val(ADR_gxhWeightPeackType, gxhWeightPeackType, 4);	  Delay_ms(10);
						        gxhCurrentType=2;
				            AllFruitLed_Off();
        				   PEACH_LED_ON;
							    gxhCurrentType = EE_Write_val(ADR_gxhCurrentType, gxhCurrentType, 4);	//add 2016.2.23
				 
				          Voice_Play(VOICE_TYPE3);// 복숭아 
				          Delay_ms(2000);
				          Pojang_Display_Voice();  //0420  15 KG 포장 
				          Delay_ms(2000);
				         Voice_Play(178);//등급설정이 완료되었습니다.
				         i15dan=0;
				          SaveDan(2); // 
				          USART1_puts("peach  15 kg pojang saved !");
  
				   break;

				 
	         default: break;
		 }				 
			 
				break; 

		 
				 case '4': // 사과
					 
				  switch(bluetooth_which_pojang)
			 {
				 case 'A': // 5 kg pojang 
				     			 gxhWeightAppleType=1; // pojang 4.5 kg 
						       gxhWeightAppleType = EE_Write_val(ADR_gxhWeightAppleType, gxhWeightAppleType, 4);	  Delay_ms(10);
						        gxhCurrentType=3;
				            AllFruitLed_Off();
        				   APPLE_LED_ON;
							     gxhCurrentType = EE_Write_val(ADR_gxhCurrentType, gxhCurrentType, 4);	//add 2016.2.23
				 
				          Voice_Play(VOICE_TYPE4);// 사과 
				          Delay_ms(2000);
				          Pojang_Display_Voice();  //0420  5 KG 포장 
				          Delay_ms(2000);
				         Voice_Play(178);//등급설정이 완료되었습니다.
				         i15dan=0;
				          SaveDan(3); // 
				          USART1_puts(" apple  5 kg pojang saved !");
					  break;

				 case 'B': // 5 kg pojang
       
				           gxhWeightAppleType=2; // pojang 10 kg 
						       gxhWeightAppleType = EE_Write_val(ADR_gxhWeightAppleType, gxhWeightAppleType, 4);	  Delay_ms(10);
						        gxhCurrentType=3;
				            AllFruitLed_Off();
        				   APPLE_LED_ON;
							     gxhCurrentType = EE_Write_val(ADR_gxhCurrentType, gxhCurrentType, 4);	//add 2016.2.23
				 
				          Voice_Play(VOICE_TYPE4);// 사과 
				          Delay_ms(2000);
				          Pojang_Display_Voice();  //0420  10 KG 포장 
				          Delay_ms(2000);
				         Voice_Play(178);//등급설정이 완료되었습니다.
				         i15dan=0;
				          SaveDan(3); // 
				          USART1_puts(" apple10  kg pojang saved !");
				 break;
        
				 case 'C': // 10 kg pojang 
       
				          gxhWeightAppleType=3; // pojang 4.5 kg 
						       gxhWeightAppleType = EE_Write_val(ADR_gxhWeightAppleType, gxhWeightAppleType, 4);	  Delay_ms(10);
						        gxhCurrentType=3;
				            AllFruitLed_Off();
        				   APPLE_LED_ON;
							     gxhCurrentType = EE_Write_val(ADR_gxhCurrentType, gxhCurrentType, 4);	//add 2016.2.23
				 
				          Voice_Play(VOICE_TYPE4);// 사과 
				          Delay_ms(2000);
				          Pojang_Display_Voice();  //0420  15 KG 포장 
				          Delay_ms(2000);
				         Voice_Play(178);//등급설정이 완료되었습니다.
				          i15dan=0;
				          SaveDan(3); // 
				          USART1_puts(" apple  15 kg pojang saved !");
					  break;
	         default: break;
		 }				 
			 
					 
				 break;
		 
				 case '5': // 감  20180424
					         gxhCurrentType=4; // 감
				            AllFruitLed_Off();
        				   GAM_LED_ON;
							     gxhCurrentType = EE_Write_val(ADR_gxhCurrentType, gxhCurrentType, 4);	//add 2016.2.23
				 
				          Voice_Play(VOICE_TYPE5);// 감 
				          Delay_ms(2000);
				          Pojang_Display_Voice();  //0420  15 KG 포장 
				          Delay_ms(2000);
				         Voice_Play(178);//등급설정이 완료되었습니다.
				         i15dan=0;
				          SaveDan(4); // 
				          USART1_puts("gam 10 kg pojang saved !");
					 
				 break;
		
         case '6': // 배
					   switch(bluetooth_which_pojang)
			 {
				 case 'A': // 7.5 kg pojang 
				     			 gxhWeightPearType=1; // pojang 4.5 kg 
						       gxhWeightPearType = EE_Write_val(ADR_gxhWeightPearType, gxhWeightPearType, 4);	  Delay_ms(10);
						        gxhCurrentType=5;
				            AllFruitLed_Off();
        				   PEAR_LED_ON;
							     gxhCurrentType = EE_Write_val(ADR_gxhCurrentType, gxhCurrentType, 4);	//add 2016.2.23
				 
				          Voice_Play(VOICE_TYPE6);// 사과 
				          Delay_ms(2000);
				          Pojang_Display_Voice();  //0420  5 KG 포장 
				          Delay_ms(2000);
				         Voice_Play(178);//등급설정이 완료되었습니다.
				         i15dan=0;
				          SaveDan(5); // 
				          USART1_puts("pear  7.5 kg pojang saved !");
					  break;

				 case 'B': // 15 kg pojang
       
				           gxhWeightPearType=2; // pojang 10 kg 
						       gxhWeightPearType = EE_Write_val(ADR_gxhWeightPearType, gxhWeightPearType, 4);	  Delay_ms(10);
						        gxhCurrentType=5;
				            AllFruitLed_Off();
        				   PEAR_LED_ON;
							     gxhCurrentType = EE_Write_val(ADR_gxhCurrentType, gxhCurrentType, 4);	//add 2016.2.23
				 
				          Voice_Play(VOICE_TYPE6);// 사과 
				          Delay_ms(2000);
				          Pojang_Display_Voice();  //0420  10 KG 포장 
				          Delay_ms(2000);
				         Voice_Play(178);//등급설정이 완료되었습니다.
				          i15dan=0;
				          SaveDan(5); // 
				          USART1_puts(" pear 15  kg pojang saved !");
				 break;
        
				   default: break;
		 }				 
			 
				 break;

				 case '7': //한라봉
				         gxhCurrentType=6; // 한라봉
				         AllFruitLed_Off();
        				  HALABONG_LED_ON;
							    gxhCurrentType = EE_Write_val(ADR_gxhCurrentType, gxhCurrentType, 4);	//add 2016.2.23
				          Voice_Play(VOICE_TYPE7);// 감 
				          Delay_ms(2000);
				          Pojang_Display_Voice();  //0420  15 KG 포장 
				          Delay_ms(2000);
				         Voice_Play(178);//등급설정이 완료되었습니다.
				         i25dan=0;
				          SaveDan(6); // 
				          USART1_puts("halabong 3 kg pojang saved !");
					
				 break;
			case '8': //레드향
								 gxhCurrentType=7; //레드향
				         AllFruitLed_Off();
			          RED_LED_ON;
        				  //HALABONG_LED_ON;
							  
			            gxhCurrentType = EE_Write_val(ADR_gxhCurrentType, gxhCurrentType, 4);	//add 2016.2.23
				          Voice_Play(VOICE_TYPE8);// 레드향 
				          Delay_ms(2000);
				          Pojang_Display_Voice();  //0420  15 KG 포장 
				          Delay_ms(2000);
				         Voice_Play(178);//등급설정이 완료되었습니다.
			           i19dan=0;
				          SaveDan(7); // 
				          USART1_puts("redhyang 3 kg pojang saved !");
				 
				 break;
		 
				case '9': //특작
			  switch(bluetooth_which_pojang) //0426
			 {
				 case 'A': // 4.5 kg pojang 
				     			 gxhWeightSpecialType=1; // pojang 4.5 kg 
						       gxhWeightSpecialType = EE_Write_val(ADR_gxhWeightSpecialType, gxhWeightSpecialType, 4);	  Delay_ms(10);
						        gxhCurrentType=8;
				            AllFruitLed_Off();
        				   SPECIAL_LED_ON;
							   gxhCurrentType = EE_Write_val(ADR_gxhCurrentType, gxhCurrentType, 4);	//add 2016.2.23
				 
				          Voice_Play(VOICE_TYPE9);// 특작
				          Delay_ms(2000);
				          Pojang_Display_Voice();  //3 kg 
				          Delay_ms(2000);
				         Voice_Play(178);//등급설정이 완료되었습니다.
				          i19dan=0;
				          SaveDan(8); // 
				          USART1_puts("special 3 kg pojang saved !");
					  break;

				 case 'B': // 5 kg pojang

				 				    gxhWeightSpecialType=2; // pojang 4.5 kg 
						       gxhWeightSpecialType = EE_Write_val(ADR_gxhWeightSpecialType, gxhWeightSpecialType, 4);	  Delay_ms(10);
						        gxhCurrentType=8;
				            AllFruitLed_Off();
        				   SPECIAL_LED_ON;
							    gxhCurrentType = EE_Write_val(ADR_gxhCurrentType, gxhCurrentType, 4);	//add 2016.2.23
				 
				          Voice_Play(175);// 특작메론
				          Delay_ms(2000);
				          Pojang_Display_Voice();  //3 kg 
				          Delay_ms(2000);
				         Voice_Play(178);//등급설정이 완료되었습니다.
				           i19dan=0;
				          SaveDan(8); // 
				          USART1_puts("special melog  3 kg pojang saved !");

				 
				 break;
        
				 case 'C': // 10 kg pojang
                    gxhWeightSpecialType=3; // pojang 4.5 kg 
						       gxhWeightSpecialType = EE_Write_val(ADR_gxhWeightSpecialType, gxhWeightSpecialType, 4);	  Delay_ms(10);
						        gxhCurrentType=8;
				            AllFruitLed_Off();
        				   SPECIAL_LED_ON;
							    gxhCurrentType = EE_Write_val(ADR_gxhCurrentType, gxhCurrentType, 4);	//add 2016.2.23
				 				          Voice_Play(176);// 특작 대봉
				          Delay_ms(2000);
				          Pojang_Display_Voice();  //10 kg 
				          Delay_ms(2000);
				         Voice_Play(178);//등급설정이 완료되었습니다.
				          i19dan=0 ;
				          SaveDan(8); // 
				          USART1_puts("special  daebong   10 kg pojang saved !");
					 
					  break;

				 case 'D': // 15 kg pojang
                   gxhWeightSpecialType=4; // pojang 4.5 kg 
						       gxhWeightSpecialType = EE_Write_val(ADR_gxhWeightSpecialType, gxhWeightSpecialType, 4);	  Delay_ms(10);
						       gxhCurrentType=8;
				           AllFruitLed_Off();
        				  SPECIAL_LED_ON;
							    gxhCurrentType = EE_Write_val(ADR_gxhCurrentType, gxhCurrentType, 4);	//add 2016.2.23
				 				   Voice_Play(177);// 특작 파브리카
				          Delay_ms(2000);
				          Pojang_Display_Voice();  //10 kg 
				          Delay_ms(2000);
				         Voice_Play(178);//등급설정이 완료되었습니다.
				          i19dan=0;
				          SaveDan(8); // 
				          USART1_puts("special  papurica   15 kg pojang saved !");
				   break;
       default: break;
		 }	
      break;

				case 'A': //단호박
					        gxhCurrentType=9; // 한라봉
				         AllFruitLed_Off();
        				  PUMPKIN_LED_ON;
				          gxhCurrentType=9;
							    gxhCurrentType = EE_Write_val(ADR_gxhCurrentType, gxhCurrentType, 4);	//add 2016.2.23
				          Voice_Play(16);// 단호박  
				          Delay_ms(2000);
				          Pojang_Display_Voice();  //0420  15 KG 포장 
				          Delay_ms(2000);
				         Voice_Play(178);//등급설정이 완료되었습니다.
				          i17dan=0;
				          SaveDan(9); // 
				          USART1_puts("PUMPKIN 3 kg pojang saved !");
				 break;
				 default: break;


	 }		 
	 
 } 
	 
	// #endif
}	 
 FINISH_SAVE:
	 
 		  for(i=0;i<RxCounter4;i++)
		  USART1_putchar(RxBuffer4[i]);

   RxCounter4=0; 
 }	




	
	
//}	
	
//hold_key_timer


 
/* END OF FILE */
