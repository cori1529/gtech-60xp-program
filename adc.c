/*
	adc.c
*/

#include "stm32f10x_lib.h"

#include "adc.h"
#include "key.h"
#include "set.h"
#include "key.h"
#include "lcd.h"
#include "var.h"

unsigned long  ad_buf[200];
unsigned long adc_timer;


union {
  unsigned long rlong;
         struct {
 unsigned char byte3;
 unsigned char byte2;
 unsigned char byte1;
 unsigned char byte0;
		         
}bytes;
	   
}tempdword;

unsigned long ADvalue;
unsigned char ADi; 
unsigned char AD_NEG,AD_SPEED;



//extern unsigned long ADvalue;


#define  ADC_SLCK_HIGH                     GPIO_SetBits(GPIOB, GPIO_Pin_14)
#define  ADC_SLCK_LOW                     GPIO_ResetBits(GPIOB, GPIO_Pin_14)
#define  ADC_POWER_DOWN_OFF              GPIO_SetBits(GPIOB, GPIO_Pin_12)


//#define    ADC_DATA_READY         if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) == 0x01 &&ADi==0 )


void adc_delay(int time)
{
	int	 duration;

	for(duration=0; duration<time; duration++) {}
}

#define ADC_DELAY   50
//#define ADC_DELAY   500

void CS5555_Clk(void) 
{ ADC_SLCK_HIGH;  adc_delay(ADC_DELAY); // Delay_ms(1); //  adc_delay(ADC_DELAY); 
  ADC_SLCK_LOW;   adc_delay(ADC_DELAY); //  Delay_ms(1); // adc_delay(ADC_DELAY);
}


unsigned long Read_AD_Value(void) 
{ 
 //ADC_SLCK_LOW; //adc_delay(ADC_DELAY); //Delay_ms(1);// adc_delay(100); 
u32 data_ready=0;
 ADvalue=0;
 AD_NEG=0;
  
 //CS5555_Clk(); //¤W¤Éªu±µ¦¬¸ê?,¤U?ªu?·s¸ê? 
/*
if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) == 0x01)
{	
	while(1)
 {
	 adc_delay(1000);  //150 usec
	//data_ready=GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13); 
	 if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) == 0x00)
	 { 
		 //adc_delay(1000);  //150 usec 
	  
		 break;
	 }
	 };
 
 
	 
 }
 */
  while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13));
 // adc_delay(3);  //150 usec	
 // while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13));
  

  //adc_delay(100);  //15 usec	
 
 
 for(ADi=0;ADi<24;ADi++) 
    {
		 ADvalue=ADvalue<<1;/// CS5555_Clk(); //¤W¤Éªu±µ¦¬¸ê?,¤U?ªu?·s¸ê? 
     ADC_SLCK_HIGH;  adc_delay(ADC_DELAY); // 300 nsec
		 if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) == 0x01 &&ADi==0 ){AD_NEG=1;}
     if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) ==0X01)
			      {
		       //adc_delay(ADC_DELAY);
		      //   if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) ==0X01)
			       ADvalue++;
						
						}
     ADC_SLCK_LOW;adc_delay(ADC_DELAY);
		 
		} // of for 
 	
//		adc_delay(100);
		CS5555_Clk();//  force to DOUT high
//		adc_delay(100);
 		
		
 /*
		
		for(i=0;i<10;i++)
		{
  ADC_SLCK_HIGH;   adc_delay(ADC_DELAY);  // force to DOUT high
 	ADC_SLCK_LOW;   adc_delay(ADC_DELAY);
	  }
	*/		
			//adc_delay(1000);  //150 usec	
 
		
	// adc_delay(100);  15 usec	 15000 nsec
	// adc_delay(10)     1500 nse
  // adc_delay(1)     150 nsec		
		
////CS5555_Clk();
return ADvalue;

} // of function                       //¦VCS5555µo°eµ²?¼Ð»x?¤¸ 


void INIT_CS5555(void)            //CS5555ªì©l¤Æ
{
	
	// Delay_us(10);
	adc_delay(100);
  ADC_SLCK_LOW;
	ADC_POWER_DOWN_OFF;
	Delay_ms(1000);


}



unsigned long read_filtered_adc(void)
{
	unsigned long lxdAdcValue;
    float NewValue,Difference,Average;
	int i;
	//gxhFilter=25;
	lxdAdcValue = Read_AD_Value();


  
	if (gxhFilter)
	{
	 for (i = 100 - 1; i > 0; i--)  ad_buf[i] = ad_buf[i-1];
    // for (i = 10 - 1; i > 0; i--)  ad_buf[i] = ad_buf[i-1];
	 	 
		ad_buf[0] = lxdAdcValue;
	  for (i = gxhFilter - 1; i > 0; i--)
		{
		  lxdAdcValue += ad_buf[i];
		}
	   lxdAdcValue = lxdAdcValue / gxhFilter;
	}
	else  gxhFilter = 1;

	//return lxdAdcValue/150; // span value = 31000 
    //return lxdAdcValue/65;  // span value = 73920
	
	//return lxdAdcValue/31;  // span about 15,000

	//return lxdAdcValue/15;	//2016.2.26 span 1/30,000  2mv/V loadcell
	//return (lxdAdcValue/50);//2016.08.03 ÀÎÀå¾ÐÃà¿ë
	return (lxdAdcValue/20);//2016.08.03 ÀÎÀå¾ÐÃà¿ë
}











