/*
	delay.c
*/

#include "stm32f10x_type.h"
#include "stm32f10x_lib.h"

extern vu16	guDelayMs;	//use delay only
						 
vu32 multiplier;

 
/*******************************************************************************
* Function Name  : TimingDelay_Decrement
* Description    : Decrements the TimingDelay variable.
* Input          : None
* Output         : TimingDelay
* Return         : None
*******************************************************************************/
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00) TimingDelay--;
}


void TM_Delay_Init(void) {

//	char str[50];

    RCC_ClocksTypeDef RCC_Clocks;
    
    /* Get system clocks */
    RCC_GetClocksFreq(&RCC_Clocks);
    
    /* While loop takes 4 cycles */
    /* For 1 us delay, we need to divide with 4M */
//org   multiplier = RCC_Clocks.HCLK_Frequency / 4000000;	//late 
//	multiplier = RCC_Clocks.HCLK_Frequency / 14400000;	//faster 12sec per 60sec
	multiplier = RCC_Clocks.HCLK_Frequency / 12000000;	//
	
/*
	for(;;) {
		sprintf(str, "HCLK_Frequency:%ld", RCC_Clocks.HCLK_Frequency);
		USART2_puts(str);
	    multiplier = RCC_Clocks.HCLK_Frequency / 4000000;
		sprintf(str, "multipliex:%ld", multiplier);
		USART2_puts(str);
	}
*/
}
 
void TM_DelayMicros(u32 micros) {
    /* Multiply micros with multipler */
    /* Substract 10 */
    micros = micros * multiplier - 10;
    /* 4 cycles for one loop */
    while (micros--);
}
 
void TM_DelayMillis(u32 millis) {
    /* Multiply millis with multipler */
    /* Substract 10 */
   // millis = 1000 * millis * multiplier - 10;
     millis = 840 * millis * multiplier - 10;
   
   
    /* 4 cycles for one loop */
    while (millis--);
}
 
#ifdef ______example_tm_delay___
int main(void) {
    /* Initialize system */
    SystemInit();
    
    /* Initialize delay */
    TM_Delay_Init();
    
    /* Initialize onboard leds */
    TM_DISCO_LedInit();
 
    while (1) {
        /* Toggle leds */
        TM_DISCO_LedToggle(LED_GREEN);
        
        /* Delay 1000 micros */
        TM_DelayMicros(1000);
    }
#endif	//#ifdef ______example_tm_delay___


/*******************************************************************************
* Function Name  : Delay_us
* Description    : Inserts a delay time.
* Input          : nTime: specifies the delay time length, in milliseconds.
* Output         : None
* Return         : None
*******************************************************************************/
void Delay_us(u32 nTime)
{

//	TM_DelayMicros(nTime);

  // Enable the SysTick Counter
  SysTick_CounterCmd(SysTick_Counter_Enable);
  
  TimingDelay = nTime;

  while(TimingDelay != 0);

  // Disable SysTick Counter
  SysTick_CounterCmd(SysTick_Counter_Disable);
  // Clear SysTick Counter
  SysTick_CounterCmd(SysTick_Counter_Clear);
}

/*******************************************************************************
* Function Name  : Delay_us
* Description    : Inserts a delay time.
* Input          : nTime: specifies the delay time length, in milliseconds.
* Output         : None
* Return         : None
*******************************************************************************/
void Delay_ms(u32 nTime)
{
	TM_DelayMillis(nTime*2);

//	guDelayMs = nTime;
//	while(guDelayMs);

#ifdef ___systicck__
	
  // Enable the SysTick Counter
  SysTick_CounterCmd(SysTick_Counter_Enable);
  
  TimingDelay = (nTime*2000);

  while(TimingDelay != 0);

  // Disable SysTick Counter
  SysTick_CounterCmd(SysTick_Counter_Disable);
  // Clear SysTick Counter
  SysTick_CounterCmd(SysTick_Counter_Clear);
#endif	
}

/* end of file */
