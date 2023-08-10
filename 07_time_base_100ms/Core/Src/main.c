#include <string.h>
#include "main.h"

void SystemClock_Config(void);
void Error_handler(void);
void TIMER6_Init(void);

TIM_HandleTypeDef htimer6;

int main(void)
{
	HAL_Init();
  SystemClock_Config();
  TIMER6_Init();

  while(1);

  return 0;
}

void SystemClock_Config(void)
{
}

void TIMER6_Init(void)
{
	htimer6.Instance = TIM6;
	htimer6.Init.Prescaler = 24;
	htimer6.Init.Period = 64000-1;
	if (HAL_TIM_Base_Init(&htimer6) != HAL_OK)
	{
		Error_handler();
	}
}

void Error_handler(void)
{
	// Trap here or blink any LED's to indicate error
	while(1);
}


