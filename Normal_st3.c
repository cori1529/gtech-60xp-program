/*  -----------------------------------------------------------
   Filename  :  normal.c

	Ver 2.00.000 (2007. 9. 3)
	 1. 음성 ic AP840N to aP8942A로 변경
	 2. STOP (T1 = 1; T1 =0;) 사이에 delay(6) 추가함.
	 
	Ver 1.00.000 (2005.5.20)
	 1. grape - 12dan   (4, 5, 10kg)
		jado1 - 5dan    (10kg) (daiseok)
		gam   - 5dan    (10kg)
		pear  - 12dan   (7.5, 15kg)
		apple - 12dan   (10)
		peach - 12dan   (5, 10, 15kg)


			   case  GRAPE_K:
							VoicePlay(VOICE_TYPE1); delay(10);
							gxhCurrentType = 0;
			   case  JADOO_K:
							VoicePlay(VOICE_TYPE2); delay(10);
							gxhCurrentType = 1;
			   case  PEAR_K:
							VoicePlay(VOICE_TYPE4); delay(10);
							gxhDanSetType =  2;
			   case  APPLE_K:
							VoicePlay(VOICE_TYPE5); delay(10);
							gxhDanSetType =  3;
				case GAM_K:
							VoicePlay(VOICE_TYPE9); delay(10);
							gxhCurrentType = 4;
			   	case  PEACH_K:
							VoicePlay(VOICE_TYPE6); delay(10);
							gxhDanSetType =  2;
							gxhCurrentType = 2;
				case HANLABONG_K:
							VoicePlay(VOICE_TYPE7); delay(10);
							gxhCurrentType = 6;
				case REDHAYNG_K:
							VoicePlay(VOICE_TYPE8); delay(10);
							gxhCurrentType = 7;
				case TEUKJAK_K:
							VoicePlay(VOICE_TYPE3); delay(10);
							gxhCurrentType = 8;




   -------------------------------------------------------------------------
   Version   :  1.01.000 (2002. 2. 20)
			 - Voice apple
			 - gxhWeightAppleType=1,gxhWeightAppleType=2,gxhWeightAppleType=3,

   Version   :  1.00.000 (2000. 12.  1)
			 - GRAPE_K, JADO1_K, JADO2_K
			   PEAR_K, APPLE_K, PEACK_K
			 - grape - 12dan   (4, 5, 10kg)
			   jado1 - 5dan    (10kg) (daiseok)
			   jado2 - 5dan    (10kg) (pomosa)
			   pear  - 12dan   (7.5, 15kg)
			   apple - 12dan   (10)
			   peack - 12dan   (5, 10, 15kg)
			 - voice
			   voice_grape
			   voice_jado1
			   voice_jado2
			   voice_pear
			   voice_peack
			   voice_apple

   Copyright (c) 2001-2002 by G-TECH
   All Rights Reserved.
	------------------------------------------------------------
	normal.c

	Ver1.10.000 (2001. 9. 10)
	 - pear 1dan, 2dan, 3dan

	Ver 1.02.000 (2001. 6. 29)
	 - when dan setting,
	 - change DanDisplay(char dan)
	 - add void DanSetting()


	VER 1.01  (2001. 5. 19)
	 - dan soun heung
	VER 1.00  (2000. 12. 3)
	 - dan soun heung
	Ver 1.10- (2000. 10. 6)	;--------
	 - M1,M2,M3 correction
	Ver 1.10- (1998. 8. 3)
	 - Change display count
	 - display grade
	Ver 1.05 (1998. 8. 3)
	 - change order Large to Small
	   1 dan is largest
	Ver 1.20 (1998. 1. 8)
	 - change compiler iar 4.20 to keil 5.02
	Ver 1.13 (1997. 12. 29)
	 - double check external input
	 - new BOOL	bEx0AuxKey = 0,
				bEx1AuxKey = 0,
				bEx2AuxKey = 0,
				bEx3AuxKey = 0;
	 - +- 1digit display speed ..........
	 - Remove BYTE  send_count = 0; not used
	 - Change char to BYTE
	 - HoldCount;
	 - LoopCount = 0;
	 - Rename
	 - count2 to hAutoZeroCounter = 0;
	 - sec_now to hKeyTimeOutCounter
	 - Changed if(!gnxTareStatus) to if(!gnxTareStatus && gxhZTDigit)
	 - No Autozero When gxhAutoZero is "0"
	 - Add bHoldFlag = 0
	 - Hold Function
	 - Add unstable processing  hUnstableCount;
	 - New long lPreWeight = 0;
	 - New BYTE	hUpdateCount = 0;
	Ver 1.12 (1997. 12. 29)
	 - Change UINT to BYTE
	 - sec_now = 0;
	 - count2  = 0;
	 - az_ok;
	 - StableCount = 0;
	 - Remove Byte dp_pos=0
	Ver 1.11 (1997. 12. 26)
	 - Delete  BYTE  rch = 0;
	Ver 1.10 (1997. 12. 8)
	 - lost file
	Ver org. (1997. 4. 22)
	 - COMPILER  IAR 4.20

	Copyright (c) 1997-2000 by G-TECH
	All Rights Reserved.
*/
//#include <reg51.h>       /* required header for output, bit_set, etc. */
#include "REG51RD2.h"
#include <stdio.h>
#include <string.h>
#include "adc.h"
#include "def.h"
#include "eeprom.h"
#include "var.h"
#include "vfd.h"
#define	LED_ON	1
#define	LED_OFF	0

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


void VoicePlay(unsigned char voice)		//0514
{
    int  i;

	VOICE_SBT = 0;	//T1 = 0;
	delay(10);		//no
	delay(10);		//unstable
	delay(10);		//ok stable
 
  //  for(i=0;i<100;i++)
  //  delay(1);
	/*
	VOICE_SBT = 1;	//T1 = 1;
	delay(10);
	delay(10);
    delay(10);
	delay(10);
	delay(10);
	*/
  //  VOICE_SBT = 1;	//T1 = 1;
  //  delay(10);
	
  //  for(i=0;i<300;i++)	
  //  VOICE(voice-1);	// voice data loading
    
	VOICE(voice-1);	    // voice data loading
	
    for(i=0;i<3;i++)	
    delay(10);

	VOICE_SBT = 1;		//strobe high -> voice output

    for(i=0;i<30;i++)	 
    delay(10);

   	VOICE_SBT = 0;

//	delay(10);
/*	
	VOICE(voice-1);	//nop
	delay(10);
	delay(10);
	delay(10);
*/	


//	VOICE_SBT = 0;
	
   /*
	VOICE_SBT = 0;	//T1 = 0;
	delay(10);		//no
	delay(10);		//unstable
	delay(10);		//ok stable
	delay(10);		//no
	delay(10);		//unstable
	delay(10);		//ok stable
	delay(10);		//no
	delay(10);		//unstable
	delay(10);		//ok stable
	VOICE_SBT = 0;	//T1 = 0;
	delay(10);		//no
	delay(10);		//unstable
	delay(10);		//ok stable
	delay(10);		//no
	delay(10);		//unstable
	delay(10);		//ok stable
	delay(10);		//no
	delay(10);		//unstable
	delay(10);		//ok stable
	VOICE_SBT = 0;	//T1 = 0;
	delay(10);		//no
	delay(10);		//unstable
	delay(10);		//ok stable
	delay(10);		//no
	delay(10);		//unstable
	delay(10);		//ok stable
	delay(10);		//no
	delay(10);		//unstable
	delay(10);		//ok stable

	VOICE(voice-1);	//nop
	VOICE(voice-1);	//nop
	VOICE(voice-1);	//nop
	delay(1);		//no
	VOICE_SBT = 1;	//T1 = 1;
	VOICE_SBT = 1;	//T1 = 1;
	VOICE_SBT = 1;	//T1 = 1;
	VOICE_SBT = 1;	//T1 = 1;
	VOICE_SBT = 1;	//T1 = 1;
	VOICE_SBT = 1;	//T1 = 1;
	VOICE_SBT = 1;	//T1 = 1;
	VOICE_SBT = 1;	//T1 = 1;
	VOICE_SBT = 1;	//T1 = 1;
	VOICE_SBT = 1;	//T1 = 1;
	VOICE_SBT = 1;	//T1 = 1;
	delay(1);		//no
	delay(10);		//no
	delay(10);		//unstable
	delay(10);		//ok stable
	delay(10);		//no
	delay(10);		//unstable
	delay(10);		//ok stable
	delay(10);		//no
	delay(10);		//unstable
	delay(10);		//ok stable
	delay(10);		//no
	delay(10);		//unstable
	delay(10);		//ok stable
	delay(10);		//no
	delay(10);		//unstable
	delay(10);		//ok stable
	delay(10);		//no
	delay(10);		//unstable
	delay(10);		//ok stable
  	for(i=0;i<100;i++) delay(10);

*/
}

