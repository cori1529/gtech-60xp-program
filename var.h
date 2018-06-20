/*
	var.h
*/

#ifndef ___VAR_H___
#define ___VAR_H___

#define SET_END		 	-100	//저장 안하고 SET MODE 종료
#define	SET_FORWARD		-101	//저장 안하고 다음 번호로
#define	SET_BACKWARD	-102	//저장 안하고 앞으로
#define	SET_AGAIN		-103	//다시
#define	SET_RESTART		-194	//처음부터 다시
#define SET_FUNCNO_1 	-201	//SET 모드 1이면 리턴값이 0이므로 -201로 변환해서 반환함
#define  PUSH_BUTTON_ON        if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0x00)
	

#define VHI					1
//#define V60ST				2
#define	VOICE_GRADER		2
#define VOICE_MODE_GRADE	3
#define VOICE_WEIGH			4
#define	VMODEWEIGH			VOICE_WEIGH
#define	VOICE_SET   		5

#define	VOICE_TYPE1			8	//grape
#define	VOICE_TYPE2			9	//jadu

#define	VOICE_TYPE3			10	//복숭아 
#define	VOICE_TYPE4			11	//사과 
#define	VOICE_TYPE5			12	//감 
#define	VOICE_TYPE6			13	//배 
#define	VOICE_TYPE7			14	//hanlabong
#define	VOICE_TYPE8			15	//redhyang
#define	VOICE_PUMKIN		16	//단호박



#define	VOICE_TYPE9			20	//특작 

#define	VOICE_P2			21
#define	VOICE_P25			22 // 2.5 포장
#define	VOICE_P3			23 // 3 포장
#define	VOICE_P4			24
#define	VOICE_P45			25
#define	VOICE_P5			26
#define	VOICE_P75			27
#define	VOICE_P10			28
#define	VOICE_P15			29

#define	VOICE_1				30
#define	VOICE_2				31
#define	VOICE_3				32
#define	VOICE_4				33
#define	VOICE_5				34
#define	VOICE_6 			35
#define	VOICE_7 			36
#define	VOICE_8 			37
#define	VOICE_9 			38
#define	VOICE_10			39
#define	VOICE_11			40
#define	VOICE_12    		41
#define	VOICE_13    		42
#define	VOICE_14    		43						  //DAN_MAX+24
													  //DAN_MAX+48
#define	VOICE_15    		44
#define	VOICE_16    		45
#define	VOICE_17    		46
#define	VOICE_18    		47
#define	VOICE_19    		48
#define	VOICE_20    		49
#define	VOICE_21    		50
#define	VOICE_22    		51
#define	VOICE_23    		52
#define	VOICE_24    		53
#define	VOICE_25    		54
#define	VOICE_26    		55
#define	VOICE_27    		56
#define	VOICE_28    		57
#define	VOICE_29    		58
#define	VOICE_30    		59
#define	VOICE_TING  		60

#define	VOICE_THANK  		61
#define	VOICE_POWEROFF  		62
#define	VOICE_SPEEDUP  		63
#define	VOICE_SPEEDDOWN		64


#define   BLE_ATCOMMAND      GPIO_SetBits(GPIOB, GPIO_Pin_15)
#define   BLE_DATAMODE         GPIO_ResetBits(GPIOB, GPIO_Pin_15)
 
//#//define	VOICE_NOP	  		50





//#define	VOICE_SPEED   		47
//#define	VOICE_BOTONG   		44
//#define	VOICE_P3   		44


//#define	BOJUNG    		 45
//#define	BOJUNG_RELEASE   46

/*
#define	VOICE_20    		47
#define	VOICE_21    		48
#define	VOICE_22    		49
#define	VOICE_23    		50
#define	VOICE_24    		51
#define	VOICE_25    		52
*/










extern float           gnfFactor;
extern unsigned long   gdwFactor;
extern unsigned long   gdwFactorFraction;	//가수부   sign(1) exponent(8) fraction(23)
extern unsigned long   gdwFactorexponent;	//지수부
extern float		       gnfEquFactor;
extern float		       gnfRefFactor;
extern float 		       gnfVoltPerMv;

extern u8  Fnd_rotation;
extern u16 Display_buffer[14];


/* usart.c */
extern volatile u16 guRS422485TxEnableTimer;
extern volatile u16 guCommLedOnTimer;

extern char TxBuf1[];
extern char TxBuf2[];
extern char TxBuf3[];
extern char TxBuf4[];

extern char USART1_DMA_send_enabled;
extern char USART2_DMA_send_enabled;
extern char USART3_DMA_send_enabled;
extern char UART4_DMA_send_enabled;

extern unsigned long ADvalue;
extern unsigned char ADi; 
extern unsigned char AD_NEG,AD_SPEED;

extern u16 key_timer;





//void Voice_Play(int number);

//extern  int device_address;
//extern 	int  first_sw, second_sw,third_sw,fourth_sw, fifth_sw;

 







#endif	//___VAR_H___

/* end of file */