void adc_initial(void)   /* self-Calibration */
{
    unsigned int b;
//	gxhFilter=30;
//	gxhFilter=45;	//2012³â 9¿ù 25ÀÏ 
//	gxhFilter=35;	//2012³â 11¿ù 15ÀÏ 
  	gxhFilter=30;	//2013³â 5¿ù 28ÀÏ Áö±×¿ë  

    for (b=0; b < MAX_FILTER; b++) ad_buf[b] = 0;

// IC15 ADS1251  Initialization 
// PC1 : CLK      PC2: DOUT      PC3: SCLK     

// IC18 ADS1251  Initialization 
// PF10 : CLK      PF9: DOUT      PF8: SCLK     

// IC21 ADS1251  Initialization 
// PC2 : CLK      PC0: DOUT      PC1: SCLK  
 
   GPIO_ResetBits(GPIOC, GPIO_Pin_1);  // SCLK LOW  , NO SYNCMODE
   
 //  GPIO_ResetBits(GPIOF, GPIO_Pin_8);  // SCLK LOW  , NO SYNCMODE


} // of adc_initial() 
/*--------------------------------------------------------------------------
;       ADS1251 BASIC INPUT/OUTPUT SUBROUTINE (AD7710 BIOS)
;---------------------------------------------------------------------------
 
 GSE-1600
 CLK (PB_7)
 Dout , /DRDY(PE_4)
 SCLK (PE3)

  E7  E6  E5    E4            E3    E2   E1   E0      
			   Dout, /DRDY	  SCLK 
*/
 
 /***************************************
	 VENDING MACHINE
 // PC1 : CLK      PC2: DOUT , /DRDY     PC3: SCLK 

 // IC6 ADS1251  Initialization 
// PC4 : CLK      PC6: DOUT      PC5: SCLK    

Clock : 7.680khz = 130.2 uSec

 *****************************************/

/*** GBS-350****/



//PB 14: SCLK    PB15: CLK  PB13 DOUT

void RDDATA_delay()
{
	int	 duration;

//	for(duration=0; duration<15; duration++) {} - ok
	for(duration=0; duration<20; duration++) {}
}

unsigned long RDDATA(void)
{
   #define  DELAY_TIMER1    0// original 100 10	//130 // VERY VERY CRITICAL !!!!!!!
 
   	char ch;
    unsigned long ad_value = 0;

   	while(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13));		
   	while( GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13));		//24 * clk
	while(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13));		//6 * clk
   	while( GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13));		//6 D-OUT
    
   	for (ch = 0; ch < 24; ch++)
   	{
      	GPIO_SetBits(GPIOB, GPIO_Pin_14);  // SCLK HIGH
//     Delay_us(DELAY_TIMER1);
//		TM_DelayMicros(DELAY_TIMER1);
		RDDATA_delay();
			
		ad_value <<= 1;
		ad_value += (unsigned long)(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13)); //	

		GPIO_ResetBits(GPIOB, GPIO_Pin_14);  // SCLK LOW  , NO SYNCMODE
//		Delay_us(DELAY_TIMER1);
//		TM_DelayMicros(DELAY_TIMER1);
		RDDATA_delay();
  	}
	    
   	return ad_value;
}


/*** GBS-350****/



unsigned long adc_value(void)
{

	//return RDDATA() >> 4;
    // return RDDATA() >> 3;
	//return RDDATA() >> 5;
	return RDDATA();
}

/*
unsigned long read_filtered_adc(void)
{
	unsigned long lxdAdcValue;
	int i;
	lxdAdcValue = adc_value();
	if (gxhFilter)
	{
	  for (i = MAX_FILTER - 1; i > 0; i--)  ad_buf[i] = ad_buf[i-1];
	  ad_buf[0] = lxdAdcValue;
	  for (i = gxhFilter - 1; i > 0; i--)
		{
		  lxdAdcValue += ad_buf[i];
		}
	   lxdAdcValue = lxdAdcValue / gxhFilter;
	}
	else  gxhFilter = 1;

	//return lxdAdcValue/150; // span value = 31000 
    //return lxdAdcValue/65;  // span value = 73920
	
	//return lxdAdcValue/31;  // span about 15,000

	//return lxdAdcValue/15;	//2016.2.26 span 1/30,000  2mv/V loadcell
	return lxdAdcValue/12;//2016.08.03 ÀÎÀå¾ÐÃà¿ë
}

  */
