/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : i2c_ee.c
* Author             : MCD Application Team
* Version            : V2.0.1
* Date               : 06/13/2008
* Description        : This file provides a set of functions needed to manage the
*                      communication between I2C peripheral and I2C M24C08 EEPROM.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_lib.h"

#include "cal.h"
#include "delay.h"
#include "i2c_ee.h"
#include "lcd.h"
#include "set.h"
#include "var.h"
#include "usart.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define I2C_Speed              100000
//#define I2C_Speed              50000
/*
// 24c16
16 kbit
2048 byte (128 page, 1 page=16 byte)
compose of 8 block 

1 block= 256 byte (16 page)

*/
#define I2C1_SLAVE_ADDRESS7    0xA0
#define I2C_PageSize           16    //0322
																				
u16 EEPROM_ADDRESS;

/* Private function prototypes -----------------------------------------------*/
void I2C_Configuration(void);

u32 EE_Write_val(int adr, u32 val, u8 bytes);
u32 EE_Read_val(int adr, u8 bytes);


/******낙상침대 **********************/
 long ch1_zero_value; //  침대에서 1번의 영점값 
 long ch2_zero_value; //  침대에서 2번의 영점값 
 long ch3_zero_value; //  침대에서 3번의 영점값 
 long ch4_zero_value; //  침대에서 4번의 영점값 


  long ch_zero_value[4]; //

// long patient_value; // 환자무게
// long alert_setting; // 낙상 경보에 대한 설정 1~5단계     1단계 : 별루치우치지 않은 상태에서 경보,  5단계 : 침대밖으로 거의 나온경우에 경보 
// #define ADR_patient_value                	316 
//#define ADR_alert_setting                	320 

// long   patient_value; 
 long   alert_setting;                	 

	int				i1dan;
	int				i2dan;
	int				i3dan;
	int				i4dan;
  int				i5dan;
	int				i6dan;
	int				i7dan;
	int				i8dan;
	int				i9dan;
	int				i10dan;
	int				i11dan;
	int				i12dan;
	int				i13dan;
	int				i14dan;
	int				i15dan;
	int				i16dan;
	int				i17dan;
	int				i18dan;
	int				i19dan;
	int				i20dan;
	int				i21dan;
	int				i22dan;
  int				i23dan;
	int				i24dan;
	int				i25dan;
	
	
	
 //unsigned char gxhMotionBand; /* v_mdc_digit */
 //unsigned char gxhZTDigit;    /* zero tracking digit */
 
  unsigned char gxhZTTime; 	   /* zero tracking time (sec) */
 	unsigned char	gxhCurrentType;
	char 			gxhWeightGrapeType;  // 1 2 3 type
	char			gxhWeightJado1Type;  // 1  type
	char 			gxhWeightJado2Type;  // 1  type
	char 			gxhWeightPearType;   // 1 2   type
	char 			gxhWeightAppleType;  // 1  type
	char 			gxhWeightPeackType;  // 1 2 3 4 type
	unsigned char	gxhDanSetType;
	long gxhVoiceLevel;
  long   gxhWhichSorting;
 long gxhWeightSpecialType;
 long gxhDisplaySelect;


	
	
	 long	lBoundry[20];
  long	lCount[25]= {0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0  ,0,0,0,0,0 };		// dan counter
  long	lTotalCount=0;	// Total counter

  u8	dpoint_bak;
 unsigned char byDan[100];   // ms => byDan[25];byte	byDan[30];   // ms => byDan[25];
	int				iMinimumWeight;
	int				gxiIndex_End;
	int 	DAN_MAX=0;




	
	
	


/******낙상침대 **********************/








/*******************************************************************************
* Function Name  : I2C_Configuration
* Description    : I2C Configuration
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C_Configuration(void)
{
  I2C_InitTypeDef  I2C_InitStructure; 

  /* I2C configuration */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = I2C1_SLAVE_ADDRESS7;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;
  
  /* I2C Peripheral Enable */
  I2C_Cmd(I2C1, ENABLE);
  /* Apply I2C configuration after enabling it */
  I2C_Init(I2C1, &I2C_InitStructure);
}

/*******************************************************************************
* Function Name  : I2C_EE_Init
* Description    : Initializes peripherals used by the I2C EEPROM driver.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C_EE_Init()
{
  /* GPIO configuration */
  //GPIO_Configuration();

  /* I2C configuration */
  I2C_Configuration();

	EEPROM_ADDRESS = EEPROM_Block0_ADDRESS;

	
	
  /* depending on the EEPROM Address selected in the i2c_ee.h file */
#ifdef dpfp	
#ifdef EEPROM_Block0_ADDRESS
  /* Select the EEPROM Block0 to write on */
  EEPROM_ADDRESS = EEPROM_Block0_ADDRESS;
#endif
#ifdef EEPROM_Block1_ADDRESS
  /* Select the EEPROM Block1 to write on */
  EEPROM_ADDRESS = EEPROM_Block1_ADDRESS;
#endif
#ifdef EEPROM_Block2_ADDRESS
  /* Select the EEPROM Block2 to write on */
  EEPROM_ADDRESS = EEPROM_Block2_ADDRESS;
#endif
#ifdef EEPROM_Block3_ADDRESS
  /* Select the EEPROM Block3 to write on */
  EEPROM_ADDRESS = EEPROM_Block3_ADDRESS;
#endif
#endif
}
 //#define EEPROM_Block1_ADDRESS 0xA2
/*******************************************************************************
* Function Name  : I2C_EE_BufferWrite
* Description    : Writes buffer of data to the I2C EEPROM.
* Input          : - pBuffer : pointer to the buffer  containing the data to be 
*                    written to the EEPROM.
*                  - WriteAddr : EEPROM's internal address to write to.
*                  - NumByteToWrite : number of bytes to write to the EEPROM.
* Output         : None
* Return         : None
*******************************************************************************/
void I2C_EE_BufferWrite(u8* pBuffer, u16 WriteAddr, u16 NumByteToWrite)
{
  u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;
 /*
	if(WriteAddr>=0&& WriteAddr<=255) // block 0
	 EEPROM_ADDRESS=EEPROM_Block0_ADDRESS;
	else //if(WriteAddr>=256&& WriteAddr<=512)
	 EEPROM_ADDRESS=EEPROM_Block1_ADDRESS;
	 */
	

  Addr = WriteAddr % I2C_PageSize;
  count = I2C_PageSize - Addr;
  NumOfPage =  NumByteToWrite / I2C_PageSize;
  NumOfSingle = NumByteToWrite % I2C_PageSize;
 
  /* If WriteAddr is I2C_PageSize aligned  */
  if(Addr == 0) 
  {
    /* If NumByteToWrite < I2C_PageSize */
    if(NumOfPage == 0) 
    {
      I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      I2C_EE_WaitEepromStandbyState();
    }
    /* If NumByteToWrite > I2C_PageSize */
    else  
    {
      while(NumOfPage--)
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, I2C_PageSize); 
    	I2C_EE_WaitEepromStandbyState();
        WriteAddr +=  I2C_PageSize;
        pBuffer += I2C_PageSize;
      }

      if(NumOfSingle!=0)
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
        I2C_EE_WaitEepromStandbyState();
      }
    }
  }
  /* If WriteAddr is not I2C_PageSize aligned  */
  else 
  {
    /* If NumByteToWrite < I2C_PageSize */
    if(NumOfPage== 0) 
    {
      I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      I2C_EE_WaitEepromStandbyState();
    }
    /* If NumByteToWrite > I2C_PageSize */
    else
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / I2C_PageSize;
      NumOfSingle = NumByteToWrite % I2C_PageSize;	
      
      if(count != 0)
      {  
        I2C_EE_PageWrite(pBuffer, WriteAddr, count);
        I2C_EE_WaitEepromStandbyState();
        WriteAddr += count;
        pBuffer += count;
      } 
      
      while(NumOfPage--)
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, I2C_PageSize);
        I2C_EE_WaitEepromStandbyState();
        WriteAddr +=  I2C_PageSize;
        pBuffer += I2C_PageSize;  
      }
      if(NumOfSingle != 0)
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle); 
        I2C_EE_WaitEepromStandbyState();
      }
    }
  }  
}

/*******************************************************************************
* Function Name  : I2C_EE_ByteWrite
* Description    : Writes one byte to the I2C EEPROM.
* Input          : - pBuffer : pointer to the buffer  containing the data to be 
*                    written to the EEPROM.
*                  - WriteAddr : EEPROM's internal address to write to.
* Output         : None
* Return         : None
*******************************************************************************/
void I2C_EE_ByteWrite(u8* pBuffer, u8 WriteAddr)
{
  /* Send STRAT condition */
  I2C_GenerateSTART(I2C1, ENABLE);

  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));  

  /* Send EEPROM address for write */
  I2C_Send7bitAddress(I2C1, EEPROM_ADDRESS, I2C_Direction_Transmitter);
  
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
      
  /* Send the EEPROM's internal address to write to */
  I2C_SendData(I2C1, WriteAddr);
  
  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  /* Send the byte to be written */
  I2C_SendData(I2C1, *pBuffer); 
   
  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  
  /* Send STOP condition */
  I2C_GenerateSTOP(I2C1, ENABLE);
}

/*******************************************************************************
* Function Name  : I2C_EE_PageWrite
* Description    : Writes more than one byte to the EEPROM with a single WRITE
*                  cycle. The number of byte can't exceed the EEPROM page size.
* Input          : - pBuffer : pointer to the buffer containing the data to be 
*                    written to the EEPROM.
*                  - WriteAddr : EEPROM's internal address to write to.
*                  - NumByteToWrite : number of bytes to write to the EEPROM.
* Output         : None
* Return         : None
*******************************************************************************/
void I2C_EE_PageWrite(u8* pBuffer, u16 WriteAddr, u8 NumByteToWrite)
{
	
	if(WriteAddr>=0&& WriteAddr<=255) // block 0
	 EEPROM_ADDRESS=EEPROM_Block0_ADDRESS;
	else if(WriteAddr>=255&& WriteAddr<=512) // block 1
	 EEPROM_ADDRESS=EEPROM_Block1_ADDRESS;
	else if(WriteAddr>=(256*2)&& WriteAddr<=(256*3)) //  block 2
	 EEPROM_ADDRESS=EEPROM_Block2_ADDRESS;
	else if(WriteAddr>=(256*3)&& WriteAddr<=(256*4)) //  block 2
	 EEPROM_ADDRESS=EEPROM_Block3_ADDRESS;          // block 3
	
	
	
  /* Send START condition */
  I2C_GenerateSTART(I2C1, ENABLE);
  
  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)); 
  
  /* Send EEPROM address for write */
  I2C_Send7bitAddress(I2C1, EEPROM_ADDRESS, I2C_Direction_Transmitter);/////////////////////

  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));  

  /* Send the EEPROM's internal address to write to */    
  I2C_SendData(I2C1, WriteAddr);  

  /* Test on EV8 and clear it */
  while(! I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  /* While there is data to be written */
  while(NumByteToWrite--)  
  {
    /* Send the current byte */
    I2C_SendData(I2C1, *pBuffer); 

    /* Point to the next byte to be written */
    pBuffer++; 
  
    /* Test on EV8 and clear it */
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  }

  /* Send STOP condition */
  I2C_GenerateSTOP(I2C1, ENABLE);
}

