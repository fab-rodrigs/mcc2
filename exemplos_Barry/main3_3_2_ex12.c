#include "main.h"
#include "cmsis_os.h"
#include "usb_device.h"

#include "usbd_cdc_if.h"
#include "atraso.h"
#include "task.h"


void SystemClock_Config(void);
static void MX_GPIO_Init(void);
// ---------------------------------------------------------------------------------

xSemaphoreHandle xMutexUART;
xSemaphoreHandle xBinarySemaphore;

//--------------------------------------------------------------------------------
static void vPeriodicTask(void *pvParameters)
{
	while(1)
	{	
		vTaskDelay(500/portTICK_RATE_MS);
		
		xSemaphoreTake(xMutexUART,portMAX_DELAY);
		printf("Periodic task - About to generate an interrupt.\n");
		xSemaphoreGive(xMutexUART);
		
		NVIC_SetPendingIRQ((IRQn_Type)EXTI0_IRQn);	// ativa interrup��o por software
		
		xSemaphoreTake(xMutexUART,portMAX_DELAY);
		printf("Periodic task - Interrupt generated.\n\n");
		xSemaphoreGive(xMutexUART);
	}
}
//--------------------------------------------------------------------------------
static void vHandlerTask(void *pvParameters)
{
	
	while(1)
	{
		xSemaphoreTake(xBinarySemaphore, portMAX_DELAY);
		
		xSemaphoreTake(xMutexUART,portMAX_DELAY);
		printf("Handler task - Processing event.\n");
		xSemaphoreGive(xMutexUART);
	}
}
//--------------------------------------------------------------------------------
void EXTI0_IRQHandler()
{
	static portBASE_TYPE xHigherPriorityTaskWoken;

	xHigherPriorityTaskWoken = pdFALSE;

	xSemaphoreGiveFromISR(xBinarySemaphore, &xHigherPriorityTaskWoken);

	if(xHigherPriorityTaskWoken == pdTRUE)
	{
		//printf("xHigherPriorityTaskWoken == pdTRUE.\n");
	}

	//use to force a context switch from an ISR
	portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
	/* Giving the semaphore unblocked a task, and the priority of the
	unblocked task is higher than the currently running task - force
	a context switch to ensure that the interrupt returns directly to
	the unblocked (higher priority) task.*/
}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
int main (void)
{
	HAL_Init();
	SystemClock_Config();

	MX_GPIO_Init();
	MX_USB_DEVICE_Init();

	//---------------------------------------------------------------------------------------------
	// Habilitar interrupção por SW
	//---------------------------------------------------------------------------------------------
	NVIC_EnableIRQ((IRQn_Type) EXTI0_IRQn);		// usa EXTI0 para gerar interrupção por software

	/* configura Preempt Priority and Subpriority interrupt para o valor default (no STM32 é diferente!)*/
	NVIC_SetPriorityGrouping(0);

	/*IMPORTANTISSIMO
    	The priority of the interrupt has to be set to below configMAX_SYSCALL_INTERRUPT_PRIORITY
    	(FreeRTOSConfig.h), caso contrario a ISR que chama API do FreeRTOS vai travar!
    	Cuidado, pois há uma inversão, no ARM quando menor o nr, maior a prioridade!*/

	NVIC_SetPriority(EXTI0_IRQn, 6);	// ajusta prioridade para nivel 6, com 	configMAX_SYSCALL_INTERRUPT_PRIORITY = 5
	//---------------------------------------------------------------------------------------------

	//RTOS
	xMutexUART = xSemaphoreCreateMutex();
	xBinarySemaphore = xSemaphoreCreateBinary();
	
	if(xBinarySemaphore != NULL)
	{
		xTaskCreate(vHandlerTask,"Handler",256, NULL, 3, NULL);
		xTaskCreate(vPeriodicTask, "Periodic", 256, NULL, 1, NULL);
		
		vTaskStartScheduler();
	}
		
	while (1);
}
//--------------------------------------------------------------------------------

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED_Pin */
  GPIO_InitStruct.Pin = LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
// Redirect printf() to USB Virtual COM Port
int _write(int file, char *ptr, int len)
{
    while(CDC_Transmit_FS((uint8_t*) ptr, len)!=0);
    return len;
}
/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */

  /* USER CODE BEGIN 5 */

  /* USER CODE END 5 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM4 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM4) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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