#ifdef dpf0d
unsigned long read_filtered_adc(void)
{
	unsigned long lxdAdcValue;
    float NewValue,Difference,Average;
	int i;
	gxhFilter=30;
	lxdAdcValue = CS5532_Result();

 #ifdef pdpf 
         NewValue = CS5532_Result();
         Difference = abs ( NewValue-Average );
         if ( Difference > 256 ) /* No software average for large steps */
           Average = NewValue;
        else if ( Difference > 64 ) /* Exponential average of 8 for medium step size */
          (float)Average = (float)(Average * 0.875 + NewValue * 0.125);
         else /* Exp. ave. of 64 for steady state noise reduction */
          (float)Average = (float)(Average * 0.984375 + NewValue * 0.015625);

		  (long)lxdAdcValue= Average;


  #endif






	if (gxhFilter)
	{
	  for (i = MAX_FILTER - 1; i > 0; i--)  ad_buf[i] = ad_buf[i-1];
	  ad_buf[0] = lxdAdcValue;
	  for (i = gxhFilter - 1; i > 0; i--)
		{
		  lxdAdcValue += ad_buf[i];
		}
	   lxdAdcValue = lxdAdcValue / gxhFilter;
	}
	else  gxhFilter = 1;

	//return lxdAdcValue/150; // span value = 31000 
    //return lxdAdcValue/65;  // span value = 73920
	
	//return lxdAdcValue/31;  // span about 15,000

	//return lxdAdcValue/15;	//2016.2.26 span 1/30,000  2mv/V loadcell
	return lxdAdcValue/12;//2016.08.03 ÀÎÀå¾ÐÃà¿ë
}

#endif

//F33 ¿µÁ¡»©±â ±â´É (gxlZeroMinus)
unsigned long read_filtered_adc_with_ZeroMinus(void)
{
	return read_filtered_adc() - gxlZeroMinus;
}

 //////////////////////CS5532
 
void delay(unsigned int milisecond)
/* n mS delay */
{
   unsigned char i;
    while(milisecond--)
      {
   i=200;
          while(i--);
      
}

}

void short_delay(void)
{
 char i;
  i=20;
  while( i-- );

}



 #define   CS5532_SDI 1

 void CS5532_Write_Byte(unsigned char ch)
{
   CS5532_CS_ENABLE;

   /* Wait for SPIy Tx buffer empty */
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
   /* Send SPIz data */
  SPI_I2S_SendData(SPI2, ch);

  CS5532_CS_DISABLE;
	
													 
}



  void SPI_Write_Byte(unsigned char ch)
{

   /* Wait for SPIy Tx buffer empty */
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
   /* Send SPIz data */
  SPI_I2S_SendData(SPI2, ch);
	

}

 
char SPI_WriteByte_ReadWait(char TxData) // ¾²°í ÀÐ´Â´Ù 
{
  u8 RxData=0;
  while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE)==RESET); //¾²°í                                                    
  SPI_I2S_SendData(SPI2,TxData);

   while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)==RESET);	 //ÀÐ´Â´Ù. 
  RxData=SPI_I2S_ReceiveData(SPI2);
  return RxData;
} 


char SPI_ReadByte(void)//
        

{
 u8 RxData=0;
  /*
   while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)==RESET);	 //ÀÐ´Â´Ù. 
  RxData=SPI_I2S_ReceiveData(SPI2);
  return RxData;
  */
 return (SPI_WriteByte_ReadWait(0xff));
}







 unsigned char CS5532_Read_Byte(void)
{
   char spi_read_data	;
   CS5532_CS_ENABLE;
    /* Wait for SPIy Tx buffer empty */
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
   	spi_read_data=SPI_I2S_ReceiveData(SPI2);
   CS5532_CS_DISABLE;
   return spi_read_data;

}


void CS5532_Write_Dword(unsigned long ld)
{
  tempdword.rlong=ld;
   /*
   CS5532_Write_Byte(tempdword.bytes.byte3);
   CS5532_Write_Byte(tempdword.bytes.byte2);
   CS5532_Write_Byte(tempdword.bytes.byte1);
   CS5532_Write_Byte(tempdword.bytes.byte0);
   */
   SPI_WriteByte_ReadWait(tempdword.bytes.byte0);
   SPI_WriteByte_ReadWait(tempdword.bytes.byte1);
   SPI_WriteByte_ReadWait(tempdword.bytes.byte2);
   SPI_WriteByte_ReadWait(tempdword.bytes.byte3);
   
   //	tempdword.rlong	tempdword.bytes.byte3


  // SPI_WriteByte_ReadWait

}

