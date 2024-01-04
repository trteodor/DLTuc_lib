#include "main.h"
#include <stddef.h>


volatile uint32_t SoftTimer;
volatile uint32_t SysTime=0;
static void (*StSysTickCallBack)(uint32_t TickCount) = NULL;
void SysTick_Handler(void);
void EnableAccesPwrDomain(void);


void LowLevelSystemInit(LowPowerMode_t LowPowerMode)
{
		/*Frist off all config NVIC */
	NVIC_SetPriorityGrouping(PRIGROUP_4G_4S);


	//Config the PLL
	PLL_Config64MHZ(); /*PLL After stop mode must be reinitialized */

	#ifdef UseSysTick
	ConfigSysTick1ms();
	#endif //UseSysTick

	EnableAccesPwrDomain(); /*...*/
 
	if( PWR->CSR & PWR_CSR_SBF ) /*WakeUp DeepSleep Flag*/
	{
		/* Clear Deep Sleep flag (Set because cleared by HW) */
		PWR->CR |= ((PWR_CSR_SBF) << 2);
		/*Wake up from standby!!!! */
		/*User can send debug log or something.. */
	}

	/*Select Low Power Mode!!!*/
/*********************************************************************/	/*********************************************************************/
if(LowPowerMode == SleepMode)
{
	/**/
	PWR->CR &= ~PWR_CR_LPDS; /*Clean this bit!!!*/
	PWR->CR &= ~PWR_CR_PDDS; /*!< If Set Power when Down Deepsleep */
	SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk; /*In Normal mode Clean*/
}
else if(LowPowerMode == StopMode)
{
	PWR->CR |= PWR_CR_LPDS; /*!< Low-Power Deepsleep (Turn Of Voltage regulator when DeepSleep) */
	PWR->CR &= ~PWR_CR_PDDS; /*(Clean) !< If Set Power when Down Deepsleep */
	SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk; /*Set For StopMode*/
}
else if(LowPowerMode == Standby)
{
	PWR->CR |= PWR_CR_LPDS; /*!< Low-Power Deepsleep (Turn Of Voltage regulator when DeepSleep) */
	PWR->CR |= PWR_CR_PDDS; /*(Set to Standby) !< If Set Power when Down Deepsleep */
	SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk; /*(Set to Standby) Set For StopMode*/
}
/*********************************************************************/	/*********************************************************************/

	#ifdef UseSysTick
		SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk; /*Stop SysTick in Stop Mode SysTick Must be Turn Off*/ 
	#endif

}


/******************************************************************************************************************/
void TIM1_Cnf() 
{	
		RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
		/*Konfiguracja - przerwanie co 3 sekundy gdy APBH1clk = 64MHz */
		TIM1->PSC = 6399;
		TIM1->ARR = 30000;
		TIM1->DIER |= TIM_DIER_UIE;
		TIM1->CR1 |= TIM_CR1_CEN;	

		NVIC_EnableIRQ(TIM1_UP_IRQn);
}

/******************************************************************************************************************/
void ConfigSysTick1ms(void)
{
	uint32_t prio;
	SysTick_Config(64000/8); /*Dzielimy 64mhz na 8 co daje dzielnik 8K*/
	SysTick->CTRL &= ~SysTick_CTRL_CLKSOURCE_Msk; // Aby uzyskac 1kHz dziele taktowanie systicka jeszcze przez 8
	prio = NVIC_EncodePriority(PRIGROUP_4G_4S, 0, 0);
	NVIC_SetPriority(SysTick_IRQn, prio);
}
/******************************************************************************************************************/
void EnableAccesPwrDomain(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_PWREN | RCC_APB1ENR_BKPEN; /*Enable Acces to PWR and BKP domain*/
	PWR->CR  |= PWR_CR_DBP;
	/*Wewnetrzny oscylator wlacz*/
	RCC->CSR |= RCC_CSR_LSION;

	while( (RCC->CSR & RCC_CSR_LSIRDY) == 0 ); /*Wait for LSI Ready */

	RCC->BDCR |= RCC_BDCR_RTCEN | RCC_BDCR_RTCSEL_LSI;
}
/******************************************************************************************************************/
void SysTick_Handler(void)
{
	SysTime++;
	if(StSysTickCallBack != NULL)
	{
		StSysTickCallBack(SysTime);
	}
}

uint32_t GetSysTime(void)
{
	return SysTime;
}


void RegisterSysTickCallBack(void(*SysTickCallBack)(uint32_t TickCount))
{
	StSysTickCallBack = SysTickCallBack;
}

void DelayMs(uint32_t Ticks)
{
	volatile uint32_t StartTicks = SysTime;

	while( (SysTime - StartTicks) < Ticks) {
		/*Waiting*/
	}
}



void TIM1_UP_IRQHandler(void){
 if (TIM1->SR & TIM_SR_UIF){
TIM1->SR = ~TIM_SR_UIF;
	 SoftTimer++;
	//  tooglePIN(GPIOA, Psm5);
 }
}