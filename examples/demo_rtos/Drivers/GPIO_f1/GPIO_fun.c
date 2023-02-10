#include "GPIO_fun.h"
#define GPIO_NUMBER           16u

void tooglePIN(GPIO_TypeDef *GPIOx, Gpio_Ch_st GPIO_Pin)
{
  uint32_t odr;
  odr = GPIOx->ODR;
  GPIOx->BSRR = ((odr & GPIO_Pin) << GPIO_NUMBER) | (~odr & GPIO_Pin); //taki trick
}


void GPIO_PinSet(GPIO_TypeDef *GPIOx, Gpio_Ch_st GPIO_Pin)
{
  GPIOx->BSRR = GPIO_Pin;
  
}

void GPIO_PinReset(GPIO_TypeDef *GPIOx, Gpio_Ch_st GPIO_Pin)
{
  uint32_t odr;
  odr = GPIOx->ODR;
  GPIOx->BSRR = ((odr & GPIO_Pin) << GPIO_NUMBER);
}

void GPIO_Pin_Cfg(GPIO_TypeDef * const port, GpioPin_t pin, GpioMode_t mode)
{
	//nadpisanie nic nie zepsuje alternatywa bitowa to alternatywa - no kosztem wydajnosci no ale
	if(port==GPIOA)
	{
		RCC->APB2ENR |=  RCC_APB2ENR_IOPAEN;
	}
		if(port==GPIOB)
	{
		RCC->APB2ENR |=  RCC_APB2ENR_IOPBEN;
	}
		if(port==GPIOC)
	{
		RCC->APB2ENR |=  RCC_APB2ENR_IOPCEN;
	}
		if(port==GPIOD)
	{
		RCC->APB2ENR |=  RCC_APB2ENR_IOPDEN;
	}
	
	if(pin<8)
	{
		port->CRL &= ~((uint32_t)(0xf << (pin*4) ));
		port->CRL |= mode << (pin*4);
	}
	else
	{
		uint8_t pinH = pin - 8;  //to avoid warnings
		port->CRH &= ~((uint32_t)(0xf << (pinH*4) ));
		port->CRH |= mode << (pinH*4);
	}
}	

