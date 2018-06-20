/*
	usart.c
	
	https://github.com/FabLabSeoul/WingProject/wiki/STM32-USART-DMA
*/

#include <string.h>

#include "stm32f10x_dma.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_nvic.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"

#include "delay.h"
#include "lcd.h"
#include "set.h"	//eeprom 변수들
#include "usart.h"
#include "var.h"

void USARTx_setting(USART_TypeDef* USARTx, long baudrate, char parity);
int USARTx_print_DMA(DMA_Channel_TypeDef *txDMAChannel, char *txt);

void USART3TX_enable(void);
void USART3TX_disable(void);

void USART1_putchar(char ch);
void USART1_puts(char *str);
void USART1_print(char *str);
int USART1_puts_DMA(char *txt);
int USART1_TC(void);

void USART2_putchar(char ch);
void USART2_puts(char *str);
void USART2_print(char *str);
int USART2_puts_DMA(char *txt);
int USART2_TC(void);

void USART3_putchar(char ch);
void USART3_puts(char *str);
void USART3_print(char *str);
int USART3_puts_DMA(char *txt);
int USART3_TC(void);

void USART_2and3_putchar(char ch);
void USART_2and3_puts( char *str);
void USART_2and3_print(char *str);

void USARTx_putchar(USART_TypeDef* USARTx, char ch);
void USARTx_puts(USART_TypeDef* USARTx, char *str);
void USARTx_print(USART_TypeDef* USARTx, char *str);

void USART4_putchar(char ch);
void USART4_puts(char *str);
void USART4_print(char *str);
int USART4_puts_DMA(char *txt);
int USART4_TC(void);

//const long F_Baudrate[10] = { 115200, 76800, 57600, 38400, 28800, 19200, 14400, 9600, 4800, 2400 };
const long F_Baudrate[10] = { 115200, 76800, 57600, 38400, 28800, 19200, 14400, 9600, 4800, 2400 };
const long CL_Baudrate[5] = {9600, 4800, 2400, 1200, 600 };//for current loop baudrate set


void USARTx_Initial()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	USARTx_setting( USART1, 115200, 0);
	
	
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);

   	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOC, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(GPIOC, &GPIO_InitStructure);	


		
		
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
   USART_Init(UART4, &USART_InitStructure);
	 
	 USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
	 
	 USART_DMACmd(UART4, USART_DMAReq_Tx, ENABLE);
	 
	 USART_Cmd(UART4, ENABLE);
 
}
void USAR4_115200(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);

   	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOC, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(GPIOC, &GPIO_InitStructure);	


		
		
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
   USART_Init(UART4, &USART_InitStructure);
	 
	 USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
	 
	 USART_DMACmd(UART4, USART_DMAReq_Tx, ENABLE);
	 
	 USART_Cmd(UART4, ENABLE);
 
	
	
	
}