//ver 2.01 add because aP8942A
void VoiceStop()
{
	VOICE_SBT = 0;	//T1 = 0;
//ok	delay(100);		//6);	
	delay(10);		//no
	delay(10);		//unstable
	delay(10);		//ok stable
	VOICE(42);	//nop
	VOICE_SBT = 1;	//T1 = 1;
//ok	delay(100);		//6);	
	delay(10);		//no
	delay(10);		//no
	delay(10);		//no
	delay(10);		//ok		
	delay(10);		//stable
	VOICE_SBT = 0;	//T1 = 0;
	delay(10);		
	delay(10);		
	delay(10);		
}	
//int 	DAN_MAX	= 0;
void normal_mode()
{

   // int 	DAN_MAX	= 0;
	BYTE	exit_flag = 0,
			WeighMode = 1,  /* when 0 key input mode else weigh mode */
			ch = 0;
//			net_sign     = 0,
//			gross_sign   = 0;
	BYTE	sec_now = 0;    /* time out counter with key wait */
	BYTE	hKeyTimeOutCounter = 0;    /* time out counter with key wait */
	BYTE	hAutoZeroCounter  = 0;	/* zero tracking counter */
	BYTE	az_ok;          /* time of zero tracking */
	BYTE	StableCount = 0,	/* stable processing counter */
			hUnstableCount = 0; /* Unstable processing counter */
	UINT	timeb = 0;		/* set mode */
	long	diff1, diff11,	/* for ...... */
			diff2,			/* for stable */
			diff21,			/* for stable */
			over_weight,
			zero_range,
			limit,
			prev_adc1 = 0;
	long	lPreWeight = 0;
	BYTE	hUpdateCount = 0;

	long  raw_data;	  /* float raw_data; */
	int   i;
//	int   j;

	long  s_weight = 0;
	long  s_weight1 = 0;
	long  s_weight2 = 0;


	int		iVoice = 0;

//	BYTE  over_flag = 0;    /* overflow flag */
	BOOL	bWorking = 0; /* working between start and stop */

	byte  gross_flag = 0;    /* when gross flag */

	long  lxlInNum = 0;         /* key input number */
	long	lxlInLimit = 0;    /* key input limit  maximum input / 10 */

	BOOL	bGradeReady = 0;

	long	lOneWeight;
	long	lPreRef = 0;
	BOOL	bScaleMode = 1;
	BOOL	bGradeMode = 0;
	BOOL	bSetMode = 0;
	int		bSetMode1 = 0;   // sul jung mode - pum mok x
	int		bSetMode2 = 1;   // sul jung mode

	BYTE	hEndCount = 5;
	BYTE	end_flag = 0;    /* determine done */

	byte	byDanIndex = 0;
	BYTE	byVoiceOff = 0;
	BOOL	bBlink = 0;
	BYTE	byBlinkCounter = 0;
	BOOL	bPreSignPlus = 1;
	BOOL	bGrossSignPlus = 1;
	BOOL	bNetSignPlus = 1;
	byte	byDisplayUpdate = 5;	/* ver 0.03 */
	BYTE	byCountIndex = 0;
	BOOL	bModeCount = 0;
	UINT	command = 0;
	UINT	command1 = 0;
	long	lKeyWait = 0;

	byte	byDisplayMode = 0;	// 0:counter, 1:dan total, 2:weigh
	byte	byGrade = 1;		//ver1.00 --> 0 // current grade
//	long	lGradeWeight = 0;	// current grade weigh
	int		iKey = 0;

	int		iSpeedMode = 0;		//botong speed

	dpoint_bak = v_decimal_point;

	lTotalCount = 0;
//	for(i=0; i<19; i++) lCount[i] = 0;
    for(i=0; i<26; i++) lCount[i] = 0;


//	gxhCurrentType = 1;
/*

	VfdPrintf("%05d", (int)gxhCurrentType);	  	//unsigned char
	delay(1000);

	VfdPrintf("%05d", (int)i1dan);	  	//unsigned char
	delay(1000);
	VfdPrintf("%05d", (int)i2dan);	  	//unsigned char
	delay(1000);
	VfdPrintf("%05d", i3dan);	  	//unsigned char
	delay(1000);
	VfdPrintf("%05d", i4dan);	  	//unsigned char
	delay(1000);
	VfdPrintf("%05d", i5dan);	  	//unsigned char
	delay(1000);
	VfdPrintf("%05d", i6dan);	  	//unsigned char
	delay(1000);
	VfdPrintf("%05d", i7dan);	  	//unsigned char
	delay(1000);
	VfdPrintf("%05d", i8dan);	  	//unsigned char
	delay(1000);
*/


  /*
  
	if (i1dan < 0 || i2dan < 0 || i3dan < 0 || i4dan < 0 || i5dan < 0 ||
		i6dan < 0 || i7dan < 0 || i8dan < 0 || i9dan < 0 || i10dan < 0 ||
		i11dan < 0 || i12dan < 0 || i13dan < 0 || i14dan < 0 || i15dan < 0)
	{
		vputs("RELOD");
		DefaultDanSetting();
		SaveDan(0);
		delay(1000);
	}

   */
//VER 1.01
  	if (i1dan < 0 || i2dan < 0 || i3dan < 0 || i4dan < 0 || i5dan < 0 ||
		i6dan < 0 || i7dan < 0 || i8dan < 0 || i9dan < 0 || i10dan < 0 ||
		i11dan < 0 || i12dan < 0 || i13dan < 0 || i14dan < 0 || i15dan < 0 ||
		i16dan < 0 || i17dan < 0 || i18dan < 0 || i19dan < 0 || i20dan < 0 ||
		i21dan < 0 || i22dan < 0 || i23dan < 0 || i24dan < 0 )
	{
		vputs("RELOD");
		DefaultDanSetting();
		SaveDan(0);
		delay(1000);
	}

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
 



//	DanDisplay();

	if ((read_filtered_adc() < 300L) || (65535L <= read_filtered_adc()))
	{  vputs("ERR 2"); beep(100); delay(500); return; }

	limit = v_capacity * power(10, v_decimal_point);
	over_weight = limit + limit/10;	/* set over load range */

			 /*  +- for banolim */
//	zero_range = (limit + v_ei_multiply_factor/2) /v_ei_multiply_factor;    /* zero key range is 10% */
	zero_range = limit;

	az_ok = 20;
	if (9 < gxhMotionBand) gxhMotionBand = 1; /* DIGIT of stable condition */
	gxhMotionBand = 0;
	gxhFilter = 1;

//	DISPBUF[6] |= 0x04;

	CalcMinimumWeight(); //// 1.01

////	fast_evalue((long)iMinimumWeight); delay(5000);

//	v_division=50;

/*
	VfdDan (1); fast_evalue((long)i1dan); delay(1000);
	VfdDan (2); fast_evalue((long)i2dan); delay(1000);
	VfdDan (3); fast_evalue((long)i3dan); delay(1000);
	VfdDan (4); fast_evalue((long)i4dan); delay(1000);
	VfdDan (5); fast_evalue((long)i5dan); delay(1000);
	VfdDan (6); fast_evalue((long)i6dan); delay(1000);
	VfdDan (7); fast_evalue((long)i7dan); delay(1000);
	VfdDan (8); fast_evalue((long)i8dan); delay(1000);
	VfdDan (9); fast_evalue((long)i9dan); delay(1000);
	VfdDan(10); fast_evalue((long)i10dan); delay(1000);
	VfdDan(11); fast_evalue((long)i11dan); delay(1000);
	VfdDan(12); fast_evalue((long)i12dan); delay(1000);
*/

/**
	VfdPrintf("%05lu", v_capacity);	  	//unsigned long
	delay(1000);
	VfdPrintf("%05u", v_division);		//unsinged int
	delay(1000);
	VfdPrintf("%05u", v_decimal_point);	//unsigned char
	delay(1000);
	VfdPrintf("%05lu", v_zero);	  		//unsigned long
	delay(1000);
	VfdPrintf("%05lu", v_adc_org);	  		//unsigned long
	delay(1000);
***/

	do
	{
		if ((command == RIGHT_K) ? keypushdouble() : keypush())	  //설정키가 눌리면 더블키 체크 , 아니면 그냥 키 테그 
		{

//			iKey = KEYCODE1*0x100 + KEYCODE;
//			ch = key_disp[KEYCODE];
//			switch (ch)

			switch (GetKeyCode())
			{
				case HUNDRED_K:	//M1
					if (bSetMode2 == 1) { retry_message(); break;}	  // default =1	   // 1026
					command = 0;
				//	if(gxhCurrentType == 0 || gxhCurrentType == 3) // 포도  감 
				 //	if(gxhCurrentType == 0 || gxhCurrentType == 4) // 포도  감   //1.10
                    if(gxhCurrentType == 0 || gxhCurrentType == 5|| gxhCurrentType == 4) // 포도  배   //1.11   , 포도 와 배는 

					{
						byDan[byDanIndex*3]++;
						if (v_decimal_point == 3) byDan[byDanIndex*3] %= 20;
						else byDan[byDanIndex*3] %= 10;
					}
					else
					{
						byDan[byDanIndex*3]++;
						byDan[byDanIndex*3] %= 10;
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

				case ZERO_K:  /* zero grader */
					if (!bScaleMode) { retry_message(); break; }
					command = command1 = ZERO_K;
					lKeyWait = 0;
					if (bModeCount)	// 카운트확인 모드 
					{
						switch (byCountIndex)
						{
							case 0:
								lTotalCount = 0;
								for(i=0; i<20; i++) lCount[i] = 0;
								fast_evalue (lTotalCount);
								break;
							case 1:
								lCount[1] = 0;
								fast_evalue(lCount[1]);
								break;
							case 2:
								lCount[2] = 0;
								fast_evalue(lCount[2]);
								break;
							case 3:
								lCount[3] = 0;
								fast_evalue(lCount[3]);
								break;
							case 4:
								lCount[4] = 0;
								fast_evalue(lCount[4]);
								break;
							case 5:
								lCount[5] = 0;
								fast_evalue(lCount[5]);
								break;
							case 6:
								lCount[6] = 0;
								fast_evalue(lCount[6]);
								break;
							case 7:
								lCount[7] = 0;
								fast_evalue(lCount[7]);
								break;
							case 8:
								lCount[8] = 0;
								fast_evalue(lCount[8]);
								break;
							case 9:
								lCount[9] = 0;
								fast_evalue(lCount[9]);
								break;
							case 10:
								lCount[10] = 0;
								fast_evalue(lCount[10]);
								break;
							case 11:
								lCount[11] = 0;
								fast_evalue(lCount[11]);
								break;
							case 12:
								lCount[12] = 0;
								fast_evalue(lCount[12]);
								break;
							case 13:
								lCount[13] = 0;
								fast_evalue(lCount[13]);
								break;
							case 14:
								lCount[14] = 0;
								fast_evalue(lCount[14]);
								break;
							case 15:
								lCount[15] = 0;
								fast_evalue(lCount[15]);
								break;
							case 16:
								lCount[16] = 0;
								fast_evalue(lCount[16]);
								break;
							case 17:
								lCount[17] = 0;
								fast_evalue(lCount[17]);
								break;
							case 18:
								lCount[18] = 0;
								fast_evalue(lCount[18]);
								break;
							case 19:
								lCount[19] = 0;
								fast_evalue(lCount[19]);
								break;
						
						//VER 1.01
                           	case 20:
								lCount[20] = 0;
								fast_evalue(lCount[20]);
								break;
							case 21:
								lCount[21] = 0;
								fast_evalue(lCount[21]);
								break;
							case 22:
								lCount[22] = 0;
								fast_evalue(lCount[22]);
								break;
							case 23:
								lCount[23] = 0;
								fast_evalue(lCount[23]);
								break;
							case 24:
								lCount[24] = 0;
								fast_evalue(lCount[24]);
								break;
						    case 25:
								lCount[25] = 0;
								fast_evalue(lCount[25]);
								break;



						//VER 1.01

						
						
						
							default:
								break;
						}
						break;
					} // 카운트 확인 모드 
//					if (!bScaleMode) { retry_message(); break; }
					if (gnxTareStatus) { retry_message(); break; }
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
						byBlinkCounter = 30;	//2014.3.6 10;
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
							case 0:  byCountIndex %= (16); break;
							case 1:  byCountIndex %= (16); break;
							case 2:  byCountIndex %= (16); break;
							case 3:  byCountIndex %= (16); break;
							case 4:  byCountIndex %= (16); break;
							case 5:  byCountIndex %= (16); break;
						//	case 6:  byCountIndex %= (20); break;
						    case 6:  byCountIndex %= (26); break; //ver1.01,한라봉 
						
						
							case 7:  byCountIndex %= (20); break;
							case 8:  byCountIndex %= (20); break;
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


							fast_evalue (lTotalCount);  VfdDan(0);   DISPBUF[5]=0X00;  DISPBUF[6]=0X78;   //VfdDan('T');  //VfdDan('T'); //KIM
							break;
						case 1: fast_evalue(lCount[1]); VfdDan(1);
							break;
						case 2: fast_evalue(lCount[2]); VfdDan(2);
							break;
						case 3: fast_evalue(lCount[3]); VfdDan(3);
							break;
						case 4: fast_evalue(lCount[4]); VfdDan(4);
							break;
						case 5: fast_evalue(lCount[5]); VfdDan(5);
							break;
						case 6: fast_evalue(lCount[6]); VfdDan(6);
							break;
						case 7:	fast_evalue(lCount[7]); VfdDan(7);
							break;
						case 8:	fast_evalue(lCount[8]); VfdDan(8);
							break;
						case 9:	fast_evalue(lCount[9]); VfdDan(9);
							break;
						case 10: fast_evalue(lCount[10]); VfdDan(10);
							break;
						case 11: fast_evalue(lCount[11]); VfdDan(11);
							break;
						case 12: fast_evalue(lCount[12]); VfdDan(12);
							break;
						case 13: fast_evalue(lCount[13]); VfdDan(13);
							break;
						case 14: fast_evalue(lCount[14]); VfdDan(14);
							break;
						case 15: fast_evalue(lCount[15]); VfdDan(15);
							break;
						case 16: fast_evalue(lCount[16]); VfdDan(16);
							break;
						case 17: fast_evalue(lCount[17]); VfdDan(17);
							break;
						case 18: fast_evalue(lCount[18]); VfdDan(18);
							break;
						case 19: fast_evalue(lCount[19]); VfdDan(19);
							break;
					   
					    case 20: fast_evalue(lCount[20]); VfdDan(20);
							break;
					   
					  	case 21: fast_evalue(lCount[21]); VfdDan(21);
							break;
					   
					    case 22: fast_evalue(lCount[22]); VfdDan(22);
							break;
					    case 23: fast_evalue(lCount[23]); VfdDan(23);
							break;
					   
					    case 24: fast_evalue(lCount[24]); VfdDan(24);
							break;
					   case 25: fast_evalue(lCount[25]); VfdDan(25);
							break;
					   
					
						default:
							break;
					}
					break;
				case WEIGH_K:  /* batch stop - number 9 */
					if (command == GRAPE_K) recall_dan(0);
					if (command == JADOO_K) recall_dan(1);
//					if (command == JADO2_K) recall_dan(2);
					if (command == PEACH_K) recall_dan(2);
					if (command == APPLE_K) recall_dan(3);
					if (command == GAM_K) recall_dan(4);
					if (command == PEAR_K) recall_dan(5);

					if (command == HANLABONG_K) recall_dan(6);
					if (command == REDHAYNG_K) recall_dan(7);
					if (command == TEUKJAK_K) recall_dan(8);

					if (   command == GRAPE_K || command == JADOO_K && PEACH_K
						|| command == APPLE_K || command == GAM_K || command == PEAR_K
						|| command == HANLABONG_K || command == REDHAYNG_K || command == TEUKJAK_K)
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
/*
						VOICE(0);
						VOICE_SBT = 1;
//						delay(6);	//ver 2.00 add because aP8942A
						VoiceStop();
						VOICE_SBT = 0;

						VOICE(VMODEWEIGH); byVoiceOff = VOICEOFFDELAY;
*/
						VoicePlay(VMODEWEIGH); byVoiceOff = VOICEOFFDELAY;
					}
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
//					fast_evalue(iMinimumWeight);
         //           vputs("TTTTT");
			//		delay(1000);
//vputs("     ");
					if (bSetMode1 == 1) { retry_message(); break;}
					lKeyWait = 0;
					recall_dan(gxhCurrentType);		//ver 1.02
					CalcMinimumWeight();
					vputs("     ");
				   /*
					if(gxhCurrentType==6)// 한라봉  ver 1.01
				 	DAN_MAX = 24;	/// kkkk 
					else if(gxhCurrentType==7 || gxhCurrentType==8)
					DAN_MAX = 18;	/// kkkk 
					else
					DAN_MAX = 14;	/// kkkk 
				*/	
					 
				
					gxhDanSetType =  2;

					if ((i = iDanCheck()) != 0)
					{	retry_message();
						delay(1000);
						byDanIndex = i - 10 + 1;
						bScaleMode = 0;
						bBlink = 1;
						switch(gxhCurrentType)
						{
							case 0:	command = command1 = SETTYPE1_K; break;
							case 1:	command = command1 = SETTYPE2_K; break;
							case 2:	command = command1 = SETTYPE3_K; break;
							case 3:	command = command1 = SETTYPE4_K; break;
							case 4:	command = command1 = SETTYPE5_K; break;
							case 5:	command = command1 = SETTYPE6_K; break;
							case 6:	command = command1 = SETTYPE7_K; break;
							case 7:	command = command1 = SETTYPE8_K; break;
							case 8:	command = command1 = SETTYPE9_K; break;
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
					fast_evalue(lOneWeight);

/**
					VOICE(0);
					VOICE_SBT = 1;
//					delay(6);	//ver 2.00 add because aP8942A
					VoiceStop();
					VOICE_SBT = 0;
					byVoiceOff = VOICEOFFDELAY;
***/
					switch(gxhCurrentType)
					{
						case 0:
//								VOICE(VOICE_TYPE1);   delay(400); VOICE(0);delay(200);
//								VoiceStop();	//add ver 2.01-2
								VoicePlay(VOICE_TYPE1); //delay(400); delay(200); delay(400); delay(200); delay(400); delay(200);
								                         delay(1800); 

								if     (gxhWeightGrapeType == 1)
								{	VoicePlay(VOICE_P4); /*VOICE(VOICE_P4);*/      delay(200); /*VOICE(0);*/ }
								else if(gxhWeightGrapeType == 2)
								{	VoicePlay(VOICE_P5); /*VOICE(VOICE_P5);*/      delay(200); /*VOICE(0);*/ }
								else if(gxhWeightGrapeType == 3)
								{	VoicePlay(VOICE_P10); /*VOICE(VOICE_P10);*/      delay(200); /*VOICE(0);*/ }
								break;
						case 1:
/**
								VOICE(VOICE_TYPE2);   delay(400); VOICE(0);delay(200);
								VoiceStop();	//add ver 2.01-2
								VOICE(VOICE_P10);     delay(200); VOICE(0);
**/
								VoicePlay(VOICE_TYPE2); delay(1800);  // delay(400); delay(200); delay(400); delay(200); delay(400); delay(200);
								VoicePlay(VOICE_P10);     delay(200);
								break;
						case 3:
						    	VoicePlay(VOICE_TYPE4);  delay(1800);  //delay(400); delay(200); delay(400); delay(200); delay(400); delay(200);
									if     (gxhWeightAppleType == 1)
									  {	VoicePlay(VOICE_P5);      delay(200); }
									else if(gxhWeightAppleType == 2)
									  {	VoicePlay(VOICE_P10);      delay(200); }
									else if(gxhWeightAppleType == 3)
									  {	VoicePlay(VOICE_P15);     delay(200); }
						     /*
						      	VoicePlay(VOICE_TYPE3);   delay(500); delay(200); delay(400); delay(200); delay(400); delay(200);
									if     (gxhWeightPeackType == 1)
									  {	VoicePlay(VOICE_P45);      delay(200); }
									else if(gxhWeightPeackType == 2)
									  {	VoicePlay(VOICE_P5);      delay(200); }
									else if(gxhWeightPeackType == 3)
									  {	VoicePlay(VOICE_P10);     delay(200); }
									else if(gxhWeightPeackType == 4)
									  {	VoicePlay(VOICE_P15);     delay(200); }
							   */
/**
								VOICE(VOICE_TYPE3);   delay(300); VOICE(0);delay(200);
								VoiceStop();	//add ver 2.01-2
								VOICE(VOICE_P10);     delay(200); VOICE(0);
**/								/*
								VoicePlay(VOICE_TYPE5);   delay(400); delay(200); delay(400); delay(200); delay(400); delay(200);
								VoicePlay(VOICE_P10);     delay(200);
								*/
								break;
						case 2:
/**
								VOICE(VOICE_TYPE4);   delay(100); VOICE(0);delay(200);
								VoiceStop();	//add ver 2.01-2
**/								/*
								VoicePlay(VOICE_TYPE6);   delay(400); delay(200); delay(400); delay(200); delay(400); delay(200);
								if     (gxhWeightPearType == 1)
//2014.3.5 ?????				{	VoicePlay(VOICE_P7);      delay(200);}
								{	VoicePlay(VOICE_P75);      delay(200);}
								else if(gxhWeightPearType == 2)
								{	VoicePlay(VOICE_P15);     delay(200);}
								*/
								 
								VoicePlay(VOICE_TYPE3);  delay(1900);  //delay(500); delay(200); delay(400); delay(200); delay(400); delay(200);
									if     (gxhWeightPeackType == 1)
									  {	VoicePlay(VOICE_P45);      delay(200); }
									else if(gxhWeightPeackType == 2)
									  {	VoicePlay(VOICE_P5);      delay(200); }
									else if(gxhWeightPeackType == 3)
									  {	VoicePlay(VOICE_P10);     delay(200); }
									else if(gxhWeightPeackType == 4)
									  {	VoicePlay(VOICE_P15);     delay(200); }
								
								

							//		VoicePlay(VOICE_TYPE5);   delay(400); delay(200); delay(400); delay(200); delay(400); delay(200);
							//	VoicePlay(VOICE_P10);     delay(200);
							
								break;
						case 4:

						   	    VoicePlay(VOICE_TYPE5); delay(1900);   //delay(400); delay(200); delay(400); delay(200); delay(400); delay(200);
								VoicePlay(VOICE_P10);     delay(200);
					
/**
                             			VoicePlay(VOICE_TYPE2);   delay(400); delay(200); delay(400); delay(200); delay(400); delay(200);
								VoicePlay(VOICE_P10);     delay(200);
					
								VOICE(VOICE_TYPE5);   delay(400); VOICE(0);delay(200);
								VoiceStop();	//add ver 2.01-2
									if     (gxhWeightAppleType == 1)
									  {	VOICE(VOICE_P5);      delay(200); VOICE(0); }
									else if(gxhWeightAppleType == 2)
									  {	VOICE(VOICE_P10);      delay(200); VOICE(0); }
									else if(gxhWeightAppleType == 3)
									  {	VOICE(VOICE_P15);     delay(200); VOICE(0); }
***/							/*
								VoicePlay(VOICE_TYPE4);   delay(400); delay(200); delay(400); delay(200); delay(400); delay(200);
									if     (gxhWeightAppleType == 1)
									  {	VoicePlay(VOICE_P5);      delay(200); }
									else if(gxhWeightAppleType == 2)
									  {	VoicePlay(VOICE_P10);      delay(200); }
									else if(gxhWeightAppleType == 3)
									  {	VoicePlay(VOICE_P15);     delay(200); }
									  */
								break;
						case 5:

						     	VoicePlay(VOICE_TYPE6);  delay(1900);  //delay(400); delay(200); delay(400); delay(200); delay(400); delay(200);
								if     (gxhWeightPearType == 1)
//2014.3.5 ?????				{	VoicePlay(VOICE_P7);      delay(200);}
								{	VoicePlay(VOICE_P75);      delay(200);}
								else if(gxhWeightPearType == 2)
								{	VoicePlay(VOICE_P15);     delay(200);}

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
//								VoicePlay(VOICE_TYPE6);   delay(500); delay(200); delay(400); delay(200); delay(400); delay(200);
							 /*
							
								VoicePlay(VOICE_TYPE3);   delay(500); delay(200); delay(400); delay(200); delay(400); delay(200);
									if     (gxhWeightPeackType == 1)
									  {	VoicePlay(VOICE_P45);      delay(200); }
									else if(gxhWeightPeackType == 2)
									  {	VoicePlay(VOICE_P5);      delay(200); }
									else if(gxhWeightPeackType == 3)
									  {	VoicePlay(VOICE_P10);     delay(200); }
									else if(gxhWeightPeackType == 4)
									  {	VoicePlay(VOICE_P15);     delay(200); }
							   */
							
								break;

						case 6:
								VoicePlay(VOICE_TYPE7); delay(1800);   //delay(400); delay(200); delay(400); delay(200); delay(400); delay(200);delay(400); delay(200);
								VoicePlay(VOICE_P3);     delay(200);
								break;

						case 7:
								VoicePlay(VOICE_TYPE8); delay(1800);  // delay(400); delay(200); delay(400); delay(200); delay(400); delay(200); delay(400); delay(200);
								VoicePlay(VOICE_P3);     delay(200);
								break;

						case 8:
								VoicePlay(VOICE_TYPE9); delay(1800);  // delay(400); delay(200); delay(400); delay(200); delay(400); delay(200); delay(400); delay(200);
								VoicePlay(VOICE_P3);     delay(200);
								break;

						default: break;
					}
//					vputs("TYPE ");
//					VfdDan(gxhCurrentType+1);
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
					fast_evalue(0);
//					fast_evalue((long)iMinimumWeight);
					break;
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
							VoicePlay(VMODEWEIGH);

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

				case SETTYPE1_K:
					DAN_MAX = 14;
/*
					VoiceStop();	//add ver 2.01-2
					VOICE(VOICE_SET);
					beep(30); delay(20);
					VOICE(0);
**/
					VoicePlay(VOICE_SET);
					beep(30); delay(20);

					gxhCurrentType = 0;
					recall_dan(gxhCurrentType);
					i15dan = 0;
					command = command1 = SETTYPE1_K;
					bBlink = 1;
					byBlinkCounter = 30;	//2014.3.6 10;
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
					CalcMinimumWeight();//ver 1.01

					DAN_MAX = 14;	//v1.10 20150715
					WeighMode = 1;	//MTS
					break;
				case SETTYPE2_K:
					DAN_MAX = 14;
/**
					VoiceStop();	//add ver 2.01-2
					VOICE(VOICE_SET);
					beep(30); delay(20);
					VOICE(0);
**/
					VoicePlay(VOICE_SET);
					beep(30); delay(20);

					gxhCurrentType = 1;
					recall_dan(gxhCurrentType);
					command = command1 = SETTYPE2_K;
					i15dan = 0;
					bBlink = 1;
						byBlinkCounter = 30;	//2014.3.6 10;
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
				case SETTYPE3_K:   //특작 
					DAN_MAX = 18;
/**
//2014.2.5					VoiceStop();	//add ver 2.01-2
					VOICE(VOICE_SET);
					beep(30); delay(20);
					VOICE(0);
**/
					VoicePlay(VOICE_SET);
					beep(30); delay(20);

					gxhCurrentType = 8;
					recall_dan(gxhCurrentType);
					i19dan = 0;
					command = command1 = SETTYPE3_K;
					bBlink = 1;
						byBlinkCounter = 30;	//2014.3.6 10;
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
				case SETTYPE4_K:
					DAN_MAX = 14;
/**
//2014.2.5					VoiceStop();	//add ver 2.01-2
					VOICE(VOICE_SET);
					beep(30); delay(20);
					VOICE(0);
**/
					VoicePlay(VOICE_SET);
					beep(30); delay(20);

					gxhCurrentType = 5;
					recall_dan(gxhCurrentType);
					command = command1 = SETTYPE4_K;
					i15dan = 0;
					bBlink = 1;
						byBlinkCounter = 30;	//2014.3.6 10;
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
				case SETTYPE5_K:
					DAN_MAX = 14;
/**
//2014.2.5					VoiceStop();	//add ver 2.01-2
					VOICE(VOICE_SET);
					beep(30); delay(20);
					VOICE(0);
**/
					VoicePlay(VOICE_SET);
					beep(30); delay(20);

					gxhCurrentType = 3;
					recall_dan(gxhCurrentType);
					command = command1 = SETTYPE5_K;
					i15dan = 0;
					bBlink = 1;
						byBlinkCounter = 30;	//2014.3.6 10;
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

				case SETTYPE6_K:
					DAN_MAX = 14;
/**
//2014.2.5					VoiceStop();	//add ver 2.01-2
					VOICE(VOICE_SET);
					beep(30); delay(20);
					VOICE(0);
**/
					VoicePlay(VOICE_SET);
					beep(30); delay(20);

					gxhCurrentType = 2;
					recall_dan(gxhCurrentType);
					command = command1 = SETTYPE6_K;
					i15dan = 0;
					bBlink = 1;
				    byBlinkCounter = 30;	//2014.3.6 10;
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
				case SETTYPE7_K:   //한라봉 
				   // DAN_MAX = 18;
				     DAN_MAX = 24;	//ver1.01
					 
					 
					 VoicePlay(VOICE_SET);
					 beep(30); delay(20);

					gxhCurrentType = 6;
					recall_dan(gxhCurrentType);
					command = command1 = SETTYPE7_K;
				//	i15dan = 0;
				//	i26dan = 0;
				
					bBlink = 1;
					byBlinkCounter = 10;
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
				case SETTYPE8_K:  //레드향 
					DAN_MAX = 18;
					VoicePlay(VOICE_SET);
					beep(30); delay(20);

					gxhCurrentType = 7;
					recall_dan(gxhCurrentType);
					command = command1 = SETTYPE8_K;
				//	i15dan = 0;
			    	i19dan = 0;
				
					bBlink = 1;
						byBlinkCounter = 30;	//2014.3.6 10;
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
				case SETTYPE9_K:	  //0725
					DAN_MAX = 14;
					VoicePlay(VOICE_SET);
					beep(30); delay(20);

					gxhCurrentType = 4;
					recall_dan(gxhCurrentType);
					command = command1 = SETTYPE9_K;
					i15dan = 0;
					bBlink = 1;
						byBlinkCounter = 30;	//2014.3.6 10;
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

/*
			   case  ONOFF_K:
					for(j=0;j<7;j++)
					{
					  DISPBUF[j] = 0x00;
					}
					for(;;)
					{
						if (keypushanbbik())
						{
							if (GetKeyCode() == ONOFF_K )
							{   beep(50);
								v_decimal_point = dpoint_bak;
								return;
							}
						}
					}
					break;
*/

			   case  GRAPE_K:
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
							write_eep(ADR_gxhDanSetType, (UINT)gxhDanSetType);
							CALL_FN = FN_read_eep;
/***
							VoiceStop();	//add ver 2.01-2
							VOICE(VOICE_TYPE1); delay(10); VOICE(0);
***/
							VoicePlay(VOICE_TYPE1); delay(10);
							gxhCurrentType = 0;
							recall_dan(gxhCurrentType);
							CalcMinimumWeight();
							vputs("TYPE ");
							VfdDan(gxhCurrentType+1);
							CALL_FN = FN_write_eep;
							write_eep(ADR_gxhCurrentType, (UINT)gxhCurrentType);
							CALL_FN = FN_read_eep;
							beep(300); delay(1500);
							break;
						}
						else if (command1 != SETTYPE1_K &&   command1 != SETTYPE2_K &&
								 command1 != SETTYPE3_K &&   command1 != SETTYPE4_K &&
								 command1 != SETTYPE5_K &&   command1 != SETTYPE6_K &&
								 command1 != SETTYPE7_K &&   command1 != SETTYPE8_K &&
								 command1 != SETTYPE9_K)
						{	retry_message();
							break;
						}
					}
			   case  JADOO_K:
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
							write_eep(ADR_gxhDanSetType, (UINT)gxhDanSetType);
							CALL_FN = FN_read_eep;
/***
							VoiceStop();	//add ver 2.01-2
							VOICE(VOICE_TYPE2); delay(10); VOICE(0);
***/
							VoicePlay(VOICE_TYPE2); delay(10);
							gxhCurrentType = 1;
							recall_dan(gxhCurrentType);
							CalcMinimumWeight();
							vputs("TYPE ");
							VfdDan(gxhCurrentType+1);
							CALL_FN = FN_write_eep;
							write_eep(ADR_gxhCurrentType, (UINT)gxhCurrentType);
							CALL_FN = FN_read_eep;
							beep(300); delay(1500);
							break;
						}
						else if (command1 != SETTYPE1_K &&   command1 != SETTYPE2_K &&
								 command1 != SETTYPE3_K &&   command1 != SETTYPE4_K &&
								 command1 != SETTYPE5_K &&   command1 != SETTYPE6_K &&
								 command1 != SETTYPE7_K &&   command1 != SETTYPE8_K &&
								 command1 != SETTYPE9_K)
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
							write_eep(ADR_gxhDanSetType, (UINT)gxhDanSetType);
							CALL_FN = FN_read_eep;
							VoiceStop();	//add ver 2.01-2
							VOICE = VOICE_TYPE3; delay(10); VOICE = 0;
							gxhCurrentType = 2;
							recall_dan(gxhCurrentType);
							CalcMinimumWeight();
							vputs("TYPE ");
							VfdDan(gxhCurrentType+1);
							CALL_FN = FN_write_eep;
							write_eep(ADR_gxhCurrentType, (UINT)gxhCurrentType);
							CALL_FN = FN_read_eep;
							beep(300); delay(1500);
							break;
						}
						else if (command1 != SETTYPE1_K &&   command1 != SETTYPE2_K &&
								 command1 != SETTYPE3_K &&   command1 != SETTYPE4_K &&
								 command1 != SETTYPE5_K &&   command1 != SETTYPE6_K)
						{	retry_message();
							break;
						}
					}