/*******************************************************************************
* Function Name  : I2C_EE_BufferRead
* Description    : Reads a block of data from the EEPROM.
* Input          : - pBuffer : pointer to the buffer that receives the data read 
*                    from the EEPROM.
*                  - ReadAddr : EEPROM's internal address to read from.
*                  - NumByteToRead : number of bytes to read from the EEPROM.
* Output         : None
* Return         : None
*******************************************************************************/
void I2C_EE_BufferRead(u8* pBuffer, u16 ReadAddr, u16 NumByteToRead)
{ 
 
 if(ReadAddr>=0&& ReadAddr<=255) // block 0
	 EEPROM_ADDRESS=EEPROM_Block0_ADDRESS;
	else if(ReadAddr>=255&& ReadAddr<=512) // block 1
	 EEPROM_ADDRESS=EEPROM_Block1_ADDRESS;
	else if(ReadAddr>=(256*2)&& ReadAddr<=(256*3)) //  block 2
	 EEPROM_ADDRESS=EEPROM_Block2_ADDRESS;
	else if(ReadAddr>=(256*3)&& ReadAddr<=(256*4)) //  block 2
	 EEPROM_ADDRESS=EEPROM_Block3_ADDRESS;          // block 3
	
	
  /* Send START condition */
  I2C_GenerateSTART(I2C1, ENABLE);
  
  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
   
  /* Send EEPROM address for write */
  I2C_Send7bitAddress(I2C1, EEPROM_ADDRESS, I2C_Direction_Transmitter);

  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  
  /* Clear EV6 by setting again the PE bit */
  I2C_Cmd(I2C1, ENABLE);

  /* Send the EEPROM's internal address to write to */
  I2C_SendData(I2C1, ReadAddr);  

  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  
  /* Send STRAT condition a second time */  
  I2C_GenerateSTART(I2C1, ENABLE);
  
  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
  
  /* Send EEPROM address for read */
  I2C_Send7bitAddress(I2C1, EEPROM_ADDRESS, I2C_Direction_Receiver);//////////////////which block
  
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
  
  /* While there is data to be read */
  while(NumByteToRead)  
  {
    if(NumByteToRead == 1)
    {
      /* Disable Acknowledgement */
      I2C_AcknowledgeConfig(I2C1, DISABLE);
      
      /* Send STOP Condition */
      I2C_GenerateSTOP(I2C1, ENABLE);
    }

    /* Test on EV7 and clear it */
    if(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))  
    {      
      /* Read a byte from the EEPROM */
      *pBuffer = I2C_ReceiveData(I2C1);

      /* Point to the next location where the byte read will be saved */
      pBuffer++; 
      
      /* Decrement the read bytes counter */
      NumByteToRead--;        
    }   
  }

  /* Enable Acknowledgement to be ready for another reception */
  I2C_AcknowledgeConfig(I2C1, ENABLE);
}

/*******************************************************************************
* Function Name  : I2C_EE_WaitEepromStandbyState
* Description    : Wait for EEPROM Standby state
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C_EE_WaitEepromStandbyState(void)      
{
  vu16 SR1_Tmp = 0;
 
  do
  {
    // Send START condition 
    I2C_GenerateSTART(I2C1, ENABLE);
    // Read I2C1 SR1 register 
    SR1_Tmp = I2C_ReadRegister(I2C1, I2C_Register_SR1);
    //Send EEPROM address for write 
    I2C_Send7bitAddress(I2C1, EEPROM_ADDRESS, I2C_Direction_Transmitter);
  }while(!(I2C_ReadRegister(I2C1, I2C_Register_SR1) & 0x0002));
  
  // Clear AF flag 
  I2C_ClearFlag(I2C1, I2C_FLAG_AF);
}


#ifdef edf 


//mode = 0:분동캘, mode=1 등가입력
void write_cal_factors(char mode)
{
//	u32 factor;
//	float fFactor;
	union { float f; u32 dw; u16 u[2]; u8 c[4];} temp;
 
	if(mode == 2) //등가입력 reference cal
	{
//fFactor = gnfFactor;
/*
vputnum(0,0,gnfFactor*10000); Delay_ms(1000);	
vputnum(0,1,gnfFactor*10000); Delay_ms(1000);	
vputnum(0,2,gnfFactor*10000); Delay_ms(1000);	
vputnum(0,3,gnfFactor*10000); Delay_ms(1000);	
*/

		temp.f = gnfFactor;
		temp.dw = EE_Write_val(ADR_gnfRefFactor, temp.dw, 4);	//add 2016.3.13
		gnfRefFactor = temp.f;

/*
vputnum(0,0,temp.dw); Delay_ms(1000);	
vputnum(0,1,temp.dw); Delay_ms(1000);	
vputnum(0,2,temp.dw); Delay_ms(1000);	
vputnum(0,3,temp.dw); Delay_ms(1000);	

vputnum(0,0,gnfRefFactor*(float)power(10, 9)); Delay_ms(1000);	
vputnum(0,1,gnfRefFactor*(float)power(10, 9)); Delay_ms(1000);	
vputnum(0,2,gnfRefFactor*(float)power(10, 9)); Delay_ms(1000);	
vputnum(0,3,gnfRefFactor*(float)power(10, 9)); Delay_ms(1000);	
*/
	}
	else
	{
		gdwWhichCal = EE_Write_val(ADR_gdwWhichCal, gdwWhichCal, 4);	//add 2016.3.3
		
		gxlDecimalPoint = EE_Write_val(ADR_gxlDecimalPoint, gxlDecimalPoint, 4);	//add 2016.3.30

		gnlCapacity = EE_Write_val(ADR_gnlCapacity, gnlCapacity, 4);	//add 2016.3.8
		v_division = EE_Write_val(ADR_v_division, v_division, 4);	//add 2016.2.23
		v_zero = EE_Write_val(ADR_v_zero, v_zero, 4);	////영점(zero) a/d 값 add 2016.2.23	
		
		v_i_resolution = EE_Write_val(ADR_v_i_resolution, v_i_resolution, 4);	//add 2016.2.23
		v_ei_multiply_factor = EE_Write_val(ADR_v_ei_multiply_factor, v_ei_multiply_factor, 4);	//add 2016.2.23

		switch(mode)
		{
			case 0:	//분동 캘이면
///				gdwFactor = (u32)(gnfFactor * (float)power(10, 9)); //0.56148231*1000000000=561482310    정수형으로 저장 

				gdwFactor = (u32)(gnfFactor * (float)power(10, 5)); //0.56148231*1000000000=561482310    정수형으로 저장 
				gdwFactor = EE_Write_val(ADR_gdwFactor, gdwFactor, 4);	//add 2016.2.23
//				factor = gdwFactor;
				/*
				temp.f = gnfFactor;
				factor = EE_Write_val(ADR_gdwFactor, temp.dw, 4);	//add 2016.2.23
				*/			
				break;
			
			case 1: //등가입력 캘
//				gxlEquivalInput = EE_Write_val(ADR_gxlEquivalInput, gxlEquivalInput, 4);	//add 2016.2.23
//				factor = gxlEquivalInput * 10000;
/*
				gxlEquivalInput = (u32)(gnfFactor * (float)power(10, 9)); //0.56148231*1000000000=561482310    정수형으로 저장 
				gxlEquivalInput = EE_Write_val(ADR_gxlEquivalInput, gxlEquivalInput, 4);	//add 2016.2.23
*/
				gxlEquivalInput = EE_Write_val(ADR_gxlEquivalInput, gxlEquivalInput, 4);	//add 2016.2.23

				temp.f = gnfFactor;
				temp.dw = EE_Write_val(ADR_gnfEquFactor, temp.dw, 4);	//add 2016.3.13
				gnfEquFactor = temp.f;
				break;
		}
	}
/*
vputnum(0,0,gdwWhichCal); Delay_ms(1000);
vputnum(0,1,gdwWhichCal); Delay_ms(1000);	
vputnum(0,2,gdwWhichCal); Delay_ms(1000);	
vputnum(0,3,gdwWhichCal); Delay_ms(1000);	
*/
} /* of write_cal_factors() */

#endif






void write_cal_factors(void)
{
	union { float f; u32 dw; u16 u[2]; u8 c[4];} temp;
	
	 
		  gnfFactor  = (u32)(gnfFactor * (float)power(10, 5)); //0.56148231*1000000000=561482310    정수형으로 저장 
		  gnfFactor  = EE_Write_val(ADR_gnfFactor , gnfFactor , 4);	 Delay_ms(10);  //add 2016.2.23
	    v_zero = EE_Write_val(ADR_v_zero, v_zero, 4);	  Delay_ms(10);////영점(zero) a/d 값 add 2016.2.23	
      v_capacity = EE_Write_val(ADR_v_capacity, v_capacity, 4);	  Delay_ms(10);////영점(zero) a/d 값 add 2016.2.23	
      v_division = EE_Write_val(ADR_v_division, v_division, 4);	  Delay_ms(10);////영점(zero) a/d 값 add 2016.2.23	
      v_decimal_point = EE_Write_val(ADR_v_decimal_point, v_decimal_point, 4);	  Delay_ms(10);////영점(zero) a/d 값 add 2016.2.23	

} /* of write_cal_factors() */









//낙상침대요 
void  write_zero_factors()

{
 union { float f; u32 dw; u16 u[2]; u8 c[4];} temp;

		 #ifdef dpfdpf  
            temp.dw=ch_zero_value[0];
	       	I2C_EE_BufferWrite(temp.c,ADR_ch1_zero_value, 4);
	        I2C_EE_BufferRead( temp.c, ADR_ch1_zero_value , 4);
            ch_zero_value[0]=temp.dw;
		   

			temp.dw=ch_zero_value[1];
	       	I2C_EE_BufferWrite(temp.c,ADR_ch2_zero_value, 4);
	        I2C_EE_BufferRead( temp.c, ADR_ch2_zero_value , 4);
            ch_zero_value[1]=temp.dw;
		   
		    temp.dw=ch_zero_value[2];
	       	I2C_EE_BufferWrite(temp.c,ADR_ch3_zero_value, 4);
	        I2C_EE_BufferRead( temp.c, ADR_ch3_zero_value , 4);
            ch_zero_value[2]=temp.dw;
		   
		   temp.dw=ch_zero_value[3];
	       	I2C_EE_BufferWrite(temp.c,ADR_ch4_zero_value, 4);
	        I2C_EE_BufferRead( temp.c, ADR_ch4_zero_value , 4);
            ch_zero_value[3]=temp.dw;
		   #endif
}





void read_zero_factors(void)
{
 #define MAX_VALUE  50000
 #define MIN_VALUE  0


 
 union { float f; u32 dw; u16 u[2]; u8 c[4];} temp;
   #ifdef dpfdpf
   
    gxlDecimalPoint = EE_Read_val( ADR_gxlDecimalPoint, 4);	//Decimal point
   
   
   
    I2C_EE_BufferRead( temp.c,ADR_ch1_zero_value , 4);
     ch_zero_value[0]=temp.dw;
   if(ch_zero_value[0]<MIN_VALUE || ch_zero_value[0]>MAX_VALUE) // 
   ch_zero_value[0]  =3000; 	
  
  
    I2C_EE_BufferRead( temp.c,ADR_ch2_zero_value , 4);
     ch_zero_value[1]=temp.dw;
   if(ch_zero_value[1]<MIN_VALUE || ch_zero_value[1]>MAX_VALUE) // 
   ch_zero_value[1]  =3000; 	
  
    I2C_EE_BufferRead( temp.c,ADR_ch3_zero_value , 4);
     ch_zero_value[2]=temp.dw;
   if(ch_zero_value[2]<MIN_VALUE || ch_zero_value[0]>MAX_VALUE) // 
   ch_zero_value[2]  =3000; 	
  

   I2C_EE_BufferRead( temp.c,ADR_ch4_zero_value , 4);
     ch_zero_value[3]=temp.dw;
   if(ch_zero_value[3]<MIN_VALUE || ch_zero_value[3]>MAX_VALUE) // 
   ch_zero_value[3]  =3000; 	
  
   // 결보단계 설정  1~5 단계    1단계 약함    5단계 갛함
   I2C_EE_BufferRead( temp.c,ADR_alert_setting , 4);
     alert_setting=temp.dw;
   if(alert_setting<0 || alert_setting>7) // 
     alert_setting =3; 	
  #endif

 // alert_setting


}
  


