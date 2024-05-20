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

  /*
   * CAN1 GPIO Configuration:
   *   PA11 --> CAN1_RX
   *   PA12 --> CAN1_TX
   */
  gpio_can.Pin = GPIO_PIN_11 | GPIO_PIN_12;
  gpio_can.Mode = GPIO_MODE_AF_PP;
  gpio_can.Pull = GPIO_NOPULL;
  gpio_can.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_can.Alternate = GPIO_AF9_CAN1;
  HAL_GPIO_Init(GPIOA, &gpio_can);
}
