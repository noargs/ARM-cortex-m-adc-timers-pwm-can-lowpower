#include <string.h>
#include "stm32f4xx_hal.h"
#include "main.h"

#define TRUE                         1
#define FALSE                        0

void UART2_Init(void);
void Error_handler(void);

UART_HandleTypeDef huart2;

int main(void)
{
	RCC_OscInitTypeDef osc_init;
	RCC_ClkInitTypeDef clk_init;

  HAL_Init();

  UART2_Init();


  while(1);

  return 0;
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
  	// There's a problem
  	Error_handler();
  }
}


void Error_handler(void)
{
	// Trap here or blink any LED's to indicate error
	while(1);
}


