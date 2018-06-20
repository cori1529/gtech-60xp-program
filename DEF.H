/*---------------------------------------------------*
   Filename  :  def.h
   Date      :  Select  System
   Version   :  1.00.000 (2000. 12.  1)
			 -	set_type4, set_type5, set_type6
			 -  grape_k, jado1_k, jado2_k, pear_k, apple_k, peack_k,
			 -  key code change
			 -  voice_type4, voice_type5, voice_type6,
   Version   :  1.10.000 (1997. 12.  8)
			 - move to adc.h CONTROL_REG
			 - move to adc.h DATA_REG
			 - move to adc.h ADPERSEC
   Version   :  1.00.000 (1997. 03. 21)
			 - COMPILER  IAR 4.20

   Copyright (c) 2001-2002 by G-TECH
   All Rights Reserved.
 *---------------------------------------------------*/
/*
		DEF.H

	Ver 1.10 (1997. 12. 8)
	 - move to adc.h CONTROL_REG
	 - move to adc.h DATA_REG
	 - move to adc.h ADPERSEC
	VER 1.00 - 1997. 3. 21   GI-8500
	  1. COMPILER  IAR 4.20

	Copyright (c) 1997 by G-TECH
	All Rights Reserved.
*/

#ifndef __DEF_H__
#define __DEF_H__

typedef unsigned char 	BOOL;
typedef unsigned char 	byte;
typedef unsigned char 	BYTE;
typedef unsigned int	UINT;
typedef unsigned int	WORD;
typedef unsigned long	DWORD;

#define TRUE            1
#define FALSE           0

#define  HIDE		0
#define  SHOW		!HIDE

/* CAL & SET MENU */
#define HOW_MANY_CAL    7
#define HOW_MANY_TEST   6

/*----------- KEY ---------*/

/* ST3
#define	ZERO_K			0x0001
#define	GRAPE_K			0x0002
#define	PEACK_K			0x0004
#define DEUNGUB_K       0x0008
#define	JADO1_K			0x0010
#define	TYPE1_K			0x0020
#define	WEIGH_K			0x0040
#define	JADO2_K			0x0080
#define	TYPE2_K			0x0100
#define	GRADE_K			0x0200
#define	PEAR_K			0x0400
#define	TYPE3_K			0x0800
#define	ONOFF_K			0x1000
#define	APPLE_K			0x2000
#define	SULJUNG_K		0x4000

#define SETTYPE1_K		SULJUNG_K+GRAPE_K
#define SETTYPE2_K		SULJUNG_K+JADO1_K
#define SETTYPE3_K		SULJUNG_K+JADO2_K
#define SETTYPE4_K		SULJUNG_K+PEAR_K
#define SETTYPE5_K		SULJUNG_K+APPLE_K
#define SETTYPE6_K		SULJUNG_K+PEACK_K

#define STORE_K         ONOFF_K+ZERO_K
#define COUNT_K         DEUNGUB_K

#define	RIGHT_K			SULJUNG_K
#define HUNDRED_K		TYPE1_K
#define TEN_K			TYPE2_K
#define ONE_K			TYPE3_K
*/

//ST5
#define	HANLABONG_K		0x0001
#define	APPLE_K			0x0002
#define	GRAPE_K			0x0003

#define	REDHAYNG_K		0x0004
#define	GAM_K			0x0008
#define	JADOO_K			0x000C

#define	TEUKJAK_K		0x0010
#define	PEAR_K			0x0020
#define	PEACH_K			0x0030

#define	SPEED_K			0x0040
#define	GRADE_K			0x0080
#define	WEIGH_K			0x00C0

#define HUNDRED_K		0x0100
#define TEN_K			0x0200
#define ONE_K			0x0300

#define	ZERO_K			0x0400
#define DEUNGUB_K       0x0800
//#define DEUNGUB_K       0x00cc


#define	SULJUNG_K		0x0C00

#define SETTYPE1_K		SULJUNG_K+GRAPE_K
#define SETTYPE2_K		SULJUNG_K+JADOO_K
//#define SETTYPE3_K		SULJUNG_K+JADO2_K
#define SETTYPE3_K		SULJUNG_K+TEUKJAK_K
#define SETTYPE4_K		SULJUNG_K+PEAR_K
#define SETTYPE5_K		SULJUNG_K+APPLE_K
#define SETTYPE6_K		SULJUNG_K+PEACH_K

#define SETTYPE7_K		SULJUNG_K+HANLABONG_K
#define SETTYPE8_K		SULJUNG_K+REDHAYNG_K
#define SETTYPE9_K		SULJUNG_K+GAM_K

//#define STORE_K         ONOFF_K+ZERO_K
#define COUNT_K         DEUNGUB_K

#define	RIGHT_K			SULJUNG_K


