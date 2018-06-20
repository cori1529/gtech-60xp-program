/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : stm32f10x_it.c
* Author             : MCD Application Team
* Version            : V2.0.1
* Date               : 06/13/2008
* Description        : Main Interrupt Service Routines.
*                      This file provides template for all exceptions handler
*                      and peripherals interrupt service routine.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"

#include "stm32f10x_type.h"
#include "stm32f10x_gpio.h"

#include "set.h"
#include "usart.h"
#include "var.h"
#include "key.h"
#include "adc.h"

extern vu8 Usart1_Interrupt_flag;

extern vu8 USARTx_Interrupt_flag;

extern vu8 RxBuffer2[];
extern vu8 RxCounter2;
extern vu8 Usart2_Interrupt_flag;
extern vu8 Rx2Cmd[];

extern vu8 RxBuffer3[];
extern vu8 RxCounter3;
extern vu8 Usart3_Interrupt_flag;
extern vu8 Rx3Cmd[];

extern vu8 RxBuffer4[];
extern vu8 RxCounter4;
extern vu8 Usart4_Interrupt_flag;
extern vu8 Rx4Cmd[];

extern vu8 Usart4_Interrupt_flag_test;




//vu16 TIM2_1ms;
//extern vu16 CCR1_Val;
//u16 capture = 0;

// for no prescaler => 153518.123667377Hz(76.8KHZ), 200 SAMPLING
#define	CCR1_Val		469	
//#define	COUNT_1SEC	156	//LATE 1SEC PER MINUTE
#define	COUNT_1SEC	153

//1ms
//#define	CCR1_Val		400	//about 0.5sec
//#define	CCR1_Val		350	//about 0.5sec
//#define	CCR1_Val		250	//about 10/8sec
//#define	CCR1_Val			300	//about 9/10sec
//#define	CCR1_Val			310	//about 1sec

volatile u16 guCurrentLoopSendTimer;	//current loop send timer //AD-8920 전송 간격 타이머
volatile u16 guCommLedOnTimer;	//on time ms
volatile u16 guRS422485TxEnableTimer;
volatile u16 guStableDelayTimer = 4000;	//F29에 사용함. ms timer
volatile u16 Serial_Timer;
volatile u16 guDelayMs;	//use delay only
volatile u16 ref_cal_mode_check_timer;	//ref cal 모드에서 "T" 키 연속입력 시간 제한 타이머 
volatile u16 Enter_timer;
volatile u16 Enter_flag;

extern vu16 guClockMs;
extern vu8 ghClockSec;
extern vu8 ghClockMin;
extern vu8 ghClockHour;
extern vu8 ghClockDate;
extern vu8 ghClockMonth;
extern vu16 ghClockYear;

extern volatile u16	KeyCheckTimer; 

extern char HoldFlag;
extern long HoldStateTimer;
extern int HoldTimerFlag;
extern int AverageHoldTimer;
extern int AverageHoldTimerFlag;
extern s32 Hold_Value;
extern u16 stable_count_flag;
extern u16 stable_counter;
extern u16 sending_interval_counter;
 u16 key_timer;
extern u16 hold_key_timer;
extern u16 start_key_timer;
extern u16 save_key_timer;
//extern u32 adc_timer;





/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : NMIException
* Description    : This function handles NMI exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NMIException(void)
{
}

/*******************************************************************************
* Function Name  : HardFaultException
* Description    : This function handles Hard Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void HardFaultException(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : MemManageException
* Description    : This function handles Memory Manage exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void MemManageException(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : BusFaultException
* Description    : This function handles Bus Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void BusFaultException(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : UsageFaultException
* Description    : This function handles Usage Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UsageFaultException(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : DebugMonitor
* Description    : This function handles Debug Monitor exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DebugMonitor(void)
{
}

/*******************************************************************************
* Function Name  : SVCHandler
* Description    : This function handles SVCall exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SVCHandler(void)
{
}

/*******************************************************************************
* Function Name  : PendSVC
* Description    : This function handles PendSVC exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void PendSVC(void)
{
}

/*******************************************************************************
* Function Name  : SysTickHandler
* Description    : This function handles SysTick Handler.
* Input          : None
* Output         : None
* Return         : None
***********************************************************초********************/
void SysTickHandler(void)
{
	
	adc_timer++;
//	 key_timer++;
//	USART1_puts(str);
//	TimingDelay_Decrement();
 // 	glStableDelayTimer++;
//	GPIO_WriteBit(GPIOB, GPIO_Pin_15, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_15)));
}

