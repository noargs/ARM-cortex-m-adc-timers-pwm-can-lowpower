#include "main_app.h"

void HAL_MspInit(void)
{
  //Here will do low level processor specific inits.
  //1. Set up the priority grouping of the arm cortex mx processor
  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  //2. Enable the required system exceptions of the arm cortex mx processor
  SCB->SHCSR |= 0x7 << 16; //usage fault, memory fault and bus fault system exceptions

  //3. configure the priority for the system exceptions
  HAL_NVIC_SetPriority(MemoryManagement_IRQn,0,0);
  HAL_NVIC_SetPriority(BusFault_IRQn,0,0);
  HAL_NVIC_SetPriority(UsageFault_IRQn,0,0);
}

void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
  //1. Enable the clock for timer 2 and corresponding gpio
  __HAL_RCC_TIM2_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  //2. Configure a gpio to behave as TIM2 channel 1 (PA0, PA5, PA15, or PB8 can be used)
  //   PA0 will be used as TIM2 channel, whether it is input or output channel
  //   will be decided by channel mode configuration CC1S[1:0]
  GPIO_InitTypeDef time2ch1_gpio;
  time2ch1_gpio.Pin = GPIO_PIN_0;
  time2ch1_gpio.Mode = GPIO_MODE_AF_PP;
  time2ch1_gpio.Alternate = GPIO_AF1_TIM2;
  HAL_GPIO_Init(GPIOA, &time2ch1_gpio);

  //3. nvic settings
  HAL_NVIC_SetPriority(TIM2_IRQn, 15, 0);
  HAL_NVIC_EnableIRQ(TIM2_IRQn);
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
  GPIO_InitTypeDef gpio_uart;
  //here we are going to do the low level inits. of the USART2 peripheral

  //1. enable the clock for the USART2 peripheral as well as for GPIOA peripheral
  __HAL_RCC_USART2_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  //2 . Do the pin muxing configurations
  gpio_uart.Pin = GPIO_PIN_2;
  gpio_uart.Mode =GPIO_MODE_AF_PP;
  gpio_uart.Pull = GPIO_PULLUP;
  gpio_uart.Speed = GPIO_SPEED_FREQ_LOW;
  gpio_uart.Alternate =  GPIO_AF7_USART2; //UART2_TX
  HAL_GPIO_Init(GPIOA,&gpio_uart);

  gpio_uart.Pin = GPIO_PIN_3; //UART2_RX
  HAL_GPIO_Init(GPIOA,&gpio_uart);

  //3 . Enable the IRQ and set up the priority (NVIC settings )
  HAL_NVIC_EnableIRQ(USART2_IRQn);
  HAL_NVIC_SetPriority(USART2_IRQn,15,0);

}
