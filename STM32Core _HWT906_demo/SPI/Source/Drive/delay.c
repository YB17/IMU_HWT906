#include "stm32f10x.h"
//#include "delay.h"

unsigned long ulTickms = 0;
void SysTick_Init(void)
{
	SysTick->LOAD = SystemCoreClock/8000-1;
	SysTick->VAL = 0;
	SysTick->CTRL = 0x03;//SysTick??
}

void delay_ms(unsigned long nTime)
{
    ulTickms = 0;
    while(ulTickms < nTime);
}

void SysTick_Handler(void)
{
	ulTickms++;
}