unsigned long  CS5532_Read_Dword(void)
{
   /*
  
   tempdword.bytes.byte3=CS5532_Read_Byte();
   tempdword.bytes.byte2=CS5532_Read_Byte();
   tempdword.bytes.byte1=CS5532_Read_Byte();
   tempdword.bytes.byte0=CS5532_Read_Byte();
   */
   tempdword.bytes.byte0=SPI_ReadByte();
   tempdword.bytes.byte1=SPI_ReadByte();
   tempdword.bytes.byte2=SPI_ReadByte();
   tempdword.bytes.byte3=SPI_ReadByte();
  
  
   
   //SPI_ReadByte
   return tempdword.rlong;

}


 void CS5532_Write_Command(unsigned char cmd)
{

   CS5532_Write_Byte(cmd);

   
}

//CS5532 Initial    0818

void CS5532_Serial_Reset(void)     //?ÍúÏ¢?êÈà÷Ù¤ÖµÙ¼ãÒ
{

 /*
  unsigned char i=16;
 //  CS5532_CS_ENABLE;
   short_delay();
   while(i--)
    CS5532_Write_Byte(SYNC1);
    CS5532_Write_Byte(SYNC0);
 //  CS5532_CS_DISABLE;
  */



   u8 i=0;
     for(i=0;i<50;i++)
         {
           SPI_WriteByte_ReadWait(0xff);	   // ÀÌ·¸ÄÉ ÇØ¾ßÁö¸¸ 
         }
         SPI_WriteByte_ReadWait(0xfe);		  //SPI_WriteByte_ReadWait
}



void CS5532_Initial()	 //0819
{
 #define DELAY_TIMER  100


   u8 i=0;
   u32 tmp=0,reg1=0,reg2=0,reg3=0,reg0=0;
   long Configuratin_Register, Gain_Register,Offset_Resister,Channel_Setup_Register;
    CS5532_CS_ENABLE;
//   while(tmp!=0x30000000)
    while(1)
  
   {
   CS5532_CS_ENABLE; Delay_ms(DELAY_TIMER);
    CS5532_Serial_Reset();
 
    SPI_WriteByte_ReadWait(0x03);	// writing cofiguration register 
    CS5532_Write_Dword(0x20000000);  // reset , ref voltage setup
 

    SPI_WriteByte_ReadWait(0x0b); //READING "Confuguration register"
    Configuratin_Register= CS5532_Read_Dword();
   	/*********** Respective register value after reset success
	// Configuration Register: 00000000(H)
    // Offset Registers: 00000000(H)
    // Gain Registers: 01000000(H)
    // Channel Setup Registers: 00000000(H)
	Respective register value after reset success ************/
   
   
    SPI_WriteByte_ReadWait(0x0b); //READING "Confuguration register"
	CS5532_Read_Dword();
    
	SPI_WriteByte_ReadWait(0x0a); //reading gain resigter
    Gain_Register=CS5532_Read_Dword();
  
    SPI_WriteByte_ReadWait(0x09); //reading offser resigter
    Offset_Resister=CS5532_Read_Dword();
  	 
   	SPI_WriteByte_ReadWait(0x0d); //channel setup resigter
    Channel_Setup_Register=CS5532_Read_Dword();
  	
   if(Configuratin_Register==0x30000000 && 	Gain_Register==0x01000000 && Channel_Setup_Register==0x00 && Offset_Resister==0x00)
    {
	 // Á¤»óÀûÀ¸·Î  CS5532 °¡ RESET ÀÌ ¿Ï·áµÇ¸é 
	  vputs("CS-5532");
	  Delay_ms(2000);
	/*
	 switch(which)
	   {
	    case 1:
	         vputs("CH-0001"); Delay_ms(2000);
		     break;
        case 2:
	         vputs("CH-0002"); Delay_ms(2000);
		     break;
	    case 3:
	         vputs("CH-0003"); Delay_ms(2000);
		     break;
	    case 4:
	         vputs("CH-0004"); Delay_ms(2000);
		     break;
		}
  	  */

	  vputs("INITIAL");
	  Delay_ms(2000);
	  vputs("SUCCESS");
	  Delay_ms(2000);
	  break;

	}
  }

  CS5532_Config();
  CS5532_CS_DISABLE_1;
  CS5532_CS_DISABLE_2;
  CS5532_CS_DISABLE_3;
  CS5532_CS_DISABLE_4;
   Delay_ms(DELAY_TIMER);

}


