/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdint.h>
#include <stdbool.h>
#include "WS2812.h"
#include "Buzzer.h"
#include "oneButton.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DEVICE_AUTO_OFF_TIME_MS     (5ULL * 60ULL * 1000ULL)
#define MAX_BUTTON_STAMP_POINTS     5
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim14;
DMA_HandleTypeDef hdma_tim3_ch1;

/* USER CODE BEGIN PV */
typedef struct
{
    uint16_t hue;
    uint8_t sat;
    uint8_t val;
}hsvColorTypedef;

hsvColorTypedef xmasColors[] =
{
        {       // Red color
                .hue = (uint16_t)(0.0 / 360.0 * 65535),
                .sat = (uint8_t)(94.0 / 100.0 * 255.0),
                .val = 13
        },
        {       // Green color
                .hue = (uint16_t)(105.0 / 360.0 * 65535),
                .sat = (uint8_t)(82.0 / 100.0 * 255.0),
                .val = 5
        },
        {       // Yellow color
                .hue = (uint16_t)(35.0 / 360.0 * 65535),
                .sat = (uint8_t)(98.0 / 100.0 * 255.0),
                .val = 15
        },
        {       // Blue color
                .hue = (uint16_t)(202.0 / 360.0 * 65535),
                .sat = (uint8_t)(94.0 / 100.0 * 255.0),
                .val = 4
        },
        {       // Pink color
                .hue = (uint16_t)(359.0 / 360.0 * 65535),
                .sat = (uint8_t)(80.0 / 100.0 * 255.0),
                .val = 13
        },
        {       // White color
                .hue = (uint16_t)(200.0 / 360.0 * 65535),
                .sat = (uint8_t)(20.0 / 100.0 * 255.0),
                .val = 5
        },
        {       // Warm white color
                .hue = (uint16_t)(30.0 / 360.0 * 65535),
                .sat = (uint8_t)(81.0 / 100.0 * 255.0),
                .val = 10
        }
};

uint32_t onTime = 0;
int ledColorShift = 0;
uint32_t ledShiftDelay = 0;

bool risingInt = false;
bool fallingInt = false;