*/
			   case  PEAR_K:
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
							write_eep(ADR_gxhDanSetType, (UINT)gxhDanSetType);
							CALL_FN = FN_read_eep;
/***
							VoiceStop();	//add ver 2.01-2
							VOICE(VOICE_TYPE4); delay(10); VOICE(0);
***/

							VoicePlay(VOICE_TYPE6); delay(10);

//							gxhCurrentType = 3;
							gxhCurrentType = 5;
							recall_dan(gxhCurrentType);
							CalcMinimumWeight();
							vputs("TYPE ");
							VfdDan(gxhCurrentType+1);
							CALL_FN = FN_write_eep;
							write_eep(ADR_gxhCurrentType, (UINT)gxhCurrentType);
							CALL_FN = FN_read_eep;
							beep(300); delay(1500);
							break;
						}
						else if (command1 != SETTYPE1_K &&   command1 != SETTYPE2_K &&
								 command1 != SETTYPE3_K &&   command1 != SETTYPE4_K &&
								 command1 != SETTYPE5_K &&   command1 != SETTYPE6_K &&
								 command1 != SETTYPE7_K &&   command1 != SETTYPE8_K &&
								 command1 != SETTYPE9_K)
						{	retry_message();
							break;
						}
					}

			   case  APPLE_K:
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
							if (bGradeMode == 1 || bSetMode1 == 1) { retry_message(); break;}

							lTotalCount = 0;
							lCount[1] = lCount[2] = lCount[3] = 0;
							lCount[4] = lCount[5] = lCount[6] = 0;
							lCount[7] = lCount[8] = lCount[9] = 0;
							lCount[10] = lCount[11] = lCount[12] = 0;
							lCount[13] = lCount[14] = lCount[15] = 0;

							DAN_MAX = 14;
							gxhDanSetType = 2;
							CALL_FN = FN_write_eep;
							write_eep(ADR_gxhDanSetType, (UINT)gxhDanSetType);
							CALL_FN = FN_read_eep;
