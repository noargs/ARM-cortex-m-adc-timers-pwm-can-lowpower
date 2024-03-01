#include <main_app.h>

void HAL_MspInit(void)
{
  // Here we do low level `processor` specific inits.
  //1. Set up the priority grouping of the ARM Cortex MX processor
  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  //2. Enable the required System exceptions of the ARM Cortex MX processor
  SCB->SHCSR |= 0x7 << 16; // enable usage fault, mem fault and bus fault exception

  //3. Configure the priority for the System exceptions
  HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
  HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
  HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
}


void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htimer)
{
  //1. enable the clock for the TIM6 peripheral
	__HAL_RCC_TIM6_CLK_ENABLE();

	//2. enable the IRQ of TIM6
	HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);

	//3. setup the priority for `TIM6_DAC_IRQn`
	HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 15, 0);
}
