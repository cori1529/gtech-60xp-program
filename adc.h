/*
	adc.h
*/

#ifndef __ADC_H
#define __ADC_H

#define	ADC_SATURATION      1048575L	/* ad saturation value */
#define	ADPERSEC			200			/* ad number per second */
#define	MAX_FILTER   200

#define _80SPEED   1250  // CS5555 GP-60XP
//#define _80SPEED   1255  // CS5555 GP-60XP


//#define _80SPEED   1300  // CS5555 GP-60XP


//#define _80SPEED   20  // CS5555 GP-60XP



#define  CS5532_CS_DISABLE  GPIO_SetBits(GPIOA, GPIO_Pin_11)
#define  CS5532_CS_ENABLE   GPIO_ResetBits(GPIOA, GPIO_Pin_11)


#define  CS5532_CS_DISABLE_1  GPIO_SetBits(GPIOA, GPIO_Pin_11)
#define  CS5532_CS_ENABLE_1   GPIO_ResetBits(GPIOA, GPIO_Pin_11)



#define  CS5532_CS_DISABLE_2  GPIO_SetBits(GPIOA, GPIO_Pin_12)
#define  CS5532_CS_ENABLE_2   GPIO_ResetBits(GPIOA, GPIO_Pin_12)

#define  CS5532_CS_DISABLE_3  GPIO_SetBits(GPIOA, GPIO_Pin_13)
#define  CS5532_CS_ENABLE_3   GPIO_ResetBits(GPIOA, GPIO_Pin_13)

#define  CS5532_CS_DISABLE_4  GPIO_SetBits(GPIOA, GPIO_Pin_14)
#define  CS5532_CS_ENABLE_4   GPIO_ResetBits(GPIOA, GPIO_Pin_14)




#define    ADC_DATA_READY         if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == 0x00)




void           adc_initial(void);
									
unsigned long  adc_value(void);
unsigned long  read_adc( unsigned char );
unsigned long  read_filtered_adc(void);
unsigned long read_filtered_adc_with_ZeroMinus(void);

unsigned long  read_filtered_adc_nowait(void);
void           write_adc( unsigned long);
unsigned long RDDATA(void);
void  ADWRITE(unsigned long cmd);
unsigned long RDDATA_NOWAIT(unsigned char which_reg);
unsigned long adc_value_nowait(void);
long CS5532_Result_Fillterd(void);


/*
#define  ADC_SLCK_HIGH                     GPIO_SetBits(GPIOA, GPIO_Pin_5)
#define  ADC_SLCK_LOW                     GPIO_ResetBits(GPIOA, GPIO_Pin_5)
#define  ADC_POWER_DOWN_OFF        GPIO_SetBits(GPIOB, GPIO_Pin_12)

	*/
 



#define  SYNC1               0xff
#define  SYNC0               0xfe
#define  NULL_BYTE           0x00


// Command Register
#define  REG_ARRAY_ACCESS    (0x01<<6)

#define  CMD_CH1             (0x00<<4)
#define  CMD_CH2             (0x01<<4)
#define  CMD_CH3             (0x02<<4)
#define  CMD_CH4             (0x03<<4)

#define  CMD_WRITE           (0x00<<3)
#define  CMD_READ            (0x01<<3)

#define  REG_OFFSET          0x01
#define  REG_GAIN            0x02
#define  REG_CONFIG          0x03
#define  REG_SETUP           0x05

#define  START_SINGLE        (0x00<<6)
#define  START_CONTINUOUS    (0x01<<6)
#define  SETUP1              (0x00<<3)
#define  SETUP2              (0x01<<3)
#define  SETUP3              (0x02<<3)
#define  SETUP4              (0x03<<3)
#define  SETUP5              (0x04<<3)
#define  SETUP6              (0x05<<3)
#define  SETUP7              (0x06<<3)
#define  SETUP8              (0x07<<3)

#define  NORMAL_CONVERSION   (0x80+0x00)
#define  SELF_OFFSET_CAL     (0x80+0x01)
#define  SELF_GAIN_CAL       (0x80+0x02)
#define  SYSTEM_OFFSET_CAL   (0x80+0x05)
#define  SYSTEM_GAIN_CAL     (0x80+0x06)



// Configuration Register
#define  STANDBY_MODE        (0x00L<<31)
#define  SLEEP_MODE          (0x01L<<31)

#define  POWER_SAVE_MODE     (0x01L<<30)

#define  NORMAL_MODE_2         (0x00L<<29)
#define  SYSTEM_RESET        (0x01L<<29)

#define  RESET_STATUS        (0x01L<<28)

#define  SHORT_INPUTS        (0x01L<<27)

#define  A0_GUARD            (0x01L<<26)

