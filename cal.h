/*
	cal.h
*/

#ifndef __CAL_H
#define __CAL_H

#include "stm32f10x_type.h"

extern s32	v_adc_org;
extern s32  v_adc1_buf;      //  = 0 

extern s32  v_adc1_org; //  = 0 
extern unsigned long  v_zero;
extern unsigned long  v_e_resolution;
extern unsigned long  v_i_resolution;
extern unsigned long  v_capacity;
extern unsigned long  v_setting_weight; //   = 5000; 
extern unsigned long  v_setting_rated_output;
extern unsigned char   v_decimal_point;


extern s32 Current_temp; //  현재온도 
extern vu16  ADCConvertedValue;

extern long  v_division;
extern long  v_ei_multiply_factor;

//extern unsigned char   v_decimal_point;
extern unsigned char   SIGNFLAG;

extern  u32	dZeroValue;	// zero calibration data - gain 1 - 64 
extern  u32	dFullValue;	// full calibration data - gain 1 - 64 
///unsigned char	bRepeat;



/*-------------  calibration mode function  ----------------*/
void    cal_mode(void);
void    cal_mode_1(void);
void	cal_mode_2(void);
void	cal_mode_eq_ref(void);
void calc_equ_factor(void);

int    little_span_adjust(void);
void    zero_span_set(unsigned char);
void	factor_access(void);

u32 power(int x, u8 n);

int Set_Weight(void);//  분동무게 구하기 


void cal_mode_TEST(void);

//void zero_set(void);
//void span_set(void);
int calc_factor(void);


#endif // CAL_H

/* end of file */
