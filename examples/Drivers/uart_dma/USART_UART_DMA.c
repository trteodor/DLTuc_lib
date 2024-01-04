#include "USART_UART_DMA.h"

#include <stddef.h>
#include "main.h"

/*
* Legacy old driver solution...
* Let's leave the driver as is for now
*/
UART_DMA_Handle_Td TUART2;

UART_DMA_Handle_Td *UART2_Init115200(void (*TransmitEndCallback)(void), void (*ReceiveEndCallback)(uint16_t Size) )
{
	/*Config UART For Logs */
	  	//Config UART Pins
	GPIO_Pin_Cfg(GPIOA,Px2, gpio_mode_alternate_PP_10MHz);
	GPIO_Pin_Cfg(GPIOA,Px3, gpio_mode_alternate_PP_10MHz);

	//Config Struct for UART ECHO with DMA
	TUART2.Instance=USART2;
	TUART2.UART_DMA_RX_CHANNEL=6;
	TUART2.UART_DMA_TX_CHANNEL=7;
	TUART2.NbofRecData=0;
	TUART2.ubNbDataToTransmit=0;
	TUART2.ubReceptionComplete=1;  //This must be 1 initialized
	TUART2.ubTransmissionComplete=1; //This must be 1 initialized
	TUART2.NbDataToReceive=500;
	TUART2.TransmitEndCallback = TransmitEndCallback;
	TUART2.ReceiveEndCallback = ReceiveEndCallback;

	USARTx_DMA_Config(&TUART2);
	return &TUART2;
}

/*
It's rly simple implementation but here is rly much error's i know how to repair it, but time...
*/

UART_DMA_Handle_Td *UART2HandlerPointer;

void TUART_DMA_Receive(UART_DMA_Handle_Td *USARTX, uint8_t *rxBuf, uint16_t size);
void USARTx_DMA_Config(UART_DMA_Handle_Td *USARTX);
static void USART_Tr_DMA_CNGG(UART_DMA_Handle_Td *USARTX);

static const uint8_t CHANNEL_OFFSET_TAB[] =
{
  (uint8_t)(DMA1_Channel1_BASE - DMA1_BASE),
  (uint8_t)(DMA1_Channel2_BASE - DMA1_BASE),
  (uint8_t)(DMA1_Channel3_BASE - DMA1_BASE),
  (uint8_t)(DMA1_Channel4_BASE - DMA1_BASE),
  (uint8_t)(DMA1_Channel5_BASE - DMA1_BASE),
  (uint8_t)(DMA1_Channel6_BASE - DMA1_BASE),
  (uint8_t)(DMA1_Channel7_BASE - DMA1_BASE)
};
#define DMA1_CH_OFFSET ((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMA1 + CHANNEL_OFFSET_TAB

void USART_Init(UART_DMA_Handle_Td *USARTX)
{
		USARTX->Instance->SR=0;
		USARTX->Instance->DR=0;
		USARTX->Instance->BRR = 279;  //config baud
		USARTX->Instance->CR1 = USART_CR1_UE | USART_CR1_TE | USART_CR1_RE | USART_CR1_IDLEIE ;
		USARTX->Instance->CR3 |= USART_CR3_DMAR | USART_CR3_DMAT;
}
void USARTx_DMA_Config(UART_DMA_Handle_Td *USARTX)
{
		if(USARTX->Instance == USART1)
		{
			RCC->APB2ENR |= RCC_APB2ENR_USART1EN;  //Enable USART1 periph
		}
			if(USARTX->Instance == USART2)
		{
			UART2HandlerPointer=USARTX;
			RCC->APB1ENR |= RCC_APB1ENR_USART2EN;  //Enable USART2 periph
		}
			if(USARTX->Instance == USART3)
		{
			RCC->APB1ENR |= RCC_APB1ENR_USART3EN;  //Enable USART3 periph
		}

		USART_Init((void *)UART2HandlerPointer);
		USARTX->DMA_Tx_Config= DMA_CCR_EN | DMA_CCR_MINC | DMA_CCR_TCIE | DMA_CCR_DIR;
		USARTX->DMA_Rx_Config= DMA_CCR_EN | DMA_CCR_MINC | DMA_CCR_TCIE;

		//DMA Enable
		RCC->AHBENR |= RCC_AHBENR_DMA1EN;

		uint32_t prio;
		prio = NVIC_EncodePriority(PRIGROUP_4G_4S, 3, 0);
		NVIC_SetPriority(DMA1_Channel6_IRQn, prio);

		prio = NVIC_EncodePriority(PRIGROUP_4G_4S, 1, 0);
		NVIC_SetPriority(DMA1_Channel7_IRQn, prio);
		NVIC_EnableIRQ(DMA1_Channel7_IRQn);

		prio = NVIC_EncodePriority(PRIGROUP_4G_4S, 3, 0);
		NVIC_SetPriority(USART2_IRQn, prio);
		NVIC_EnableIRQ(USART2_IRQn);
}

void TUART_DMA_Trasmit(UART_DMA_Handle_Td *USARTX, uint8_t *txBuf,uint8_t Size)
{
	while(USARTX->State!=Ready);

	USARTX->UART_DMA_TX_Buffer=txBuf;
	USARTX->ubNbDataToTransmit=Size;

	USART_Tr_DMA_CNGG(USARTX);
}

