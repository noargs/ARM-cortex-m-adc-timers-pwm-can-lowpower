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

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
  GPIO_InitTypeDef tim2_oc_ch_gpios;

  //1. enable the peripheral clock for the timer2 peripheral
  __HAL_RCC_TIM2_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  //2. configure a gpios to behave as timer2 channel 1, 2, 3, and 4
  // PAO  => TIM2_CH1
  // PA1  => TIM2_CH2
  // PB10 => TIM2_CH3
  // PB2  => TIM2_CH4
  tim2_oc_ch_gpios.Pin = GPIO_PIN_0;     // use GPIO_PIN_5 for on-board LED or connect external to GPIO_PIN_0 (either case, port A)
  tim2_oc_ch_gpios.Mode = GPIO_MODE_AF_PP;
  tim2_oc_ch_gpios.Pull = GPIO_NOPULL;
  tim2_oc_ch_gpios.Speed = GPIO_SPEED_FREQ_LOW;
  tim2_oc_ch_gpios.Alternate = GPIO_AF1_TIM2;
  HAL_GPIO_Init(GPIOA, &tim2_oc_ch_gpios);

  //3. nvic settings
  HAL_NVIC_SetPriority(TIM2_IRQn, 15, 0);
  HAL_NVIC_EnableIRQ(TIM2_IRQn);
}