/***
							VoiceStop();	//add ver 2.01-2
							VOICE(VOICE_TYPE5); delay(10); VOICE(0);
***/

							VoicePlay(VOICE_TYPE4); delay(10);

//							gxhCurrentType = 4;
							gxhCurrentType = 3;
							recall_dan(gxhCurrentType);
							CalcMinimumWeight();
							vputs("TYPE ");
							VfdDan(gxhCurrentType+1);
							CALL_FN = FN_write_eep;
							write_eep(ADR_gxhCurrentType, (UINT)gxhCurrentType);
							CALL_FN = FN_read_eep;
							beep(300); delay(1500);
							break;
						}
						else if (command1 != SETTYPE1_K &&   command1 != SETTYPE2_K &&
								 command1 != SETTYPE3_K &&   command1 != SETTYPE4_K &&
								 command1 != SETTYPE5_K &&   command1 != SETTYPE6_K &&
								 command1 != SETTYPE7_K &&   command1 != SETTYPE8_K &&
								 command1 != SETTYPE9_K)
						{	retry_message();
							break;
						}

					}

			   	case  PEACH_K:
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
							write_eep(ADR_gxhDanSetType, (UINT)gxhDanSetType);
							CALL_FN = FN_read_eep;
/***
							VoiceStop();	//add ver 2.01-2
							VOICE(VOICE_TYPE6); delay(10); VOICE(0);
***/

							VoicePlay(VOICE_TYPE3); delay(10);
//							gxhCurrentType = 5;
							gxhCurrentType = 2;

							recall_dan(gxhCurrentType);
							CalcMinimumWeight();
							vputs("TYPE ");
							VfdDan(gxhCurrentType+1);
							CALL_FN = FN_write_eep;
							write_eep(ADR_gxhCurrentType, (UINT)gxhCurrentType);
							CALL_FN = FN_read_eep;
							beep(300); delay(1500);
							break;
						}
						else if (command1 != SETTYPE1_K &&   command1 != SETTYPE2_K &&
								 command1 != SETTYPE3_K &&   command1 != SETTYPE4_K &&
								 command1 != SETTYPE5_K &&   command1 != SETTYPE6_K &&
								 command1 != SETTYPE7_K &&   command1 != SETTYPE8_K &&
								 command1 != SETTYPE9_K)
						{	retry_message();
							break;
						}
					}

				case HANLABONG_K:
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
						
						
						
							gxhDanSetType =  2;
							CALL_FN = FN_write_eep;
							write_eep(ADR_gxhDanSetType, (UINT)gxhDanSetType);
							CALL_FN = FN_read_eep;
/***
							VoiceStop();	//add ver 2.01-2
							VOICE(VOICE_TYPE6); delay(10); VOICE(0);
***/

							VoicePlay(VOICE_TYPE7); delay(10);
