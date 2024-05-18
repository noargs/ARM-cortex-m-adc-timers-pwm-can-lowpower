#include <stdio.h>
#include <string.h>

#include "main_app.h"

void SystemClock_Config_HSE(uint8_t clock_freq);
void GPIO_Init(void);
void TIMER2_Init(void);
void Error_handler(void);

TIM_HandleTypeDef htimer2;

int main(void)
{
  uint16_t brightness = 0;    // ccr is of 16-bit
  HAL_Init();
  SystemClock_Config_HSE(SYS_CLOCK_FREQ_50_MHZ); // using HSE on TIMER app is RECOMMENDED as output will improve alot
  GPIO_Init();
  TIMER2_Init();

  if (HAL_TIM_PWM_Start(&htimer2, TIM_CHANNEL_1) != HAL_OK)
  {
	Error_handler();
  }

  while (1)
  {
	while(brightness < htimer2.Init.Period)
	{
	  // here, we constantly feed the pulse value which eventuelly programmed into ccr (capture and compare register)
	  brightness+=10;
	  __HAL_TIM_SET_COMPARE(&htimer2, TIM_CHANNEL_1, brightness);
	  HAL_Delay(1); // 1ms delay as processor running higher frequency (50MHz) than the timer (10MHz)
	}

	while (brightness > 0)
	{
	  brightness-=10;
	  __HAL_TIM_SET_COMPARE(&htimer2, TIM_CHANNEL_1, brightness);
	  HAL_Delay(1);
	}
  }

  return 0;
}

void SystemClock_Config_HSE(uint8_t clock_freq)
{
  RCC_OscInitTypeDef Osc_Init;
  RCC_ClkInitTypeDef Clock_Init;
  uint8_t flash_latency=0;

  Osc_Init.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_LSE | RCC_OSCILLATORTYPE_HSI ;
  Osc_Init.HSEState = RCC_HSE_ON;
  Osc_Init.LSEState = RCC_LSE_ON;
  Osc_Init.HSIState = RCC_HSI_ON;
  Osc_Init.PLL.PLLState = RCC_PLL_ON;
  Osc_Init.PLL.PLLSource = RCC_PLLSOURCE_HSE;

  switch(clock_freq)
  {
  case SYS_CLOCK_FREQ_50_MHZ:
	Osc_Init.PLL.PLLM = 4;
	Osc_Init.PLL.PLLN = 50;
	Osc_Init.PLL.PLLP = RCC_PLLP_DIV2;
	Osc_Init.PLL.PLLQ = 2;
	Osc_Init.PLL.PLLR = 2;
	Clock_Init.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
		|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	Clock_Init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	Clock_Init.AHBCLKDivider = RCC_SYSCLK_DIV1;
	Clock_Init.APB1CLKDivider = RCC_HCLK_DIV2;
	Clock_Init.APB2CLKDivider = RCC_HCLK_DIV1;
	flash_latency = 1;
	break;

  case SYS_CLOCK_FREQ_84_MHZ:
	Osc_Init.PLL.PLLM = 4;
	Osc_Init.PLL.PLLN = 84;
	Osc_Init.PLL.PLLP = RCC_PLLP_DIV2;
	Osc_Init.PLL.PLLQ = 2;
	Osc_Init.PLL.PLLR = 2;
	Clock_Init.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
		|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	Clock_Init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	Clock_Init.AHBCLKDivider = RCC_SYSCLK_DIV1;
	Clock_Init.APB1CLKDivider = RCC_HCLK_DIV2;
	Clock_Init.APB2CLKDivider = RCC_HCLK_DIV1;
	flash_latency = 2;
	break;

  case SYS_CLOCK_FREQ_120_MHZ:
	Osc_Init.PLL.PLLM = 4;
	Osc_Init.PLL.PLLN = 120;
	Osc_Init.PLL.PLLP = RCC_PLLP_DIV2;
	Osc_Init.PLL.PLLQ = 2;
	Osc_Init.PLL.PLLR = 2;
	Clock_Init.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
		|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	Clock_Init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	Clock_Init.AHBCLKDivider = RCC_SYSCLK_DIV1;
	Clock_Init.APB1CLKDivider = RCC_HCLK_DIV4;
	Clock_Init.APB2CLKDivider = RCC_HCLK_DIV2;
	flash_latency = 3;
	break;

  default:
	return ;
  }

  if (HAL_RCC_OscConfig(&Osc_Init) != HAL_OK)
  {
	Error_handler();
  }



  if (HAL_RCC_ClockConfig(&Clock_Init, flash_latency) != HAL_OK)
  {
	Error_handler();
  }


  /*Configure the systick timer interrupt frequency (for every 1 ms) */
	  uint32_t hclk_freq = HAL_RCC_GetHCLKFreq();
  HAL_SYSTICK_Config(hclk_freq/1000);

  /**Configure the Systick
   */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

void GPIO_Init(void)
{
  __HAL_RCC_GPIOA_CLK_ENABLE();
  GPIO_InitTypeDef led_gpio;
  led_gpio.Pin = GPIO_PIN_5;
  led_gpio.Mode = GPIO_MODE_OUTPUT_PP;
  led_gpio.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &led_gpio);
}

void TIMER2_Init(void)
{
  htimer2.Instance = TIM2;
  htimer2.Init.Period = 10000-1;                                           // to improve the precision use, Period=1000, and prescaler 49
  htimer2.Init.Prescaler = 4999;                                           // 1 means count clock will be 25MHz
  if (HAL_TIM_PWM_Init(&htimer2) != HAL_OK)
  {
	Error_handler();
  }

  TIM_OC_InitTypeDef tim2_pwm_config;
  memset(&tim2_pwm_config, 0, sizeof(tim2_pwm_config));

  tim2_pwm_config.OCMode = TIM_OCMODE_PWM1;
  tim2_pwm_config.OCPolarity = TIM_OCPOLARITY_HIGH;
  tim2_pwm_config.Pulse = 0;
  if (HAL_TIM_PWM_ConfigChannel(&htimer2, &tim2_pwm_config, TIM_CHANNEL_1) != HAL_OK)
  {
	Error_handler();
  }
}

void Error_handler(void)
{
  while(1);
}