void TUART_DMA_Receive(UART_DMA_Handle_Td *USARTX, uint8_t *rxBuf, uint16_t size)
{
	USARTX->NbDataToReceive=size;
	USARTX->UART_DMA_RX_Buffer=rxBuf;

	((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMA1 + CHANNEL_OFFSET_TAB[ USARTX->UART_DMA_RX_CHANNEL - 1U])))->CCR=0;	
	((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMA1 + CHANNEL_OFFSET_TAB[ USARTX->UART_DMA_RX_CHANNEL - 1U])))->CPAR = (uint32_t) &USARTX->Instance->DR;
	((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMA1 + CHANNEL_OFFSET_TAB[ USARTX->UART_DMA_RX_CHANNEL - 1U])))->CMAR =(uint32_t) USARTX->UART_DMA_RX_Buffer;
	((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMA1 + CHANNEL_OFFSET_TAB[ USARTX->UART_DMA_RX_CHANNEL - 1U])))->CNDTR = USARTX->NbDataToReceive;

	MODIFY_REG(((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMA1 + CHANNEL_OFFSET_TAB[USARTX->UART_DMA_RX_CHANNEL - 1U])))->CCR,
             DMA_CCR_DIR | DMA_CCR_MEM2MEM | DMA_CCR_CIRC | DMA_CCR_PINC | DMA_CCR_MINC | DMA_CCR_PSIZE | DMA_CCR_MSIZE | DMA_CCR_PL,
             USARTX->DMA_Rx_Config);
}

static void USART_Tr_DMA_CNGG(UART_DMA_Handle_Td *USARTX)
{
	UART2HandlerPointer->State=Busy;

	((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMA1 + CHANNEL_OFFSET_TAB[ USARTX->UART_DMA_TX_CHANNEL - 1])))->CCR=0;
	((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMA1 + CHANNEL_OFFSET_TAB[ USARTX->UART_DMA_TX_CHANNEL - 1])))->CPAR = (uint32_t)&USARTX->Instance->DR;
	((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMA1 + CHANNEL_OFFSET_TAB[ USARTX->UART_DMA_TX_CHANNEL - 1])))->CMAR = (uint32_t) USARTX->UART_DMA_TX_Buffer;
	((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMA1 + CHANNEL_OFFSET_TAB[ USARTX->UART_DMA_TX_CHANNEL - 1])))->CNDTR = USARTX->ubNbDataToTransmit;

	MODIFY_REG(((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMA1 + CHANNEL_OFFSET_TAB[USARTX->UART_DMA_TX_CHANNEL - 1U])))->CCR,
             DMA_CCR_DIR | DMA_CCR_MEM2MEM | DMA_CCR_CIRC | DMA_CCR_PINC | DMA_CCR_MINC | DMA_CCR_PSIZE | DMA_CCR_MSIZE | DMA_CCR_PL,
             USARTX->DMA_Tx_Config);
}

static  void End_Rec(UART_DMA_Handle_Td *USARTX)
{
	USARTX->NbofRecData = USARTX->NbDataToReceive - ((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMA1 +
											CHANNEL_OFFSET_TAB[ USARTX->UART_DMA_RX_CHANNEL - 1U])))->CNDTR;

	if(NULL != TUART2.ReceiveEndCallback)
	{
		TUART2.ReceiveEndCallback(USARTX->NbofRecData);
	}
}
static void USART_CallBack(UART_DMA_Handle_Td *USARTX)
{
	if ( USARTX->Instance->SR & USART_SR_IDLE)
	{
		__IO uint32_t dreg;
		dreg = USARTX->Instance->SR;
		UART2HandlerPointer->State=Ready;
		(void) dreg; //micro delay
		dreg = USARTX->Instance->DR;
		(void) dreg;	//micro delay
		USARTX->NbofRecData = USARTX->NbDataToReceive - ((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMA1 +
											CHANNEL_OFFSET_TAB[ USARTX->UART_DMA_RX_CHANNEL - 1U])))->CNDTR;

		if(NULL != TUART2.ReceiveEndCallback)
		{
			TUART2.ReceiveEndCallback(USARTX->NbofRecData);
		}
	}
}

void DMA1_Channel7_IRQHandler()   //USART2_TX_DMA_Channel (HARDWARE) /*Transmit END*/
{
	if(DMA1->ISR & DMA_ISR_TCIF7)
	{
		UART2HandlerPointer->State=Ready;
		DMA1->IFCR = DMA_IFCR_CTCIF7 | DMA_IFCR_CHTIF7 | DMA_IFCR_CGIF7; //Clear intr flag
		// In the line above the register reference -modification required for others USART
		if(TUART2.TransmitEndCallback != NULL)
		{
			TUART2.TransmitEndCallback();
		}
	}
}
void DMA1_Channel6_IRQHandler() //USART2_RX_DMA_Channel (HARDWARE)
{
			if(DMA1->ISR & DMA_ISR_TCIF6)
	{
		DMA1->IFCR = DMA_IFCR_CTCIF6 | DMA_IFCR_CHTIF6 | DMA_IFCR_CGIF6;  //Clear intr flag
		// In the line above the register reference - modification required for others USART
		End_Rec(UART2HandlerPointer);
	}
}

void USART2_IRQHandler(void)
{
		USART_CallBack(UART2HandlerPointer);
}