//							gxhCurrentType = 7;
							gxhCurrentType = 6;

							recall_dan(gxhCurrentType);
							CalcMinimumWeight();
							vputs("TYPE ");
							VfdDan(gxhCurrentType+1);
							CALL_FN = FN_write_eep;
							write_eep(ADR_gxhCurrentType, (UINT)gxhCurrentType);
							CALL_FN = FN_read_eep;
							beep(300); delay(1500);
							break;
						}
						else if (command1 != SETTYPE1_K &&   command1 != SETTYPE2_K &&
								 command1 != SETTYPE3_K &&   command1 != SETTYPE4_K &&
								 command1 != SETTYPE5_K &&   command1 != SETTYPE6_K &&
								 command1 != SETTYPE7_K &&   command1 != SETTYPE8_K &&
								 command1 != SETTYPE9_K)
						{	retry_message();
							break;
						}
					}

				case GAM_K:
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
							write_eep(ADR_gxhDanSetType, (UINT)gxhDanSetType);
							CALL_FN = FN_read_eep;
/***
							VoiceStop();	//add ver 2.01-2
							VOICE(VOICE_TYPE6); delay(10); VOICE(0);
***/

							VoicePlay(VOICE_TYPE5); delay(10);
//							gxhCurrentType = 5;
							gxhCurrentType =4;

							recall_dan(gxhCurrentType);
							CalcMinimumWeight();
							vputs("TYPE ");
							VfdDan(gxhCurrentType+1);
							CALL_FN = FN_write_eep;
							write_eep(ADR_gxhCurrentType, (UINT)gxhCurrentType);
							CALL_FN = FN_read_eep;
							beep(300); delay(1500);
							break;
						}
						else if (command1 != SETTYPE1_K &&   command1 != SETTYPE2_K &&
								 command1 != SETTYPE3_K &&   command1 != SETTYPE4_K &&
								 command1 != SETTYPE5_K &&   command1 != SETTYPE6_K &&
								 command1 != SETTYPE7_K &&   command1 != SETTYPE8_K &&
								 command1 != SETTYPE9_K)
						{	retry_message();
							break;
						}
					}

				case REDHAYNG_K:
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
							CALL_FN = FN_write_eep;
							write_eep(ADR_gxhDanSetType, (UINT)gxhDanSetType);
							CALL_FN = FN_read_eep;
/***
							VoiceStop();	//add ver 2.01-2
							VOICE(VOICE_TYPE6); delay(10); VOICE(0);
***/

							VoicePlay(VOICE_TYPE8); delay(10);
//							gxhCurrentType = 8;
							gxhCurrentType = 7;

							recall_dan(gxhCurrentType);
							CalcMinimumWeight();
							vputs("TYPE ");
							VfdDan(gxhCurrentType+1);
							CALL_FN = FN_write_eep;
							write_eep(ADR_gxhCurrentType, (UINT)gxhCurrentType);
							CALL_FN = FN_read_eep;
							beep(300); delay(1500);
							break;
						}
						else if (command1 != SETTYPE1_K &&   command1 != SETTYPE2_K &&
								 command1 != SETTYPE3_K &&   command1 != SETTYPE4_K &&
								 command1 != SETTYPE5_K &&   command1 != SETTYPE6_K &&
								 command1 != SETTYPE7_K &&   command1 != SETTYPE8_K &&
								 command1 != SETTYPE9_K)
						{	retry_message();
							break;
						}
					}

				case TEUKJAK_K:
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
							CALL_FN = FN_write_eep;
							write_eep(ADR_gxhDanSetType, (UINT)gxhDanSetType);
							CALL_FN = FN_read_eep;
/***
							VoiceStop();	//add ver 2.01-2
							VOICE(VOICE_TYPE6); delay(10); VOICE(0);
***/

							VoicePlay(VOICE_TYPE9); delay(10);
//							gxhCurrentType = 9;
							gxhCurrentType = 8;

							recall_dan(gxhCurrentType);
							CalcMinimumWeight();
							vputs("TYPE ");
							VfdDan(gxhCurrentType+1);
							CALL_FN = FN_write_eep;
							write_eep(ADR_gxhCurrentType, (UINT)gxhCurrentType);
							CALL_FN = FN_read_eep;
							beep(300); delay(1500);
							break;
						}
						else if (command1 != SETTYPE1_K &&   command1 != SETTYPE2_K &&
								 command1 != SETTYPE3_K &&   command1 != SETTYPE4_K &&
								 command1 != SETTYPE5_K &&   command1 != SETTYPE6_K &&
								 command1 != SETTYPE7_K &&   command1 != SETTYPE8_K &&
								 command1 != SETTYPE9_K)
						{	retry_message();
							break;
						}
					}

				case SPEED_K:
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
								VoicePlay(BOJUNG); delay(10);
								LAMPSPEED = LED_ON;
							}
							else
							{
								iSpeedMode = 0;	//botong
							    VoicePlay(BOJUNG_RELEASE); delay(10);
								LAMPSPEED = LED_OFF;
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

			} // of switch	getkeycode
		} // of if

        
		  if(iSpeedMode == 1)
		  LAMPSPEED = LED_ON;
		  else 
		  LAMPSPEED = LED_OFF;


		/*---------- READ A/D ----------*/
		raw_data = read_filtered_adc();
//2014.2.5		raw_data = adc_value();

		if (bBlink)
		{
			if (byBlinkCounter)
			{	byBlinkCounter--;
//				if (byBlinkCounter == 25)
				if (byBlinkCounter == 15)	// 2014.3.6 5
				{
					DISPBUF[0] = 0;
					DISPBUF[1] = 0;
					DISPBUF[2] = 0;
					DISPBUF[3] = 0;
					DISPBUF[4] = 0;
//					DISPBUF[5] = 0;
				}
			}
			else
			{
//				byBlinkCounter = 50;
						byBlinkCounter = 30;	//2014.3.6 10;
				DanDisplay(byDanIndex+1);

//add ver1.10 20150715	CalcMinimumWeight();//ver1.01
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

		v_adc1_buf = (long)(gnfFactor * raw_data);
		diff1 = v_adc1_buf - v_adc_org;
	
	
		diff2 = v_adc1_buf - prev_adc1;	/* for stable */
        //new function
	//	if(diff2<0)
	//	   diff2=diff2*-1;
 		//new function

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

		/* external resolution */
//ver1.20		gxlGrossWeight = ((diff1 + 10) / 20) * v_division;
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
		else	// Ver 0.3
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

    if(!iSpeedMode) //보정  기능이 없을때 
	{ 
	 
		 if (0 <= diff21 && diff21 <= 2) //안정범위    original : 2
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




		if (1 < StableCount)	/* stable LAMP, RELAY ON */
//		if (2 < StableCount)	/* stable LAMP, RELAY ON */
//		if (4 < StableCount)	/* stable LAMP, RELAY ON */
		{  	LAMPST = LED_ON;
			if (StableCount == 5) prev_adc1 = v_adc1_buf;
			if (10 < StableCount) StableCount = 6;
			hUnstableCount = 0;
		}

//		LAMPZR = LAMPST;

		/*---------- Zero lamp ON/OFF ----------*/
		LAMPZR = LED_OFF;
		if(!gxlWeight)
		{ 	LAMPZR = LED_ON;
			hAutoZeroCounter = 0; /* initialize auto zero timer for exect time */
		}

		/*---------- DISPLAY  weight ----------*/
//ms/at	if (gxlGrossWeight > over_weight) { vputs(" OVER  "); beep(100); }
		if ((((v_adc1_buf - v_adc_init + v_ei_multiply_factor/2) / v_ei_multiply_factor) * v_division) > over_weight)  //0508
		{
//jadue		vputs(" OVER"); beep(1000);
		 }
		if (bScaleMode)
		{	SIGNFLAG = !bNetSignPlus;	/* weight sign */
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
				fast_evalue(s_weight1);
			//	fast_evalue(gxlWeight);
            //	fast_evalue(diff21);


				byDisplayUpdate = 5;
				DISPBUF[5] &= 0x80;
				DISPBUF[6] = 0;
				s_weight = 0;
			}
		}
		else //
		{
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

			
			
			
			if (lOneWeight > iMinimumWeight/3) end_flag = 1;
			else end_flag = 0;
			if (!bGrossSignPlus) end_flag = 0;

			if (LAMPST && end_flag && bGradeReady && bSetMode2 == 1)
			{
				{
					LAMPZR = LED_ON; //delay(100);	//kim

					if (i1dan <= (int)lOneWeight) // lOneWeight : 현재선별한 무게가 
					{
						VoicePlay(VOICE_1);
						lCount[1]++; lTotalCount++;
						fast_evalue(lCount[1]);
						VfdDan(1);
						iVoice=1;
					}
					else if (i2dan <= (int)lOneWeight)
					{
						VoicePlay(VOICE_2);
						lCount[2]++; lTotalCount++;
						fast_evalue(lCount[2]);
						VfdDan(2);
						iVoice=2;
					}
					else if (i3dan <= (int)lOneWeight && (3 <= DAN_MAX+1))
					{
						VoicePlay(VOICE_3);
						lCount[3]++; lTotalCount++;
						fast_evalue(lCount[3]);
						VfdDan(3);
						iVoice=3;
					}
					else if (i4dan <= (int)lOneWeight && (4 <= DAN_MAX+1))
					{
						VoicePlay(VOICE_4);
						lCount[4]++; lTotalCount++;
						fast_evalue(lCount[4]);
						VfdDan(4);
						iVoice=4;
					}
					else if (i5dan <= (int)lOneWeight && (5 <= DAN_MAX+1))
					{
						VoicePlay(VOICE_5);
						lCount[5]++; lTotalCount++;
						fast_evalue(lCount[5]);
						VfdDan(5);
						iVoice=5;
					}
					else if((gxhDanSetType == 2))
					{
						if (i6dan <= (int)lOneWeight && (6 <= DAN_MAX+1))
						{
							VoicePlay(VOICE_6);
							lCount[6]++; lTotalCount++;
							fast_evalue(lCount[6]);
							VfdDan(6);
							iVoice=6;
						}
						else if (i7dan <= (int)lOneWeight && (7 <= DAN_MAX+1))
						{
							VoicePlay(VOICE_7);
							lCount[7]++; lTotalCount++;
							fast_evalue(lCount[7]);
							VfdDan(7);
							iVoice=7;
						}
						else if (i8dan <= (int)lOneWeight && (8 <= DAN_MAX+1))
						{
							VoicePlay(VOICE_8);
							lCount[8]++; lTotalCount++;
							fast_evalue(lCount[8]);
							VfdDan(8);
							iVoice=8;
						}
						else if (i9dan <= (int)lOneWeight && (9 <= DAN_MAX+1))
						{
							VoicePlay(VOICE_9);
							lCount[9]++; lTotalCount++;
							fast_evalue(lCount[9]);
							VfdDan(9);
							iVoice=9;
						}
						else if (i10dan <= (int)lOneWeight && (10 <= DAN_MAX+1))
						{
							VoicePlay(VOICE_10);
							lCount[10]++; lTotalCount++;
							fast_evalue(lCount[10]);
							VfdDan(10);
							iVoice=10;
						}
						else if (i11dan <= (int)lOneWeight && (11 <= DAN_MAX+1))
						{
							VoicePlay(VOICE_11);
							lCount[11]++; lTotalCount++;
							fast_evalue(lCount[11]);
							VfdDan(11);
							iVoice=11;
						}
						else if (i12dan <= (int)lOneWeight && (12 <= DAN_MAX+1))
						{
							VoicePlay(VOICE_12);
							lCount[12]++; lTotalCount++;
							fast_evalue(lCount[12]);
							VfdDan(12);
							iVoice=12;
						}
						else if (i13dan <= (int)lOneWeight && (13 <= DAN_MAX+1))
						{
							VoicePlay(VOICE_13);
							lCount[13]++; lTotalCount++;
							fast_evalue(lCount[13]);
						 	VfdDan(13);
						//	VfdDan();

							iVoice=13;
						}
						else if (i14dan <= (int)lOneWeight && (14 <= DAN_MAX+1))
						{
							VoicePlay(VOICE_14);
							lCount[14]++; lTotalCount++;
							fast_evalue(lCount[14]);
							VfdDan(14);
							iVoice=14;
						}
					
					
					
				     else if (i15dan <= (int)lOneWeight && (15 <= DAN_MAX+1))
					  
					  //ver 1.01
				//	  else if (i15dan <= (int)lOneWeight && (15 <= DAN_MAX+1) && (gxhCurrentType==6|| gxhCurrentType==7||gxhCurrentType==8))//한라봉, 레드향,특장 
					
						{
							VoicePlay(VOICE_15);
							lCount[15]++; lTotalCount++;
							fast_evalue(lCount[15]);
							VfdDan(15);
							iVoice=15;
						}
					
					

                      	else if (i16dan <= (int)lOneWeight && (16 <= DAN_MAX+1))
						{
							VoicePlay(VOICE_16);
							lCount[16]++; lTotalCount++;
							fast_evalue(lCount[16]);
							VfdDan(16);
							iVoice=16;
						}
					
					   	else if (i17dan <= (int)lOneWeight && (17 <= DAN_MAX+1))
						{
							VoicePlay(VOICE_17);
							lCount[17]++; lTotalCount++;
							fast_evalue(lCount[17]);
							VfdDan(17);
							iVoice=17;
						}

					   	else if (i18dan <= (int)lOneWeight && (18 <= DAN_MAX+1))
						{
							
							VoicePlay(VOICE_18);
							lCount[18]++; lTotalCount++;
							fast_evalue(lCount[18]);
							VfdDan(18);
							iVoice=18;
						}

				//      else if (i19dan <= (int)lOneWeight && (19 <= DAN_MAX+1) && (gxhCurrentType==6))  // ver1.01 .한라봉일때만 
				         else if (i19dan <= (int)lOneWeight && (19 <= DAN_MAX+1))  // ver1.01 .한라봉일때만 
				
						{
	                         VoicePlay(VOICE_19);
							 lCount[19]++; lTotalCount++;
							 fast_evalue(lCount[19]);
							 VfdDan(19);
							 iVoice=19;
                       
						}

					   else if (i20dan <= (int)lOneWeight && (20 <= DAN_MAX+1))
						{
							VoicePlay(VOICE_20);
							lCount[20]++; lTotalCount++;
							fast_evalue(lCount[20]);
							VfdDan(20);
							iVoice=20;
						}

				      else if (i21dan <= (int)lOneWeight && (21 <= DAN_MAX+1))
						{
							VoicePlay(VOICE_21);
							lCount[21]++; lTotalCount++;
							fast_evalue(lCount[21]);
							VfdDan(21);
							iVoice=21;
						}
                     
                     else if (i22dan <= (int)lOneWeight && (22 <= DAN_MAX+1))
						{
							VoicePlay(VOICE_22);
							lCount[22]++; lTotalCount++;
							fast_evalue(lCount[22]);
							VfdDan(22);
							iVoice=22;
						}
					 else if (i23dan <= (int)lOneWeight && (23 <= DAN_MAX+1))
						{
							VoicePlay(VOICE_23);
							lCount[23]++; lTotalCount++;
							fast_evalue(lCount[23]);
							VfdDan(23);
							iVoice=23;
						}

					 else if (i24dan <= (int)lOneWeight && (24 <= DAN_MAX+1))
						{
							VoicePlay(VOICE_24);
							lCount[24]++; lTotalCount++;
							fast_evalue(lCount[24]);
							VfdDan(24);
							iVoice=24;
						}
 				/*	
				   else if((int)lOneWeight > iMinimumWeight)
						{

						   if(gxhCurrentType==6)// 한라봉일때 
						    {
							VoicePlay(VOICE_25);
							lCount[25]++; lTotalCount++;
							fast_evalue(lCount[25]);
							VfdDan(25);
							iVoice=25;
                            }

                         else if(gxhCurrentType==7 || gxhCurrentType==8)//레드향 또는 특작 
						   {
							VoicePlay(VOICE_19);
							lCount[19]++; lTotalCount++;
							fast_evalue(lCount[19]);
							VfdDan(19);
							iVoice=19;
                           }

						 else //if(gxhCurrentType==7 || gxhCurrentType==8)//레드향 또는 특작 
						   {
							VoicePlay(VOICE_15);
							lCount[15]++; lTotalCount++;
							fast_evalue(lCount[15]);
							VfdDan(15);
							iVoice=15;
                           }

						    

						}
				 */
			
			
			
					  

					
						
						else if((int)lOneWeight > iMinimumWeight)	  //0213
						{
						  	
							if(DAN_MAX<20)
							VoicePlay(DAN_MAX+24);
							else
							//VoicePlay(DAN_MAX+48);
							VoicePlay(DAN_MAX+29);
							
							lCount[DAN_MAX+2]++; lTotalCount++;
							fast_evalue(lCount[DAN_MAX+2]);
							VfdDan(DAN_MAX+2);
							iVoice=(DAN_MAX+2);
						}
						else if((int)lOneWeight <= iMinimumWeight && (int)lOneWeight > iMinimumWeight/3)
						{
							VoicePlay(VOICE_TING);
						}
					  

					
					
					
					}

					bPreSignPlus = bNetSignPlus;
					lPreRef = gxlWeight;
					byVoiceOff = VOICEOFFDELAY;
					bGradeReady = 0;
					end_flag = 0;
				}
			}//  of if 	if (LAMPST && end_flag && bGradeReady && bSetMode2 == 1)



			bPreSignPlus = bNetSignPlus;
		}

		if (byVoiceOff) byVoiceOff--;
		else  {	bGradeReady = 1; }

//		if (byVoiceOff <= 1) VOICE(0);

		if (5 == StableCount)
		{	lPreRef = gxlWeight;		//MTS
			bPreSignPlus = bNetSignPlus;
		}
	} while (!exit_flag);
	ES = 0;
} /* of normal_mode() */