void read_cal_factors()
{
//	float factor;
	union { float f; u32 dw; u16 u[2]; u8 c[4];} temp;
 

	
	
	v_zero = EE_Read_val( ADR_v_zero, 4);	//zero a/d 값
	gnfFactor = EE_Read_val( ADR_gnfFactor, 4);	//cal factor
	gnfFactor = (float)gnfFactor / (float)power(10, 5);
	v_decimal_point = EE_Read_val( ADR_v_decimal_point, 4);	//Decimal point
	v_capacity = EE_Read_val( ADR_v_capacity, 4);
	v_division = EE_Read_val( ADR_v_division, 4);
	v_decimal_point = EE_Read_val( ADR_v_decimal_point, 4);
	
	
	
	
	gxhMotionBand = EE_Read_val (ADR_gxhMotionBand,4);
	gxhZTDigit = EE_Read_val (ADR_gxhZTDigit,4);
	gxhZTTime = EE_Read_val (ADR_gxhZTTime,4);
	gxhCurrentType = EE_Read_val (ADR_gxhCurrentType,4);
	gxhWeightGrapeType = EE_Read_val (ADR_gxhWeightGrapeType,4);  //4,5,10
	gxhWeightJado1Type = EE_Read_val (ADR_gxhWeightJado1Type,4);  //10
	gxhWeightJado2Type = EE_Read_val (ADR_gxhWeightJado2Type,4);  //10
	gxhWeightPearType = EE_Read_val (ADR_gxhWeightPearType,4);    //7.5,15
	gxhWeightAppleType = EE_Read_val (ADR_gxhWeightAppleType,4);  //15
	gxhWeightPeackType = EE_Read_val (ADR_gxhWeightPeackType,4);  //5,10,15
	gxhDanSetType = EE_Read_val (ADR_gxhDanSetType,4);
	gxhVoiceLevel=EE_Read_val (ADR_gxhVoiceLevel,4);
	gxhWhichSorting=EE_Read_val (ADR_gxhWhichSorting,4);
  gxhWeightSpecialType=EE_Read_val (ADR_gxhWeightSpecialType,4);
	
	////gxhDisplaySelect=EE_Read_val (ADR_gxhDisplaySelect,4);  //0(default) : 선별시 등급 갯수 표시,  1: 선별시  선별무게 표시


//gxhWeightSpecialType

} /* of read_cal_factors() */



void cond_check()  /* EEPROM valid check */
{
   if((gnfFactor <= 0.0) || (10.0 < gnfFactor))
   {
/*      vputs("ERR EEP"); beep(300); delay(500); */
   }

   /* if ( v_zero > 500000L ) v_autocal_flag = 1; */

   if ((v_zero < 300) || (65000L < v_zero))
   { 
//	  sprintf(str, "%5lu", v_zero); vputs(&str[1]); delay(500);
   }

   if (gxhMotionBand > 9)
   {  gxhMotionBand = 3;
//	  eeprom_error( ADR_gxhMotionBand, gxhMotionBand);
   }

   if (v_capacity > 1000)
   {  v_capacity = 60;
	  /////eeprom_error( ADR_v_capacity, v_capacity);
   }

   if ( v_division < 0 || 500 < v_division)
   {  v_division = 5;
	  /////eeprom_error( ADR_v_division, v_division);
   }

   if ( v_decimal_point > 3 )
   {  v_decimal_point = 3;
	  ////eeprom_error( ADR_v_decimal_point, v_decimal_point);
   }

   if (9 < gxhZTDigit)
   {  gxhZTDigit = 2; ///eeprom_error (ADR_gxhZTDigit, gxhZTDigit); 
		 }

   if (9 < gxhZTTime)
   {  gxhZTTime = 3; ///eeprom_error (ADR_gxhZTTime, gxhZTTime);
   }

   if (9 < gxhMotionBand)
   {  gxhMotionBand = 2;/// eeprom_error (ADR_gxhMotionBand, gxhMotionBand); 
		 }

   if (9 < gxhCurrentType)
   {  gxhCurrentType = 0; ///eeprom_error (ADR_gxhCurrentType, gxhCurrentType); 
		 }

   if (3 < gxhWeightGrapeType)
   {  gxhWeightGrapeType = 1; ///eeprom_error (ADR_gxhWeightGrapeType, gxhWeightGrapeType);
		 }
   //2
   if (3 < gxhWeightJado1Type)
   {  gxhWeightJado1Type = 1; ///eeprom_error (ADR_gxhWeightJado1Type, gxhWeightJado1Type); 
		 }

   if (3 < gxhWeightJado2Type)
   {  gxhWeightJado2Type = 1; ///eeprom_error (ADR_gxhWeightJado2Type, gxhWeightJado2Type); 
		 }

   if (3 < gxhWeightPearType)
   {  gxhWeightPearType = 1; ///eeprom_error (ADR_gxhWeightPearType, gxhWeightPearType); 
		 }
   //2
   if (3 < gxhWeightAppleType)
   {  gxhWeightAppleType = 1; //eeprom_error (ADR_gxhWeightAppleType, gxhWeightAppleType);
		 }
   //3
   if (5 < gxhWeightPeackType)
   {  gxhWeightPeackType = 1; //eeprom_error (ADR_gxhWeightPeackType, gxhWeightPeackType); 
		 }


   if (3 < gxhDanSetType)
   {  gxhDanSetType = 1;/// eeprom_error (ADR_gxhDanSetType, gxhDanSetType); 
		 }
	 
		if(gxhVoiceLevel<0 || gxhVoiceLevel>6) 
   			gxhVoiceLevel=3;
		
		if(gxhWhichSorting<0 || gxhWhichSorting >1)
			gxhWhichSorting=0;  //  선별방법  0: 등급, 1 : 과
		 
		if(gxhWeightSpecialType<1 || gxhWeightSpecialType >3)
		gxhWeightSpecialType=1;  //  1 :  특작 3 kg , 2: 특작 melon   3: 특작 대봉,  4: 특작 파프니카
	

		//	if(gxhDisplaySelect<0 || gxhDisplaySelect >1)
		//		gxhDisplaySelect=1;  //  선별시 화면 선택 0( default): 선별시 등급갯수표시,  1: 선별시 선별무게 표시
	
		
		
		
//gxhDisplaySelect		
		
		//gxhWeightSpecialType
		
		
		
		
		
		

} /* of cond_check */


