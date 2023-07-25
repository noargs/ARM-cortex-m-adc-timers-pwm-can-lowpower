#include "stm32f4xx_hal.h"
#include "main.h"

void SystemClockConfig(void);

int main(void)
{
  HAL_Init();
  SystemClockConfig();

  return 0;
}

void SystemClockConfig(void)
{

}