/*******************************************************************************
* Function Name  : WWDG_IRQHandler
* Description    : This function handles WWDG interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void WWDG_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : PVD_IRQHandler
* Description    : This function handles PVD interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void PVD_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TAMPER_IRQHandler
* Description    : This function handles Tamper interrupt request. 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TAMPER_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : RTC_IRQHandler
* Description    : This function handles RTC global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RTC_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : FLASH_IRQHandler
* Description    : This function handles Flash interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void FLASH_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : RCC_IRQHandler
* Description    : This function handles RCC interrupt request. 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RCC_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : EXTI0_IRQHandler
* Description    : This function handles External interrupt Line 0 request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI0_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : EXTI1_IRQHandler
* Description    : This function handles External interrupt Line 1 request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI1_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : EXTI2_IRQHandler
* Description    : This function handles External interrupt Line 2 request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI2_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : EXTI3_IRQHandler
* Description    : This function handles External interrupt Line 3 request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI3_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : EXTI4_IRQHandler
* Description    : This function handles External interrupt Line 4 request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI4_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA1_Channel1_IRQHandler
* Description    : This function handles DMA1 Channel 1 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel1_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA1_Channel2_IRQHandler
* Description    : This function handles DMA1 Channel 2 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel2_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA1_IT_TC2) == SET)
    {
        DMA_ClearITPendingBit(DMA1_IT_TC2);
//		USART3_DMA_send_enabled = 1;
//		USART3TX_disable();
		guRS422485TxEnableTimer = gxhBaudRate*20;	//6;	//2016.3.29 6; //about 6 ms
	}
}

/*******************************************************************************
* Function Name  : DMA1_Channel3_IRQHandler
* Description    : This function handles DMA1 Channel 3 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel3_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA1_Channel4_IRQHandler
* Description    : This function handles DMA1 Channel 4 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel4_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA1_IT_TC4) == SET)
    {
        DMA_ClearITPendingBit(DMA1_IT_TC4);
		USART1_DMA_send_enabled = 1;
	}
}

/*******************************************************************************
* Function Name  : DMA1_Channel5_IRQHandler
* Description    : This function handles DMA1 Channel 5 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel5_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA1_Channel6_IRQHandler
* Description    : This function handles DMA1 Channel 6 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel6_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA1_Channel7_IRQHandler
* Description    : This function handles DMA1 Channel 7 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel7_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA1_IT_TC7) == SET)
    {
        DMA_ClearITPendingBit(DMA1_IT_TC7);
		USART2_DMA_send_enabled = 1;
	}
}

/*******************************************************************************
* Function Name  : ADC1_2_IRQHandler
* Description    : This function handles ADC1 and ADC2 global interrupts requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ADC1_2_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : USB_HP_CAN_TX_IRQHandler
* Description    : This function handles USB High Priority or CAN TX interrupts 
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USB_HP_CAN_TX_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : USB_LP_CAN_RX0_IRQHandler
* Description    : This function handles USB Low Priority or CAN RX0 interrupts 
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USB_LP_CAN_RX0_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : CAN_RX1_IRQHandler
* Description    : This function handles CAN RX1 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CAN_RX1_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : CAN_SCE_IRQHandler
* Description    : This function handles CAN SCE interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CAN_SCE_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : EXTI9_5_IRQHandler
* Description    : This function handles External lines 9 to 5 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI9_5_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM1_BRK_IRQHandler
* Description    : This function handles TIM1 Break interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM1_BRK_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM1_UP_IRQHandler
* Description    : This function handles TIM1 overflow and update interrupt 
*                  request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM1_UP_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM1_TRG_COM_IRQHandler
* Description    : This function handles TIM1 Trigger and commutation interrupts 
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM1_TRG_COM_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM1_CC_IRQHandler
* Description    : This function handles TIM1 capture compare interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM1_CC_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM2_IRQHandler
* Description    : This function handles TIM2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
******************************************************************************/
void TIM2_IRQHandler(void)
{
	u16	portc;
/*
////	if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)
	{
////		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
//		capture = TIM_GetCapture1(TIM2);
//		TIM_SetCompare1(TIM2, capture + CCR1_Val);
////		TIM_SetCompare1(TIM2, TIM_GetCapture1(TIM2) + CCR1_Val);
*/

	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);


		// Pin PC.06 toggling with frequency = 73.24 Hz
		//GPIO_WriteBit(GPIOC, GPIO_Pin_4|GPIO_Pin_1, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_4|GPIO_Pin_1)));
		////GPIO_WriteBit(GPIOC, GPIO_Pin_2, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_2))); //GBS-350
		//	GPIO_WriteBit(GPIOC, GPIO_Pin_1, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_1)));
