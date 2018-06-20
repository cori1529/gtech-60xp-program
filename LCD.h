/*
	lcd.h
*/

#ifndef __LCD_H
#define __LCD_H

#include "stm32f10x_type.h"

//GP-60XP (2018년형 NEW 선별기)


#define  SEGMENT_A       GPIO_Pin_0      
#define  SEGMENT_B       GPIO_Pin_1        
#define  SEGMENT_C       GPIO_Pin_2        
#define  SEGMENT_D       GPIO_Pin_3        
#define  SEGMENT_E       GPIO_Pin_4        
#define  SEGMENT_F       GPIO_Pin_5        
#define  SEGMENT_G       GPIO_Pin_6        
#define  SEGMENT_DOT     GPIO_Pin_7



#define  DANSU_LED_ON   Display_buffer[12]=(Display_buffer[12]|0x01)
#define  DANSU_LED_OFF   Display_buffer[12]=(Display_buffer[12]&0xfe)

#define  POJANG_LED_ON    Display_buffer[12]=(Display_buffer[12]|0x02)//+=SEGMENT_B
#define  POJANG_LED_OFF   Display_buffer[12]=(Display_buffer[12]&0xfd)//-=SEGMENT_B

#define  ZERO_LED_ON    Display_buffer[12]=(Display_buffer[12]|0x04)  // 0000 0100 

#define  ZERO_LED_OFF   Display_buffer[12]=(Display_buffer[12]&0xfb)  // 1111  1011

#define  SPEED_LED_ON    Display_buffer[12]=(Display_buffer[12]|0x08)//+=SEGMENT_D  0000 1000
#define  SPEED_LED_OFF   Display_buffer[12]=(Display_buffer[12]&0xf7)//-=SEGMENT_D  1111 0111 

#define  GRAPE_LED_ON    Display_buffer[12]=(Display_buffer[12]|0x10)//+=SEGMENT_E
#define  GRAPE_LED_OFF   Display_buffer[12]=(Display_buffer[12]&0xef)//-=SEGMENT_E

#define  JADOO_LED_ON    Display_buffer[12]=(Display_buffer[12]|0x20)//+=SEGMENT_F
#define  JADOO_LED_OFF   Display_buffer[12]=(Display_buffer[12]&0xdf)//-=SEGMENT_F

#define  PEACH_LED_ON    Display_buffer[12]=(Display_buffer[12]|0x40)//+=SEGMENT_G
#define  PEACH_LED_OFF   Display_buffer[12]=(Display_buffer[12]&0xbf)//-=SEGMENT_G


#define  APPLE_LED_ON    Display_buffer[13]=(Display_buffer[13]|0x02)//+=SEGMENT_B
#define  APPLE_LED_OFF   Display_buffer[13]=(Display_buffer[13]&0xfd)//-=SEGMENT_B

#define  GAM_LED_ON    Display_buffer[13]=(Display_buffer[13]|0x04);//+=SEGMENT_C
#define  GAM_LED_OFF   Display_buffer[13]=(Display_buffer[13]&0xfb);//-=SEGMENT_C

#define  PEAR_LED_ON    Display_buffer[13]=(Display_buffer[13]|0x08);//+=SEGMENT_D
#define  PEAR_LED_OFF   Display_buffer[13]=(Display_buffer[13]&0xf7);//-=SEGMENT_D

#define  HALABONG_LED_ON    Display_buffer[13]=(Display_buffer[13]|0x01);//+=SEGMENT_A
#define  HALABONG_LED_OFF   Display_buffer[13]=(Display_buffer[13]&0xfe);//-=SEGMENT_A

#define  RED_LED_ON    Display_buffer[13]=(Display_buffer[13]|0x10);//+=SEGMENT_E
#define  RED_LED_OFF   Display_buffer[13]=(Display_buffer[13]&0xef);//-=SEGMENT_E

#define  PUMPKIN_LED_ON    Display_buffer[13]=(Display_buffer[13]|0x20);//+=SEGMENT_F
#define  PUMPKIN_LED_OFF   Display_buffer[13]=(Display_buffer[13]&0xdf);//-=SEGMENT_F

#define  SPECIAL_LED_ON    Display_buffer[13]=(Display_buffer[13]|0x40);//+=SEGMENT_G
#define  SPECIAL_LED_OFF   Display_buffer[13]=(Display_buffer[13]&0xbf);//-=SEGMENT_G