void USARTx_setting(USART_TypeDef* USARTx, long baudrate, char parity)
{
//	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	if(USARTx == USART1)
	{
		/* Enable USART1 clocks */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
		
		// Configurtion  USART1 GPIO
		// Configure USART1 Rx (PA.10) as input floating 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		// Configure USART1 Tx (PA.09) as alternate function push-pull 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		// for dma
		// Enable USART1 DMA TX request
		//USART_DMACmd(USARTx, USART_DMAReq_Tx | USART_DMAReq_Rx, ENABLE);
		USART_DMACmd(USARTx, USART_DMAReq_Tx, ENABLE);

		// Enable USART1 DMA TX, RX Channel
		//DMA_Cmd(DMA1_Channel4, ENABLE);
		//DMA_Cmd(DMA1_Channel5, ENABLE);		
	}	
	else if(USARTx == USART2)																					 
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
		
		// Configurtion  USART2 GPIO/
		// Configure USART2 Rx (PA.3) as input floating 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		// Configure USART2 Tx (PA.2) as alternate function push-pull 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(GPIOA, &GPIO_InitStructure);	

		/* Enable the DMA1 Channel2 Interrupt */
/*
		NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
*/
		// for dma
		// Enable USART2 DMA TX request
		//USART_DMACmd(USARTx, USART_DMAReq_Tx | USART_DMAReq_Rx, ENABLE);
		USART_DMACmd(USARTx, USART_DMAReq_Tx, ENABLE);

		// Enable USART2 DMA TX, RX Channel
//		DMA_Cmd(DMA1_Channel7, ENABLE);
//		DMA_Cmd(DMA1_Channel6, ENABLE);	
  
	}
	else if(USARTx == USART3)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

		// Configurtion  USART3 GPIO
		// Configure USART3 Rx (PB.11) as input floating 
		
		//485 TX ENABLE ACTIVE HIGH, 422 ALWAYS HIGH
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12;	//485 TX ENABLE, NORMAL LOW
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOB, &GPIO_InitStructure);

		// for dma
		// Enable USART3 DMA TX request
		//USART_DMACmd(USARTx, USART_DMAReq_Tx | USART_DMAReq_Rx, ENABLE);
		USART_DMACmd(USARTx, USART_DMAReq_Tx, ENABLE);

		// Enable USART3 DMA TX, RX Channel
		//DMA_Cmd(DMA1_Channel2, ENABLE);
		//DMA_Cmd(DMA1_Channel3, ENABLE);		
	}
	else if(USARTx == UART4)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);

		// Configure USART4 Tx (PC.10) as alternate function push-pull 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(GPIOC, &GPIO_InitStructure);

		// for dma
		// Enable USART4 DMA TX request
		//USART_DMACmd(USARTx, USART_DMAReq_Tx | USART_DMAReq_Rx, ENABLE);
		USART_DMACmd(USARTx, USART_DMAReq_Tx, ENABLE);

		// Enable USART4 DMA TX, RX Channel
		//DMA_Cmd(DMA2_Channel5, ENABLE);
		//DMA_Cmd(DMA2_Channel3, ENABLE);		
	}	
	
	else if(USARTx == UART5)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);

   	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOD, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(GPIOC, &GPIO_InitStructure);	


		
		
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
   USART_Init(UART5, &USART_InitStructure);
	 
	 USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
	 USART_Cmd(UART5, ENABLE);
  
		
		
		
 /*

		// for dma
		// Enable USART4 DMA TX request
		//USART_DMACmd(USARTx, USART_DMAReq_Tx | USART_DMAReq_Rx, ENABLE);
		USART_DMACmd(USARTx, USART_DMAReq_Tx, ENABLE);

		// Enable USART4 DMA TX, RX Channel
		DMA_Cmd(DMA2_Channel5, ENABLE);
		DMA_Cmd(DMA2_Channel3, ENABLE);		
 */
}	
	
	
	
	else return;
	
	// Configure USARTx
	USART_DeInit(USARTx);
	USART_InitStructure.USART_BaudRate = baudrate;	//115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// Parity
	switch(parity)
	{
		case 1: 
			//USART_InitStructure.USART_WordLength = USART_WordLength_9b;
			USART_InitStructure.USART_Parity = USART_Parity_Odd; 
			break;
		case 2:
			//USART_InitStructure.USART_WordLength = USART_WordLength_9b;
			USART_InitStructure.USART_Parity = USART_Parity_Even; 
			break;
		default: 
			//USART_InitStructure.USART_WordLength = USART_WordLength_8b;
			USART_InitStructure.USART_Parity = USART_Parity_No; 
			break;
	}	
	
	//Stop bit
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// Hardware flow control disabled (RTS and CTS signals)
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// Receive and transmit enabled
	if(USARTx != UART4) USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	else  USART_InitStructure.USART_Mode = USART_Mode_Tx;
	
/*
	USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
	USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
	USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
	USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;
	USART_ClockInit(USART1, &USART_ClockInitStructure);
*/
	USART_Init(USARTx, &USART_InitStructure);

  	//Enable USARTx Receive and Transmit interrupts 
	if(USARTx != UART4) USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);
	//USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);

	//Enable the USARTx
	USART_Cmd(USARTx, ENABLE);
}


