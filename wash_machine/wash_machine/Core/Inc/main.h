/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SB_Pin GPIO_PIN_0
#define SB_GPIO_Port GPIOA
#define PRE_Pin GPIO_PIN_1
#define PRE_GPIO_Port GPIOA
#define MOL_Pin GPIO_PIN_2
#define MOL_GPIO_Port GPIOA
#define LAV_Pin GPIO_PIN_3
#define LAV_GPIO_Port GPIOA
#define ENX_Pin GPIO_PIN_4
#define ENX_GPIO_Port GPIOA
#define ESV_Pin GPIO_PIN_5
#define ESV_GPIO_Port GPIOA
#define CEN_Pin GPIO_PIN_6
#define CEN_GPIO_Port GPIOA
#define ON_Pin GPIO_PIN_9
#define ON_GPIO_Port GPIOA
#define SP_Pin GPIO_PIN_11
#define SP_GPIO_Port GPIOA
#define SNC_Pin GPIO_PIN_13
#define SNC_GPIO_Port GPIOA
#define SNB_Pin GPIO_PIN_15
#define SNB_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
