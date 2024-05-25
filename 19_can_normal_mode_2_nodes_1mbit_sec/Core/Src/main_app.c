#include <stdio.h>
#include <string.h>

#include "main_app.h"

void SystemClock_Config_HSE(uint8_t clock_freq);
void GPIO_Init(void);
void Error_handler(void);
void UART2_Init(void);
void TIMER6_Init(void);
void CAN1_Init(void);
void CAN1_Tx(void);
void CAN_FilterConfig(void);
void send_response(uint32_t StdId);
void led_manage_output(uint8_t led_no);

TIM_HandleTypeDef htim6;
CAN_HandleTypeDef hcan1;
UART_HandleTypeDef huart2;
CAN_RxHeaderTypeDef rx_header;

uint8_t led_no = 0;
uint8_t req_counter = 0;

int main(void)
{
  HAL_Init();
  SystemClock_Config_HSE(SYS_CLOCK_FREQ_84_MHZ);
  GPIO_Init();
  UART2_Init();
  TIMER6_Init();
  CAN1_Init();
  CAN_FilterConfig();

  if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_TX_MAILBOX_EMPTY | CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_BUSOFF) != HAL_OK)
  {
	Error_handler();
  }

  if (HAL_CAN_Start(&hcan1) != HAL_OK)
  {
	Error_handler();
  }


  while (1);

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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();


  GPIO_InitTypeDef ledgpio;
  ledgpio.Pin = GPIO_PIN_5;
  ledgpio.Mode = GPIO_MODE_OUTPUT_PP;
  ledgpio.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA,&ledgpio);


  ledgpio.Pin = GPIO_PIN_9 | GPIO_PIN_8 | GPIO_PIN_6;
  ledgpio.Mode = GPIO_MODE_OUTPUT_PP;
  ledgpio.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC,&ledgpio);

  ledgpio.Pin = GPIO_PIN_8;
  ledgpio.Mode = GPIO_MODE_OUTPUT_PP;
  ledgpio.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB,&ledgpio);

  ledgpio.Pin = GPIO_PIN_13;
  ledgpio.Mode = GPIO_MODE_IT_FALLING;
  ledgpio.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC,&ledgpio);

  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void CAN1_Init(void)
{
  hcan1.Instance = CAN1;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.AutoBusOff = ENABLE;
  hcan1.Init.AutoRetransmission = ENABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;

  // Settings related to CAN bit timings
  hcan1.Init.Prescaler = 3;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;    // always 1 quanta
  hcan1.Init.TimeSeg1 = CAN_BS1_11TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_2TQ;

  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
	Error_handler();
  }

}

void CAN1_Tx(void)
{
  CAN_TxHeaderTypeDef tx_header;
  tx_header.DLC = 1;               // how many bytes want to send
  tx_header.StdId = 0x65D;
  tx_header.IDE = CAN_ID_STD;
  tx_header.RTR = CAN_RTR_DATA;    // Data frame or Request frame (Remote frame also called Request frame)

  uint8_t message;
  message = ++led_no;
  if (led_no == 4)
  {
	led_no = 0;
  }

  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);

  uint32_t tx_mailbox;
  if (HAL_CAN_AddTxMessage(&hcan1, &tx_header, &message, &tx_mailbox) != HAL_OK)
  {
	Error_handler();
  }
}

void CAN_FilterConfig(void)
{
  CAN_FilterTypeDef can1_filter;

  can1_filter.FilterActivation = ENABLE;
  can1_filter.FilterBank = 0;
  can1_filter.FilterFIFOAssignment = CAN_RX_FIFO0;
  can1_filter.FilterIdHigh = 0x0000;
  can1_filter.FilterIdLow = 0x0000;
  can1_filter.FilterMaskIdHigh = 0x0000;
  can1_filter.FilterMaskIdLow = 0x0000;
  can1_filter.FilterMode = CAN_FILTERMODE_IDMASK;

  /*
   * by default 32-bit and R1(ID), R2(MASK) register will each considered as 32 bit
   * if 16-bit filter scale selected then R1(ID), R1(MASK), R2(ID), and R2(MASK) registers
   * each 16 bit wide
   *
   * consult RM page: 1057, Figure 391. Filter bank scale configuration - Register organization
   */
  can1_filter.FilterScale = CAN_FILTERSCALE_32BIT;

  if (HAL_CAN_ConfigFilter(&hcan1, &can1_filter) != HAL_OK)
  {
	Error_handler();
  }
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

void TIMER6_Init(void)
{
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 4999;
  htim6.Init.Period = 10000-1;

  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
	Error_handler();
  }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  CAN_TxHeaderTypeDef tx_header;
  uint32_t tx_mailbox;
  uint8_t message; // no meaning, will be discarded by the API
  if (req_counter == 4)
  {
	// N1 sending Remote frame to N2
	tx_header.DLC = 2; // N1 demanding 2 bytes of reply
	tx_header.StdId = 0x651;
	tx_header.IDE = CAN_ID_STD;
	tx_header.RTR = CAN_RTR_REMOTE;

	if (HAL_CAN_AddTxMessage(&hcan1, &tx_header, &message, &tx_mailbox) != HAL_OK)
	{
	  Error_handler();
	}
	req_counter = 0;
  } else
  {
	CAN1_Tx();
	req_counter++;
  }
}

