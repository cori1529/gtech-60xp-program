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


// ����ħ�� 
#define ZERO_KEY		 1	  //  ����Ű
#define START_KEY		 2	  //  ����Ű 
#define HOLD_KEY		 3	  //  Ȧ��Ű 
#define RESET_KEY		 4	  //  ����Ű 
#define SAVE_KEY		 5	  //  ����Ű 
// ����ħ�� 

// �����е�  201702
#define RIGHT_ARROW      1  //ZERO_��KEY	//	 1	  //  ����Ű
#define LEFT_ARROW       2  //START_KEY	//	 2	  //  ����Ű 
#define CLEAR            3  //HOLD_KEY	//	 3	  //  Ȧ��Ű 
#define FUNCTOIN_KEY     4  // RESET_KEY	//	 4	  //  ����Ű 
#define ENTER_KEY        5  // SAVE_KEY	//	 5	  //  ����Ű 
// �����е� 


/************  GP-60XP KEY ****************************/


#define   POJANG_K         15      // ����Ű
#define   WHICH_FRUIT_K    12      // �������� Ű

#define   HUNDRED_K		    9          // 100 Ű     
#define   TEN_K			      14       //  10 Ű
#define   ONE_K			      13       //   1 Ű
#define   GRADE_K         6        // ����Ű
#define   WHICH_GRAED_K   10       // �������
#define   GRAED_SPEED_K    11       // �����ӵ�
#define   WEIGH_K         7      //  ����
#define   SULJUNG_K        8       // ����Ű
#define   DEUNGUB_K        3       // ����Ű
#define   ZERO_K          4       // ����Ű

#define   POWER_K          5      // ����ŰŰ
#define   VOICEUP_K        1       //���� UP Ű
#define   VOICEDOWN_K      2          //���� DOWN Ű

#define   COUNT_K         DEUNGUB_K
#define	  RIGHT_K			    SULJUNG_K
#define	  SET_K			        GRADE_K
#define   CLEAR_K			      WEIGH_K
#define   EXIT_K            SET_K
#define   ESC_K             CLEAR_K
#define   TEST_K            SET_K


// ������ �Է½� ���� ���õǾ��� TYPE �� �����̳�?
// ����: ����Ű�� �������¿��� ������Ű�� ������ �������� ����
//�ű� : ����Ű�� 2���̻� ������ ������ ���� ���õǾ��� ���������� �´� ������ �Ѵ�.

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


//���ϼ���
//����: ���Ը�忡��  �ش����Ű�� 2���̻� ������ ������  ����
//�ű� : ���� ����Ű�� ������ ���� ������� ���õǾ�

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
