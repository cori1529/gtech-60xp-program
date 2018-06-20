/*
	usart.h
*/


#ifndef	__USART_H
#define	__USART_H

#include "stm32f10x_usart.h"

#define	__CURRENT_LOOP_ENABLED__	1


void USARTx_setting(USART_TypeDef* USARTx, long baudrate, char parity);
void USARTx_Initial(void);
void USAR4_115200(void);

int USARTx_print_DMA(DMA_Channel_TypeDef *txDMAChannel, char *txt);

void USART1_putchar(char ch);
void USART1_puts(char *str);
void USART1_print(char *str);
int USART1_puts_DMA(char *txt);
int USART1_TC(void);

void USART2_putchar(char ch);
void USART2_puts(char *str);
//void USART2_puts_ble(char *str);

void USART2_print(char *str);
int USART2_print_DMA(char *str);
int USART2_puts_DMA(char *txt);
int USART2_TC(void);

void USART3_putchar(char ch);
void USART3_puts(char *str);
void USART3_print(char *str);
int USART3_print_DMA(char *str);
int USART3_puts_DMA(char *txt);
int USART3_TC(void);

void USART_2and3_putchar(char ch);
void USART_2and3_puts( char *str);
void USART_2and3_print(char *str);

void UART4_putchar(char ch);
void UART4_puts(char *str);
void UART4_print(char *str);
int USART4_puts_DMA(char *txt);
int USART4_TC(void);

void USARTx_putchar(USART_TypeDef* USARTx, char ch);
void USARTx_puts(USART_TypeDef* USARTx, char *str);
void USARTx_print(USART_TypeDef* USARTx, char *str);

void USART3TX_disable(void);


#endif

/* end of file */
