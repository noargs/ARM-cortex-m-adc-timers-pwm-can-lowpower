#include <string.h>
#include "stm32f4xx_hal.h"
#include "main.h"


void SystemClockConfig(void);
void UART2_Init(void);
void Error_handler(void);
uint8_t convert_to_capital(uint8_t data);

UART_HandleTypeDef huart2;

char *user_data = "The application is running\r\n";

int main(void)
{
  HAL_Init();
  SystemClockConfig();
  UART2_Init();

  uint16_t len_of_data = strlen(user_data);
  HAL_UART_Transmit(&huart2, (uint8_t*)user_data, len_of_data, HAL_MAX_DELAY);


  while(1);

  return 0;
}

void SystemClockConfig(void)
{

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

uint8_t convert_to_capital(uint8_t data)
{
	if (data >= 'a' && data <= 'z')
	{
		data = data - ('a' - 'A'); // lookup ASCII table `a`=65 - `A`=97
	}
	return data;
}

void Error_handler(void)
{
	// Trap here or blink any LED's to indicate error
	while(1);
}


