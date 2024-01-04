#include "RTC.h"
#include "stdint.h"
#include "stdbool.h"


/*Pick RTC Alarm Value beeteewn 10 - 100000ms*/
void ConfigRTC(uint16_t Prescaler, uint16_t ValALRL ) /*LSI 40000Hz - to get 1s */
{

		while ( (RTC->CRL & RTC_CRL_RSF) == 0);
		while ( (RTC->CRL & RTC_CRL_RTOFF) ==0);
		RTC->CRL |= RTC_CRL_CNF; /*Disable RTC Write Protection*/

		RTC->CNTH = (0 >> 16U);
		RTC->CNTL = (0 & RTC_CNTL_RTC_CNT); /*Beginning values...*/
		RTC->ALRH = (0 >> 16U);
		RTC->ALRL = (ValALRL & RTC_ALRL_RTC_ALR); /*When obtain this value generate alarm */
		RTC->CRL &= ~(RTC_CRL_SECF | RTC_CRL_OWF); /* Clear Second and overflow flags */
		RTC->CRH |= RTC_CRH_ALRIE; /* Configure the Alarm interrupt */
		RTC->CRL &= ~RTC_CRL_ALRF;

		RTC->PRLL = Prescaler-1; /*Prescaler*/
		
		RTC->CRL &= ~RTC_CRL_CNF; /*Enable RTC Write Protection*/
		while ( (RTC->CRL & RTC_CRL_RTOFF) ==0); /*Wait for Synchro */

		/* RTC Alarm Interrupt Configuration: EXTI configuration */
		EXTI->IMR |= EXTI_IMR_MR17;
		EXTI->RTSR |= EXTI_IMR_MR17;

		/*Enable RTC Interruppt*/
		NVIC_ClearPendingIRQ(RTC_IRQn);
		NVIC_EnableIRQ(RTC_IRQn);

		NVIC_ClearPendingIRQ(RTC_Alarm_IRQn);
		NVIC_EnableIRQ(RTC_Alarm_IRQn);

		/* Clear all related wakeup flags */
		/*Imporatant info: clear this flag here may do problems!!!!!!*/
		PWR->CR |= PWR_CSR_WUF << 2; /* Clear DeepSleep flag (Set because cleared by HW) */

}

bool RTC_AlarmFlag;
/******************************************************************************************************************/
void RTC_Alarm_IRQHandler(void)
{
	EXTI->PR |= EXTI_RTSR_TR18; /*Cleared when set..*/
	RTC_AlarmFlag = true;
	
}
/******************************************************************************************************************/
void RTC_IRQHandler(void) /*Re_Enable RTC*/
{
	if ( RTC->CRL & RTC_CRL_ALRF )
	{
		RTC_AlarmFlag = true;
		while ( (RTC->CRL & RTC_CRL_RTOFF) == 0 );
		RTC->CRL |= RTC_CRL_CNF;
		RTC->CRL &= ~RTC_CRL_ALRF;
		RTC->CNTH = (0 >> 16U);
		RTC->CNTL = (0 & RTC_CNTL_RTC_CNT); /*Beginning values...*/
		RTC->ALRH = (0 >> 16U);
		RTC->ALRL = (1 & RTC_ALRL_RTC_ALR); /*When obtain this value generate alarm */
		RTC->CRL &= ~RTC_CRL_CNF;
		while ( (RTC->CRL & RTC_CRL_RTOFF) == 0 );
	}
	if ( (RTC->CRL & RTC_CRL_SECF) == 1 )
	{
		while ( (RTC->CRL & RTC_CRL_RTOFF) == 0 );
		RTC->CRL &=  ~RTC_CRL_SECF;
	}
}