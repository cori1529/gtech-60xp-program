#ifndef	__SETTING_H
#define	__SETTING_H

#include "stm32f10x_type.h"

#define   MAX_FILTER   20

extern vu32 gnlCapacity;	//저장용 소숫점 뺀 capacity

extern vu32 gxlDecimalPoint; //gxlDecimalPoin 소수점 위치설정

extern vu32 gxhWeightBackup;
extern vu32 gxhMotionBand;
extern vu32 gxhZTDigit;
extern vs32 gxuAutoZero;
extern vu32 gxhFilter;
extern vu32 gxhZTKStable;
extern vu32 gxlZeroKeyRange;
extern vu32 gxlTareKeyRange;
extern vu32 gxlZeroKeyDisabled;
extern vu32 gxlTareKeyDisabled;

extern vu32 gxhHoldType;
extern vu32 gxhEIMode;
extern vu32 gxlKeySet;
extern vu32 gxlCodeNumSet;
extern vu32 gxlHoldOffTime;
extern vu32 gxhRelayMode;
extern vu32 gxlDataOutDelay;
extern vu32 gxlRelayOnDelay;
extern vu32 gxlRelayOnTime;


extern vu32 gdhParity;
extern vu32 gxhBaudRate;
extern vu32 gxhSendMode;
extern vu32 gxlSerialMethod;
extern vu32 gxhMyAddress;
extern vu32 gxhRsFormat;
extern vu32 gxlBCCSelect;
extern vu32 gxlTransPerSec;
extern vu32 gxlWeightUnit;


extern vs32 gxlDisplaySpeed;
extern vs32 gxlStableOnDelay;
extern vs32 gxlPushPull;
extern vs32 gxhAroundZero;
extern vs32 gxlZeroDisplayRange;
extern vs32 gxlZeroMinus;
extern vs32 gxlAnalogOutMax;

extern vu32 gxlEquivalInput;
extern vu32 gdwWhichCal;
extern vu32 gxhGain;

extern u32 SP1;
extern u32 SP2;

extern vu32	gxhCurrentLoopBaudRate;

 /*********낙상침대 2016년11월 추가 *********************/

extern vu32 gxhZero_1;
extern vu32 gxhZero_2;
extern vu32 gxhZero_3;
extern vu32 gxhZero_4;






/*********낙상침대 2016년11월 추가 *********************/





long lValueSetMode(char number ,long init_value, long limit);
long lValueSetMode_5digit_with_dp(char f_num, long init_value, long limit, char dp);
long lValueSetMode_dp(char f_no ,long init_value, long limit, char dp);
void setting_mode(void);
void default_set(void);


#endif	// __SETTING_H

/* end of file */
