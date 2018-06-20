/*
	dma.c

 * USART DMA setting on STM32
 * USART1 Tx --> DMA Channel 4
 * USART1 Rx --> DMA Channel 5
 * USART2 Tx --> DMA Channel 7
 * USART2 Rx --> DMA Channel 6
 * USART3 Tx --> DMA Channel 2
 * USART3 Rx --> DMA Channel 3
 * UART4 Tx -->  DMA2 Channel 5
 * UART4 Rx -->  DMA2 Channel 3
*/

#include "stm32f10x_type.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_nvic.h"
#include "stm32f10x_it.h"


#include "var.h"

#define TxBufferSize1   (countof(TxBuffer1) - 1)
#define countof(a)   (sizeof(a) / sizeof(*(a)))

u8 TxBuffer1[] = "USART DMA Interrupt: USARTy -> USARTz using DMA Tx and Rx Flag";
u8 RxBuffer1[TxBufferSize1];
u8 NbrOfDataToRead = TxBufferSize1;

void DMA_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;

	// DMA clock enable
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
	
	// USARTy_Tx_DMA_Channel (triggered by USARTy Tx event) Config
	DMA_DeInit(DMA1_Channel4);    
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART1->DR;
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)TxBuf1;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = TxBufferSize1;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel4, &DMA_InitStructure);

	//Enable DMA1 channel IRQ Channel 
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
 	//Enable DMA1 Channe4 Transfer Complete interrupt
	DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);


	// USARTy_Rx_DMA_Channel (triggered by USARTy Rx event) Config
	DMA_DeInit(DMA1_Channel5);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART1->DR;
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)RxBuffer1;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = TxBufferSize1;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;   
	DMA_Init(DMA1_Channel5, &DMA_InitStructure);
	
	//USART2 
	// USARTy_Tx_DMA_Channel (triggered by USARTy Tx event) Config
	DMA_DeInit(DMA1_Channel7);    
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART2->DR;
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)TxBuf2;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = TxBufferSize1;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel7, &DMA_InitStructure);

	// USARTy_Rx_DMA_Channel (triggered by USARTy Rx event) Config
	DMA_DeInit(DMA1_Channel6);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART2->DR;
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)RxBuffer1;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = TxBufferSize1;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;   
	DMA_Init(DMA1_Channel6, &DMA_InitStructure);	
	
	//Enable DMA1 channel IRQ Channel 
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
 	//Enable DMA1 Channe7 Transfer Complete interrupt
	DMA_ITConfig(DMA1_Channel7, DMA_IT_TC, ENABLE);


	//USART3
	// USARTy_Tx_DMA_Channel (triggered by USARTy Tx event) Config
	DMA_DeInit(DMA1_Channel2);    
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART3->DR;
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)TxBuf3;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = TxBufferSize1;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel2, &DMA_InitStructure);

	// USARTy_Rx_DMA_Channel (triggered by USARTy Rx event) Config
	DMA_DeInit(DMA1_Channel3);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART3->DR;
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)RxBuffer1;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = TxBufferSize1;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;   
	DMA_Init(DMA1_Channel3, &DMA_InitStructure);	
		
	//Enable DMA1 channel IRQ Channel 
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
 	//Enable DMA1 Channe2 Transfer Complete interrupt
	DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);


	//USART4
	// USARTy_Tx_DMA_Channel (triggered by USARTy Tx event) Config
	DMA_DeInit(DMA2_Channel5);    
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&UART4->DR;
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)TxBuf4;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = TxBufferSize1;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA2_Channel5, &DMA_InitStructure);

	// USARTy_Rx_DMA_Channel (triggered by USARTy Rx event) Config
	DMA_DeInit(DMA2_Channel3);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&UART4->DR;
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)RxBuffer1;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = TxBufferSize1;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;   
	DMA_Init(DMA2_Channel3, &DMA_InitStructure);
	
		
	//Enable DMA2 channel IRQ Channel 
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Channel4_5_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
 	//Enable DMA2 Channe5 Transfer Complete interrupt
	DMA_ITConfig(DMA2_Channel5, DMA_IT_TC, ENABLE);
}

/* end of file */