// #define SEND_DELAY 3
void USART1_putchar(char ch)
{
//	int dly = 300;	//baudrate 115200
	int dly = 300*48;	//baudrate 2400

/***
	while((USART_GetFlagStatus(USART1, USART_IT_TXE) == RESET) && --dly);
//	while((USART_GetFlagStatus(USART1, USART_IT_TXE) == RESET));
	USART_SendData(USART1, ch);
//	while((USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) && --dly);
	//Delay_ms(SEND_DELAY);
****/

	while((USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) && --dly);
	USART_SendData(USART1, ch);
	
	guCommLedOnTimer = 15;
}

/*
void USART1_puts(char *str)
{
	while(*str)USART1_putchar(*str++);
	USART1_putchar(0x0d); //Delay_ms(SEND_DELAY);
	USART1_putchar(0x0a); //Delay_ms(SEND_DELAY);
}
  */
void USART1_print(char *str)
{
	while(*str)USART1_putchar(*str++);
}

	 
/*
and section 23.2.17.7 USART_ClearITPendingBit page 558 it can clear pending TC interrupt

and the other option is to write to Tx data register 
"TC flag can be also cleared by
software sequence: a read operation to USART_SR register
(USART_GetFlagStatus()) followed by a write operation to
USART_TDR register (USART_SendData())."

"TC pending bit can be also cleared
by software sequence: a read operation to USART_SR
register (USART_GetITStatus()) followed by a write operation
to USART_TDR register (USART_SendData())."
*/

void USART2_putchar(char ch)
{
//	int dly = 300;	//baudrate 115200
	int dly = 300*48;	//baudrate 2400

////	while((USART_GetFlagStatus(USART2, USART_IT_TXE) == RESET) && --dly);
//	while((USART_GetFlagStatus(USART2, USART_IT_TXE) == RESET));

	while((USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET) && --dly);
	USART_SendData(USART2, ch);
//	while((USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET) && --dly);
 
	 //while((USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET) );
   // Delay_ms(2);

	//Delay_ms(SEND_DELAY);
	guCommLedOnTimer = 15;
}

void USART1_puts(char *str)
{
	while(*str)USART1_putchar(*str++);
	USART1_putchar(0x0d); //Delay_ms(SEND_DELAY);
	USART1_putchar(0x0a); //Delay_ms(SEND_DELAY);
}

  
void USART2_puts(char *str)
{
 //   USART2_putchar(0x02);
	while(*str)USART2_putchar(*str++);
   // USART2_putchar('o'); //Delay_ms(SEND_DELAY);

//	USART2_putchar('k'); //Delay_ms(SEND_DELAY);

	USART2_putchar(0x0d); //Delay_ms(SEND_DELAY);
	USART2_putchar(0x0a); //Delay_ms(SEND_DELAY);
}



void USART2_print(char *str)
{
	while(*str)USART2_putchar(*str++);
}


void USART3_putchar(char ch)
{
//	int dly = 300;	//baudrate 115200
	int dly = 300*48;	//baudrate 2400

	USART3TX_enable();

////	while((USART_GetFlagStatus(USART2, USART_IT_TXE) == RESET) && --dly);
//	while((USART_GetFlagStatus(USART2, USART_IT_TXE) == RESET));

	while((USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET) && --dly);
	USART_SendData(USART3, ch);
//	while((USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET) && --dly);
 
	 //while((USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET) );
   // Delay_ms(2);

	//Delay_ms(SEND_DELAY);

	guCommLedOnTimer = 15;
}

void USART3_puts(char *str)
{
	while(*str)USART3_putchar(*str++);
	USART3_putchar(0x0d); //Delay_ms(SEND_DELAY);
	USART3_putchar(0x0a); //Delay_ms(SEND_DELAY);
}

void USART3_print(char *str)
{
	while(*str)USART3_putchar(*str++);
}


void USARTx_putchar(USART_TypeDef* USARTx, char ch)
{
//	int dly = 300;	//baudrate 115200
	int dly = 300*48;	//baudrate 2400

	while((USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET) && --dly);
	USART_SendData(USARTx, ch);
//	while((USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET) && --dly);
	guCommLedOnTimer = 15;
}