#define CHAR_BAR         SEGMENT_G

#define  LAMP_ZERO		SEGMENT_A
#define  LAMP_STBL		SEGMENT_B
#define  LAMP_HOLD		SEGMENT_C
#define  LAMP_TARE		SEGMENT_D
#define  LAMP_LOW		SEGMENT_E
#define  LAMP_HIGH		SEGMENT_F
#define  LAMP_COMM		SEGMENT_G
#define  LAMP_DOT		SEGMENT_DOT		//NOT USED
   
#define  SEGMENT_ZERO       SEGMENT_A+ SEGMENT_B +SEGMENT_C+ SEGMENT_D+SEGMENT_E+ SEGMENT_F       
#define  SEGMENT_ONE        SEGMENT_B+ SEGMENT_C      
#define  SEGMENT_TWO        SEGMENT_A+ SEGMENT_B +SEGMENT_G+ SEGMENT_E + SEGMENT_D       
#define  SEGMENT_THREE      SEGMENT_A+ SEGMENT_B +SEGMENT_G+ SEGMENT_C + SEGMENT_D       
#define  SEGMENT_FOUR       SEGMENT_B +SEGMENT_G+ SEGMENT_C + SEGMENT_F        
#define  SEGMENT_FIVE       SEGMENT_A +SEGMENT_G+ SEGMENT_C + SEGMENT_F+ SEGMENT_D       
#define  SEGMENT_SIX        SEGMENT_A +SEGMENT_F+ SEGMENT_E + SEGMENT_D+ SEGMENT_C + SEGMENT_G        
#define  SEGMENT_SEVEN      SEGMENT_A +SEGMENT_B+ SEGMENT_C + SEGMENT_F        
#define  SEGMENT_EIGHT      SEGMENT_A +SEGMENT_B+ SEGMENT_C + SEGMENT_D +SEGMENT_E+ SEGMENT_F + SEGMENT_G      
#define  SEGMENT_NINE      SEGMENT_A +SEGMENT_B+ SEGMENT_C +  SEGMENT_F + SEGMENT_G  
#define  SEGMENT_NONE      ~SEGMENT_A +~SEGMENT_B+ ~SEGMENT_C +  ~SEGMENT_D + ~SEGMENT_E+~SEGMENT_F+ ~SEGMENT_G +~SEGMENT_DOT
#define  SEGMENT_ALL		SEGMENT_A +SEGMENT_B+ SEGMENT_C + SEGMENT_D +SEGMENT_E+ SEGMENT_F + SEGMENT_G + SEGMENT_DOT      

#define  LEVEL_ONE         SEGMENT_A    
#define  LEVEL_TWO         SEGMENT_A + SEGMENT_B   
#define  LEVEL_THREE       SEGMENT_A + SEGMENT_B + SEGMENT_C  
#define  LEVEL_FOUR        SEGMENT_A + SEGMENT_B + SEGMENT_C + SEGMENT_D 
#define  LEVEL_FIVE        SEGMENT_A + SEGMENT_B + SEGMENT_C + SEGMENT_D+ SEGMENT_E  
#define  LEVEL_SIX         SEGMENT_A + SEGMENT_B + SEGMENT_C + SEGMENT_D+ SEGMENT_E + SEGMENT_F  
#define  LEVEL_NONE        ~SEGMENT_A + ~SEGMENT_B + ~SEGMENT_C + ~SEGMENT_D+ ~SEGMENT_E + ~SEGMENT_F + ~SEGMENT_G +~SEGMENT_DOT  


//#define  ZERO_ON         SEGMENT_A    
//#define  ZERO_OFF        LEVEL_NONE

#define  TARE_ON         SEGMENT_B    
#define  TARE_OFF        ~SEGMENT_B

#define  THIRD_ON        SEGMENT_C    
#define  THIRD_OFF       ~SEGMENT_C

#define  FOURTH_ON        SEGMENT_D    
#define  FOURTH_OFF       ~SEGMENT_D