void recall_dan(char dan)
{	
	i1dan = 0;
	i2dan = 0;
	i3dan = 0;
	i4dan = 0;
	i5dan = 0;
	i6dan = 0;
	i7dan = 0;
	i8dan = 0;
	i9dan = 0;
	i10dan = 0;
	i11dan = 0;
	i12dan = 0;
	i13dan = 0;
	i14dan = 0;
	i15dan = 0;
	i16dan = 0;
	i17dan = 0;
	i18dan = 0;
	i19dan = 0;
	i20dan = 0;
	i21dan = 0;
	i22dan = 0;
	i23dan = 0;
	i24dan = 0;
	i25dan = 0;
  
	switch(dan)
	{
		case 0: // 포도 
//			CALL_FN = FN_EE_Read_val;
			i1dan = EE_Read_val (ADR_i1danB1,2);//Delay_ms(3);
			i2dan = EE_Read_val (ADR_i2danB1,2);//Delay_ms(3);
			i3dan = EE_Read_val (ADR_i3danB1,2);//Delay_ms(3);
			i4dan = EE_Read_val (ADR_i4danB1,2);//Delay_ms(3);
			i5dan = EE_Read_val (ADR_i5danB1,2);//Delay_ms(3);
			i6dan = EE_Read_val (ADR_i6danB1,2);//Delay_ms(3);
			i7dan = EE_Read_val (ADR_i7danB1,2);//Delay_ms(3);
			i8dan = EE_Read_val (ADR_i8danB1,2);//Delay_ms(3);
			i9dan = EE_Read_val (ADR_i9danB1,2);//Delay_ms(3);
			i10dan =EE_Read_val (ADR_i10danB1,2);//Delay_ms(3);
			i11dan = EE_Read_val (ADR_i11danB1,2);//Delay_ms(3);
			i12dan = EE_Read_val (ADR_i12danB1,2);//Delay_ms(3);
			i13dan = EE_Read_val (ADR_i13danB1,2);//Delay_ms(3);
			i14dan = EE_Read_val (ADR_i14danB1,2);//Delay_ms(3);
			i15dan = EE_Read_val (ADR_i15danB1,2);//Delay_ms(3);
//			SaveDan(0);
			break;
		case 1: //자두

//			CALL_FN = FN_EE_Read_val;
			i1dan = EE_Read_val (ADR_i1danB2,2);//Delay_ms(3);
			i2dan = EE_Read_val (ADR_i2danB2,2);//Delay_ms(3);
			i3dan = EE_Read_val (ADR_i3danB2,2);//Delay_ms(3);
			i4dan = EE_Read_val (ADR_i4danB2,2);//Delay_ms(3);
			i5dan = EE_Read_val (ADR_i5danB2,2);//Delay_ms(3);
			i6dan = EE_Read_val (ADR_i6danB2,2);//Delay_ms(3);
			i7dan = EE_Read_val (ADR_i7danB2,2);//Delay_ms(3);
			i8dan = EE_Read_val (ADR_i8danB2,2);//Delay_ms(3);
			i9dan = EE_Read_val (ADR_i9danB2,2);//Delay_ms(3);
			i10dan =EE_Read_val (ADR_i10danB2,2);//Delay_ms(3);
			i11dan =EE_Read_val (ADR_i11danB2,2);//Delay_ms(3);
			i12dan =EE_Read_val (ADR_i12danB2,2);//Delay_ms(3);
			i13dan =EE_Read_val (ADR_i13danB2,2);//Delay_ms(3);Delay_ms(3);
			i14dan =EE_Read_val (ADR_i14danB2,2);//Delay_ms(3);
			i15dan =EE_Read_val (ADR_i15danB2,2);//Delay_ms(3);
//			SaveDan(0);
			break;
		case 2: //복숭아  15 에서 20개로 수정 (GP-60XP)
//			CALL_FN = FN_EE_Read_val;
			i1dan = (int)EE_Read_val (ADR_i1danB3,2);//Delay_ms(3);
			i2dan = (int)EE_Read_val (ADR_i2danB3,2);//Delay_ms(3);Delay_ms(3);
			i3dan = (int)EE_Read_val (ADR_i3danB3,2);//Delay_ms(3);
			i4dan = (int)EE_Read_val (ADR_i4danB3,2);//Delay_ms(3);
			i5dan = (int)EE_Read_val (ADR_i5danB3,2);//Delay_ms(3);
			i6dan = (int)EE_Read_val (ADR_i6danB3,2);//Delay_ms(3);
			i7dan = (int)EE_Read_val (ADR_i7danB3,2);Delay_ms(3);
			i8dan = (int)EE_Read_val (ADR_i8danB3,2);Delay_ms(3);
			i9dan = (int)EE_Read_val (ADR_i9danB3,2);Delay_ms(3);
			i10dan = (int)EE_Read_val (ADR_i10danB3,2);Delay_ms(3);
			i11dan = (int)EE_Read_val (ADR_i11danB3,2);Delay_ms(3);
			i12dan = (int)EE_Read_val (ADR_i12danB3,2);Delay_ms(3);
			i13dan = (int)EE_Read_val (ADR_i13danB3,2);Delay_ms(3);
			i14dan = (int)EE_Read_val (ADR_i14danB3,2);Delay_ms(3);
			i15dan = (int)EE_Read_val (ADR_i15danB3,2);Delay_ms(3);
 /*		 
		i16dan = (int)EE_Read_val (ADR_i16danB3,2);Delay_ms(3);
			i17dan = (int)EE_Read_val (ADR_i17danB3,2);Delay_ms(3);
			i18dan = (int)EE_Read_val (ADR_i18danB3,2);Delay_ms(3);
			i19dan = (int)EE_Read_val (ADR_i19danB3,2);Delay_ms(3);Delay_ms(3);
			i20dan = (int)EE_Read_val (ADR_i20danB3,2);Delay_ms(3);
*/
//			SaveDan(0);
			break;
		case 3://사과   15개에서 20개로 늘림
//			CALL_FN = FN_EE_Read_val;
			i1dan = (int)EE_Read_val (ADR_i1danB4,2);Delay_ms(3);
			i2dan = (int)EE_Read_val (ADR_i2danB4,2);Delay_ms(3);
			i3dan = (int)EE_Read_val (ADR_i3danB4,2);Delay_ms(3);
			i4dan = (int)EE_Read_val (ADR_i4danB4,2);Delay_ms(3);
			i5dan = (int)EE_Read_val (ADR_i5danB4,2);Delay_ms(3);
			i6dan = (int)EE_Read_val (ADR_i6danB4,2);Delay_ms(3);
			i7dan = (int)EE_Read_val (ADR_i7danB4,2);Delay_ms(3);
			i8dan = (int)EE_Read_val (ADR_i8danB4,2);Delay_ms(3);
			i9dan = (int)EE_Read_val (ADR_i9danB4,2);Delay_ms(3);
			i10dan = (int)EE_Read_val (ADR_i10danB4,2);Delay_ms(3);Delay_ms(3);
			i11dan = (int)EE_Read_val (ADR_i11danB4,2);Delay_ms(3);
			i12dan = (int)EE_Read_val (ADR_i12danB4,2);Delay_ms(3);
			i13dan = (int)EE_Read_val (ADR_i13danB4,2);Delay_ms(3);
			i14dan = (int)EE_Read_val (ADR_i14danB4,2);Delay_ms(3);
			i15dan = (int)EE_Read_val (ADR_i15danB4,2);Delay_ms(3);
    /*  
		i16dan = (int)EE_Read_val (ADR_i16danB4,2);Delay_ms(3);
			i17dan = (int)EE_Read_val (ADR_i17danB4,2);Delay_ms(3);
			i18dan = (int)EE_Read_val (ADR_i18danB4,2);Delay_ms(3);
			i19dan = (int)EE_Read_val (ADR_i19danB4,2);Delay_ms(3);
			i20dan = (int)EE_Read_val (ADR_i20danB4,2);Delay_ms(3);
*/
		
		//			SaveDan(0);
			break;
		case 4://감   15개
	//		CALL_FN = FN_EE_Read_val;
			i1dan = (int)EE_Read_val (ADR_i1danB5,2);Delay_ms(3);
			i2dan = (int)EE_Read_val (ADR_i2danB5,2);Delay_ms(3);
			i3dan = (int)EE_Read_val (ADR_i3danB5,2);Delay_ms(3);
			i4dan = (int)EE_Read_val (ADR_i4danB5,2);Delay_ms(3);
			i5dan = (int)EE_Read_val (ADR_i5danB5,2);Delay_ms(3);
			i6dan = (int)EE_Read_val (ADR_i6danB5,2);Delay_ms(3);Delay_ms(3);
			i7dan = (int)EE_Read_val (ADR_i7danB5,2);Delay_ms(3);
			i8dan = (int)EE_Read_val (ADR_i8danB5,2);Delay_ms(3);
			i9dan = (int)EE_Read_val (ADR_i9danB5,2);Delay_ms(3);
			i10dan = (int)EE_Read_val (ADR_i10danB5,2);Delay_ms(3);
			i11dan = (int)EE_Read_val (ADR_i11danB5,2);Delay_ms(3);Delay_ms(3);
			i12dan = (int)EE_Read_val (ADR_i12danB5,2);Delay_ms(3);
			i13dan = (int)EE_Read_val (ADR_i13danB5,2);Delay_ms(3);
			i14dan = (int)EE_Read_val (ADR_i14danB5,2);Delay_ms(3);
			i15dan = (int)EE_Read_val (ADR_i15danB5,2);Delay_ms(3);
      
		
		//			SaveDan(0);
			break;
		case 5: // 배  15개에서 20개로 늘림
		//	CALL_FN = FN_EE_Read_val;
			i1dan = (int)EE_Read_val (ADR_i1danB6,2);Delay_ms(3);
			i2dan = (int)EE_Read_val (ADR_i2danB6,2);Delay_ms(3);
			i3dan = (int)EE_Read_val (ADR_i3danB6,2);Delay_ms(3);
			i4dan = (int)EE_Read_val (ADR_i4danB6,2);Delay_ms(3);
			i5dan = (int)EE_Read_val (ADR_i5danB6,2);Delay_ms(3);
			i6dan = (int)EE_Read_val (ADR_i6danB6,2);Delay_ms(3);Delay_ms(3);
			i7dan = (int)EE_Read_val (ADR_i7danB6,2);Delay_ms(3);
			i8dan = (int)EE_Read_val (ADR_i8danB6,2);Delay_ms(3);
			i9dan = (int)EE_Read_val (ADR_i9danB6,2);Delay_ms(3);
			i10dan = (int)EE_Read_val (ADR_i10danB6,2);Delay_ms(3);
			i11dan = (int)EE_Read_val (ADR_i11danB6,2);Delay_ms(3);
			i12dan = (int)EE_Read_val (ADR_i12danB6,2);Delay_ms(3);
			i13dan = (int)EE_Read_val (ADR_i13danB6,2);Delay_ms(3);
			i14dan = (int)EE_Read_val (ADR_i14danB6,2);Delay_ms(3);
			i15dan = (int)EE_Read_val (ADR_i15danB6,2);Delay_ms(3);Delay_ms(3);
   /*     
		i16dan = (int)EE_Read_val (ADR_i16danB6,2);Delay_ms(3);
			i17dan = (int)EE_Read_val (ADR_i17danB6,2);Delay_ms(3);
			i18dan = (int)EE_Read_val (ADR_i18danB6,2);Delay_ms(3);
			i19dan = (int)EE_Read_val (ADR_i19danB6,2);Delay_ms(3);
			i20dan = (int)EE_Read_val (ADR_i20danB6,2);Delay_ms(3);
*/
		
		//			SaveDan(0);
			break;
		case 6:	// 한라봉 
	//		CALL_FN = FN_EE_Read_val;
			i1dan = (int)EE_Read_val (ADR_i1danB7,2);Delay_ms(3);
			i2dan = (int)EE_Read_val (ADR_i2danB7,2);Delay_ms(3);
			i3dan = (int)EE_Read_val (ADR_i3danB7,2);Delay_ms(3);
			i4dan = (int)EE_Read_val (ADR_i4danB7,2);Delay_ms(3);
			i5dan = (int)EE_Read_val (ADR_i5danB7,2);Delay_ms(3);
			i6dan = (int)EE_Read_val (ADR_i6danB7,2);Delay_ms(3);
			i7dan = (int)EE_Read_val (ADR_i7danB7,2);Delay_ms(3);
			i8dan = (int)EE_Read_val (ADR_i8danB7,2);Delay_ms(3);
			i9dan = (int)EE_Read_val (ADR_i9danB7,2);Delay_ms(3);
			i10dan = (int)EE_Read_val (ADR_i10danB7,2);Delay_ms(3);
			i11dan = (int)EE_Read_val (ADR_i11danB7,2);Delay_ms(3);
			i12dan = (int)EE_Read_val (ADR_i12danB7,2);Delay_ms(3);
			i13dan = (int)EE_Read_val (ADR_i13danB7,2);Delay_ms(3);
			i14dan = (int)EE_Read_val (ADR_i14danB7,2);Delay_ms(3);
			i15dan = (int)EE_Read_val (ADR_i15danB7,2);Delay_ms(3);
			i16dan = (int)EE_Read_val (ADR_i16danB7,2);Delay_ms(3);
			i17dan = (int)EE_Read_val (ADR_i17danB7,2);Delay_ms(3);
			i18dan = (int)EE_Read_val (ADR_i18danB7,2);Delay_ms(3);
			i19dan = (int)EE_Read_val (ADR_i19danB7,2);Delay_ms(3);
      i20dan = (int)EE_Read_val (ADR_i20danB7,2);Delay_ms(3);
			i21dan = (int)EE_Read_val (ADR_i21danB7,2);Delay_ms(3);
			i22dan = (int)EE_Read_val (ADR_i22danB7,2);Delay_ms(3);
			i23dan = (int)EE_Read_val (ADR_i23danB7,2);Delay_ms(3);
			i24dan = (int)EE_Read_val (ADR_i24danB7,2);Delay_ms(3);
	    i25dan = (int)EE_Read_val (ADR_i25danB7,2);Delay_ms(3);

//			SaveDan(0);
			break;
		case 7://레드향 19개에서 20개로
//			CALL_FN = FN_EE_Read_val;
			i1dan = (int)EE_Read_val (ADR_i1danB8,2);Delay_ms(3);
			i2dan = (int)EE_Read_val (ADR_i2danB8,2);Delay_ms(3);
			i3dan = (int)EE_Read_val (ADR_i3danB8,2);Delay_ms(3);
			i4dan = (int)EE_Read_val (ADR_i4danB8,2);Delay_ms(3);
			i5dan = (int)EE_Read_val (ADR_i5danB8,2);Delay_ms(3);
			i6dan = (int)EE_Read_val (ADR_i6danB8,2);Delay_ms(3);
			i7dan = (int)EE_Read_val (ADR_i7danB8,2);Delay_ms(3);Delay_ms(3);
			i8dan = (int)EE_Read_val (ADR_i8danB8,2);Delay_ms(3);
			i9dan = (int)EE_Read_val (ADR_i9danB8,2);Delay_ms(3);
			i10dan = (int)EE_Read_val (ADR_i10danB8,2);Delay_ms(3);
			i11dan = (int)EE_Read_val (ADR_i11danB8,2);Delay_ms(3);
			i12dan = (int)EE_Read_val (ADR_i12danB8,2);Delay_ms(3);
			i13dan = (int)EE_Read_val (ADR_i13danB8,2);Delay_ms(3);
			i14dan = (int)EE_Read_val (ADR_i14danB8,2);Delay_ms(3);
			i15dan = (int)EE_Read_val (ADR_i15danB8,2);Delay_ms(3);
			i16dan = (int)EE_Read_val (ADR_i16danB8,2);Delay_ms(3);
			i17dan = (int)EE_Read_val (ADR_i17danB8,2);Delay_ms(3);
			i18dan = (int)EE_Read_val (ADR_i18danB8,2);Delay_ms(3);
			i19dan = (int)EE_Read_val (ADR_i19danB8,2);Delay_ms(3);
	//	  i20dan = (int)EE_Read_val (ADR_i20danB8,2);Delay_ms(3);
//			SaveDan(0);
			break;
		case 8: // 특작  19개에서 25개
	//		CALL_FN = FN_EE_Read_val;
			i1dan = (int)EE_Read_val (ADR_i1danB9,2);Delay_ms(3);
			i2dan = (int)EE_Read_val (ADR_i2danB9,2);Delay_ms(3);
			i3dan = (int)EE_Read_val (ADR_i3danB9,2);Delay_ms(3);
			i4dan = (int)EE_Read_val (ADR_i4danB9,2);Delay_ms(3);
			i5dan = (int)EE_Read_val (ADR_i5danB9,2);Delay_ms(3);Delay_ms(3);
			i6dan = (int)EE_Read_val (ADR_i6danB9,2);Delay_ms(3);
			i7dan = (int)EE_Read_val (ADR_i7danB9,2);Delay_ms(3);
			i8dan = (int)EE_Read_val (ADR_i8danB9,2);Delay_ms(3);
			i9dan = (int)EE_Read_val (ADR_i9danB9,2);Delay_ms(3);
			i10dan = (int)EE_Read_val (ADR_i10danB9,2);Delay_ms(3);
			i11dan = (int)EE_Read_val (ADR_i11danB9,2);Delay_ms(3);
			i12dan = (int)EE_Read_val (ADR_i12danB9,2);Delay_ms(3);
			i13dan = (int)EE_Read_val (ADR_i13danB9,2);Delay_ms(3);
			i14dan = (int)EE_Read_val (ADR_i14danB9,2);Delay_ms(3);
			i15dan = (int)EE_Read_val (ADR_i15danB9,2);Delay_ms(3);
			i16dan = (int)EE_Read_val (ADR_i16danB9,2);Delay_ms(3);
			i17dan = (int)EE_Read_val (ADR_i17danB9,2);Delay_ms(3);
			i18dan = (int)EE_Read_val (ADR_i18danB9,2);Delay_ms(3);
			i19dan = (int)EE_Read_val (ADR_i19danB9,2);Delay_ms(3);
  /*		 
		 i20dan = (int)EE_Read_val (ADR_i20danB9,2);Delay_ms(3);
     	 
		 i21dan = (int)EE_Read_val (ADR_i21danB9,2);Delay_ms(3);
			i22dan = (int)EE_Read_val (ADR_i22danB9,2);Delay_ms(3);
			i23dan = (int)EE_Read_val (ADR_i23danB9,2);Delay_ms(3);
			i24dan = (int)EE_Read_val (ADR_i24danB9,2);Delay_ms(3);
		  i25dan = (int)EE_Read_val (ADR_i25danB9,2);Delay_ms(3);
		*/
//			SaveDan(0);
			break;

   		case 9: // 단호박 신규 20개
	//		CALL_FN = FN_EE_Read_val;
			i1dan = (int)EE_Read_val (ADR_i1danB10,2);Delay_ms(3);
			i2dan = (int)EE_Read_val (ADR_i2danB10,2);Delay_ms(3);
			i3dan = (int)EE_Read_val (ADR_i3danB10,2);Delay_ms(3);
			i4dan = (int)EE_Read_val (ADR_i4danB10,2);Delay_ms(3);
			i5dan = (int)EE_Read_val (ADR_i5danB10,2);Delay_ms(3);
			i6dan = (int)EE_Read_val (ADR_i6danB10,2);Delay_ms(3);
			i7dan = (int)EE_Read_val (ADR_i7danB10,2);Delay_ms(3);
			i8dan = (int)EE_Read_val (ADR_i8danB10,2);Delay_ms(3);
			i9dan = (int)EE_Read_val (ADR_i9danB10,2);Delay_ms(3);
			i10dan = (int)EE_Read_val (ADR_i10danB10,2);Delay_ms(3);
			i11dan = (int)EE_Read_val (ADR_i11danB10,2);Delay_ms(3);
			i12dan = (int)EE_Read_val (ADR_i12danB10,2);Delay_ms(3);
			i13dan = (int)EE_Read_val (ADR_i13danB10,2);Delay_ms(3);
			i14dan = (int)EE_Read_val (ADR_i14danB10,2);Delay_ms(3);
			i15dan = (int)EE_Read_val (ADR_i15danB10,2);Delay_ms(3);
			i16dan = (int)EE_Read_val (ADR_i16danB10,2);Delay_ms(3);
			i17dan = (int)EE_Read_val (ADR_i17danB10,2);Delay_ms(3);
			/*
			i18dan = (int)EE_Read_val (ADR_i18danB10,2);Delay_ms(3);
			i19dan = (int)EE_Read_val (ADR_i19danB10,2);Delay_ms(3);
		  i20dan = (int)EE_Read_val (ADR_i20danB10,2);Delay_ms(3);
		  */
		
//			SaveDan(0);
			break;





		default:
			break;
	}
}