void CS5532_Initial_Multiple(char channel)
{
 #define DELAY_TIMER  100


   u8 i=0;
   u8 which;
   u32 tmp=0,reg1=0,reg2=0,reg3=0,reg0=0;
   long Configuratin_Register, Gain_Register,Offset_Resister,Channel_Setup_Register;
   which=channel;

   
   switch(which)
   {
   
   	case 1:
	 CS5532_CS_ENABLE_1;    CS5532_CS_DISABLE_2;  CS5532_CS_DISABLE_3;   CS5532_CS_DISABLE_4;
	   USART1_puts("channel-1 initial start!!");
	  break;
    case 2:
	  CS5532_CS_ENABLE_2;    CS5532_CS_DISABLE_1;  CS5532_CS_DISABLE_3;   CS5532_CS_DISABLE_4;
	  USART1_puts("channel-2 initial start!!");
	  break;
    case 3:
	  CS5532_CS_ENABLE_3;    CS5532_CS_DISABLE_1;  CS5532_CS_DISABLE_2;   CS5532_CS_DISABLE_4;
	  USART1_puts("channel-3 initial start!!");
	  break;
    case 4:
	  CS5532_CS_ENABLE_4;    CS5532_CS_DISABLE_1;  CS5532_CS_DISABLE_2;   CS5532_CS_DISABLE_3;
	  USART1_puts("channel-4 initial start!!");
	  break;
	default: break;
    }
   	 
 
//   while(tmp!=0x30000000)
    while(1)
  
   {
  //  CS5532_CS_ENABLE; Delay_ms(DELAY_TIMER);
    CS5532_Serial_Reset();
 
    SPI_WriteByte_ReadWait(0x03);	// writing cofiguration register 
    CS5532_Write_Dword(0x20000000);  // reset , ref voltage setup
 

    SPI_WriteByte_ReadWait(0x0b); //READING "Confuguration register"
    Configuratin_Register= CS5532_Read_Dword();
   	/*********** Respective register value after reset success
	// Configuration Register: 00000000(H)
    // Offset Registers: 00000000(H)
    // Gain Registers: 01000000(H)
    // Channel Setup Registers: 00000000(H)
	Respective register value after reset success ************/
   
   
    SPI_WriteByte_ReadWait(0x0b); //READING "Confuguration register"
	CS5532_Read_Dword();
    
	SPI_WriteByte_ReadWait(0x0a); //reading gain resigter
    Gain_Register=CS5532_Read_Dword();
  
    SPI_WriteByte_ReadWait(0x09); //reading offser resigter
    Offset_Resister=CS5532_Read_Dword();
  	 
   	SPI_WriteByte_ReadWait(0x0d); //channel setup resigter
    Channel_Setup_Register=CS5532_Read_Dword();
  	
   if(Configuratin_Register==0x30000000 && 	Gain_Register==0x01000000 && Channel_Setup_Register==0x00 && Offset_Resister==0x00)
    {
	 // Á¤»óÀûÀ¸·Î  CS5532 °¡ RESET ÀÌ ¿Ï·áµÇ¸é 
	  vputs("CS-5532");
	  Delay_ms(2000);

	 /// USART1_puts(" All channel initial success!!");
	  
	  	 switch(which)
	   {
	    case 1:
	         //vputs("CH-0001"); Delay_ms(500);
			 USART1_puts("channel-1 initial success!!");
		     break;
        case 2:
	         //vputs("CH-0002"); Delay_ms(500);
		     USART1_puts("channel-2 initial success!!");
	
			 break;
	    case 3:
	         //vputs("CH-0003"); Delay_ms(500);
			 USART1_puts("channel-3 initial success!!");
	
		     break;
	    case 4:
	         //vputs("CH-0004"); Delay_ms(500);
			 USART1_puts("channel-4 initial success!!");
	
		     break;
		}
	 /*	
	  vputs("INITIAL");
	  Delay_ms(2000);
	  vputs("SUCCESS");
	  Delay_ms(2000);
	 */
	  break;

	}
  }

  CS5532_Config();
   switch(which)
   {
   
   	case 1:
	  CS5532_CS_DISABLE_1;   
	  break;
    case 2:
	  CS5532_CS_DISABLE_2;    
	  break;
    case 3:
	  CS5532_CS_DISABLE_3;    
	  break;
    case 4:
	  CS5532_CS_DISABLE_4;    
	  break;
	default: break;

    
   }

 ////  USART1_puts("ADC all channel initial success!!");
/*	
      vputs("INITIAL");
	  Delay_ms(3000);
	  vputs("SUCCESS");
	  Delay_ms(3000);
 */ 
  //CS5532_CS_DISABLE; Delay_ms(DELAY_TIMER);

}