#define  NUM_0   SEGMENT_A + SEGMENT_B + SEGMENT_C + SEGMENT_D + SEGMENT_E + SEGMENT_F       
#define  NUM_1   SEGMENT_B + SEGMENT_C      
#define  NUM_2   SEGMENT_A + SEGMENT_B + SEGMENT_G + SEGMENT_E + SEGMENT_D       
#define  NUM_3   SEGMENT_A + SEGMENT_B + SEGMENT_G + SEGMENT_C + SEGMENT_D       
#define  NUM_4   SEGMENT_B + SEGMENT_G + SEGMENT_C + SEGMENT_F        
#define  NUM_5   SEGMENT_A + SEGMENT_G + SEGMENT_C + SEGMENT_F + SEGMENT_D       
#define  NUM_6   SEGMENT_A + SEGMENT_F + SEGMENT_E + SEGMENT_D + SEGMENT_C + SEGMENT_G        
#define  NUM_7   SEGMENT_A + SEGMENT_B + SEGMENT_C + SEGMENT_F        
#define  NUM_8   SEGMENT_A + SEGMENT_B + SEGMENT_C + SEGMENT_D + SEGMENT_E + SEGMENT_F + SEGMENT_G      
#define  NUM_9   SEGMENT_A + SEGMENT_B + SEGMENT_C + SEGMENT_D + SEGMENT_F + SEGMENT_G  

#define  ZERO_DOT    SEGMENT_ZERO+SEGMENT_DOT


#define	  CH_A       SEGMENT_A + SEGMENT_B + SEGMENT_C + SEGMENT_F + SEGMENT_E + SEGMENT_G
#define   CH_B       SEGMENT_F + SEGMENT_E + SEGMENT_G + SEGMENT_C + SEGMENT_D
#define   CH_C       SEGMENT_A + SEGMENT_D + SEGMENT_E + SEGMENT_F 
#define   CH_D       SEGMENT_B + SEGMENT_C + SEGMENT_G + SEGMENT_E + SEGMENT_D 
#define   CH_E       SEGMENT_A + SEGMENT_D + SEGMENT_E + SEGMENT_F + SEGMENT_G
#define   CH_F       SEGMENT_A + SEGMENT_F + SEGMENT_E + SEGMENT_G
#define   CH_G       SEGMENT_A + SEGMENT_C + SEGMENT_D + SEGMENT_E + SEGMENT_F + SEGMENT_G
#define   CH_H       SEGMENT_B + SEGMENT_C + SEGMENT_E + SEGMENT_F + SEGMENT_G
#define   CH_I       SEGMENT_B + SEGMENT_C
#define   CH_J       SEGMENT_B + SEGMENT_C  //// MODIFY
#define   CH_K       SEGMENT_F + SEGMENT_E + SEGMENT_C + SEGMENT_G	  //// MODIFY
#define   CH_L       SEGMENT_D + SEGMENT_E + SEGMENT_F
#define   CH_M       SEGMENT_C + SEGMENT_E + SEGMENT_G + SEGMENT_A
#define   CH_N       SEGMENT_C + SEGMENT_E + SEGMENT_G
#define   CH_O       SEGMENT_A + SEGMENT_B + SEGMENT_C + SEGMENT_D + SEGMENT_E + SEGMENT_F
#define   CH_P       SEGMENT_A + SEGMENT_B + SEGMENT_F + SEGMENT_E + SEGMENT_G
#define   CH_Q       SEGMENT_A + SEGMENT_B + SEGMENT_F + SEGMENT_E + SEGMENT_G // MODIFY
#define   CH_R       SEGMENT_G + SEGMENT_E
#define   CH_S       SEGMENT_G + SEGMENT_C + SEGMENT_F + SEGMENT_A + SEGMENT_D
#define   CH_T       SEGMENT_D + SEGMENT_E + SEGMENT_F + SEGMENT_G 
#define   CH_U       SEGMENT_F + SEGMENT_E + SEGMENT_C + SEGMENT_D + SEGMENT_B
#define   CH_V       SEGMENT_F + SEGMENT_E + SEGMENT_C + SEGMENT_D  // MODIFY
#define   CH_W       SEGMENT_E + SEGMENT_C + SEGMENT_D  // MODIFY
#define   CH_X       SEGMENT_E + SEGMENT_C + SEGMENT_D  // MODIFY
#define   CH_Y       SEGMENT_F + SEGMENT_B + SEGMENT_G + SEGMENT_C + SEGMENT_D  // MODIFY
#define   CH_Z       SEGMENT_A + SEGMENT_B + SEGMENT_G + SEGMENT_E + SEGMENT_D 


