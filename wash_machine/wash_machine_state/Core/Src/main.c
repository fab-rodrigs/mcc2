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

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
typedef enum {SB, PRE, MOL, LAV, ENX, ESV, CEN} Tipo_Estado;

// Prototipagem
void stand_by(void);
void preenchendo(void);
void molho(void);
void lavagem(void);
void enxague(void);
void esvaziar(void);
void centrifugar(void);

// Tabela de ponteiros para funções
void (*tabela_estados[])() = {stand_by, preenchendo, molho, lavagem, enxague, esvaziar, centrifugar};

Tipo_Estado estado_atual;
Tipo_Estado estado_anterior = SB;

int main(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();

  inic_sistema();

  while (1)
  {
	tabela_estados[estado_atual]();
  }

}


void inic_sistema()
{
	estado_atual = SB;
}

void stand_by()
{
	HAL_GPIO_WritePin(GPIOA, SB_Pin, 1);
	HAL_Delay(2000);
	HAL_GPIO_WritePin(GPIOA, SB_Pin, 0);

	if (1) // HAL_GPIO_ReadPin(GPIOA, ON_Pin) && ~(HAL_GPIO_ReadPin(GPIOA, SP_Pin)) && estado_anterior == SB
	{
		estado_atual = PRE;
	}
	else if (HAL_GPIO_ReadPin(GPIOA, ON_Pin) && ~(HAL_GPIO_ReadPin(GPIOA, SP_Pin)) && estado_anterior == MOL)
	{
		estado_atual = MOL;
	}
	else if (HAL_GPIO_ReadPin(GPIOA, ON_Pin) && ~(HAL_GPIO_ReadPin(GPIOA, SP_Pin)) && estado_anterior == LAV)
	{
		estado_atual = LAV;
	}
	else if (HAL_GPIO_ReadPin(GPIOA, ON_Pin) && ~(HAL_GPIO_ReadPin(GPIOA, SP_Pin)) && estado_anterior == ENX)
	{
		estado_atual = ENX;
	}
	else if (HAL_GPIO_ReadPin(GPIOA, ON_Pin) && ~(HAL_GPIO_ReadPin(GPIOA, SP_Pin)) && estado_anterior == ESV)
	{
		estado_atual = ESV;
	}
	else if (HAL_GPIO_ReadPin(GPIOA, ON_Pin) && ~(HAL_GPIO_ReadPin(GPIOA, SP_Pin)) && estado_anterior == CEN)
	{
		estado_atual = CEN;
	}

}

void preenchendo()
{
	HAL_GPIO_WritePin(GPIOA, PRE_Pin, 1);
	HAL_Delay(2000);
	HAL_GPIO_WritePin(GPIOA, PRE_Pin, 0);

	if (1) // HAL_GPIO_ReadPin(GPIOA, AV_Pin)) || HAL_GPIO_ReadPin(GPIOA, SNC_Pin))
	{
		estado_atual = MOL;
	}
	else if(HAL_GPIO_ReadPin(GPIOA, SP_Pin) || ~HAL_GPIO_ReadPin(GPIOA, ON_Pin))
	{
		estado_atual = SB;
		estado_anterior = PRE;
	}

}

void molho()
{
	HAL_GPIO_WritePin(GPIOA, MOL_Pin, 1);
	HAL_Delay(2000);
	HAL_GPIO_WritePin(GPIOA, MOL_Pin, 0);
	estado_atual = LAV;

	if (HAL_GPIO_ReadPin(GPIOA, AV_Pin))
	{
		estado_atual = LAV;
	}
	else if(HAL_GPIO_ReadPin(GPIOA, SP_Pin)) //|| ~HAL_GPIO_ReadPin(GPIOA, ON_Pin))
	{
		estado_atual = SB;
		estado_anterior = MOL;
	}

}

void lavagem()
{
	HAL_GPIO_WritePin(GPIOA, LAV_Pin, 1);
	HAL_Delay(2000);
	HAL_GPIO_WritePin(GPIOA, LAV_Pin, 0);
	estado_atual = ENX;

	if (HAL_GPIO_ReadPin(GPIOA, AV_Pin))
	{
		estado_atual = ENX;
	}
	else if(HAL_GPIO_ReadPin(GPIOA, SP_Pin)) //|| ~HAL_GPIO_ReadPin(GPIOA, ON_Pin))
	{
		estado_atual = SB;
		estado_anterior = LAV;
	}
}

void enxague()
{
	HAL_GPIO_WritePin(GPIOA, ENX_Pin, 1);
	HAL_Delay(2000);
	HAL_GPIO_WritePin(GPIOA, ENX_Pin, 0);
	estado_atual = ESV;

	if (HAL_GPIO_ReadPin(GPIOA, AV_Pin))
	{
		estado_atual = ESV;
	}
	else if(HAL_GPIO_ReadPin(GPIOA, SP_Pin)) //|| ~HAL_GPIO_ReadPin(GPIOA, ON_Pin))
	{
		estado_atual = SB;
		estado_anterior = ENX;
	}
}

void esvaziar()
{
	HAL_GPIO_WritePin(GPIOA, ESV_Pin, 1);
	HAL_Delay(2000);
	HAL_GPIO_WritePin(GPIOA, ESV_Pin, 0);
	estado_atual = CEN;

	if (HAL_GPIO_ReadPin(GPIOA, AV_Pin) || HAL_GPIO_ReadPin(GPIOA, SNB_Pin))
	{
		estado_atual = CEN;
	}
	else if(HAL_GPIO_ReadPin(GPIOA, SP_Pin)) //|| ~HAL_GPIO_ReadPin(GPIOA, ON_Pin))
	{
		estado_atual = SB;
		estado_anterior = ESV;
	}
}

void centrifugar()
{
	HAL_GPIO_WritePin(GPIOA, CEN_Pin, 1);
	HAL_Delay(2000);
	HAL_GPIO_WritePin(GPIOA, CEN_Pin, 0);
	estado_atual = SB;

	if (HAL_GPIO_ReadPin(GPIOA, AV_Pin))
	{
		estado_atual = SB;
	}
	else if(HAL_GPIO_ReadPin(GPIOA, SP_Pin)) //|| ~HAL_GPIO_ReadPin(GPIOA, ON_Pin))
	{
		estado_atual = SB;
		estado_anterior = CEN;
	}
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
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
  HAL_GPIO_WritePin(GPIOA, SB_Pin|PRE_Pin|MOL_Pin|LAV_Pin
                          |ENX_Pin|ESV_Pin|CEN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : SB_Pin PRE_Pin MOL_Pin LAV_Pin
                           ENX_Pin ESV_Pin CEN_Pin */
  GPIO_InitStruct.Pin = SB_Pin|PRE_Pin|MOL_Pin|LAV_Pin
                          |ENX_Pin|ESV_Pin|CEN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : ON_Pin SP_Pin SNC_Pin SNB_Pin
                           AV_Pin */
  GPIO_InitStruct.Pin = ON_Pin|SP_Pin|SNC_Pin|SNB_Pin
                          |AV_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

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

#ifdef  USE_FULL_ASSERT
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