void CS5532_Config(void)	   //0819
{

    SPI_WriteByte_ReadWait(0x03);	// writing cofiguration register 
    CS5532_Write_Dword(0x00080000);  // reset , ref voltage setup


	SPI_WriteByte_ReadWait(0x05);	// writing channel setup register 
//	CS5532_Write_Dword(0x31803180);  // gain=1, rate=15hz

    CS5532_Write_Dword(((CSR_CH1+PGA_1+DATARATE_200+BIPOLAR_MODE +START_NO_DELAY+CAL_DATA_CH1)<<16)
+(CSR_CH1+PGA_1+DATARATE_200+BIPOLAR_MODE +START_NO_DELAY+CAL_DATA_CH1));
	 SPI_WriteByte_ReadWait(0xc0);

   /*
   SPI_WriteByte_ReadWait(CMD_WRITE+REG_CONFIG);		// Configure register ¾²±â 
   CS5532_Write_Dword(NORMAL_MODE_2+A0_GUARD+VREF_HIGH+A0_A1_REG_SETUP+CAL_DATA_PHYSICAL+LINE_FREQ_50);
   
   // normal mode_2 :  normal mode  
   // A0_GUARD(d26)
   //VREF_HIGH(d25) : 2.5<ref <Va
   //A0_A1_REG_SETUP(d22)
   
   
   
   
   SPI_WriteByte_ReadWait(CMD_WRITE+CMD_CH1+REG_SETUP);  //0X05
   // Ã¤³Î 1¹ø  setup registor writhing 

   CS5532_Write_Dword(((CSR_CH1+PGA_2+DATARATE_200+UNIPOLAR_MODE+START_NO_DELAY+CAL_DATA_CH1)<<16)
                     
+(CSR_CH1+PGA_2+DATARATE_200+UNIPOLAR_MODE+START_NO_DELAY+CAL_DATA_CH1));

   // CSR_CH1      :Ã¤³Î 1¹ø
   // PGA_1        : GAIN 1
   // DATARATE_200 : sampling rate 200 È¸ 
   
   
   
   CS5532_Dump_Regs();

   SPI_WriteByte_ReadWait(START_SINGLE+SETUP1+SELF_OFFSET_CAL);	//
   delay(1000);
   
   //single conversion + self-offset calibration
   
   
   CS5532_Dump_Regs();

   SPI_WriteByte_ReadWait(START_CONTINUOUS+SETUP1+NORMAL_CONVERSION);//

   //  conversion continous + normal conversion

	*/


}




void CS5532_Config_Multi(char channel)
{

   char which;

		 which=channel;


  SPI_WriteByte_ReadWait(0x05);	// writing channel setup register 


  switch(channel)
  {
   /*
   case 1:
     CS5532_Write_Dword(((CSR_CH1+PGA_1+DATARATE_50+UNIPOLAR_MODE+START_NO_DELAY+CAL_DATA_CH1)<<16)
                       +(CSR_CH1+PGA_1+DATARATE_50+UNIPOLAR_MODE+START_NO_DELAY+CAL_DATA_CH1));
	 break;
    
    case 2:
     CS5532_Write_Dword(((CSR_CH2+PGA_1+DATARATE_50+UNIPOLAR_MODE+START_NO_DELAY+CAL_DATA_CH2)<<16)
                       +(CSR_CH2+PGA_1+DATARATE_50+UNIPOLAR_MODE+START_NO_DELAY+CAL_DATA_CH2));
	 break;
   case 3:
     CS5532_Write_Dword(((CSR_CH3+PGA_1+DATARATE_50+UNIPOLAR_MODE+START_NO_DELAY+CAL_DATA_CH3)<<16)
                       +(CSR_CH3+PGA_1+DATARATE_50+UNIPOLAR_MODE+START_NO_DELAY+CAL_DATA_CH3));
	 break;
   case 4:
     CS5532_Write_Dword(((CSR_CH4+PGA_1+DATARATE_50+UNIPOLAR_MODE+START_NO_DELAY+CAL_DATA_CH4)<<16)
                       +(CSR_CH4+PGA_1+DATARATE_50+UNIPOLAR_MODE+START_NO_DELAY+CAL_DATA_CH4));
	 break;
   default: break;
	*/

    /*
	case 1:
     CS5532_Write_Dword(((CSR_CH1+PGA_1+DATARATE_200+UNIPOLAR_MODE+START_NO_DELAY+CAL_DATA_CH1)<<16)
                       +(CSR_CH1+PGA_1+DATARATE_200+UNIPOLAR_MODE+START_NO_DELAY+CAL_DATA_CH1));
	 break;
    
    case 2:
     CS5532_Write_Dword(((CSR_CH2+PGA_1+DATARATE_200+UNIPOLAR_MODE+START_NO_DELAY+CAL_DATA_CH2)<<16)
                       +(CSR_CH2+PGA_1+DATARATE_200+UNIPOLAR_MODE+START_NO_DELAY+CAL_DATA_CH2));
	 break;
   case 3:
     CS5532_Write_Dword(((CSR_CH3+PGA_1+DATARATE_200+UNIPOLAR_MODE+START_NO_DELAY+CAL_DATA_CH3)<<16)
                       +(CSR_CH3+PGA_1+DATARATE_200+UNIPOLAR_MODE+START_NO_DELAY+CAL_DATA_CH3));
	 break;
   case 4:
     CS5532_Write_Dword(((CSR_CH4+PGA_1+DATARATE_200+UNIPOLAR_MODE+START_NO_DELAY+CAL_DATA_CH4)<<16)
                       +(CSR_CH4+PGA_1+DATARATE_200+UNIPOLAR_MODE+START_NO_DELAY+CAL_DATA_CH4));
	 break;
   default: break;
	*/

  	case 1:
     CS5532_Write_Dword(((CSR_CH1+PGA_1+DATARATE_200+BIPOLAR_MODE+START_NO_DELAY+CAL_DATA_CH1)<<16)
                       +(CSR_CH1+PGA_1+DATARATE_200+BIPOLAR_MODE+START_NO_DELAY+CAL_DATA_CH1));
	 break;
    
    case 2:
     CS5532_Write_Dword(((CSR_CH2+PGA_1+DATARATE_200+BIPOLAR_MODE+START_NO_DELAY+CAL_DATA_CH2)<<16)
                       +(CSR_CH2+PGA_1+DATARATE_200+BIPOLAR_MODE+START_NO_DELAY+CAL_DATA_CH2));
	 break;
   case 3:
     CS5532_Write_Dword(((CSR_CH3+PGA_1+DATARATE_200+BIPOLAR_MODE+START_NO_DELAY+CAL_DATA_CH3)<<16)
                       +(CSR_CH3+PGA_1+DATARATE_200+BIPOLAR_MODE+START_NO_DELAY+CAL_DATA_CH3));
	 break;
   case 4:
     CS5532_Write_Dword(((CSR_CH4+PGA_1+DATARATE_200+BIPOLAR_MODE+START_NO_DELAY+CAL_DATA_CH4)<<16)
                       +(CSR_CH4+PGA_1+DATARATE_200+BIPOLAR_MODE+START_NO_DELAY+CAL_DATA_CH4));
	 break;
   default: break;
	




  }// of switch 




  	 SPI_WriteByte_ReadWait(0xc0);



}







