/*  -----------------------------------------------------------
   Filename  :  ST3.c
   
	1. compiler	
		Keil uVision 4 C51 v 9.02a Portable
		B:\Dropbox\project\tool

		IDE-Version:
		μVision V4.10
		Copyright (C) 2010 KEIL, An ARM(R) Company

		License Information:
		WinUE WinUE
		Windows uE
		LIC=YACTG-VBNEB-LY8V3-ADKJB-NP01M-W0Z98

		Tool Version Numbers:
		Toolchain:        PK51 Prof. Developers Kit  Version: 9.02		
		Toolchain Path:    C:\Keil\C51\BIN\
		C Compiler:         C51.Exe       V9.02
		Assembler:          A51.Exe       V8.02
		Linker/Locator:     BL51.Exe       V6.22
		Librarian:             LIB51.Exe       V4.24		
		Hex Converter:      OH51.Exe       V2.6
		CPU DLL:               S8051.DLL       V3.73
		Dialog DLL:         DP51.DLL       V2.61
		Target DLL:             BIN\MON51.DLL       V2.45
		Dialog DLL:         TP51.DLL       V2.58		

----------------------------------------------------------------------
	Ver 1.10 (2015.7.15)
	 
	 - Ver 1.01 에서 변경함.
	 -  등급 설정시 "0"인 단을 표시하지 않는거 무조건 마지막 단까지 표시하게 변경
----------------------------------------------------------------------

	Ver 2.12-2 (2008. 6. 30)
	 - 자두 선별단 값 변경
	   260, 240, 220, 200, 180, 160, 140, 120, 100, 80, 65, 50
	   
	Ver	2.01.000 (2007. 9. 14)
		- voice  delay   	
 

	Ver 2.00-2 (2007. 9. 3)
	 - 음성 ic 변경 API840N 에서 aP8942A로 변경
	 
	Ver 1.00-2 (2006. 4. 6)
	 - 영업부 협조요청서 (SD2006-3 / 2006년 4월 6일)
 1. 복숭아 선별단 값이 바뀜
             4.5kg포장갯수5kg포장갯수10kg포장갯수15kg포장갯수
   1 단(등급) 629  7  612  g  8  568  g 17  598  g 24
   2 단(등급) 555  8  548  g  9  510  g 19  534  g 27 
   3 단(등급) 496  9  496  g 10  464  g 21  482  g 30
   4 단(등급) 449 10  452  g 11  424  g 23  440  g 33 
   5 단(등급) 409 11  416  g 12  392  g 25  406  g 36 
   6 단(등급) 377 12  384  g 13  364  g 27  374  g 39 
   7 단(등급) 349 13  358  g 14  340  g 29  348  g 42 
   8 단(등급) 325 14  334  g 15  318  g 31  326  g 45 
   9 단(등급) 304 15  314  g 16  300  g 33  306  g 48
  10 단(등급) 285 16  296  g 17  282  g 35  288  g 51
  11 단(등급) 269 17  280  g 18  268  g 37  274  g 54
  12 단(등급) 254 18  266  g 19  254  g 39  260  g 57
  13 단(등급) 236 19  250  g 20  243  g 41  250  g 60
  14 단(등급) 225 20  214  g 21  235  g 43  238  g 63
   
	Ver 1.00.000 (2005. 5. 20)
	 1. from st2-1.20-1
	 2. grape, jadu, gam, bai, apple, peach



   -----------------------------------------------------------------
   Version   :  1.01.000 (2002. 2. 20)
			 - SET.C
			   Apple - 5kg, 10kg, 15kg
			 - NORMAL
			   Voice apple
			   gxhWeightAppleType=1,gxhWeightAppleType=2,gxhWeightAppleType=3,

   Version   :  1.00.000 (2000. 12.  1)
			 - key 3 * 5
			 - 93c56 change
			 - grape - 12dan   (4, 5, 10kg)
			   jado1 - 5dan    (10kg)
			   jado2 - 5dan    (10kg)
			   pear  - 12dan   (7.5, 15kg)
			   apple - 12dan   (10)
			   peack - 12dan   (5, 10, 15kg)
			 - set_mode (grape, pear, peack  select package)
			 - voice_rom change
			   voice_grape package
			   voice_jado1 package
			   voice_jado2 package
			   voice_pear package
			   voice_peack package
			   voice_apple package

   Copyright (c) 2001-2002 by G-TECH
   All Rights Reserved.
	------------------------------------------------------------

	60T.C

	Ver1.10.000 (2001. 9. 10)
	 - pear 1dan, 2dan, 3dan
	Ver1.00.000 (2000. 12. 3)
	 - dan soun heung
	Ver 1.00.000 (2000. 2. 25)
	 - delete count key
	 - delete count lamp
	 - add store key
	 - add M1, M2, M3 key
	 - resolution 1/6,000
	 - no key sort
	--------------------------------------------------------------
	MT
	Ver 1.02 (1999. 5. 14)
		- institution v_capacity 300Kg,600Kg
	Ver 1.01
		-
	Ver 1.00
		- basic GP-MS

	Ver 1.11 (1998. 8. 11)
	 + rename gi8500 to GP
	   RAM     0000 - 1XXX
	   KEY     6XXX - 7XXX
	   EXT LED 8XXX - 9XXX
	   DISPLAY AXXX - BXXX
	   VOICE   CXXX - DXXX
	Ver Import (1998. 6. 11)
	  + pyunghwa auto grader version 0.01

	Ver 1.20 (1998. 1. 9)
	  + Change Compile IAR 4.20C to KEIL 5.02
	  + change cpu 8032 to 8031
	  + adc.c
		- change ad speed 40/sec to 50/sec
	Ver 1.14 (1998. 1. 5)
	  + gi8500a.s03
		- replace RET to RETi
	Ver 1.13 (1997. 12. 29)
	  + display holding test
		- update hold function
	Ver 1.12 (1997. 12. 29)
	  + test only
		- display holding test
			- update key routine
	  + gi8500a.s03
		- Remove _keypush()
		- Remove kbhit(void)
		- Remove WAITKEYR(void);
		- Remove EEERASEALL:      ;chip erase all
	  + key.h
		- Replace because display holding until key press (noise)
			from    do { KEYINENC(); } while (!qKEYIN);
			to		KEYINENC();
					if (!qKEYIN) return 0;
	  + normal.c
		- Change UINT to BYTE
			- sec_now = 0;
			- count2  = 0;
			- az_ok;
			- StableCount = 0;
		- Remove Byte dp_pos=0
	Ver 1.11 (1997. 12. 26)
	  + normal.c
		- Add SendBuffer[6] = gxhMyAddress;
		- Read out High Limit value
		- Read out Low  Limit value
	Ver 1.10 (1997. 12. 4)
	  + gi8500.c
		- modified logo GI-8500 to GI-8500L
	  + set.c
		- Moved unsigned int dp_value_set to cal.c
		- modified DWORD dwValueSet(...) for Capacity set and Load weight
	  + cal.c
		- OneShot calibration
		- Changed maximum d from 999 to 9999.
		- Changed maximum Capacity from 50000 to 9999999.
		- New cal.h
	  + normal.c (file lost)
/		- Read out High Limit value
/		- Read out Low  Limit value
/		- Read out Tare value
/		- Read out Fall High limit value
/		- Read out Fall Low limit value
/		- Read out Serial No value
/		- Read out Rom Version
/		- Read out Print form
	Ver 1.01 (1997. 9. 10)
		- print.h
		- replace : below point always 00
		- 	  : total count + 1
		- logo() moved from vfd.c to gi8500.c
		- declear logo() moved from def.h to gi8500.c

	Copyright (c) 1997-1998 by G-TECH
	All Rights Reserved.
*/


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





#define  MODEL		"GP SERIES"
#define  MODELNO	69597195


void main()
{
	
	while(1)    //  loop
	{

		normal_mode();
	} /* of while(1) */
} /* of void main() */