////    	GPIO_WriteBit(GPIOB, GPIO_Pin_15, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_15)));
   	

//		TIM2_1ms++;
//		if(COUNT_1SEC <= TIM2_1ms)
		{
			/*********** segment data and digit masking ***********************/
			// c15 c14 c13 c12    c11 c10 c9 c8   c7 c6 c5 c4   c3 c2 c1 c0 
			//                            l2  l1 dot g   f  e    d  c  b  a  
			
			portc = GPIO_ReadOutputData(GPIOC) & 0xFC00;	//segment and DIGIT  GPIO_Pin_8
			GPIO_Write(GPIOC, portc);	//clear display data
	    // 15 14 13 12    11 10 9 8    7 6 5 4   3 2 1 0
			GPIO_Write(GPIOA, GPIO_ReadOutputData(GPIOA) & 0x07F0); //OFF DIGIT     0,1,2,3     PIN_11 ~ PIN_15

      // 15 14 13 12    11 10 9 8    7 6 5 4   3 2 1 0
			GPIO_Write(GPIOB, GPIO_ReadOutputData(GPIOB) & 0xFFC7); //OFF DIGIT     3,4,5

     /*********** segment data and digit masking ***********************/




///		    GPIO_ResetBits(GPIOC, GPIO_Pin_8);	 //  RESET
		  	//GPIO_WriteBit(GPIOA, GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15)));

			if(guDelayMs) guDelayMs--;	//use delay only


			if(KeyCheckTimer) --KeyCheckTimer;

		//	stable_counter++;// for usart 2 sendint interval
		
		// GPIO_WriteBit(GPIOA, GPIO_Pin_5, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_5))); 
		 sending_interval_counter++;
		//// key_timer++;
		 hold_key_timer++;
		 start_key_timer++;
	   save_key_timer++;
			//if(stable_count_flag) stable_counter++;

			if(ref_cal_mode_check_timer) --ref_cal_mode_check_timer;
			
			if(Enter_flag) Enter_timer++;
			
			if(guStableDelayTimer) guStableDelayTimer--;
//			guStableDelayTimer++;

			if(Serial_Timer) Serial_Timer--;
			//RS422, 485 TX ENABLE
			if(guRS422485TxEnableTimer)
			{	guRS422485TxEnableTimer--;
				if(!guRS422485TxEnableTimer)
				{	USART3TX_disable();
					USART3_DMA_send_enabled = 1;
				}
			}
			if(guCommLedOnTimer) guCommLedOnTimer--;
			if(guCurrentLoopSendTimer) guCurrentLoopSendTimer--;

			if(AverageHoldTimerFlag) AverageHoldTimer++;
			if(HoldFlag) HoldStateTimer++;
	
   	   Fnd_rotation++;
			if(Fnd_rotation>13)
			 Fnd_rotation=0; 
		   
			switch(Fnd_rotation)
			{											   
			  case 0:
	             GPIO_Write(GPIOC,portc |Display_buffer[0]); // data writhing   
               GPIO_SetBits(GPIOA, GPIO_Pin_11);
      	break;
         case 1:
	             GPIO_Write(GPIOC,portc |Display_buffer[1]); // data writhing   
				  GPIO_SetBits(GPIOA, GPIO_Pin_12);
      	 break;
        case 2:
	             GPIO_Write(GPIOC,portc |Display_buffer[2]); // data writhing
               GPIO_SetBits(GPIOA, GPIO_Pin_13);				
      	 break;
        case 3:
	             GPIO_Write(GPIOC,portc |Display_buffer[3]); // data writhing
               GPIO_SetBits(GPIOA, GPIO_Pin_14);				
      	 break;
        case 4:
	             GPIO_Write(GPIOC,portc |Display_buffer[4]); // data writhing
               GPIO_SetBits(GPIOA, GPIO_Pin_15);				
      	 break;
              
        case 5:
	             GPIO_Write(GPIOC,portc |Display_buffer[5]); // data writhing   
               GPIO_SetBits(GPIOA, GPIO_Pin_0);	         // DIGIT 6	
               st_loc=GPIO_ReadInputData(GPIOA);
				        st_loc>>=4;  // 0000 1111 
                st_loc&=0x07;
               switch(st_loc)
								{
                  case 1:
										   st_value=1;
									     break;
                  case 2:
										   st_value=2;
									     break;
                  case 4:
										   st_value=3;
									     break;
									default: 
									 break;
 	           	 }
        break;
         case 6:
	             GPIO_Write(GPIOC,portc |Display_buffer[6]); // data writhing   
               GPIO_SetBits(GPIOA, GPIO_Pin_1);	         // DIGIT 7	
              
 				        st_loc_2=GPIO_ReadInputData(GPIOA);
				        st_loc_2>>=4;  // 0000 1111 
                st_loc_2&=0x07;
              switch(st_loc_2)
								{
                  case 1:
										   st_value=4;
									     break;
                  case 2:
										   st_value=5;
									     break;
                  case 4:
										   st_value=6;
									     break;
									default: 
									break;
								}									
				       
		  	 break;
        case 7:
	             GPIO_Write(GPIOC,portc |Display_buffer[7]); // data writhing   
               GPIO_SetBits(GPIOA, GPIO_Pin_2);	         // DIGIT 8		
               st_loc_3=GPIO_ReadInputData(GPIOA);
				        st_loc_3>>=4;  // 0000 1111 
                st_loc_3&=0x07;
                switch(st_loc_3)
								{
                  case 1:
										   st_value=7;
									     break;
                  case 2:
										   st_value=8;
									     break;
                  case 4:
										   st_value=9;
									     break;
									default: 
									break;
            		}									
				
				break;
        case 8:
	             GPIO_Write(GPIOC,portc |Display_buffer[8]); // data writhing   
               GPIO_SetBits(GPIOA, GPIO_Pin_3);	
				       st_loc_4=GPIO_ReadInputData(GPIOA);
				        st_loc_4>>=4;  // 0000 1111 
                st_loc_4&=0x07;
                switch(st_loc_4)
								{
                  case 1:
										   st_value=10;
									     break;
                  case 2:
										   st_value=11;
									     break;
                  case 4:
										   st_value=12;
									     break;
									default: 
									break;
 
								}									

  			break;
        case 9:
	             GPIO_Write(GPIOC,portc |Display_buffer[9]); // data writhing   
               GPIO_SetBits(GPIOB, GPIO_Pin_3);	         // DIGIT 10
				          st_loc_5=GPIO_ReadInputData(GPIOA);
				        st_loc_5>>=4;  // 0000 1111 
                st_loc_5&=0x07;
                switch(st_loc_5)
								{
                  case 1:
										   st_value=13;
									     break;
                  case 2:
										   st_value=14;
									     break;
                  case 4:
										   st_value=15;
									     break;
									default: 
									break;
 
								}									

      	 break;
        
				case 10:
	             GPIO_Write(GPIOC,portc |Display_buffer[10]); // data writhing   
               GPIO_SetBits(GPIOB, GPIO_Pin_4);	         // DIGIT 11				
	    	 break;
        case 11:
	             GPIO_Write(GPIOC,portc |Display_buffer[11]); // data writhing   
               GPIO_SetBits(GPIOB, GPIO_Pin_5);	         // DIGIT 12				
	    	 break;
        
        case 12:
	             GPIO_Write(GPIOC,portc |Display_buffer[12]); // data writhing   
               GPIO_SetBits(GPIOC, GPIO_Pin_8);	         // LED 1ST ARRAY				
	    	 break;
        case 13:
	             GPIO_Write(GPIOC,portc |Display_buffer[13]); // data writhing   
               GPIO_SetBits(GPIOC, GPIO_Pin_9);	         // LED 2ND ARRAY				
	    	 break;
        

				
				 default: break;
		}
	
    if(!st_loc &&!st_loc_2&&!st_loc_3&&!st_loc_4&&!st_loc_5)// key	
			  st_value=0;			
	 

			
	    /*
		   	Fnd_rotation++;
			if(Fnd_rotation>4)
			Fnd_rotation=0; 
		   
			switch(Fnd_rotation)
			{											   
			  case 0:
	             GPIO_Write(GPIOG,Display_buffer[0]); // data writhing    
	             GPIO_ResetBits(GPIOF, GPIO_Pin_11);	 //  RESET HIGH
	             GPIO_ResetBits(GPIOF, GPIO_Pin_12);	 //  RESET HIGH
	             GPIO_ResetBits(GPIOF, GPIO_Pin_13);	 //  RESET HIGH
	             GPIO_ResetBits(GPIOF, GPIO_Pin_14);	 //  RESET HIGH
	             GPIO_SetBits(GPIOF, GPIO_Pin_15);	 //  RESET HIGH
				 break;
        	  case 1:
	             GPIO_Write(GPIOG,Display_buffer[1]); // data writhing    
	             GPIO_ResetBits(GPIOF, GPIO_Pin_11);	 //  RESET HIGH
	             GPIO_ResetBits(GPIOF, GPIO_Pin_12);	 //  RESET HIGH
	             GPIO_ResetBits(GPIOF, GPIO_Pin_13);	 //  RESET HIGH
	             GPIO_SetBits(GPIOF, GPIO_Pin_14);	 //  RESET HIGH
	             GPIO_ResetBits(GPIOF, GPIO_Pin_15);	 //  RESET HIGH
				 break;
		      case 2:
	             GPIO_Write(GPIOG,Display_buffer[2]); // data writhing    
	             GPIO_ResetBits(GPIOF, GPIO_Pin_11);	 //  RESET HIGH
	             GPIO_ResetBits(GPIOF, GPIO_Pin_12);	 //  RESET HIGH
	             GPIO_SetBits(GPIOF, GPIO_Pin_13);	 //  RESET HIGH
	             GPIO_ResetBits(GPIOF, GPIO_Pin_14);	 //  RESET HIGH
	             GPIO_ResetBits(GPIOF, GPIO_Pin_15);	 //  RESET HIGH
				 break;
        	  case 3:
	             GPIO_Write(GPIOG,Display_buffer[3]); // data writhing    
	             GPIO_ResetBits(GPIOF, GPIO_Pin_11);	 //  RESET HIGH
	             GPIO_SetBits(GPIOF, GPIO_Pin_12);	 //  RESET HIGH
	             GPIO_ResetBits(GPIOF, GPIO_Pin_13);	 //  RESET HIGH
	             GPIO_ResetBits(GPIOF, GPIO_Pin_14);	 //  RESET HIGH
	             GPIO_ResetBits(GPIOF, GPIO_Pin_15);	 //  RESET HIGH
				 break;
			  case 4:
	             GPIO_Write(GPIOG,Display_buffer[4]); // data writhing    
	             GPIO_SetBits(GPIOF, GPIO_Pin_11);	 //  RESET HIGH
	             GPIO_ResetBits(GPIOF, GPIO_Pin_12);	 //  RESET HIGH
	             GPIO_ResetBits(GPIOF, GPIO_Pin_13);	 //  RESET HIGH
	             GPIO_ResetBits(GPIOF, GPIO_Pin_14);	 //  RESET HIGH
	             GPIO_ResetBits(GPIOF, GPIO_Pin_15);	 //  RESET HIGH
				 break;
				 default: break;
		}
			 */

		} //if(156<TIM2_1ms)
	}
}