uint32_t intTimes[MAX_BUTTON_STAMP_POINTS] = {0};
bool intStates[MAX_BUTTON_STAMP_POINTS] = {0};
uint8_t intPins[MAX_BUTTON_STAMP_POINTS] = {0};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM14_Init(void);
/* USER CODE BEGIN PFP */
OneButton btn1;
OneButton btn2;
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM3_Init();
  MX_TIM14_Init();
  /* USER CODE BEGIN 2 */

  // Enable the supply for the buzzer and LEDs.
  HAL_GPIO_WritePin(PERIPH_EN_GPIO_Port, PERIPH_EN_Pin, GPIO_PIN_RESET);
  // Wait a little bit.
  HAL_Delay(100ULL);

  // Initialize the buzzer library.
  buzzer_begin(&htim14);

  // Initialize the WS LED library.
  if (!ws2812_begin(&htim3, &hdma_tim3_ch1, TIM_CHANNEL_1, 12, 2, 9, 6, 3))
  {
      while(1);
  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      // Check the button(s).
      btn1.loop();
      btn2.loop();

      if (btn1.isMultipleTap())
      {
          uint8_t _numberOfTaps = btn1.getNumberOfTaps() % 5;

          // Set the new colors on LEDs.
          for (int i = 0; i < 12; i++)
          {
              ws2812_setPixelColor(i, ws2812_hsv(xmasColors[_numberOfTaps].hue, xmasColors[_numberOfTaps].sat, xmasColors[_numberOfTaps].val));
          }

          // Push the new colors to the LEDs.
          ws2812_show();

          HAL_Delay(2000ULL);
      }

      if (btn1.isLongPressed())
      {
          // Set the new colors on LEDs.
          for (int i = 0; i < 12; i++)
          {
              ws2812_setPixelColor(i, ws2812_hsv(xmasColors[5].hue, xmasColors[5].sat, xmasColors[5].val));
          }

          // Push the new colors to the LEDs.
          ws2812_show();

          HAL_Delay(2000ULL);
      }

      if (btn2.isLongPressed())
      {
          // Set the new colors on LEDs.
          for (int i = 0; i < 12; i++)
          {
              ws2812_setPixelColor(i, ws2812_hsv(xmasColors[6].hue, xmasColors[6].sat, xmasColors[6].val));
          }

          // Push the new colors to the LEDs.
          ws2812_show();

          HAL_Delay(2000ULL);
      }

      if (btn2.isMultipleTap())
      {
          uint8_t _numberOfTaps = btn2.getNumberOfTaps() % 5;

          // Set the new colors on LEDs.
          for (int i = 0; i < 12; i++)
          {
              ws2812_setPixelColor(i, ws2812_hsv(xmasColors[(i % (_numberOfTaps + 1))].hue, xmasColors[(i % (_numberOfTaps + 1))].sat, xmasColors[(i % (_numberOfTaps + 1))].val));
          }

          // Push the new colors to the LEDs.
          ws2812_show();

          HAL_Delay(2000ULL);
      }

      if (btn1.isPressed())
      {
          buzzer_toneDuration(1000ULL, 100ULL, TIM_CHANNEL_1);
          HAL_Delay(500ULL);
      }

      // Go thought the colors every one second.
      if ((uint32_t)(HAL_GetTick() - ledShiftDelay) > 1000ULL)
      {
          // Update the timer variable.
          ledShiftDelay = HAL_GetTick();

          // Set the new colors on LEDs.
          for (int i = 0; i < 12; i++)
          {
              ws2812_setPixelColor(i, ws2812_hsv(xmasColors[(i + ledColorShift) % 5].hue, xmasColors[(i + ledColorShift) % 5].sat, xmasColors[(i + ledColorShift) % 5].val));
          }
          ledColorShift++;

          // Push the new colors to the LEDs.
          ws2812_show();
      }

      // If 10 minutes passed, shut down the ornament and prepare everything for the power up on the button.
      if ((uint32_t)(HAL_GetTick() - onTime) > DEVICE_AUTO_OFF_TIME_MS)
      {
          // Update the timer.
          onTime = HAL_GetTick();

          // Turn off all LEDs.
          for (int i = 0; i < 12; i++)
          {
              ws2812_setPixelColor(i, 0);
          }

          // Push the new colors to the LEDs.
          ws2812_show();

          // Disable the supply to the LEDs and buzzer.
          HAL_GPIO_WritePin(PERIPH_EN_GPIO_Port, PERIPH_EN_Pin, GPIO_PIN_SET);

          // Disable the timers for LEDs and buzzer.
          HAL_TIM_PWM_DeInit(&htim14);
          HAL_TIM_PWM_DeInit(&htim3);

          // Set the wake up pins.
          HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN1);
          HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN4);
          HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1_LOW);
          HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN4_LOW);
          __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WUF);

          // Disable the tick
          HAL_SuspendTick();

          // Shut-down whole device.
          HAL_PWREx_EnterSHUTDOWNMode();
      }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  __HAL_FLASH_SET_LATENCY(FLASH_LATENCY_0);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV2;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 2;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 9;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * @brief TIM14 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM14_Init(void)
{

  /* USER CODE BEGIN TIM14_Init 0 */

  /* USER CODE END TIM14_Init 0 */

  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM14_Init 1 */

  /* USER CODE END TIM14_Init 1 */
  htim14.Instance = TIM14;
  htim14.Init.Prescaler = 1200;
  htim14.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim14.Init.Period = 65535;
  htim14.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim14.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim14) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim14) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim14, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM14_Init 2 */

  /* USER CODE END TIM14_Init 2 */
  HAL_TIM_MspPostInit(&htim14);

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(PERIPH_EN_GPIO_Port, PERIPH_EN_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : USER_BTN2_Pin USER_BTN1_Pin */
  GPIO_InitStruct.Pin = USER_BTN2_Pin|USER_BTN1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PERIPH_EN_Pin */
  GPIO_InitStruct.Pin = PERIPH_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(PERIPH_EN_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);

  HAL_NVIC_SetPriority(EXTI2_3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI2_3_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
    //risingInt = true;
    if (GPIO_Pin == GPIO_PIN_2) btn1.updateState(HAL_GetTick(), 1);
    if (GPIO_Pin == GPIO_PIN_0) btn2.updateState(HAL_GetTick(), 1);
}

void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GPIO_PIN_2) btn1.updateState(HAL_GetTick(), 0);
    if (GPIO_Pin == GPIO_PIN_0) btn2.updateState(HAL_GetTick(), 0);
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
