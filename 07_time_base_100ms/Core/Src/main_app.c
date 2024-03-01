#include <main_app.h>
#include <string.h>

void SystemClock_Config(void);
void Error_handler(void);
void TIMER6_Init(void);
void GPIO_Init(void);

TIM_HandleTypeDef htimer6;

int main(void)
{
	HAL_Init();
  SystemClock_Config();
  GPIO_Init();
  TIMER6_Init();

  // Start the timer
  HAL_TIM_Base_Start(&htimer6);

  while(1)
  {
  	// loop until the `Update event` flag is set
  	while( !(TIM6->SR & TIM_SR_UIF));
  	// The required time delay has been elapsed

  	// User code can be executed
  	TIM6->SR = 0; // cleared by software (Ref Manual page: 637)
  	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
  }

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

void GPIO_Init(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef led_gpio;
	led_gpio.Pin = GPIO_PIN_5;
	led_gpio.Mode = GPIO_MODE_OUTPUT_PP;
	led_gpio.Pull = GPIO_NOPULL; // Default, doesnt matter
	HAL_GPIO_Init(GPIOA, &led_gpio);
}

void Error_handler(void)
{
	// Trap here or blink any LED's to indicate error
	while(1);
}