/*******************************************************************************
* Function Name  : TIM3_IRQHandler
* Description    : This function handles TIM3 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM3_IRQHandler(void)	  // 1 msec  timer
{
}

/*******************************************************************************
* Function Name  : TIM4_IRQHandler
* Description    : This function handles TIM4 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM4_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : I2C1_EV_IRQHandler
* Description    : This function handles I2C1 Event interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C1_EV_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : I2C1_ER_IRQHandler
* Description    : This function handles I2C1 Error interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C1_ER_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : I2C2_EV_IRQHandler
* Description    : This function handles I2C2 Event interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C2_EV_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : I2C2_ER_IRQHandler
* Description    : This function handles I2C2 Error interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C2_ER_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : SPI1_IRQHandler
* Description    : This function handles SPI1 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI1_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : SPI2_IRQHandler
* Description    : This function handles SPI2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI2_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : USART1_IRQHandler
* Description    : This function handles USART1 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART1_IRQHandler(void)
{
	vu16 buffer,i;

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)//  수신되었을때  
	{
		// Read one byte from the receive data register 
		buffer=USART_ReceiveData(USART1); //수신된 값을 buffer에 넣는다 .
		if(buffer!=0x1ff)//쓰레기 값이 아니고 정상적이면 
		{
			Usart1_Interrupt_flag=1;
			USARTx_Interrupt_flag = 1;	//port 1
			guCommLedOnTimer = 15;
		}
	}

/*
	// 이부분이 꼭 있어야 동작이 된다 !! 
	if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
	{  
    	// Write one byte to the transmit data register 
//    	USART_SendData(USART1, TxBuffer1[TxCounter1++]);                    
    	USART_SendData(USART1, '1');                    


//    	if(TxCounter1 == NbrOfDataToTransfer1)
    	{
      		// Disable the USART1 Transmit interrupt 
//      		USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
    	}
	}
*/
}

