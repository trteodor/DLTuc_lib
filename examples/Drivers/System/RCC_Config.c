
#include "RCC_Config.h"



void PLL_Config64MHZ(void)
{
  /*If in startup initialize PLL, PLL have be frist deinit
	I commennted initialization in Startup File so here also;
	Below, reset to default */
	 RCC->CR |= (uint32_t)0x00000001;
			RCC->CFGR &= (uint32_t)0xF8FF0000;
			RCC->CR &= (uint32_t)0xFEF6FFFF;
			RCC->CR &= (uint32_t)0xFFFBFFFF;
			RCC->CFGR &= (uint32_t)0xFF80FFFF;
			RCC->CIR = 0x009F0000;

	//Set to 64MHz Correctly! :) 
 RCC->CFGR = RCC_CFGR_PLLMULL16 | RCC_CFGR_PPRE1_2;
 while (!(RCC->CR & RCC_CR_HSIRDY ));
 RCC->CR |=  RCC_CR_PLLON;
 FLASH->ACR |= FLASH_ACR_LATENCY_1;
 while (!(RCC->CR & RCC_CR_PLLRDY));
 RCC->CFGR |= RCC_CFGR_SW_PLL;
 while ( (RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
}


	