void CS5532_System_Reset(void)
{

   CS5532_CS_ENABLE;
   short_delay();
   CS5532_Write_Command(CMD_WRITE+REG_CONFIG); 		// 
   //0x00+0x03  ?ÛÕöÇÐöðíÐï
   CS5532_Write_Dword(SYSTEM_RESET);        
  //0x01L&lt;&lt;29=0x20000000  ?RSöÇ1 P27
   delay(20); 

   CS5532_Write_Command(CMD_READ+REG_CONFIG);
  //0x08+0x03=0x0b  ?ÛÕöÇÐöðíÐï
  // CS5532_Read_Dword();

   CS5532_Write_Command(CMD_WRITE+REG_CONFIG);
  //0x00+0x03=0x03 ?ÛÕöÇÐöðíÐï
   CS5532_Write_Dword(NORMAL_MODE_2);    // (0x00L<<29)


}








void CS5532_Dump_Regs(void)
{

   SPI_WriteByte_ReadWait(CMD_READ+REG_CONFIG);     //0x08+0x03=0x0b              
 ////  printf(&quot;\nConfiguration : %08lx&quot;,CS5532_Read_Dword());

   SPI_WriteByte_ReadWait(CMD_READ+CMD_CH1+REG_OFFSET);  //0x08+0+0x01=0x09
 ////  printf(&quot;\nChannel 1 Offset: %08lx&quot;,CS5532_Read_Dword());

   SPI_WriteByte_ReadWait(CMD_READ+CMD_CH1+REG_GAIN);
    //0x08+0+0x02=0x0a;
  //// printf(&quot;\nChannel 1 GAIN: %08lx&quot;,CS5532_Read_Dword());

   SPI_WriteByte_ReadWait(CMD_READ+CMD_CH1+REG_SETUP);
  //0x08+0x0+0x05=0x0d
  //// printf(&quot;\nSETUP 1: %08lx&quot;,CS5532_Read_Dword());

}






 /*

bool  CS5532_ADC_Ready(void)
{
 // return !CS5532_SDO;
   return 1;
}
   */
/*
 long CS5532_Result(void)
{
 #define INTERVAL_TIMER 20
  char i;
 //CS5532_CS_ENABLE; Delay_ms(DELAY_TIMER);
 //SPI_WriteByte_ReadWait(0xff);
 //CS5532_CS_DISABLE; Delay_ms(DELAY_TIMER);
 
 
   SPI_WriteByte_ReadWait(0xc0);
   //Delay_us(10);
   for(i=0;i<INTERVAL_TIMER;i++)
      short_delay();
   
   ADC_DATA_READY
   {
   return  (long)( CS5532_Read_Dword() ) >>8 ;
	}

}
 */


 long CS5532_Result(void)
{
// #define INTERVAL_TIMER 100
// #define INTERVAL_TIMER_1 100

  #define INTERVAL_TIMER 100
 #define INTERVAL_TIMER_1 100


  int i,j;
//  long result;
  	float NewValue,Difference,Average;
		long final_result;
 
   SPI_WriteByte_ReadWait(0xc0);
   // Delay_us(100);
   for(i=0;i<INTERVAL_TIMER;i++)
      short_delay();
   
   ADC_DATA_READY
   {
      for(i=0;i<INTERVAL_TIMER_1;i++)
	    short_delay();
   return  (long)( CS5532_Read_Dword() ) >>8 ;
	}

}


