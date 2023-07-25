#include "stm32f4xx_hal.h"

void HAL_MspInit(void)
{
  // Here we will do low level processor specific inits.
  //1. Set up the priority grouping of the ARM Cortex MX processor
  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  //2. Enable the required System exceptions of the ARM Cortex MX processor
  SCB->SHCSR |= 0x7 << 16; // enable usage fault, mem fault and bus fault exception

  //3. Configure the priority for the System exceptions
  HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
  HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
  HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
}