void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan)
{
  char msg[50];
  sprintf(msg, "Message transmitted:M0\r\n");
  HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
}

void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef *hcan)
{
  char msg[50];
  sprintf(msg, "Message transmitted:M1\r\n");
  HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
}

void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef *hcan)
{
  char msg[50];
  sprintf(msg, "Message transmitted:M2\r\n");
  HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
  uint8_t rcvd_msg[8];
  char msg[50];

  if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, rcvd_msg) != HAL_OK)
  {
	Error_handler();
  }

  if (rx_header.StdId == 0x65D && rx_header.RTR == 0) // RTR==0 or dominant bit means Data frame
  {
	/*
	 * This is data frame sent by N1 to N2
	 */
	led_manage_output(rcvd_msg[0]);
	sprintf(msg, "Message received: %#x\r\n", rcvd_msg[0]);
  }
  else if (rx_header.StdId == 0x651 && rx_header.RTR == 2)
  {
	// This Remote frame sent by N1 to N2
	send_response(rx_header.StdId);
	return;
  }
  else if (rx_header.StdId == 0x651 && rx_header.RTR == 0)
  {
	// This is a reply (Data frame) by N2 to N1 (N1 will display that reply)
	// Reply will be 2bytes of data which we print here
	sprintf(msg, "Reply received: %#x\r\n", ((rcvd_msg[0] << 8) | rcvd_msg[1]));
  }

  HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
}

void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan)
{
  char msg[50];
  sprintf(msg, "CAN error detected\r\n");
  HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
}

void led_manage_output(uint8_t led_no)
{
  switch(led_no)
  {
  case 1 :
	HAL_GPIO_WritePin(LED1_PORT,LED1_PIN_NO,GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED2_PORT,LED2_PIN_NO,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED3_PORT,LED3_PIN_NO,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED4_PORT,LED4_PIN_NO,GPIO_PIN_RESET);
	break;
  case 2 :
	HAL_GPIO_WritePin(LED1_PORT,LED1_PIN_NO,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED2_PORT,LED2_PIN_NO,GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED3_PORT,LED3_PIN_NO,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED4_PORT,LED4_PIN_NO,GPIO_PIN_RESET);
	break;
  case 3 :
	HAL_GPIO_WritePin(LED1_PORT,LED1_PIN_NO,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED2_PORT,LED2_PIN_NO,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED3_PORT,LED3_PIN_NO,GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED4_PORT,LED4_PIN_NO,GPIO_PIN_RESET);
	break;
  case 4 :
	HAL_GPIO_WritePin(LED1_PORT,LED1_PIN_NO,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED2_PORT,LED2_PIN_NO,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED3_PORT,LED3_PIN_NO,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED4_PORT,LED4_PIN_NO,GPIO_PIN_SET);
	break;
  }
}

void send_response(uint32_t StdId)
{
  CAN_TxHeaderTypeDef tx_header;
  uint32_t tx_mailbox;
  uint8_t response[2] = { 0xAB,0XCD}; // can be anything i.e. temperature value, engine status, sensor value etc.

  tx_header.DLC = 2;
  tx_header.StdId = StdId;
  tx_header.IDE   = CAN_ID_STD;
  tx_header.RTR = CAN_RTR_DATA;

  if( HAL_CAN_AddTxMessage(&hcan1,&tx_header,response,&tx_mailbox) != HAL_OK)
  {
	Error_handler();
  }
}

void Error_handler(void)
{
  while(1);
}