#define  VREF_HIGH           (0x00L<<25)
#define  VREF_LOW            (0x01L<<25)

#define  CR_A0_0             (0x00L<<24)
#define  CR_A0_1             (0x01L<<24)

#define  CR_A1_0             (0x00L<<23)
#define  CR_A1_1             (0x01L<<23)

#define  A0_A1_REG_SETUP     (0x00L<<22)
#define  A0_A1_REG_CONFIG    (0x01L<<22)

#define  CAL_DATA_PHYSICAL   (0x00L<<20)
#define  CAL_DATA_SELECT     (0x01L<<20)

#define  LINE_FREQ_60        (0x00L<<19)
#define  LINE_FREQ_50        (0x01L<<19)



// Channel Setup Register
#define  CSR_CH1             (0x00L<<14)
#define  CSR_CH2             (0x01L<<14)
#define  CSR_CH3             (0x02L<<14)
#define  CSR_CH4             (0x03L<<14)

#define  PGA_1               (0x00L<<11)
#define  PGA_2               (0x01L<<11)
#define  PGA_4               (0x02L<<11)
#define  PGA_8               (0x03L<<11)
#define  PGA_16              (0x04L<<11)
#define  PGA_32              (0x05L<<11)
#define  PGA_64              (0x06L<<11)

#define  DATARATE_100        (0x00L<<7)
#define  DATARATE_50         (0x01L<<7)
#define  DATARATE_25         (0x02L<<7)
#define  DATARATE_12P5       (0x03L<<7)
#define  DATARATE_6P25       (0x04L<<7)
#define  DATARATE_3200       (0x08L<<7)
#define  DATARATE_1600       (0x09L<<7)
#define  DATARATE_800        (0x0aL<<7)
#define  DATARATE_400        (0x0bL<<7)
#define  DATARATE_200        (0x0cL<<7)

#define  BIPOLAR_MODE        (0x00L<<6)
#define  UNIPOLAR_MODE       (0x01L<<6)

#define  CSR_A0_0            (0x00L<<5)
#define  CSR_A0_1            (0x01L<<5)
#define  CSR_A1_0            (0x00L<<4)
#define  CSR_A1_1            (0x01L<<4)

#define  START_NO_DELAY      (0x00L<<3)
#define  START_DELAY         (0x01L<<3)

#define  TURN_ON_300NA       (0x01L<<2)

#define  CAL_DATA_CH1        (0x00L)
#define  CAL_DATA_CH2        (0x01L)
#define  CAL_DATA_CH3        (0x02L)
#define  CAL_DATA_CH4        (0x03L)

  

void delay(unsigned int milisecond);
void short_delay(void);


void CS5532_Write_Byte(unsigned char ch);
unsigned char CS5532_Read_Byte(void);
void CS5532_Write_Dword(unsigned long ld);
unsigned long  CS5532_Read_Dword(void);
void CS5532_Write_Command(unsigned char cmd);
void CS5532_Serial_Reset(void);
void CS5532_Dump_Regs(void);
void CS5532_System_Reset(void);
void CS5532_Config(void);
void CS5532_Config_Multi(char channel);

//bool  CS5532_ADC_Ready(void);
long CS5532_Result(void);
long CS5532_Result_Multiple(char channel);


 void SPI_Write_Byte(unsigned char ch);


char  SPI_WriteByte_ReadWait(char TxData);
char  SPI_ReadByte(void);



void CS5532_Initial();
void CS5532_Initial_Multiple(char channel);


void ch1_adc_test(void);
void ch2_adc_test(void);
void ch3_adc_test(void);
void ch4_adc_test(void);
/*
unsigned long ADvalue;
unsigned char ADi; 
unsigned char AD_NEG,AD_SPEED;


#define  ADC_SLCK_HIGH                     GPIO_SetBits(GPIOA, GPIO_Pin_5)
#define  ADC_SLCK_LOW                     GPIO_ResetBits(GPIOA, GPIO_Pin_5)
#define  ADC_POWER_DOWN_OFF        GPIO_SetBits(GPIOB, GPIO_Pin_12)

*/
//#define    ADC_DATA_READY         if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) == 0x01 &&ADi==0 )


/******** NEW GI-1400 (201709)*******************************/

#define  ADC_SLCK_HIGH                     GPIO_SetBits(GPIOA, GPIO_Pin_5)
#define  ADC_SLCK_LOW                      GPIO_ResetBits(GPIOA, GPIO_Pin_5)
#define  ADC_POWER_DOWN_OFF                GPIO_SetBits(GPIOB, GPIO_Pin_12)

void adc_delay(int time);
void CS5555_Clk(void); 
unsigned long Read_AD_Value(void); 


void INIT_CS5555(void);            //CS5555ªì©l¤Æ

extern unsigned long adc_timer;








  #endif