int iDanCheck()
{
	if (i1dan < i2dan)
	{
		VfdPrintf("ERR 4");
		return 10;
	}
	if (i2dan < i3dan)
	{
		VfdPrintf("ERR 4");
		return 11;
	}
	if (i3dan < i4dan)
	{

		VfdPrintf("ERR 4");
		return 12;
	}
	if (i4dan < i5dan)
	{
		VfdPrintf("ERR 4");
		return 13;
	}
	if (i5dan < i6dan)
	{
		VfdPrintf("ERR 4");
		return 14;
	}
	if (i6dan < i7dan)
	{
		VfdPrintf("ERR 4");
		return 15;
	}
	if (i7dan < i8dan)
	{
		VfdPrintf("ERR 4");
		return 16;
	}
	if (i8dan < i9dan)
	{
		VfdPrintf("ERR 4");
		return 17;
	}
	if (i9dan < i10dan)
	{
		VfdPrintf("ERR 4");
		return 18;
	}
	if (i10dan < i11dan)
	{
		VfdPrintf("ERR 4");
		return 19;
	}
	if (i11dan < i12dan)
	{
		VfdPrintf("ERR 4");
		return 20;
	}
	if (i12dan < i13dan)
	{
		VfdPrintf("ERR 4");
		return 21;
	}
	if (i13dan < i14dan)
	{
		VfdPrintf("ERR 4");
		return 22;
	}
	if (i14dan < i15dan)
	{
		VfdPrintf("ERR 4");
		return 23;
	}
	if (i15dan < i16dan)
	{
		VfdPrintf("ERR 4");
		return 24;
	}
	if (i16dan < i17dan)
	{
		VfdPrintf("ERR 4");
		return 25;
	}
	if (i17dan < i18dan)
	{
		VfdPrintf("ERR 4");
		return 26;
	}
	if (i18dan < i19dan)
	{
		VfdPrintf("ERR 4");
		return 27;
	}

//ver 1.01
   	if (i19dan < i20dan)
	{
		VfdPrintf("ERR 4");
		return 28;
	}

	if (i20dan < i21dan)
	{
		VfdPrintf("ERR 4");
		return 29;
	}

   if (i21dan < i22dan)
	{
		VfdPrintf("ERR 4");
		return 30;
	}

   if (i22dan < i23dan)
	{
		VfdPrintf("ERR 4");
		return 31;
	}

   if (i23dan < i24dan)
	{
		VfdPrintf("ERR 4");
		return 32;
	}

   if (i24dan < i25dan)
	{
		VfdPrintf("ERR 4");
		return 33;
	}

	return	0;
}