/*******************************************************************************
* Function Name  : USART2_IRQHandler
* Description    : This function handles USART2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART2_IRQHandler(void)// zigbee serial com interrupt
{
	u16 rxd;

	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		rxd = USART_ReceiveData(USART2);

		if(rxd != 0x01FF)
		{
			guCommLedOnTimer = 15;
			if(rxd == '@') //STX
			{
				RxCounter2 = 0;
			}
			else if(rxd == 0x0a) //ETX	///if(buffer1==0x03 && RxBuffer2[RxCounter2-10]==0X02 )
			{
				if(6 < RxCounter2)
				{
					Usart2_Interrupt_flag = 1;
					USARTx_Interrupt_flag = 2;	//port 2
					for(rxd=0; rxd<RxCounter2; rxd++) Rx2Cmd[rxd] = RxBuffer2[rxd];
					Rx2Cmd[rxd] = rxd;
					Rx2Cmd[rxd+1] = 0;
				}
				//RxCounter2 = 0;
			}

			RxBuffer2[RxCounter2++] = rxd;
		}  //if(buffer1!=0x1FF)  
		
		if(30 < RxCounter2) RxCounter2 = 0;
	}
}

/*******************************************************************************
* Function Name  : USART3_IRQHandler
* Description    : This function handles USART3 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART3_IRQHandler(void)
{
	u16 rxd;

	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		rxd = USART_ReceiveData(USART3);

		if(rxd != 0x01FF)
		{
			guCommLedOnTimer = 15;

			if(rxd == 0x02) //STX
			{
				RxCounter3 = 0;
			}
			else if(rxd == 0x03) //ETX	///if(buffer1==0x03 && RxBuffer2[RxCounter2-10]==0X02 )
			{
				if(6 < RxCounter3)
				{
					Usart3_Interrupt_flag = 3;
					USARTx_Interrupt_flag = 3;	//port 3
					for(rxd=0; rxd<RxCounter3; rxd++) Rx3Cmd[rxd] = RxBuffer3[rxd];
					Rx3Cmd[rxd] = rxd;
					Rx3Cmd[rxd+1] = 0;
				}
				//RxCounter3 = 0;
			}

			RxBuffer3[RxCounter3++] = rxd;
		}  //if(buffer1!=0x1FF)  
		
		if(30 < RxCounter3) RxCounter3 = 0;
	}
}

/*******************************************************************************
* Function Name  : USART3_IRQHandler
* Description    : This function handles USART3 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UART4_IRQHandler(void)
{
	u16 rxd;

	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
	{
		rxd = USART_ReceiveData(UART4);

		if(rxd != 0x01FF) // 정상적인 데이타면
		{
			RxBuffer4[RxCounter4++] = rxd; //20180410
			//Usart3_Interrupt_flag = 3;
			//guCommLedOnTimer = 15;

			if(rxd == '*') //STX
			{
				RxCounter4 = 0;
			}
			else if(rxd == '&') //ETX	///if(buffer1==0x03 && RxBuffer2[RxCounter2-10]==0X02 )
			{
				//Usart3_Interrupt_flag = 3;
				
				if(6 < RxCounter4)
				{
					Usart4_Interrupt_flag = 1;
					Usart4_Interrupt_flag_test=1;
			//		USARTx_Interrupt_flag = 3;	//port 3
			//		for(rxd=0; rxd<RxCounter3; rxd++) Rx3Cmd[rxd] = RxBuffer3[rxd];
			//		Rx3Cmd[rxd] = rxd;
		//			Rx3Cmd[rxd+1] = 0;
				}
				
				//RxCounter3 = 0;
			}

			//RxBuffer4[RxCounter4++] = rxd; //20180410
		}  //if(buffer1!=0x1FF)  
		
		if(200 < RxCounter4) RxCounter4 = 0;
	}
}




/*******************************************************************************
* Function Name  : EXTI15_10_IRQHandler
* Description    : This function handles External lines 15 to 10 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI15_10_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : RTCAlarm_IRQHandler
* Description    : This function handles RTC Alarm interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RTCAlarm_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : USBWakeUp_IRQHandler
* Description    : This function handles USB WakeUp interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USBWakeUp_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM8_BRK_IRQHandler
* Description    : This function handles TIM8 Break interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM8_BRK_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM8_UP_IRQHandler
* Description    : This function handles TIM8 overflow and update interrupt 
*                  request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM8_UP_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM8_TRG_COM_IRQHandler
* Description    : This function handles TIM8 Trigger and commutation interrupts 
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM8_TRG_COM_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM8_CC_IRQHandler
* Description    : This function handles TIM8 capture compare interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM8_CC_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : ADC3_IRQHandler
* Description    : This function handles ADC3 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ADC3_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : FSMC_IRQHandler
* Description    : This function handles FSMC global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void FSMC_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : SDIO_IRQHandler
* Description    : This function handles SDIO global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SDIO_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM5_IRQHandler
* Description    : This function handles TIM5 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM5_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : SPI3_IRQHandler
* Description    : This function handles SPI3 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI3_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : UART4_IRQHandler
* Description    : This function handles UART4 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void UART5_IRQHandler(void)
{
}


/*******************************************************************************
* Function Name  : TIM6_IRQHandler
* Description    : This function handles TIM6 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM6_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM7_IRQHandler
* Description    : This function handles TIM7 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM7_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA2_Channel1_IRQHandler
* Description    : This function handles DMA2 Channel 1 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA2_Channel1_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA2_Channel2_IRQHandler
* Description    : This function handles DMA2 Channel 2 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA2_Channel2_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA2_Channel3_IRQHandler
* Description    : This function handles DMA2 Channel 3 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA2_Channel3_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA2_Channel4_5_IRQHandler
* Description    : This function handles DMA2 Channel 4 and DMA2 Channel 5
*                  interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA2_Channel4_5_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA2_IT_TC5) == SET)
    {
        DMA_ClearITPendingBit(DMA2_IT_TC5);

		UART4_DMA_send_enabled = 1;
	}
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