void USARTx_puts(USART_TypeDef* USARTx, char *str)
{
	while(*str) USARTx_putchar(USARTx, *str++);
	USARTx_putchar(USARTx, 0x0d); //Delay_ms(SEND_DELAY);
	USARTx_putchar(USARTx, 0x0a); //Delay_ms(SEND_DELAY);
}

void USARTx_print(USART_TypeDef* USARTx, char *str)
{
	while(*str) USARTx_putchar(USARTx, *str++);
}

void USART3TX_enable()
{
	GPIO_SetBits(GPIOB, GPIO_Pin_12);
	guRS422485TxEnableTimer = 100;	//2016.3.29 6; //about 6 ms
}

void USART3TX_disable()
{
//2016.3.29	GPIO_ResetBits(GPIOB, GPIO_Pin_12);
}

//USART2 AND USART3 동시에 출력 (전송시간에 따른 시간지연을 최소화하기 위함.)
void USART_2and3_putchar(char ch)
{
//	int dly = 300;	//baudrate 115200
	int dly = 300*48;	//baudrate 2400

	USART3TX_enable();
	while((USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET) && --dly);
	USART_SendData(USART2, ch);
	USART_SendData(USART3, ch);
	
	guCommLedOnTimer = 15;
}

void USART_2and3_puts( char *str)
{
	while(*str) USART_2and3_putchar(*str++);
	USART_2and3_putchar(0x0d); //Delay_ms(SEND_DELAY);
	USART_2and3_putchar(0x0a); //Delay_ms(SEND_DELAY);
}

void USART_2and3_print(char *str)
{
	while(*str) USART_2and3_putchar(*str++);
}


void USART_2and3_puts_DMA( char *str)
{
}

void UART4_putchar(char ch)
{
//	int dly = 300;	//baudrate 115200
	int dly = 300*48;	//baudrate 2400

	while((USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET) && --dly);
	USART_SendData(UART4, ch);
	
	guCommLedOnTimer = 15;
}

void UART4_puts(char *str)
{
	while(*str) UART4_putchar(*str++);
}



int USARTx_print_DMA(DMA_Channel_TypeDef *txDMAChannel, char *txt)
{
	int len;

	len = strlen(txt);
    txDMAChannel->CMAR = (u32)txt;
    txDMAChannel->CNDTR = len;
	DMA_Cmd (txDMAChannel, ENABLE);	// Start a DMA transfer! 

	guCommLedOnTimer = 15;	// * len;

	return len;
}

int USART1_DMA_TC()
{
//	if(DMA_GetFlagStatus(DMA1_FLAG_TC4) == RESET) return -1;
	if( !USART1_DMA_send_enabled) return 0;
	else return -1;
}


int USART1_print_DMA(char *txt)
{
	DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);
//	if(DMA_GetFlagStatus(DMA1_FLAG_TC4) != RESET) return -1;
	if ( !USART1_DMA_TC()) return -1;	//error		   
//    DMA_ClearITPendingBit(DMA1_IT_TC4);
//    DMA_ClearITPendingBit(DMA1_IT_TC5);

	strcpy(TxBuf1, txt);
	DMA_Cmd(DMA1_Channel4, DISABLE);
	USART_DMACmd (USART1, USART_DMAReq_Tx, ENABLE); 
	DMA_ClearFlag(DMA1_FLAG_TC4);

    return USARTx_print_DMA(DMA1_Channel4, TxBuf1);
}


int USART1_puts_DMA(char *txt)
{
	int	len;

	DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);
//	if(DMA_GetFlagStatus(DMA1_FLAG_TC4) != RESET) return -1;
	if ( !USART1_DMA_TC()) return -1;	//error

	strcpy(TxBuf1, txt);
	len = strlen(TxBuf1);	
	TxBuf1[len] = 0x0d; TxBuf1[len+1] = 0x0a; TxBuf1[len+2] = 0;

	return USART1_print_DMA(TxBuf1);
}


int USART2_DMA_TC()
{
//	if(DMA_GetFlagStatus(DMA1_FLAG_TC7) == RESET) return -1;
	if( !USART2_DMA_send_enabled) return 0;
	else return -1;
}
	