void DefaultDanSetting() //6월20일 
{
   /*
	if(gxhCurrentType == 0)	//  포도 
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
	 */
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
	//jado - daiseok 10kg
//자두   
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
 


 // 복숭아 
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

  // 복 숭아 



// 사과 

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
//사과 

   // 감  
   
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
 
 //감 
 
 
  //배 
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
 //배 
//>>

//한라봉 

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
      */

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
//한라봉 

//레드향 
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


//특작
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
  // 특작 
}
//2014년10월6일 

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
    if(gxhCurrentType==8)
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
  // 특작 



}


 /*

void DanDisplay(char dan)
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
				fast_evalue((long)i1dan);
				gxiIndex_End=3;
				iMinimumWeight = (i1dan*2)/3;
			}
			VfdDan(1);
			break;
		case 2:
			if (i1dan&&i2dan)
			{
				fast_evalue((long)i2dan);
				gxiIndex_End=4;
				iMinimumWeight = (i2dan*2)/3;
			}
			else
			{
				vputs("-----");
				i2dan = 0;
			}
			VfdDan(2);
			break;
		case 3:
			if (i1dan&&i2dan&&i3dan)
			{
				fast_evalue((long)i3dan);
				gxiIndex_End=5;
				iMinimumWeight = (i3dan*2)/3;
			}
			else
			{
				vputs("-----");
				i3dan = 0;
			}
			VfdDan(3);
			break;
		case 4:
			if (i1dan&&i2dan&&i3dan&&i4dan)
			{
				fast_evalue((long)i4dan);
				gxiIndex_End=6;
				iMinimumWeight = (i4dan*2)/3;
			}
			else
			{
				vputs("-----");
				i4dan = 0;
			}
			VfdDan(4);
			break;
		case 5:
			if (i1dan&&i2dan&&i3dan&&i4dan&&i5dan)
			{
				fast_evalue((long)i5dan);
				gxiIndex_End=7;
				iMinimumWeight = (i5dan*2)/3;
			}
			else
			{
				vputs("-----");
				i5dan = 0;
			}
			VfdDan(5);
			break;
		case 6:
			if (i1dan&&i2dan&&i3dan&&i4dan&&i5dan&&i6dan)
			{
				fast_evalue((long)i6dan);
				gxiIndex_End=8;
				iMinimumWeight = (i6dan*2)/3;
			}
			else
			{
				vputs("-----");
				i6dan = 0;
			}
			VfdDan(6);
			break;
		case 7:
			if (i1dan&&i2dan&&i3dan&&i4dan&&i5dan&&i6dan&&i7dan)
			{
				fast_evalue((long)i7dan);
				gxiIndex_End=9;
				iMinimumWeight = (i7dan*2)/3;
			}
			else
			{
				vputs("-----");
				i7dan = 0;
			}
			VfdDan(7);
			break;
		case 8:
			if (i1dan&&i2dan&&i3dan&&i4dan&&i5dan&&i6dan&&i7dan&&i8dan)
			{
				fast_evalue((long)i8dan);
				gxiIndex_End=10;
				iMinimumWeight = (i8dan*2)/3;
			}
			else
			{
				vputs("-----");
				i8dan = 0;
			}
			VfdDan(8);
			break;
		case 9:
			if (i1dan&&i2dan&&i3dan&&i4dan&&i5dan&&i6dan&&i7dan&&i8dan&&i9dan)
			{
				fast_evalue((long)i9dan);
				gxiIndex_End=11;
				iMinimumWeight = (i9dan*2)/3;
			}
			else
			{
				vputs("-----");
				i9dan = 0;
			}
			VfdDan(9);
			break;
		case 10:
			if (i1dan&&i2dan&&i3dan&&i4dan&&i5dan&&i6dan&&i7dan&&i8dan&&i9dan&&i10dan)
			{
				fast_evalue((long)i10dan);
				gxiIndex_End=12;
				iMinimumWeight = (i10dan*2)/3;
			}
			else
			{
				vputs("-----");
				i10dan = 0;
			}
			VfdDan(10);
			break;
		case 11:
			if (i1dan&&i2dan&&i3dan&&i4dan&&i5dan&&i6dan&&i7dan&&i8dan&&i9dan&&i10dan&&i11dan)
			{
				fast_evalue((long)i11dan);
				gxiIndex_End=13;
				iMinimumWeight = (i11dan*2)/3;
			}
			else
			{
				vputs("-----");
				i11dan = 0;
			}
			VfdDan(11);
			break;
		case 12:
			if (i1dan&&i2dan&&i3dan&&i4dan&&i5dan&&i6dan&&i7dan&&i8dan&&i9dan&&i10dan&&i11dan&&i12dan)
			{
				fast_evalue((long)i12dan);
				gxiIndex_End=14;
				iMinimumWeight = (i12dan*2)/3;
			}
			else
			{
				vputs("-----");
				i12dan = 0;
			}
			VfdDan(12);
			break;
		case 13:
			if (i1dan&&i2dan&&i3dan&&i4dan&&i5dan&&i6dan&&i7dan&&i8dan&&i9dan&&i10dan&&i11dan&&i12dan&&i13dan)
			{
				fast_evalue((long)i13dan);
				gxiIndex_End=15;
				iMinimumWeight = (i13dan*2)/3;
			}
			else
			{
				vputs("-----");
				i13dan = 0;
			}
			VfdDan(13);
			break;
		case 14:
			if (i1dan&&i2dan&&i3dan&&i4dan&&i5dan&&i6dan&&i7dan&&i8dan&&i9dan&&i10dan&&i11dan&&i12dan&&i13dan&&i14dan)
			{
				fast_evalue((long)i14dan);
				gxiIndex_End=16;
				iMinimumWeight = (i14dan*2)/3;
			}
			else
			{
				vputs("-----");
				i14dan = 0;
			}
			VfdDan(14);
			break;

		case 15:
			if (i1dan&&i2dan&&i3dan&&i4dan&&i5dan&&i6dan&&i7dan&&i8dan&&i9dan&&i10dan&&i11dan&&i12dan&&i13dan&&i14dan&&i15dan)
			{
				fast_evalue((long)i15dan);
				gxiIndex_End=17;
				iMinimumWeight = (i15dan*2)/3;
			}
			else
			{
				vputs("-----");
				i15dan = 0;
			}
			VfdDan(15);
			break;

		case 16:
			if (i1dan&&i2dan&&i3dan&&i4dan&&i5dan&&i6dan&&i7dan&&i8dan&&i9dan&&i10dan&&i11dan&&i12dan&&i13dan&&i14dan&&i15dan&&i16dan)
			{
				fast_evalue((long)i16dan);
				gxiIndex_End=18;
				iMinimumWeight = (i16dan*2)/3;
			}
			else
			{
				vputs("-----");
				i16dan = 0;
			}
			VfdDan(16);
			break;

		case 17:
			if (i1dan&&i2dan&&i3dan&&i4dan&&i5dan&&i6dan&&i7dan&&i8dan&&i9dan&&i10dan&&i11dan&&i12dan&&i13dan&&i14dan&&i15dan&&i16dan&&i17dan)
			{
				fast_evalue((long)i17dan);
				gxiIndex_End=19;
				iMinimumWeight = (i17dan*2)/3;
			}
			else
			{
				vputs("-----");
				i17dan = 0;
			}
			VfdDan(17);
			break;

		case 18:
		 fast_evalue((long)i18dan);
				gxiIndex_End=20;
				iMinimumWeight = (i18dan*2)/3;
		   
			if (i1dan&&i2dan&&i3dan&&i4dan&&i5dan&&i6dan&&i7dan&&i8dan&&i9dan&&i10dan&&i11dan&&i12dan&&i13dan&&i14dan&&i15dan&&i16dan&&i17dan&&i18dan)
			{
			  // i18dan=123;
				fast_evalue((long)i18dan);
				gxiIndex_End=20;
				iMinimumWeight = (i18dan*2)/3;
			}
		else
			{
				vputs("-----");
				i18dan = 0;
			}
			 
			
			VfdDan(18);
			break;

		case 19:
			if (i1dan&&i2dan&&i3dan&&i4dan&&i5dan&&i6dan&&i7dan&&i8dan&&i9dan&&i10dan&&i11dan&&i12dan&&i13dan&&i14dan&&i15dan&&i16dan&&i17dan&&i18dan&&i19dan)
			{
				fast_evalue((long)i19dan);
				gxiIndex_End=21;
				iMinimumWeight = (i19dan*2)/3;
			}
			else
			{
				vputs("-----");
				i19dan = 0;
			}
			VfdDan(19);
			break;

		default:
				break;
	}
//	gxiIndex_End=16;

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
		default:
			iMinimumWeight = (i15dan*2)/3;
			break;
	}

	if (!i15dan) iMinimumWeight = (i14dan*2)/3;
	if (!i14dan) iMinimumWeight = (i13dan*2)/3;
	if (!i13dan) iMinimumWeight = (i12dan*2)/3;
	if (!i12dan) iMinimumWeight = (i11dan*2)/3;
	if (!i11dan) iMinimumWeight = (i10dan*2)/3;
	if (!i10dan) iMinimumWeight = (i9dan*2)/3;
	if (!i9dan)  iMinimumWeight = (i8dan*2)/3;
	if (!i8dan)  iMinimumWeight = (i7dan*2)/3;
	if (!i7dan)  iMinimumWeight = (i6dan*2)/3;
	if (!i6dan)  iMinimumWeight = (i5dan*2)/3;
	if (!i5dan)  iMinimumWeight = (i4dan*2)/3;
	if (!i4dan)  iMinimumWeight = (i3dan*2)/3;
	if (!i3dan)  iMinimumWeight = (i2dan*2)/3;
	if (!i2dan)  iMinimumWeight = (i1dan*2)/3;
	if (!i1dan)  iMinimumWeight = (i1dan*2)/3;

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
}
  */

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
				fast_evalue((long)i1dan);
				gxiIndex_End=3;
				iMinimumWeight = (i1dan*2)/3;
			}
			VfdDan(1);
			break;
		case 2:
			if (i1dan&&i2dan)
			{
				fast_evalue((long)i2dan);
				gxiIndex_End=4;
				iMinimumWeight = (i2dan*2)/3;
			}
			else
			{
				vputs("-----");
				i2dan = 0;
			}
			VfdDan(2);
			break;
		case 3:
			if (i1dan&&i2dan&&i3dan)
			{
				fast_evalue((long)i3dan);
				gxiIndex_End=5;
				iMinimumWeight = (i3dan*2)/3;
			}
			else
			{
				vputs("-----");
				i3dan = 0;
			}
			VfdDan(3);
			break;
		case 4:
			if (i1dan&&i2dan&&i3dan&&i4dan)
			{
				fast_evalue((long)i4dan);
				gxiIndex_End=6;
				iMinimumWeight = (i4dan*2)/3;
			}
			else
			{
				vputs("-----");
				i4dan = 0;
			}
			VfdDan(4);
			break;
		case 5:
			if (i1dan&&i2dan&&i3dan&&i4dan&&i5dan)
			{
				fast_evalue((long)i5dan);
				gxiIndex_End=7;
				iMinimumWeight = (i5dan*2)/3;
			}
			else
			{
				vputs("-----");
				i5dan = 0;
			}
			VfdDan(5);
			break;
		case 6:
			if (i1dan&&i2dan&&i3dan&&i4dan&&i5dan&&i6dan)
			{
				fast_evalue((long)i6dan);
				gxiIndex_End=8;
				iMinimumWeight = (i6dan*2)/3;
			}
			else
			{
				vputs("-----");
				i6dan = 0;
			}
			VfdDan(6);
			break;
		case 7:
			if (i1dan&&i2dan&&i3dan&&i4dan&&i5dan&&i6dan&&i7dan)
			{
				fast_evalue((long)i7dan);
				gxiIndex_End=9;
				iMinimumWeight = (i7dan*2)/3;
			}
			else
			{
				vputs("-----");
				i7dan = 0;
			}
			VfdDan(7);
			break;
		case 8:
			if (i1dan&&i2dan&&i3dan&&i4dan&&i5dan&&i6dan&&i7dan&&i8dan)
			{
				fast_evalue((long)i8dan);
				gxiIndex_End=10;
				iMinimumWeight = (i8dan*2)/3;
			}
			else
			{
				vputs("-----");
				i8dan = 0;
			}
			VfdDan(8);
			break;
		case 9:
			if (i1dan&&i2dan&&i3dan&&i4dan&&i5dan&&i6dan&&i7dan&&i8dan&&i9dan)
			{
				fast_evalue((long)i9dan);
				gxiIndex_End=11;
				iMinimumWeight = (i9dan*2)/3;
			}
			else
			{
				vputs("-----");
				i9dan = 0;
			}
			VfdDan(9);
			break;
		case 10:
			if (i1dan&&i2dan&&i3dan&&i4dan&&i5dan&&i6dan&&i7dan&&i8dan&&i9dan&&i10dan)
			{
				fast_evalue((long)i10dan);
				gxiIndex_End=12;
				iMinimumWeight = (i10dan*2)/3;
			}
			else
			{
				vputs("-----");
				i10dan = 0;
			}
			VfdDan(10);
			break;
		case 11:
			if (i1dan&&i2dan&&i3dan&&i4dan&&i5dan&&i6dan&&i7dan&&i8dan&&i9dan&&i10dan&&i11dan)
			{
				fast_evalue((long)i11dan);
				gxiIndex_End=13;
				iMinimumWeight = (i11dan*2)/3;
			}
			else
			{
				vputs("-----");
				i11dan = 0;
			}
			VfdDan(11);
			break;
		case 12:
			if (i1dan&&i2dan&&i3dan&&i4dan&&i5dan&&i6dan&&i7dan&&i8dan&&i9dan&&i10dan&&i11dan&&i12dan)
			{
				fast_evalue((long)i12dan);
				gxiIndex_End=14;
				iMinimumWeight = (i12dan*2)/3;
			}
			else
			{
				vputs("-----");
				i12dan = 0;
			}
			VfdDan(12);
			break;
		case 13:
			if (i1dan&&i2dan&&i3dan&&i4dan&&i5dan&&i6dan&&i7dan&&i8dan&&i9dan&&i10dan&&i11dan&&i12dan&&i13dan)
			{
				fast_evalue((long)i13dan);
				gxiIndex_End=15;
				iMinimumWeight = (i13dan*2)/3;
			}
			else
			{
				vputs("-----");
				i13dan = 0;
			}
			VfdDan(13);
			break;
		case 14:
			if (i1dan&&i2dan&&i3dan&&i4dan&&i5dan&&i6dan&&i7dan&&i8dan&&i9dan&&i10dan&&i11dan&&i12dan&&i13dan&&i14dan)
			{
				fast_evalue((long)i14dan);
				gxiIndex_End=16;
				iMinimumWeight = (i14dan*2)/3;
			}
			else
			{
				vputs("-----");
				i14dan = 0;
			}
			VfdDan(14);
			break;

		case 15:
			if (i1dan&&i2dan&&i3dan&&i4dan&&i5dan&&i6dan&&i7dan&&i8dan&&i9dan&&i10dan&&i11dan&&i12dan&&i13dan&&i14dan&&i15dan)
			{
				fast_evalue((long)i15dan);
				gxiIndex_End=17;
				iMinimumWeight = (i15dan*2)/3;
			}
			else
			{
				vputs("-----");
				i15dan = 0;
			}
			VfdDan(15);
			break;

		case 16:
			if (i1dan&&i2dan&&i3dan&&i4dan&&i5dan&&i6dan&&i7dan&&i8dan&&i9dan&&i10dan&&i11dan&&i12dan&&i13dan&&i14dan&&i15dan&&i16dan)
			{
				fast_evalue((long)i16dan);
				gxiIndex_End=18;
				iMinimumWeight = (i16dan*2)/3;
			}
			else
			{
				vputs("-----");
				i16dan = 0;
			}
			VfdDan(16);
			break;

		case 17:
			if (i1dan&&i2dan&&i3dan&&i4dan&&i5dan&&i6dan&&i7dan&&i8dan&&i9dan&&i10dan&&i11dan&&i12dan&&i13dan&&i14dan&&i15dan&&i16dan&&i17dan)
			{
				fast_evalue((long)i17dan);
				gxiIndex_End=19;
				iMinimumWeight = (i17dan*2)/3;
			}
			else
			{
				vputs("-----");
				i17dan = 0;
			}
			VfdDan(17);
			break;

		case 18:
		 fast_evalue((long)i18dan);
				gxiIndex_End=20;
				iMinimumWeight = (i18dan*2)/3;
		   
			if (i1dan&&i2dan&&i3dan&&i4dan&&i5dan&&i6dan&&i7dan&&i8dan&&i9dan&&i10dan&&i11dan&&i12dan&&i13dan&&i14dan&&i15dan&&i16dan&&i17dan&&i18dan)
			{
			  // i18dan=123;
				fast_evalue((long)i18dan);
				gxiIndex_End=20;
				iMinimumWeight = (i18dan*2)/3;
			}
		else
			{
				vputs("-----");
				i18dan = 0;
			}
			 
			
			VfdDan(18);
			break;

		case 19:
			if (i1dan&&i2dan&&i3dan&&i4dan&&i5dan&&i6dan&&i7dan&&i8dan&&i9dan&&i10dan&&i11dan&&i12dan&&i13dan&&i14dan&&i15dan&&i16dan&&i17dan&&i18dan&&i19dan)
			{
				fast_evalue((long)i19dan);
				gxiIndex_End=21;
				iMinimumWeight = (i19dan*2)/3;
			}
			else
			{
				vputs("-----");
				i19dan = 0;
			}
			VfdDan(19);
			break;


