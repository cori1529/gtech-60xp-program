#ifndef	__SETTING_H
#define	__SETTING_H

#include "stm32f10x_type.h"


#define   MAX_FILTER   50



extern u32 gxlDecimalPoint;				// gxlDecimalPoin 소수점 위치설정

extern u32 gxhWeightBackup;
extern u32 gxhMotionBand;
extern u32 gxhZTDigit;
extern u32 gxuAutoZero;
extern u32 gxhFilter;
extern u32 gxhZTKStable;
extern u32 gxlZeroKeyRange;
extern u32 gxlTareKeyRange;
extern u32 gxhHoldType;
extern u32 gxhEIMode;
extern u32 gxlKeySet;
extern u32 gxlCodeNumSet;
extern u32 gxlHoldOffTime;
extern vu32 gxhRelayMode;
extern u32 gxlDataOutDelay;
extern u32 gxlRelayOnDelay;
extern u32 gxlRelayOnTime;


extern vu32 gdhParity;
extern vu32 gxhBaudRate;
extern vu32 gxhSendMode;
extern vu32 gxlSerialMethod;
extern vu32 gxhMyAddress;
extern vu32 gxhRsFormat;
extern vu32 gxlBCCSelect;
extern vu32 gxlTransPerSec;
extern vu32 gxlWeightUnit;


extern u32 gxlDisplaySpeed;
extern u32 gxlStableOnDelay;
extern u32 gxlPushPull;
extern u32 gxhAroundZero;
extern u32 gxlZeroDisplayRange;
extern u32 gxlZeroMinus;
extern u32 gxlAnalogOutMax;
extern u32 gxlEquivalInput;
extern u32 gdwWhichCal;
extern u32 gxhGain;

extern u32 SP1;
extern u32 SP2;


long lValueSetMode(char number ,long init_value, long limit);
long lValueSetMode_5digit_with_dp(char f_num, long init_value, long limit, char dp);
void setting_mode(void);
void default_set();


#endif	// __SETTING_H