long CS5532_Result_Multiple(char channel)
{
 
 // new pcb -200È¸ ±âÁØ 
  
  /*
  #define INTERVAL_CHANNEL    3 //VERY VERY CIRITICAL
  #define INTERVAL_TIMER     5
  #define INTERVAL_TIMER_1   5
   */

 #define INTERVAL_CHANNEL    1 //VERY VERY CIRITICAL
  #define INTERVAL_TIMER     1
  #define INTERVAL_TIMER_1   1
 



 int i,j;
 char which;
 which=channel;

  /*

  int i;
  char which;
  float NewValue,Difference,Average;
  long final_result;
  which=channel;
   */
  
  
  switch(which)
  {
	case 1:
     CS5532_CS_ENABLE_1; CS5532_CS_DISABLE_2; CS5532_CS_DISABLE_3; CS5532_CS_DISABLE_4;
	  break;
    case 2:
     CS5532_CS_ENABLE_2; CS5532_CS_DISABLE_1; CS5532_CS_DISABLE_3; CS5532_CS_DISABLE_4;
	  break;
    case 3:
     CS5532_CS_ENABLE_3; CS5532_CS_DISABLE_1; CS5532_CS_DISABLE_2; CS5532_CS_DISABLE_4;
	  break;
 	case 4:
     CS5532_CS_ENABLE_4; CS5532_CS_DISABLE_1; CS5532_CS_DISABLE_2; CS5532_CS_DISABLE_3;
	  break;

   default: break;

  }

  

 Delay_ms(INTERVAL_CHANNEL);  
 SPI_WriteByte_ReadWait(0xc0);   //start _conversiion

 
  for(i=0;i<INTERVAL_TIMER;i++)
      short_delay();
 

 while(1)
 {  
   ADC_DATA_READY
   {
     // for(i=0;i<INTERVAL_TIMER_1;i++)
	  //  short_delay();
   //return  (long)( CS5532_Read_Dword() ) >>8 ;		  // 24
   return  (long)( CS5532_Read_Dword() ) >>15 ;		  // 17 bit
	
	}
 };
  CS5532_CS_DISABLE_1; CS5532_CS_DISABLE_2; CS5532_CS_DISABLE_3; CS5532_CS_DISABLE_4;
   Delay_ms(INTERVAL_CHANNEL); 

}


long CS5532_Result_Fillterd(void)   
{
	 	float NewValue,Difference,Average;
		long final_result;
        NewValue = CS5532_Result();
         Difference = abs ( NewValue-Average );
         if ( Difference > 256 ) /* No software average for large steps */
           Average = NewValue;
        else if ( Difference > 64 ) /* Exponential average of 8 for medium step size */
          (float)Average = (float)(Average * 0.875 + NewValue * 0.125);
         else /* Exp. ave. of 64 for steady state noise reduction */
          (float)Average = (float)(Average * 0.984375 + NewValue * 0.015625);
		  
	   (long)final_result=Average;
        return final_result;

}


void ch1_adc_test(void)
 {
  long test_value; vputs("TEST4");Delay_ms(1000);
  CS5532_Initial_Multiple(1);  Delay_ms(1000);
  //vputs("TEST4");
   while(1)
		 {
		test_value=CS5532_Result_Multiple(1);
		vputnum_5digit(test_value);
		Delay_ms(300);
		if(keypush())
		{
		 if(KeyCode==KEY_ENTER)
		  break;

		} //of keypush
	  };  // of while 
 }
void ch2_adc_test(void)
 {
  long test_value;
  vputs("TEST5");	Delay_ms(1000);
  CS5532_Initial_Multiple(2);  Delay_ms(1000);
   while(1)
		 {
		test_value=CS5532_Result_Multiple(2);
		vputnum_5digit(test_value);
		Delay_ms(300);
		if(keypush())
		{
		 if(KeyCode==KEY_ENTER)
		  break;

		} //of keypush
	  };  // of while 
 }
void ch3_adc_test(void)
 {
  long test_value;
  vputs("TEST6"); Delay_ms(1000);
  CS5532_Initial_Multiple(3);  Delay_ms(1000);
   while(1)
		 {
		test_value=CS5532_Result_Multiple(3);
		vputnum_5digit(test_value);
		Delay_ms(300);
		if(keypush())
		{
		 if(KeyCode==KEY_ENTER)
		  break;

		} //of keypush
	  };  // of while 
 }
void ch4_adc_test(void)
 {
  long test_value;
  vputs("TEST7");	   Delay_ms(1000);
  CS5532_Initial_Multiple(4);  Delay_ms(1000);
  //vputs("TEST 6");
   while(1)
		 {
		test_value=CS5532_Result_Multiple(4);
		vputnum_5digit(test_value);
		Delay_ms(300);
		if(keypush())
		{
		 if(KeyCode==KEY_ENTER)
		  break;

		} //of keypush
	  };  // of while 
 }




/* end of file */