#define	  CH_a       SEGMENT_A + SEGMENT_B + SEGMENT_C + SEGMENT_F + SEGMENT_E + SEGMENT_G
#define   CH_b       SEGMENT_F + SEGMENT_E + SEGMENT_G + SEGMENT_C + SEGMENT_D
#define   CH_c       SEGMENT_G + SEGMENT_E + SEGMENT_D 
#define   CH_d       SEGMENT_B + SEGMENT_C + SEGMENT_G + SEGMENT_E + SEGMENT_D 
#define   CH_e       SEGMENT_A + SEGMENT_D + SEGMENT_E + SEGMENT_F + SEGMENT_G
#define   CH_f       SEGMENT_A + SEGMENT_F + SEGMENT_E + SEGMENT_G
#define   CH_g       SEGMENT_A + SEGMENT_C + SEGMENT_D + SEGMENT_E + SEGMENT_F + SEGMENT_G
#define   CH_h       SEGMENT_B + SEGMENT_C + SEGMENT_E + SEGMENT_F + SEGMENT_G
#define   CH_i       SEGMENT_C
#define   CH_j       SEGMENT_B + SEGMENT_C  //// MODIFY
#define   CH_k       SEGMENT_F + SEGMENT_E + SEGMENT_C + SEGMENT_G	  //// MODIFY
#define   CH_l       SEGMENT_D + SEGMENT_E + SEGMENT_F
#define   CH_m       SEGMENT_C + SEGMENT_E + SEGMENT_G + SEGMENT_A
#define   CH_n       SEGMENT_C + SEGMENT_E + SEGMENT_G
#define   CH_o       SEGMENT_A + SEGMENT_B + SEGMENT_C + SEGMENT_D + SEGMENT_E + SEGMENT_F
#define   CH_p       SEGMENT_A + SEGMENT_B + SEGMENT_F + SEGMENT_E + SEGMENT_G
#define   CH_q       SEGMENT_A + SEGMENT_B + SEGMENT_F + SEGMENT_E + SEGMENT_G // MODIFY
#define   CH_r       SEGMENT_G + SEGMENT_E
#define   CH_s       SEGMENT_G + SEGMENT_C + SEGMENT_F + SEGMENT_A + SEGMENT_D
#define   CH_t       SEGMENT_D + SEGMENT_E + SEGMENT_F + SEGMENT_G 
#define   CH_u       SEGMENT_E + SEGMENT_C + SEGMENT_D 	
#define   CH_v       SEGMENT_F + SEGMENT_E + SEGMENT_C + SEGMENT_D  // MODIFY
#define   CH_w       SEGMENT_E + SEGMENT_C + SEGMENT_D  // MODIFY
#define   CH_x       SEGMENT_E + SEGMENT_C + SEGMENT_D  // MODIFY
#define   CH_y       SEGMENT_F + SEGMENT_B + SEGMENT_G + SEGMENT_C + SEGMENT_D  // MODIFY
#define   CH_z       SEGMENT_A + SEGMENT_B + SEGMENT_G + SEGMENT_E + SEGMENT_D 


#define   CH_BAR     SEGMENT_G		 
#define   CH_EQUAL   SEGMENT_G + SEGMENT_D		 









extern const u16 font_fnd[];
extern const u16 dip_switch[];

//extern const u16 dip_switch[]={NUM_0 ,NUM_1 ,NUM_2 ,NUM_3 ,NUM_4 ,NUM_5 ,NUM_6 ,NUM_7 ,NUM_8 ,NUM_9 };

void vputchar(char digit, u16 ch);
void vcls(void);
void vputs(char *str);
void vputs_3(char *str);


//2016.3.2
void vputnum(char leadingzero, char dp, long num);
void vputnumwithled(char leadingzero, char dp, long num);
void vputnum_ndigit(char leadingzero, char dp, long num, int digit);
void vputnum_5digit(long num);
void vputnum_6digit(long num);


void vputgrade2digit(long num);// 등급
void vputgwa3digit(long num); //  몇과
void vputpojang2digit(long num);// 포장




void vputerr(char *err_msg);
void vputmsg(char *msg);

void Display_Weight(long weight);
void Display_Weight_test(long weight);

void Led_All_Off(void);
void Led_All_On(void);
void Led1_On(void);
void Led2_On(void);
void Led3_On(void);
void Led4_On(void);
void Led5_On(void);
void Led1_Off(void);
void Led2_Off(void);
void Led3_Off(void);
void Led4_Off(void);
void Led5_Off(void);






#endif // LCD_H