void SaveDan(char dan) //0321
{
	switch(dan)
	{
		case 0: //   포도, 15개R_i1danB1,2);
	i1dan=	EE_Write_val(ADR_i1danB1 , i1dan , 2);	 Delay_ms(10);
  i2dan=	EE_Write_val(ADR_i2danB1 , i2dan , 2);	 Delay_ms(10);
  i3dan=	EE_Write_val(ADR_i3danB1 , i3dan , 2);	 Delay_ms(10);
  i4dan=	EE_Write_val(ADR_i4danB1 , i4dan , 2);	 Delay_ms(10);
  i5dan=	EE_Write_val(ADR_i5danB1 , i5dan , 2);	 Delay_ms(10);
  i6dan=	EE_Write_val(ADR_i6danB1 , i6dan , 2);	 Delay_ms(10);
  i7dan=	EE_Write_val(ADR_i7danB1 , i7dan , 2);	 Delay_ms(10);
  i8dan=	EE_Write_val(ADR_i8danB1 , i8dan , 2);	 Delay_ms(10);
  i9dan=	EE_Write_val(ADR_i9danB1 , i9dan , 2);	 Delay_ms(10);
  i10dan=	EE_Write_val(ADR_i10danB1 , i10dan , 2);	 Delay_ms(10);
  i11dan=	EE_Write_val(ADR_i11danB1 , i11dan , 2);	 Delay_ms(10);
  i12dan=	EE_Write_val(ADR_i12danB1 , i12dan , 2);	 Delay_ms(10);
  i13dan=	EE_Write_val(ADR_i13danB1 , i13dan , 2);	 Delay_ms(10);
  i14dan=	EE_Write_val(ADR_i14danB1 , i14dan , 2);	 Delay_ms(10);
  i15dan=	EE_Write_val(ADR_i15danB1 , i15dan , 2);	 Delay_ms(10);
		
		break;
		case 1: // 자두
	 i1dan=	EE_Write_val(ADR_i1danB2 , i1dan , 2);	 Delay_ms(10);
  i2dan=	EE_Write_val(ADR_i2danB2 , i2dan , 2);	 Delay_ms(10);
  i3dan=	EE_Write_val(ADR_i3danB2 , i3dan , 2);	 Delay_ms(10);
  i4dan=	EE_Write_val(ADR_i4danB2 , i4dan , 2);	 Delay_ms(10);
  i5dan=	EE_Write_val(ADR_i5danB2 , i5dan , 2);	 Delay_ms(10);
  i6dan=	EE_Write_val(ADR_i6danB2 , i6dan , 2);	 Delay_ms(10);
  i7dan=	EE_Write_val(ADR_i7danB2 , i7dan , 2);	 Delay_ms(10);
  i8dan=	EE_Write_val(ADR_i8danB2 , i8dan , 2);	 Delay_ms(10);
  i9dan=	EE_Write_val(ADR_i9danB2 , i9dan , 2);	 Delay_ms(10);
  i10dan=	EE_Write_val(ADR_i10danB2 , i10dan , 2);	 Delay_ms(10);
  i11dan=	EE_Write_val(ADR_i11danB2 , i11dan , 2);	 Delay_ms(10);
  i12dan=	EE_Write_val(ADR_i12danB2 , i12dan , 2);	 Delay_ms(10);
  i13dan=	EE_Write_val(ADR_i13danB2 , i13dan , 2);	 Delay_ms(10);
  i14dan=	EE_Write_val(ADR_i14danB2 , i14dan , 2);	 Delay_ms(10);
  i15dan=	EE_Write_val(ADR_i15danB2 , i15dan , 2);	 Delay_ms(10);
			
		
		break;
 case 2: //복숭아  15 -> 20개 
	   i1dan=	EE_Write_val(ADR_i1danB3 , i1dan , 2);	 Delay_ms(10);
     i2dan=	EE_Write_val(ADR_i2danB3 , i2dan , 2);	 Delay_ms(10);
     i3dan=	EE_Write_val(ADR_i3danB3 , i3dan , 2);	 Delay_ms(10);
     i4dan=	EE_Write_val(ADR_i4danB3 , i4dan , 2);	 Delay_ms(10);
     i5dan=	EE_Write_val(ADR_i5danB3 , i5dan , 2);	 Delay_ms(10);
     i6dan=	EE_Write_val(ADR_i6danB3 , i6dan , 2);	 Delay_ms(10);
     i7dan=	EE_Write_val(ADR_i7danB3 , i7dan , 2);	 Delay_ms(10);
     i8dan=	EE_Write_val(ADR_i8danB3 , i8dan , 2);	 Delay_ms(10);
     i9dan=	EE_Write_val(ADR_i9danB3 , i9dan , 2);	 Delay_ms(10);
     i10dan=	EE_Write_val(ADR_i10danB3 , i10dan , 2);	 Delay_ms(10);
     i11dan=	EE_Write_val(ADR_i11danB3 , i11dan , 2);	 Delay_ms(10);
     i12dan=	EE_Write_val(ADR_i12danB3 , i12dan , 2);	 Delay_ms(10);
     i13dan=	EE_Write_val(ADR_i13danB3 , i13dan , 2);	 Delay_ms(10);
     i14dan=	EE_Write_val(ADR_i14danB3 , i14dan , 2);	 Delay_ms(10);
     i15dan=	EE_Write_val(ADR_i15danB3 , i15dan , 2);	 Delay_ms(10);
	  /*
     i16dan=	EE_Write_val(ADR_i16danB3 , i16dan , 2);	 Delay_ms(10);
     i17dan=	EE_Write_val(ADR_i17danB3 , i17dan , 2);	 Delay_ms(10);
     i18dan=	EE_Write_val(ADR_i18danB3 , i18dan , 2);	 Delay_ms(10);
     i19dan=	EE_Write_val(ADR_i19danB3 , i19dan , 2);	 Delay_ms(10);
     i20dan=	EE_Write_val(ADR_i20danB3 , i20dan , 2);	 Delay_ms(10);
	 */
		
		break;
		case 3:// 사과 15개 -> 20개
		 i1dan=	EE_Write_val(ADR_i1danB4 , i1dan , 2);	 Delay_ms(10);
     i2dan=	EE_Write_val(ADR_i2danB4 , i2dan , 2);	 Delay_ms(10);
     i3dan=	EE_Write_val(ADR_i3danB4 , i3dan , 2);	 Delay_ms(10);
     i4dan=	EE_Write_val(ADR_i4danB4 , i4dan , 2);	 Delay_ms(10);
     i5dan=	EE_Write_val(ADR_i5danB4 , i5dan , 2);	 Delay_ms(10);
     i6dan=	EE_Write_val(ADR_i6danB4 , i6dan , 2);	 Delay_ms(10);
     i7dan=	EE_Write_val(ADR_i7danB4 , i7dan , 2);	 Delay_ms(10);
     i8dan=	EE_Write_val(ADR_i8danB4 , i8dan , 2);	 Delay_ms(10);
     i9dan=	EE_Write_val(ADR_i9danB4 , i9dan , 2);	 Delay_ms(10);
     i10dan=	EE_Write_val(ADR_i10danB4 , i10dan , 2);	 Delay_ms(10);
     i11dan=	EE_Write_val(ADR_i11danB4 , i11dan , 2);	 Delay_ms(10);
     i12dan=	EE_Write_val(ADR_i12danB4 , i12dan , 2);	 Delay_ms(10);
     i13dan=	EE_Write_val(ADR_i13danB4 , i13dan , 2);	 Delay_ms(10);
     i14dan=	EE_Write_val(ADR_i14danB4 , i14dan , 2);	 Delay_ms(10);
     i15dan=	EE_Write_val(ADR_i15danB4 , i15dan , 2);	 Delay_ms(10);
	  /* 
		i16dan=	EE_Write_val(ADR_i16danB4 , i16dan , 2);	 Delay_ms(10);
     i17dan=	EE_Write_val(ADR_i17danB4 , i17dan , 2);	 Delay_ms(10);
     i18dan=	EE_Write_val(ADR_i18danB4 , i18dan , 2);	 Delay_ms(10);
     i19dan=	EE_Write_val(ADR_i19danB4 , i19dan , 2);	 Delay_ms(10);
     i20dan=	EE_Write_val(ADR_i20danB4 , i20dan , 2);	 Delay_ms(10);
	*/
		
		break;
		case 4://감 15개
		 i1dan=	EE_Write_val(ADR_i1danB5 , i1dan , 2);	 Delay_ms(10);
     i2dan=	EE_Write_val(ADR_i2danB5 , i2dan , 2);	 Delay_ms(10);
     i3dan=	EE_Write_val(ADR_i3danB5 , i3dan , 2);	 Delay_ms(10);
     i4dan=	EE_Write_val(ADR_i4danB5 , i4dan , 2);	 Delay_ms(10);
     i5dan=	EE_Write_val(ADR_i5danB5 , i5dan , 2);	 Delay_ms(10);
     i6dan=	EE_Write_val(ADR_i6danB5 , i6dan , 2);	 Delay_ms(10);
     i7dan=	EE_Write_val(ADR_i7danB5 , i7dan , 2);	 Delay_ms(10);
     i8dan=	EE_Write_val(ADR_i8danB5 , i8dan , 2);	 Delay_ms(10);
     i9dan=	EE_Write_val(ADR_i9danB5 , i9dan , 2);	 Delay_ms(10);
     i10dan=EE_Write_val(ADR_i10danB5 , i10dan , 2);	 Delay_ms(10);
     i11dan=EE_Write_val(ADR_i11danB5 , i11dan , 2);	 Delay_ms(10);
     i12dan=EE_Write_val(ADR_i12danB5 , i12dan , 2);	 Delay_ms(10);
     i13dan=EE_Write_val(ADR_i13danB5 , i13dan , 2);	 Delay_ms(10);
     i14dan=EE_Write_val(ADR_i14danB5 , i14dan , 2);	 Delay_ms(10);
     i15dan=EE_Write_val(ADR_i15danB5 , i15dan , 2);	 Delay_ms(10);
	   
		  break;
		case 5://배 15 -> 20개로 늘림 
		 i1dan=	EE_Write_val(ADR_i1danB6 , i1dan , 2);	 Delay_ms(10);
     i2dan=	EE_Write_val(ADR_i2danB6 , i2dan , 2);	 Delay_ms(10);
     i3dan=	EE_Write_val(ADR_i3danB6 , i3dan , 2);	 Delay_ms(10);
     i4dan=	EE_Write_val(ADR_i4danB6 , i4dan , 2);	 Delay_ms(10);
     i5dan=	EE_Write_val(ADR_i5danB6 , i5dan , 2);	 Delay_ms(10);
     i6dan=	EE_Write_val(ADR_i6danB6 , i6dan , 2);	 Delay_ms(10);
     i7dan=	EE_Write_val(ADR_i7danB6 , i7dan , 2);	 Delay_ms(10);
     i8dan=	EE_Write_val(ADR_i8danB6 , i8dan , 2);	 Delay_ms(10);
     i9dan=	EE_Write_val(ADR_i9danB6 , i9dan , 2);	 Delay_ms(10);
     i10dan=EE_Write_val(ADR_i10danB6 , i10dan , 2);	 Delay_ms(10);
     i11dan=EE_Write_val(ADR_i11danB6 , i11dan , 2);	 Delay_ms(10);
     i12dan=EE_Write_val(ADR_i12danB6 , i12dan , 2);	 Delay_ms(10);
     i13dan=EE_Write_val(ADR_i13danB6 , i13dan , 2);	 Delay_ms(10);
     i14dan=EE_Write_val(ADR_i14danB6 , i14dan , 2);	 Delay_ms(10);
     i15dan=EE_Write_val(ADR_i15danB6 , i15dan , 2);	 Delay_ms(10);
    /*	  
		i16dan=EE_Write_val(ADR_i16danB6 , i16dan , 2);	 Delay_ms(10);
     i17dan=EE_Write_val(ADR_i17danB6 , i17dan , 2);	 Delay_ms(10);
     i18dan=EE_Write_val(ADR_i18danB6 , i18dan , 2);	 Delay_ms(10);
     i19dan=EE_Write_val(ADR_i19danB6 , i19dan , 2);	 Delay_ms(10);
     i20dan=EE_Write_val(ADR_i20danB6 , i20dan , 2);	 Delay_ms(10);
	 */
		break;

		case 6:// 한라봉  25개 
		 i1dan=	EE_Write_val(ADR_i1danB7 , i1dan , 2);	 Delay_ms(10);
     i2dan=	EE_Write_val(ADR_i2danB7 , i2dan , 2);	 Delay_ms(10);
     i3dan=	EE_Write_val(ADR_i3danB7 , i3dan , 2);	 Delay_ms(10);
     i4dan=	EE_Write_val(ADR_i4danB7 , i4dan , 2);	 Delay_ms(10);
     i5dan=	EE_Write_val(ADR_i5danB7 , i5dan , 2);	 Delay_ms(10);
     i6dan=	EE_Write_val(ADR_i6danB7 , i6dan , 2);	 Delay_ms(10);
     i7dan=	EE_Write_val(ADR_i7danB7 , i7dan , 2);	 Delay_ms(10);
     i8dan=	EE_Write_val(ADR_i8danB7 , i8dan , 2);	 Delay_ms(10);
     i9dan=	EE_Write_val(ADR_i9danB7 , i9dan , 2);	 Delay_ms(10);
     i10dan=EE_Write_val(ADR_i10danB7 , i10dan , 2);	 Delay_ms(10);
     i11dan=EE_Write_val(ADR_i11danB7 , i11dan , 2);	 Delay_ms(10); 
     i12dan=EE_Write_val(ADR_i12danB7 , i12dan , 2);	 Delay_ms(10);
     
		 i13dan=EE_Write_val(ADR_i13danB7 , i13dan , 2);	 Delay_ms(10);
     i14dan=EE_Write_val(ADR_i14danB7 , i14dan , 2);	 Delay_ms(10);
     i15dan=EE_Write_val(ADR_i15danB7 , i15dan , 2);	 Delay_ms(10);
	   i16dan=EE_Write_val(ADR_i16danB7 , i16dan , 2);	 Delay_ms(10);
     i17dan=EE_Write_val(ADR_i17danB7 , i17dan , 2);	 Delay_ms(10);
     i18dan=EE_Write_val(ADR_i18danB7 , i18dan , 2);	 Delay_ms(10);
     i19dan=EE_Write_val(ADR_i19danB7 , i19dan , 2);	 Delay_ms(10);
     
		 i20dan=EE_Write_val(ADR_i20danB7 , i20dan , 2);	 Delay_ms(10);
	   i21dan=EE_Write_val(ADR_i21danB7 , i21dan , 2);	 Delay_ms(10);
     i22dan=EE_Write_val(ADR_i22danB7 , i22dan , 2);	 Delay_ms(10);
     
		 i23dan=EE_Write_val(ADR_i23danB7 , i23dan , 2);	 Delay_ms(10);
     i24dan=EE_Write_val(ADR_i24danB7 , i24dan , 2);	 Delay_ms(10);
     i25dan=EE_Write_val(ADR_i25danB7 , i25dan , 2);	 Delay_ms(10);
	  
			break;

		case 7:// 레드향 19 ->  20개로 늘림
		 i1dan=	EE_Write_val(ADR_i1danB8 , i1dan , 2);	 Delay_ms(10);
     i2dan=	EE_Write_val(ADR_i2danB8 , i2dan , 2);	 Delay_ms(10);
     i3dan=	EE_Write_val(ADR_i3danB8 , i3dan , 2);	 Delay_ms(10);
     i4dan=	EE_Write_val(ADR_i4danB8 , i4dan , 2);	 Delay_ms(10);
     i5dan=	EE_Write_val(ADR_i5danB8 , i5dan , 2);	 Delay_ms(10);
     i6dan=	EE_Write_val(ADR_i6danB8 , i6dan , 2);	 Delay_ms(10);
     i7dan=	EE_Write_val(ADR_i7danB8 , i7dan , 2);	 Delay_ms(10);
     i8dan=	EE_Write_val(ADR_i8danB8 , i8dan , 2);	 Delay_ms(10);
     i9dan=	EE_Write_val(ADR_i9danB8 , i9dan , 2);	 Delay_ms(10);
     i10dan=EE_Write_val(ADR_i10danB8 , i10dan , 2);	 Delay_ms(10);
     i11dan=EE_Write_val(ADR_i11danB8 , i11dan , 2);	 Delay_ms(10);
     i12dan=EE_Write_val(ADR_i12danB8 , i12dan , 2);	 Delay_ms(10);
     i13dan=EE_Write_val(ADR_i13danB8 , i13dan , 2);	 Delay_ms(10);
     i14dan=EE_Write_val(ADR_i14danB8 , i14dan , 2);	 Delay_ms(10);
     i15dan=EE_Write_val(ADR_i15danB8 , i15dan , 2);	 Delay_ms(10);
	   i16dan=EE_Write_val(ADR_i16danB8 , i16dan , 2);	 Delay_ms(10);
     i17dan=EE_Write_val(ADR_i17danB8 , i17dan , 2);	 Delay_ms(10);
     i18dan=EE_Write_val(ADR_i18danB8 , i18dan , 2);	 Delay_ms(10);
     i19dan=EE_Write_val(ADR_i19danB8 , i19dan , 2);	 Delay_ms(10);
   //  i20dan=EE_Write_val(ADR_i20danB8 , i20dan , 2);	 Delay_ms(10);
	   
			
			break;

		case 8://특작 19개에서 25개로 늘림 
		 i1dan=	EE_Write_val(ADR_i1danB9 , i1dan , 2);	 Delay_ms(10);
     i2dan=	EE_Write_val(ADR_i2danB9 , i2dan , 2);	 Delay_ms(10);
     i3dan=	EE_Write_val(ADR_i3danB9 , i3dan , 2);	 Delay_ms(10);
     i4dan=	EE_Write_val(ADR_i4danB9 , i4dan , 2);	 Delay_ms(10);
     i5dan=	EE_Write_val(ADR_i5danB9 , i5dan , 2);	 Delay_ms(10);
     i6dan=	EE_Write_val(ADR_i6danB9 , i6dan , 2);	 Delay_ms(10);
     i7dan=	EE_Write_val(ADR_i7danB9 , i7dan , 2);	 Delay_ms(10);
     i8dan=	EE_Write_val(ADR_i8danB9 , i8dan , 2);	 Delay_ms(10);
     i9dan=	EE_Write_val(ADR_i9danB9 , i9dan , 2);	 Delay_ms(10);
     i10dan=EE_Write_val(ADR_i10danB9 , i10dan , 2);	 Delay_ms(10);
     i11dan=EE_Write_val(ADR_i11danB9 , i11dan , 2);	 Delay_ms(10);
     i12dan=EE_Write_val(ADR_i12danB9 , i12dan , 2);	 Delay_ms(10);
     i13dan=EE_Write_val(ADR_i13danB9 , i13dan , 2);	 Delay_ms(10);
     i14dan=EE_Write_val(ADR_i14danB9 , i14dan , 2);	 Delay_ms(10);
     i15dan=EE_Write_val(ADR_i15danB9 , i15dan , 2);	 Delay_ms(10);
	   i16dan=EE_Write_val(ADR_i16danB9 , i16dan , 2);	 Delay_ms(10);
     i17dan=EE_Write_val(ADR_i17danB9 , i17dan , 2);	 Delay_ms(10);
     i18dan=EE_Write_val(ADR_i18danB9 , i18dan , 2);	 Delay_ms(10);
     i19dan=EE_Write_val(ADR_i19danB9 , i19dan , 2);	 Delay_ms(10);
    /* 
		i20dan=EE_Write_val(ADR_i20danB9 , i20dan , 2);	 Delay_ms(10);
	   i21dan=EE_Write_val(ADR_i21danB9 , i21dan , 2);	 Delay_ms(10);
     i22dan=EE_Write_val(ADR_i22danB9 , i22dan , 2);	 Delay_ms(10);
     i23dan=EE_Write_val(ADR_i23danB9 , i23dan , 2);	 Delay_ms(10);
     i24dan=EE_Write_val(ADR_i24danB9 , i24dan , 2);	 Delay_ms(10);
     i25dan=EE_Write_val(ADR_i25danB9 , i25dan , 2);	 Delay_ms(10);
	   	*/
		break;

    case 9://신규   단호박 20개 0322
		 i1dan=	EE_Write_val(ADR_i1danB10 , i1dan , 2);	 Delay_ms(10);
     i2dan=	EE_Write_val(ADR_i2danB10 , i2dan , 2);	 Delay_ms(10);
     i3dan=	EE_Write_val(ADR_i3danB10 , i3dan , 2);	 Delay_ms(10);
     i4dan=	EE_Write_val(ADR_i4danB10 , i4dan , 2);	 Delay_ms(10);
     i5dan=	EE_Write_val(ADR_i5danB10 , i5dan , 2);	 Delay_ms(10);
     i6dan=	EE_Write_val(ADR_i6danB10 , i6dan , 2);	 Delay_ms(10);
     i7dan=	EE_Write_val(ADR_i7danB10 , i7dan , 2);	 Delay_ms(10);
     i8dan=	EE_Write_val(ADR_i8danB10 , i8dan , 2);	 Delay_ms(10);
     i9dan=	EE_Write_val(ADR_i9danB10 , i9dan , 2);	 Delay_ms(10);
     i10dan=EE_Write_val(ADR_i10danB10 , i10dan , 2);	 Delay_ms(10);
     i11dan=EE_Write_val(ADR_i11danB10 , i11dan , 2);	 Delay_ms(10);
     i12dan=EE_Write_val(ADR_i12danB10 , i12dan , 2);	 Delay_ms(10);
     i13dan=EE_Write_val(ADR_i13danB10 , i13dan , 2);	 Delay_ms(10);
     i14dan=EE_Write_val(ADR_i14danB10 , i14dan , 2);	 Delay_ms(10);
    
	 	 i15dan=EE_Write_val(ADR_i15danB10 , i15dan , 2);	 Delay_ms(10);
	   i16dan=EE_Write_val(ADR_i16danB10 , i16dan , 2);	 Delay_ms(10);
     i17dan=EE_Write_val(ADR_i17danB10 , i17dan , 2);	 Delay_ms(10);
    /* 
		i18dan=EE_Write_val(ADR_i18danB10 , i18dan , 2);	 Delay_ms(10);
     i19dan=EE_Write_val(ADR_i19danB10 , i19dan , 2);	 Delay_ms(10);
     i20dan=EE_Write_val(ADR_i20danB10 , i20dan , 2);	 Delay_ms(10);
	   	*/
		break;



		default:
			break;
	}
	//CALL_FN = FN_EE_Write_val;
/////	EE_Write_val(ADR_gxhCurrentType, (unsigned int)gxhCurrentType);
	//CALL_FN = FN_read_eep;
}


