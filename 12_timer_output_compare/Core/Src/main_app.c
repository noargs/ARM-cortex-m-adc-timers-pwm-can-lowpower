#include <stdio.h>
#include <string.h>

#include "main_app.h"

void SystemClock_Config_HSE(uint8_t clock_freq);
void GPIO_Init(void);
void UART2_Init(void);
void TIMER2_Init(void);
void Error_handler(void);

TIM_HandleTypeDef htimer2;
UART_HandleTypeDef huart2;

uint32_t pulse1_value = 25000;  // to produce 500 Hz
uint32_t pulse2_value = 12500;  // to produce 1000 Hz
uint32_t pulse3_value = 6250;   // to produce 2000 Hz
uint32_t pulse4_value = 3125;   // to produce 4000 Hz

uint32_t ccr_content;

int main(void)
{

  HAL_Init();
  SystemClock_Config_HSE(SYS_CLOCK_FREQ_50_MHZ); // using HSE on TIMER app is RECOMMENDED as output will improve alot
  GPIO_Init();
  UART2_Init();
  TIMER2_Init();

  if (HAL_TIM_OC_Start_IT(&htimer2, TIM_CHANNEL_1) != HAL_OK)
  {
	Error_handler();
  }

  if (HAL_TIM_OC_Start_IT(&htimer2, TIM_CHANNEL_2) != HAL_OK)
  {
	Error_handler();
  }

  if (HAL_TIM_OC_Start_IT(&htimer2, TIM_CHANNEL_3) != HAL_OK)
  {
	Error_handler();
  }

  if (HAL_TIM_OC_Start_IT(&htimer2, TIM_CHANNEL_4) != HAL_OK)
  {
	Error_handler();
  }

  while(1);

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

void UART2_Init(void)
{
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
	Error_handler();
  }
}

void TIMER2_Init(void)
{
  htimer2.Instance = TIM2;
  htimer2.Init.Period = 0xFFFFFFFF;
  htimer2.Init.Prescaler = 1;
  if (HAL_TIM_OC_Init(&htimer2) != HAL_OK)
  {
	Error_handler();
  }

  TIM_OC_InitTypeDef tim2_oc_init;
  tim2_oc_init.OCMode = TIM_OCMODE_TOGGLE;
  tim2_oc_init.OCPolarity = TIM_OCPOLARITY_HIGH; // CC1P as 0 means `TIM_OCPOLARITY_HIGH`
  tim2_oc_init.Pulse = pulse1_value;             // depends upon how much frequency need at output channel, (we need ch1 500Hz, ch2 1KHz, ch3 2KHz, ch4 4KHz)
  if (HAL_TIM_OC_ConfigChannel(&htimer2, &tim2_oc_init, TIM_CHANNEL_1) != HAL_OK)
  {
	Error_handler();
  }

  tim2_oc_init.Pulse = pulse2_value;
  if (HAL_TIM_OC_ConfigChannel(&htimer2, &tim2_oc_init, TIM_CHANNEL_2) != HAL_OK)
  {
	Error_handler();
  }

  tim2_oc_init.Pulse = pulse3_value;
  if (HAL_TIM_OC_ConfigChannel(&htimer2, &tim2_oc_init, TIM_CHANNEL_3) != HAL_OK)
  {
	Error_handler();
  }

  tim2_oc_init.Pulse = pulse4_value;
  if (HAL_TIM_OC_ConfigChannel(&htimer2, &tim2_oc_init, TIM_CHANNEL_4) != HAL_OK)
  {
	Error_handler();
  }
}

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
  // TIM2_CH1 toggling with frequency = 500Hz
  if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
  {
	ccr_content = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
	__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1, (ccr_content+pulse1_value));
  }

  // TIM2_CH2 toggling with frequency = 1000Hz
  if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
  {
	ccr_content = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
	__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2, (ccr_content+pulse2_value));
  }

  // TIM2_CH3 toggling with frequency = 2000Hz
  if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
  {
	ccr_content = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);
	__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_3, (ccr_content+pulse3_value));
  }

  // TIM2_CH4 toggling with frequency = 4000Hz
  if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
  {
	ccr_content = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4);
	__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_4, (ccr_content+pulse4_value));
  }
}

void Error_handler(void)
{
  while(1);
}