// ver 1.01


       		case 20:
			if (i1dan&&i2dan&&i3dan&&i4dan&&i5dan&&i6dan&&i7dan&&i8dan&&i9dan&&i10dan&&i11dan&&i12dan&&i13dan&&i14dan&&i15dan&&i16dan&&i17dan&&i18dan&&i19dan
			    &&i20dan)
			{
				fast_evalue((long)i20dan);
				gxiIndex_End=22;
				iMinimumWeight = (i20dan*2)/3;
			}
			else
			{
				vputs("-----");
				i20dan = 0;
			}
			VfdDan(20);
			break;

		  case 21:
			if (i1dan&&i2dan&&i3dan&&i4dan&&i5dan&&i6dan&&i7dan&&i8dan&&i9dan&&i10dan&&i11dan&&i12dan&&i13dan&&i14dan&&i15dan&&i16dan&&i17dan&&i18dan&&i19dan
			    &&i20dan&&i21dan)
			{
				fast_evalue((long)i21dan);
				gxiIndex_End=23;
				iMinimumWeight = (i21dan*2)/3;
			}
			else
			{
				vputs("-----");
				i21dan = 0;
			}
			VfdDan(21);
			break;

         case 22:
			if (i1dan&&i2dan&&i3dan&&i4dan&&i5dan&&i6dan&&i7dan&&i8dan&&i9dan&&i10dan&&i11dan&&i12dan&&i13dan&&i14dan&&i15dan&&i16dan&&i17dan&&i18dan&&i19dan
			    &&i20dan&&i21dan&&i22dan)
			{
				fast_evalue((long)i22dan);
				gxiIndex_End=24;
				iMinimumWeight = (i22dan*2)/3;
			}
			else
			{
				vputs("-----");
				i22dan = 0;
			}
			VfdDan(22);
			break;

	    case 23:
			if (i1dan&&i2dan&&i3dan&&i4dan&&i5dan&&i6dan&&i7dan&&i8dan&&i9dan&&i10dan&&i11dan&&i12dan&&i13dan&&i14dan&&i15dan&&i16dan&&i17dan&&i18dan&&i19dan
			    &&i20dan&&i21dan&&i22dan&&i23dan)
			{
				fast_evalue((long)i23dan);
				gxiIndex_End=25;
				iMinimumWeight = (i23dan*2)/3;
			}
			else
			{
				vputs("-----");
				i23dan = 0;
			}
			VfdDan(23);
			break;

		case 24:
			if (i1dan&&i2dan&&i3dan&&i4dan&&i5dan&&i6dan&&i7dan&&i8dan&&i9dan&&i10dan&&i11dan&&i12dan&&i13dan&&i14dan&&i15dan&&i16dan&&i17dan&&i18dan&&i19dan
			    &&i20dan&&i21dan&&i22dan&&i23dan&&i24dan)
			{
				fast_evalue((long)i24dan);
				gxiIndex_End=26;
				iMinimumWeight = (i24dan*2)/3;
			}
			else
			{
				vputs("-----");
				i24dan = 0;
			}
			VfdDan(24);
			break;
        /*
		 case 25:	//ver 1.01
			if (i1dan&&i2dan&&i3dan&&i4dan&&i5dan&&i6dan&&i7dan&&i8dan&&i9dan&&i10dan&&i11dan&&i12dan&&i13dan&&i14dan&&i15dan&&i16dan&&i17dan&&i18dan&&i19dan
			    &&i20dan&&i21dan&&i22dan&&i23dan&&i24dan&&i25dan)
			{
				fast_evalue((long)i25dan);
				gxiIndex_End=27;
				iMinimumWeight = (i25dan*2)/3;
			}
			else
			{
				vputs("-----");
				i25dan = 0;
			}
			VfdDan(25);
			break;
		*/
   		  



//ver1.01



		default:
				break;
	}
//	gxiIndex_End=16;
   /*
	switch(gxiIndex_End) //VER 1.01
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
	*/
   //VER 1.01
    if (!i25dan) iMinimumWeight = (i24dan*2)/3;
	if (!i24dan) iMinimumWeight = (i23dan*2)/3;
	if (!i23dan) iMinimumWeight = (i22dan*2)/3;
    if (!i22dan) iMinimumWeight = (i21dan*2)/3;
	if (!i21dan) iMinimumWeight = (i20dan*2)/3;
	if (!i20dan) iMinimumWeight = (i19dan*2)/3;
	if (!i19dan) iMinimumWeight = (i18dan*2)/3;
	if (!i18dan) iMinimumWeight = (i17dan*2)/3;
	if (!i17dan) iMinimumWeight = (i16dan*2)/3;
	if (!i16dan) iMinimumWeight = (i15dan*2)/3;
  //VER 1.01



	if (!i15dan) iMinimumWeight = (i14dan*2)/3;
	if (!i14dan) iMinimumWeight = (i13dan*2)/3;
	if (!i13dan) iMinimumWeight = (i12dan*2)/3;
	if (!i12dan) iMinimumWeight = (i11dan*2)/3;
	if (!i11dan) iMinimumWeight = (i10dan*2)/3;
	if (!i10dan) iMinimumWeight = (i9dan*2)/3;
	if (!i9dan)  iMinimumWeight = (i8dan*2)/3;
	if (!i8dan)  iMinimumWeight = (i7dan*2)/3;
	if (!i7dan)  iMinimumWeight = (i6dan*2)/3;
	if (!i6dan)  iMinimumWeight = (i5dan*2)/3;
	if (!i5dan)  iMinimumWeight = (i4dan*2)/3;
	if (!i4dan)  iMinimumWeight = (i3dan*2)/3;
	if (!i3dan)  iMinimumWeight = (i2dan*2)/3;
	if (!i2dan)  iMinimumWeight = (i1dan*2)/3;
	if (!i1dan)  iMinimumWeight = (i1dan*2)/3;
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
	byDan[54]=i19dan/100;byDan[55]=i19dan%100;byDan[55]/=10;byDan[56]=i19dan%10;   //ver1.01

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



/*
void DanSetting()
{
	if (i1dan == 0)
		i2dan=i3dan=i4dan=i5dan=i6dan=i7dan=i8dan=i9dan=i10dan=i11dan=i12dan=i13dan=i14dan=i15dan=i16dan=i17dan=i18dan=i19dan=0;
	if (i2dan == 0)
		i3dan=i4dan=i5dan=i6dan=i7dan=i8dan=i9dan=i10dan=i11dan=i12dan=i13dan=i14dan=i15dan=i16dan=i17dan=i18dan=i19dan=0;
	if (i3dan == 0)
		i4dan=i5dan=i6dan=i7dan=i8dan=i9dan=i10dan=i11dan=i12dan=i13dan=i14dan=i15dan=i16dan=i17dan=i18dan=i19dan=0;
	if (i4dan == 0)
		i5dan=i6dan=i7dan=i8dan=i9dan=i10dan=i11dan=i12dan=i13dan=i14dan=i15dan=i16dan=i17dan=i18dan=i19dan=0;
	if (i5dan == 0)
		i6dan=i7dan=i8dan=i9dan=i10dan=i11dan=i12dan=i13dan=i14dan=i15dan=i16dan=i17dan=i18dan=i19dan=0;
	if (i6dan == 0)
		i7dan=i8dan=i9dan=i10dan=i11dan=i12dan=i13dan=i14dan=i15dan=i16dan=i17dan=i18dan=i19dan=0;
	if (i7dan == 0)
		i8dan=i9dan=i10dan=i11dan=i12dan=i13dan=i14dan=i15dan=i16dan=i17dan=i18dan=i19dan=0;
	if (i8dan == 0)
		i9dan=i10dan=i11dan=i12dan=i13dan=i14dan=i15dan=i16dan=i17dan=i18dan=i19dan=0;
	if (i9dan == 0)
		i10dan=i11dan=i12dan=i13dan=i14dan=i15dan=i16dan=i17dan=i18dan=i19dan=0;
	if (i10dan == 0)
		i11dan=i12dan=i13dan=i14dan=i15dan=i16dan=i17dan=i18dan=i19dan=0;
	if (i11dan == 0)
		i12dan=i13dan=i14dan=i15dan=i16dan=i17dan=i18dan=i19dan=0;
	if (i12dan == 0)
		i13dan=i14dan=i15dan=i16dan=i17dan=i18dan=i19dan=0;
	if (i13dan == 0)
		i14dan=i15dan=i16dan=i17dan=i18dan=i19dan=0;
	if (i14dan == 0)
		i15dan=i16dan=i17dan=i18dan=i19dan=0;
	if (i15dan == 0)
		i16dan=i17dan=i18dan=i19dan=0;
	if (i16dan == 0)
		i17dan=i18dan=i19dan=0;
	if (i17dan == 0)
		i18dan=i19dan=0;
	if (i18dan == 0)
		i19dan=0;

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
}

*/

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



   	//ver1.01
	byDan[57]=i20dan/100;byDan[58]=i20dan%100;byDan[58]/=10;byDan[59]=i20dan%10;
    byDan[60]=i21dan/100;byDan[61]=i21dan%100;byDan[61]/=10;byDan[62]=i21dan%10;
	byDan[63]=i22dan/100;byDan[64]=i22dan%100;byDan[64]/=10;byDan[65]=i22dan%10;
    byDan[66]=i23dan/100;byDan[67]=i23dan%100;byDan[67]/=10;byDan[68]=i23dan%10;
    byDan[69]=i24dan/100;byDan[70]=i24dan%100;byDan[70]/=10;byDan[71]=i24dan%10;
    byDan[72]=i25dan/100;byDan[73]=i25dan%100;byDan[73]/=10;byDan[74]=i25dan%10;

 
  



 }
