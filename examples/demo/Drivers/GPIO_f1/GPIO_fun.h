#ifndef __GPIO_fun_H__
#define __GPIO_fun_H__
#include "main.h"

typedef enum __GpioPin_t
{
 Px0 = 0x0,
 Px1 = 0x1,
 Px2 = 0x2,
 Px3 = 0x3,
 Px4 = 0x4,
 Px5 = 0x5,
 Px6 = 0x6,
 Px7 = 0x7,
 Px8 = 0x8,
 Px9 = 0x9,
 Px10 = 0x10,
 Px11 = 0x11,
 Px12 = 0x12,
 Px13 = 0x13,
 Px14 = 0x14,
 Px15 = 0x15,

} GpioPin_t;

/*GPIO Pin state mask*/
typedef enum __Gpio_Ch_st {
 Psm0 = 0x00000001,
 Psm1 = 0x00000002,
 Psm2 = 0x00000004,
 Psm3 = 0x00000008,
 Psm4 = 0x00000010,
 Psm5 = 0x00000020,
 Psm6 = 0x00000040,
 Psm7 = 0x00000080,
 Psm8 = 0x00000100,
 Psm9 = 0x00000200,
 Psm10 = 0x00000400,
 Psm11 = 0x00000800,
 Psm12 = 0x00001000,
 Psm13 = 0x00002000,
 Psm14 = 0x00004000,
 Psm15 = 0x00008000,
}Gpio_Ch_st;


typedef enum __GpioMode_t {
 /* Push-Pull */
 gpio_mode_output_PP_2MHz = 2,
 gpio_mode_output_PP_10MHz = 1,
 gpio_mode_output_PP_50MHz = 3,
 /* Open-Drain */
 gpio_mode_output_OD_2MHz = 6,
 gpio_mode_output_OD_10MHz = 5,
 gpio_mode_output_OD_50MHz = 7,
 /* Push-Pull */
 gpio_mode_alternate_PP_2MHz = 10,
 gpio_mode_alternate_PP_10MHz = 9,
 gpio_mode_alternate_PP_50MHz = 11,
 /* Open-Drain */
 gpio_mode_alternate_OD_2MHz = 14,
 gpio_mode_alternate_OD_10MHz = 13,
 gpio_mode_alternate_OD_50MHz = 15,
 /* Analog input (ADC) */
 gpio_mode_input_analog = 0,
 /* Floating digital input. */
 gpio_mode_input_floating = 4,
 /* Digital input with pull-up/down (depending on the ODR reg.). */
 gpio_mode_input_pull = 8
} GpioMode_t;

extern void GPIO_PinReset(GPIO_TypeDef *GPIOx, Gpio_Ch_st GPIO_Pin);
extern void GPIO_PinSet(GPIO_TypeDef *GPIOx, Gpio_Ch_st GPIO_Pin);
extern void tooglePIN(GPIO_TypeDef *GPIOx, Gpio_Ch_st GPIO_Pin);
extern void GPIO_Pin_Cfg(GPIO_TypeDef * const port, GpioPin_t pin, GpioMode_t mode);

#endif


