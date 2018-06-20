#ifndef	__KEY_H
#define	__KEY_H

#define KEY_POWER	'1'
#define KEY_DOOR	'2'
#define KEY_UP		'3'
#define KEY_DOWN	'4'
#define KEY_SET		'5'
#define KEY_ENTER		5
#define KEY_CANCEL		4
#define KEY_SETTING		3
#define KEY_TARE		2
#define KEY_ZERO		1


// 낙상침대 
#define ZERO_KEY		 1	  //  영점키
#define START_KEY		 2	  //  시작키 
#define HOLD_KEY		 3	  //  홀드키 
#define RESET_KEY		 4	  //  리셋키 
#define SAVE_KEY		 5	  //  절전키 
// 낙상침대 

// 낙상패드  201702
#define RIGHT_ARROW      1  //ZERO_ㅏKEY	//	 1	  //  영점키
#define LEFT_ARROW       2  //START_KEY	//	 2	  //  시작키 
#define CLEAR            3  //HOLD_KEY	//	 3	  //  홀드키 
#define FUNCTOIN_KEY     4  // RESET_KEY	//	 4	  //  리셋키 
#define ENTER_KEY        5  // SAVE_KEY	//	 5	  //  절전키 
// 낙상패드 


/************  GP-60XP KEY ****************************/


#define   POJANG_K         15      // 포장키
#define   WHICH_FRUIT_K    12      // 과일종류 키

#define   HUNDRED_K		    9          // 100 키     
#define   TEN_K			      14       //  10 키
#define   ONE_K			      13       //   1 키
#define   GRADE_K         6        // 선별키
#define   WHICH_GRAED_K   10       // 선별방법
#define   GRAED_SPEED_K    11       // 선별속도
#define   WEIGH_K         7      //  저울
#define   SULJUNG_K        8       // 설정키
#define   DEUNGUB_K        3       // 설정키
#define   ZERO_K          4       // 영점키

#define   POWER_K          5      // 전원키키
#define   VOICEUP_K        1       //음량 UP 키
#define   VOICEDOWN_K      2          //음량 DOWN 키

#define   COUNT_K         DEUNGUB_K
#define	  RIGHT_K			    SULJUNG_K
#define	  SET_K			        GRADE_K
#define   CLEAR_K			      WEIGH_K
#define   EXIT_K            SET_K
#define   ESC_K             CLEAR_K
#define   TEST_K            SET_K


// 설정값 입력시 현재 선택되어진 TYPE 이 무엇이냐?
// 기존: 설정키를 누른상태에서 각과일키를 눌러서 설정모드로 진입
//신규 : 설정키를 2초이상 누르고 있으면 현재 선택되어진 과일종류에 맞는 설정을 한다.

#define	GRAPE_TYPE              90			
#define	JADOO_TYPE			        91
#define	PEACH_TYPE			        92
#define	HALABONG_TYPE		        93 


#define	APPLE_TYPE			        94
#define	GAM_TYPE			          95
#define	PEAR_TYPE			          96
#define	REDHAYNG_TYPE		        97


#define	PUMKIN_TYPE		          98
#define	TEUKJAK_TYPE		        99


//과일선택
//기존: 무게모드에서  해당과일키를 2초이상 누르고 있으면  선택
//신규 : 과일 종류키를 누름에 따라 순서대로 선택되어

#define	GRAPE_SELECT              70			
#define	JADOO_SELECT			        71
#define	PEACH_SELECT			        72
#define	HALABONG_SELECT		        73 


#define	APPLE_SELECT			        74
#define	GAM_SELECT			          75
#define	PEAR_SELECT			          76
#define	REDHAYNG_SELECT		        77


#define	PUMKIN_SELECT		          78
#define	TEUKJAK_SELECT		        79







/************  GP-60XP KEY ****************************/

extern volatile int key_count;
extern volatile	u16	 KeyCode;
extern   u16   st_loc,st_loc_2,st_loc_3,st_loc_4,st_loc_5;
extern 	u16   st_scan;
extern unsigned char key_pressed;
extern  u16 st_value;

extern u8		qKEYIN;	  //  1: not key in      0: key in
extern u8		nKEYIN;	  //  1: new key         0: not new key
extern u8 	  rKEYIN;	  //  1: key release     0: not key release







void key_init(void);
void key_test(void);
void dip_sw_adress(void); // GI-1400 dip switch address input 
void sram_test(void);
void Voice_Play(int number);
void Voice_Data_Loading(int number);
void Voice_Reset(void); //0312

void  Potential_ZeroSet(void);
void  Potential_Increment_Set(void); 
void  Potential_Decrement_Set(void); 
void  Potential_Voice_Adjustment(char i);
void register_delay(int time);


//void dip_switch(void);

void CHKEYIN(void);
unsigned char keypush(void);	
unsigned char keypush_first(void);	






extern 	int device_address;
extern 	int  first_sw, second_sw,third_sw,fourth_sw, fifth_sw;


#endif	// __KEY_H

/* end of file */