void DefaultDanSetting() //6월20일  0328
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
	
//	#ifdef dpfpf
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
 

///gxhWeightPeackType = 1;
 // 복숭아 
 if(gxhWeightPeackType == 1)	//4.5kg  0329
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
		i16dan = 0;
		i17dan = 0;
		i18dan = 0;
		i19dan = 0;
		i20dan = 0;
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
		i16dan = 0;
		i17dan = 0;
		i18dan = 0;
		i19dan = 0;
		i20dan = 0;
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
		i16dan = 0;
		i17dan = 0;
		i18dan = 0;
		i19dan = 0;
		i20dan = 0;
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
		i16dan = 0;
		i17dan = 0;
		i18dan = 0;
		i19dan = 0;
		i20dan = 0;
		SaveDan(2);
	}

  // 복 숭아 

//#ifdef dpfdpf 

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
	
//	#ifdef dfdfd 
	
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
 //#ifdef dfdfd
 
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
		i16dan = 0;
		i17dan = 0;
		i18dan = 0;
		i19dan = 0;
		i20dan = 0;
		SaveDan(5);
	}
	if(gxhWeightPearType == 2)	//15kg 0329
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
		i16dan = 0;
		i17dan = 0;
		i18dan = 0;
		i19dan = 0;
		i20dan = 0;
		SaveDan(5);
	}
 //배 