#define N_DEF           0xFFFF
/*------------ VIRTUAL KEY ----------*/
//#define GAIN_DEC_K      NO_K

#define	SET_K			GRADE_K
#define CLEAR_K			WEIGH_K
#define EXIT_K          SET_K
#define ESC_K           CLEAR_K
#define TEST_K          SET_K

#define HALFDIGIT     10        /* half of minimum division */

#define FN_none       0         /* none function */


#define VHI					1
#define V60ST				2
#define	VOICE_GRADER		3
#define VOICE_MODE_GRADE	4
#define VOICE_WEIGH			5
#define	VMODEWEIGH			VOICE_WEIGH
#define	VOICE_SET   		6

#define	VOICE_TYPE1			7	//grape
#define	VOICE_TYPE2			8	//jadu

#define	VOICE_TYPE3			9	//복숭아 
#define	VOICE_TYPE4			10	//사과 
#define	VOICE_TYPE5			11	//감 
#define	VOICE_TYPE6			12	//배 
#define	VOICE_TYPE7			13	//hanlabong
#define	VOICE_TYPE8			14	//redhyang
#define	VOICE_TYPE9			15	//특작 

#define	VOICE_P2			16
#define	VOICE_P4			17
#define	VOICE_P45			18
#define	VOICE_P5			19
#define	VOICE_P75			20
#define	VOICE_P10			21
#define	VOICE_P15			22
#define	VOICE_1				23
#define	VOICE_2				24
#define	VOICE_3				25
#define	VOICE_4				26
#define	VOICE_5				27
#define	VOICE_6 			28
#define	VOICE_7 			29
#define	VOICE_8 			30
#define	VOICE_9 			31
#define	VOICE_10			32
#define	VOICE_11			33
#define	VOICE_12    		34
#define	VOICE_13    		35
#define	VOICE_14    		36						  //DAN_MAX+24
													  //DAN_MAX+48
#define	VOICE_15    		37
#define	VOICE_16    		38
#define	VOICE_17    		39
#define	VOICE_18    		40
#define	VOICE_19    		41
#define	VOICE_TING  		42
#define	VOICE_NOP	  		43

#define	VOICE_SPEED   		47
//#define	VOICE_BOTONG   		44
#define	VOICE_P3   		44


#define	BOJUNG    		 45
#define	BOJUNG_RELEASE   46


#define	VOICE_20    		47
#define	VOICE_21    		48
#define	VOICE_22    		49
#define	VOICE_23    		50
#define	VOICE_24    		51
#define	VOICE_25    		52









/************** function definition **************/
#define  buzzer_on()	BUZZER = 0   /* buzzer on  28.7 */
#define  buzzer_off()   BUZZER = 1   /* buzzer off 28.7 */

void	beep(UINT);
UINT	checkbyte(UINT);
void    cond_check(void);
void    delay(unsigned int);
void    normal_mode(void);
DWORD   power(int, byte);
void    retry_message(void);

/*------------- display (vfd, fip) interface  ----------------*/
void    cls(void);
void    fast_evalue(DWORD);
void    fip_int(void);
void    draw_dp(BYTE dp);
void    vputs(BYTE *str);

/*-------------  setting mode function  ----------------*/
void    cal_test_set(void);
void    set_mode(void);
void    Weight_Grape_Set_Mode(void);
void    Weight_Jado1_Set_Mode(void);
void    Weight_Jado2_Set_Mode(void);
void    Weight_Pear_Set_Mode(void);
void    Weight_Apple_Set_Mode(void);
void    Weight_Peack_Set_Mode(void);

/*-------------  test mode function  ----------------*/
void    test_mode(void);
void    key_test(void);
void    disp_test(void);
void    adc_test(void);
void    sram_test(void);
void 	voice_test(void);
void 	onoff_test(void);
void 	initial_adc(void);
//void    main(void);
/*----------------  key -----------------------*/
char	keypushdouble(void);
char    keypush(void);
char 	keypushanbbik(void);
int		GetKeyCode(void);

void    KEYINENC(void);
void    CHKEYIN(void);

DWORD dwValueSet(BYTE *str, BYTE flag, DWORD init_value, DWORD limit, DWORD maxdigit);
void	factor_access(void);


void DefaultDanSetting(void);

void Current_Type_DanSetting(void);	   // 현재 선택되어진 것만 값을 읽어오는거야 



void reboot();

void VoicePlay(unsigned char voice);
void VoiceStop();

#define	VOICE_SBT	P1_3
//#define VOICE(no)	{P3 &= 0x03; P3 |= (no << 2); }
#define VOICE(no)	{P3 &= 0x03;  P3 |= (no << 2); }


#endif  /*__DEF_H__*/
/*--------------------  end of file ---------------------*/