int USART2_print_DMA(char *txt)
{
	DMA_ITConfig(DMA1_Channel7, DMA_IT_TC, ENABLE);
//	if( !USART2_DMA_send_enabled) return -1;
	if ( !USART2_DMA_TC()) return -1;	//error

	USART2_DMA_send_enabled = 0;
	//DMA_ClearITPendingBit(DMA1_IT_TC7);
	//DMA_ClearITPendingBit(DMA1_IT_TC6);

	strcpy(TxBuf2, txt);
	DMA_Cmd(DMA1_Channel7, DISABLE);
	USART_DMACmd (USART2, USART_DMAReq_Tx, ENABLE); 
	DMA_ClearFlag(DMA1_FLAG_TC7);

    return USARTx_print_DMA(DMA1_Channel7, TxBuf2);
}

int USART2_puts_DMA(char *txt)
{
	int	len;

	DMA_ITConfig(DMA1_Channel7, DMA_IT_TC, ENABLE);
	if ( !USART2_DMA_TC()) return -1;	//error

	strcpy(TxBuf2, txt);
	len = strlen(TxBuf2);
	TxBuf2[len] = 0x0d; TxBuf2[len+1] = 0x0a; TxBuf2[len+2] = 0;

    return USART2_print_DMA(TxBuf2);
}



int USART3_DMA_TC()
{
//	if(DMA_GetFlagStatus(DMA1_FLAG_TC2) == RESET) return -1;
	if( !USART3_DMA_send_enabled) return 0;
	else return -1;
}
	

int USART3_print_DMA(char *txt)
{
	DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);

//	if(DMA_GetFlagStatus(DMA1_FLAG_TC2) != RESET) return -1;
	if( !USART3_DMA_TC()) return -1;

	USART3TX_enable();
		   
//    DMA_ClearITPendingBit(DMA1_IT_TC7);
//    DMA_ClearITPendingBit(DMA1_IT_TC6);

	strcpy(TxBuf3, txt);
	DMA_Cmd(DMA1_Channel2, DISABLE);
	USART_DMACmd (USART3, USART_DMAReq_Tx, ENABLE); 
	DMA_ClearFlag(DMA1_FLAG_TC2);

    return USARTx_print_DMA(DMA1_Channel2, TxBuf3);
}


int USART3_puts_DMA(char *txt)
{
	int	len;
	DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);

//	if(DMA_GetFlagStatus(DMA1_FLAG_TC2) != RESET) return -1;
	if( !USART3_DMA_TC()) return -1;

	strcpy(TxBuf3, txt);
	len = strlen(TxBuf3);
	TxBuf3[len] = 0x0d; TxBuf3[len+1] = 0x0a; TxBuf3[len+2] = 0;

    return USART3_print_DMA(TxBuf3);
}

int UART4_DMA_TC()
{
//	if(DMA_GetFlagStatus(DMA2_FLAG_TC5) != RESET) return -1;
	if( !UART4_DMA_send_enabled) return 0;
	else return -1;
}
	

int USART4_print_DMA(char *txt)
{
	DMA_ITConfig(DMA2_Channel5, DMA_IT_TC, ENABLE);

///	if(DMA_GetFlagStatus(DMA2_FLAG_TC5) != RESET) return -1;
	if( !UART4_DMA_TC()) return -1;

	strcpy(TxBuf4, txt);
	DMA_Cmd(DMA2_Channel5, DISABLE);
	USART_DMACmd (UART4, USART_DMAReq_Tx, ENABLE); 
	DMA_ClearFlag(DMA2_FLAG_TC5);

    return USARTx_print_DMA(DMA2_Channel5, TxBuf4);
}

int USART4_puts_DMA(char *txt)
{
	int	len;

	DMA_ITConfig(DMA2_Channel5, DMA_IT_TC, ENABLE);

///	if(DMA_GetFlagStatus(DMA2_FLAG_TC5) != RESET) return -1;
	if( !UART4_DMA_TC()) return -1;

	strcpy(TxBuf4, txt);
	len = strlen(TxBuf4);
	TxBuf4[len] = 0x0d; TxBuf4[len+1] = 0x0a; TxBuf4[len+2] = 0;

    return USART4_print_DMA(TxBuf4);
}



/* end of file */