//>>
//#ifdef dfdfd
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
		SaveDan(6); //한라봉 
		
//#ifdef dfdfd
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
		i20dan = 0;  //레드향 
		SaveDan(7); 

//#ifdef dfdfd

//특작


		if(gxhWeightSpecialType==1) // 특작 3 kg 
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
		//9B/03000/02500/01900/01450/01200/01000/00000/00000/00000/00000/00000/00000/00000/00000/00000/00000/00000/00000";	
   if(gxhWeightSpecialType==2) // 특작  메론 3 kg 
      {				
		  	i1dan  = 3000;
		    i2dan  = 2500;
		    i3dan  = 1900;
		    i4dan  = 1450;
		   i5dan  =  1200;
		   i6dan  =  1000;
		   i7dan  =  0;
		   i8dan  =  0;
		   i9dan  =  0;
		   i10dan =  0;
		   i11dan =  0;
		  i12dan =  0;
		  i13dan =  0;
		  i14dan =  0;
		  i15dan =  0;
		  i16dan =  0;
		  i17dan =  0;
		  i18dan =  0;
		  i19dan = 0;
	 	  SaveDan(8);
	    }
	//		9C/00428/00358/00333/00305/00273/00255/00230/00217/00200/00189/00175/00168/00157/00151/00145/00100/00000/00000";
	 if(gxhWeightSpecialType==3) // 특작  대봉  10 kg 
      {				
		  	i1dan  = 428;
		    i2dan  = 358;
		    i3dan  = 333;
		    i4dan  = 305;
		   i5dan  =  273;
		   i6dan  =  255;
		   i7dan  =  230;
		   i8dan  =  217;
		   i9dan  =  200;
		   i10dan =  189;
		   i11dan =  175;
		  i12dan =  168;
		  i13dan =  157;
		  i14dan =  151;
		  i15dan =  145;
		  i16dan =  100;
		  i17dan =  0;
		  i18dan =  0;
		  i19dan = 0;
	 	  SaveDan(8);
	    }
	 if(gxhWeightSpecialType==4) // 특작  파프리카   15 kg 
      {
	      i1dan  = 301;
		    i2dan  = 251;
		    i3dan  = 180;
		    i4dan  = 145;
		   i5dan  =  120;
		   i6dan  =  100;
		   i7dan  =  0;
		   i8dan  =  0;
		   i9dan  =  0;
		   i10dan =  0;
		   i11dan =  0;
		  i12dan =  0;
		  i13dan =  0;
		  i14dan =  0;
		  i15dan =  0;
		  i16dan =  0;
		  i17dan =  0;
		  i18dan =  0;
		  i19dan = 0;
	 	  SaveDan(8);
	    				
		  }
	
/*	
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
		i20dan = 0;
		i21dan = 0;
		i22dan = 0;
		i23dan = 0;
		i24dan = 0;
		i25dan = 0;
		
		SaveDan(8);
  */
	
	//단호박  0322

   i1dan  = 3400;
		i2dan  = 3200;
		i3dan  = 2916;
		i4dan  = 2250;
		i5dan  = 1833;
		i6dan  = 1547;
		i7dan  = 1339;
		i8dan  = 1180;
		i9dan  = 1055;
		i10dan = 945;
		i11dan = 871;
		i12dan = 801;
		i13dan = 741;
		i14dan = 690;
		i15dan = 645;
		i16dan = 100;
		i17dan = 0;
		i18dan = 0;
		i19dan = 0;
		i20dan = 0;
		 SaveDan(9);
  
	
	
	
	
	// 특작 
	//#endif

}




#define serial_push	0

long ValidCheck(long cur_val, long min, long max, long default_val)
{
	long val;

	val = cur_val;
	if(cur_val<0 || max < cur_val) val = default_val;
	
	return val;
}

