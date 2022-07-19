#include "PrintToBuf.h"
#include "USART_UART_DMA.h"

extern UART_DMA_Handle_Td *UART2HandlerPointer;

volatile uint32_t shortblocke=0;

static char h_character;

void _putchar(char character)
{
  h_character = character;

  while(UART2HandlerPointer->State != Ready)
  {

  }
  TUART_DMA_Trasmit(UART2HandlerPointer,(uint8_t*)&h_character);
}


