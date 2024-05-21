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

void HAL_CAN_MspInit(CAN_HandleTypeDef *hcan)
{
  GPIO_InitTypeDef gpio_can;

  __HAL_RCC_CAN1_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*
   * CAN1 GPIO Configuration:
   *   PA11 --> CAN1_RX
   *   PA12 --> CAN1_TX
   */
  gpio_can.Pin = GPIO_PIN_11 | GPIO_PIN_12;
  gpio_can.Mode = GPIO_MODE_AF_PP;
  gpio_can.Pull = GPIO_PULLUP;                   // needs to be pulled up https://electronics.stackexchange.com/questions/353005/can-initialization-timeout-error-in-stm32f4/353101
  gpio_can.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_can.Alternate = GPIO_AF9_CAN1;
  HAL_GPIO_Init(GPIOA, &gpio_can);
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