void read_set_factors()
{
//	union { u32 dw; u16 u[2]; u8 c[4];} temp;
//	float facter;
	
//  	u8 str[30];
 #ifdef dpfdpfd
	gxlDecimalPoint = EE_Read_val( ADR_gxlDecimalPoint, 4);
	if(/*gxlDecimalPoint<0 ||*/ MAX_gxlDecimalPoint < gxlDecimalPoint) gxlDecimalPoint = DEFAULT_gxlDecimalPoint;
//    if(serial_push) USART1_puts(gxlDecimalPoint);
	
	// weight back up : 1  no weight back up : 0
	gxhWeightBackup = EE_Read_val( ADR_gxhWeightBackup, 4);
	if(/*gxhWeightBackup<0 ||*/ MAX_gxhWeightBackup<gxhWeightBackup) gxhWeightBackup = DEFAULT_gxhWeightBackup;
//	if(serial_push) USART1_puts(gxhWeightBackup);	

	gxhMotionBand = EE_Read_val( ADR_gxhMotionBand, 4);
	if(/*gxhMotionBand<0 ||*/ MAX_gxhMotionBand < gxhMotionBand) gxhMotionBand = DEFAULT_gxhMotionBand;
//	if(serial_push) USART1_puts(gxhMotionBand);	
		
	gxhZTDigit = EE_Read_val( ADR_gxhZTDigit, 4);
	if(/*gxhZTDigit<0 ||*/ MAX_gxhZTDigit < gxhZTDigit) gxhZTDigit = DEFAULT_gxhZTDigit;
	
	// default: 1  0.5
	gxuAutoZero = EE_Read_val( ADR_gxuAutoZero, 4);
	if(gxuAutoZero<0 || MAX_gxuAutoZero < gxuAutoZero) gxuAutoZero = DEFAULT_gxuAutoZero;

	gxhFilter = EE_Read_val( ADR_gxhFilter, 4);
	// 5초보다 크면 
	if(/*gxhFilter<0 ||*/ MAX_gxhFilter < gxhFilter) gxhFilter = DEFAULT_gxhFilter;

//long ValidCheck(long cur_val, long min, long max, long default_val)
	gxhZTKStable = EE_Read_val( ADR_gxhZTKStable, 4);
	gxhZTKStable = ValidCheck( gxhZTKStable, 0, MAX_gxhZTKStable, DEFAULT_gxhZTKStable);

	gxlZeroKeyRange = EE_Read_val( ADR_gxlZeroKeyRange, 4);
	gxlZeroKeyRange = ValidCheck( gxlZeroKeyRange, 0, MAX_gxlZeroKeyRange, DEFAULT_gxlZeroKeyRange);

	gxlTareKeyRange = EE_Read_val( ADR_gxlTareKeyRange, 4);
	gxlTareKeyRange = ValidCheck( gxlTareKeyRange, 0, MAX_gxlTareKeyRange, DEFAULT_gxlTareKeyRange);

	gxlZeroKeyDisabled = EE_Read_val( ADR_gxlZeroKeyDisabled, 4);
	gxlZeroKeyDisabled = ValidCheck( gxlZeroKeyDisabled, 0, MAX_gxlZeroKeyDisabled, DEFAULT_gxlZeroKeyDisabled);

	gxlTareKeyDisabled = EE_Read_val( ADR_gxlTareKeyDisabled, 4);
	gxlTareKeyDisabled = ValidCheck( gxlTareKeyDisabled, 0, MAX_gxlTareKeyDisabled, DEFAULT_gxlTareKeyDisabled);

	gxhHoldType = EE_Read_val( ADR_gxhHoldType, 4);
	gxhHoldType = ValidCheck( gxhHoldType, 0, MAX_gxhHoldType, DEFAULT_gxhHoldType);

	gxhEIMode = EE_Read_val( ADR_gxhEIMode, 4);
	gxhEIMode = ValidCheck( gxhEIMode, 0, MAX_gxhEIMode, DEFAULT_gxhEIMode);

	gxlKeySet = EE_Read_val( ADR_gxlKeySet, 4);
	gxlKeySet = ValidCheck( gxlKeySet, 0, MAX_gxlKeySet, DEFAULT_gxlKeySet);

	gxlCodeNumSet = EE_Read_val( ADR_gxlCodeNumSet, 4);
	gxlCodeNumSet = ValidCheck( gxlCodeNumSet, 0, MAX_gxlCodeNumSet, DEFAULT_gxlCodeNumSet);

	gxlHoldOffTime = EE_Read_val( ADR_gxlHoldOffTime, 4);
	gxlHoldOffTime = ValidCheck( gxlHoldOffTime, 0, MAX_gxlHoldOffTime, DEFAULT_gxlHoldOffTime);


	gxhRelayMode = EE_Read_val( ADR_gxhRelayMode, 4);
	gxhRelayMode = ValidCheck( gxhRelayMode, 0, MAX_gxhRelayMode, DEFAULT_gxhRelayMode);


	gxlDataOutDelay = EE_Read_val( ADR_gxlDataOutDelay, 4);
	gxlDataOutDelay = ValidCheck( gxlDataOutDelay, 0, MAX_gxlDataOutDelay, DEFAULT_gxlDataOutDelay);

	gxlRelayOnDelay = EE_Read_val( ADR_gxlRelayOnDelay, 4);
	gxlRelayOnDelay = ValidCheck( gxlRelayOnDelay, 0, MAX_gxlRelayOnDelay, DEFAULT_gxlRelayOnDelay);

	gxlRelayOnTime = EE_Read_val( ADR_gxlRelayOnTime, 4);
	gxlRelayOnTime = ValidCheck( gxlRelayOnTime, 0, MAX_gxlRelayOnTime, DEFAULT_gxlRelayOnTime);


	//serial comm		
	gdhParity = EE_Read_val( ADR_gdhParity, 4);
	gdhParity = ValidCheck( gdhParity, 0, MAX_gdhParity, DEFAULT_gdhParity);

	gxhBaudRate = EE_Read_val( ADR_gxhBaudRate, 4);
	gxhBaudRate = ValidCheck( gxhBaudRate, 0, MAX_gxhBaudRate, DEFAULT_gxhBaudRate);

  	gxhSendMode = EE_Read_val( ADR_gxhSendMode, 4);
	gxhSendMode = ValidCheck( gxhSendMode, 0, MAX_gxhSendMode, DEFAULT_gxhSendMode);

	gxlSerialMethod = EE_Read_val( ADR_gxlSerialMethod, 4);
	gxlSerialMethod = ValidCheck( gxlSerialMethod, 0, MAX_gxlSerialMethod, DEFAULT_gxlSerialMethod);

	gxhMyAddress = EE_Read_val( ADR_gxhMyAddress, 4);
	gxhMyAddress = ValidCheck( gxhMyAddress, 0, MAX_gxhMyAddress, DEFAULT_gxhMyAddress);

	gxhRsFormat = EE_Read_val( ADR_gxhRsFormat, 4);
	gxhRsFormat = ValidCheck( gxhRsFormat, 0, MAX_gxhRsFormat, DEFAULT_gxhRsFormat);

	gxlBCCSelect = EE_Read_val( ADR_gxlBCCSelect, 4);
	gxlBCCSelect = ValidCheck( gxlBCCSelect, 0, MAX_gxlBCCSelect, DEFAULT_gxlBCCSelect);

	gxlTransPerSec = EE_Read_val( ADR_gxlTransPerSec, 4);
	gxlTransPerSec = ValidCheck( gxlTransPerSec, 0, MAX_gxlTransPerSec, DEFAULT_gxlTransPerSec);

	gxlWeightUnit = EE_Read_val( ADR_gxlWeightUnit, 4);
	gxlWeightUnit = ValidCheck( gxlWeightUnit, 0, MAX_gxlWeightUnit, DEFAULT_gxlWeightUnit);

	gxlDisplaySpeed = EE_Read_val( ADR_gxlDisplaySpeed, 4);
	gxlDisplaySpeed = ValidCheck( gxlDisplaySpeed, 0, MAX_gxlDisplaySpeed, DEFAULT_gxlDisplaySpeed);

	gxlStableOnDelay = EE_Read_val( ADR_gxlStableOnDelay, 4);
	gxlStableOnDelay = ValidCheck( gxlStableOnDelay, 0, MAX_gxlStableOnDelay, DEFAULT_gxlStableOnDelay);

	gxlPushPull = EE_Read_val( ADR_gxlPushPull, 4);
	gxlPushPull = ValidCheck( gxlPushPull, 0, MAX_gxlPushPull, DEFAULT_gxlPushPull);

  	gxhAroundZero = EE_Read_val( ADR_gxhAroundZero, 4);
	gxhAroundZero = ValidCheck( gxhAroundZero, 0, MAX_gxhAroundZero, DEFAULT_gxhAroundZero);

	gxlZeroDisplayRange = EE_Read_val( ADR_gxlZeroDisplayRange, 4);
	gxlZeroDisplayRange = ValidCheck( gxlZeroDisplayRange, 0, MAX_gxlZeroDisplayRange, DEFAULT_gxlZeroDisplayRange);

	gxlZeroMinus = EE_Read_val( ADR_gxlZeroMinus, 4);
	gxlZeroMinus = ValidCheck( gxlZeroMinus, 0, MAX_gxlZeroMinus, DEFAULT_gxlZeroMinus);

	gxlAnalogOutMax = EE_Read_val( ADR_gxlAnalogOutMax, 4);
	gxlAnalogOutMax = ValidCheck( gxlAnalogOutMax, 0, MAX_gxlAnalogOutMax, DEFAULT_gxlAnalogOutMax);

	gxlEquivalInput = EE_Read_val( ADR_gxlEquivalInput, 4);
	gxlEquivalInput = ValidCheck( gxlEquivalInput, 0, MAX_gxlEquivalInput, DEFAULT_gxlEquivalInput);

	gdwWhichCal = EE_Read_val( ADR_gdwWhichCal, 4);
	gdwWhichCal = ValidCheck( gdwWhichCal, 0, MAX_gdwWhichCal, DEFAULT_gdwWhichCal);
	
	gxhCurrentLoopBaudRate = EE_Read_val( ADR_gxhCurrentLoopBaudRate, 4);
	gxhCurrentLoopBaudRate = ValidCheck( gxhCurrentLoopBaudRate, 0, MAX_gxhCurrentLoopBaudRate, DEFAULT_gxhCurrentLoopBaudRate);


	//cal capacity (max)
///	v_capacity = EE_Read_val( ADR_v_capacity, 4);
	gnlCapacity = EE_Read_val(ADR_gnlCapacity, 4);	//add 2016.3.8
	v_capacity = gnlCapacity * power(10, gxlDecimalPoint); //add 2016.3.8

 #endif
	
}




//EE_Write_val(ADR_gnfFactor , gnfFactor , 4);

u32 EE_Write_val(int adr, u32 val, u8 bytes)
{
	union { u32 dw; u16 u[2]; u8 c[4];} temp;
	u32 ret_val;

	if(bytes == 1) temp.c[0] = val;
	else if (bytes == 2) temp.u[0] = val;
	else temp.dw = val;

  I2C_EE_BufferWrite( temp.c, adr, bytes);		
	I2C_EE_BufferRead( temp.c, adr, bytes);

	/*	
	I2C_EE_BufferWrite( temp.c, adr, 4);		
	I2C_EE_BufferRead( temp.c, adr, 4);
*/
	if(bytes == 1) ret_val = temp.c[0];
	else if (bytes == 2) ret_val = temp.u[0];
	else ret_val = temp.dw;
	
	return ret_val;
}

u32 EE_Read_val(int adr, u8 bytes)
{
	union { u32 dw; u16 u[2]; u8 c[4];} temp;
	u32 ret_val;
  I2C_EE_BufferRead( temp.c, adr, bytes);
	//I2C_EE_BufferRead( temp.c, adr, 4);

	if(bytes == 1) ret_val = temp.c[0];
	else if (bytes == 2) ret_val = temp.u[0];
	else ret_val = temp.dw;
	
	return ret_val;
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
