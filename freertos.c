/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include<stdio.h>

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
/* USER CODE BEGIN Variables */
extern ADC_HandleTypeDef hadc1;
volatile uint32_t g_adcValue = 0;

/* USER CODE END Variables */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

void StartADCTask(void const * argument)
{
    printf("[ADC_TASK]   Started. Reading pot every 500ms.\r\n");

    for (;;)
    {
        /* Start one ADC conversion */
        HAL_ADC_Start(&hadc1);

        /* Wait for conversion to finish — timeout 10ms */
        if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK)
        {
            /* Read 12-bit result (0 to 4095) */
            uint32_t raw = HAL_ADC_GetValue(&hadc1);

            /* Store in global for AlertTask */
            g_adcValue = raw;

            /* Map to percentage */
            uint32_t percent = (raw * 100) / 4095;

            printf("[ADC_TASK]   Raw: %4lu | Level: %3lu%%\r\n",
                   raw, percent);
        }
        else
        {
            printf("[ADC_TASK]   ERROR: Conversion timeout!\r\n");
        }

        /* Release CPU for 500ms */
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void StartAlertTask(void const * argument)
{
    printf("[ALERT_TASK] Started. Monitoring threshold > 3000.\r\n");

    const uint32_t THRESHOLD = 3000;

    for (;;)
    {
        /* Read the shared value ADCTask wrote */
        uint32_t currentVal = g_adcValue;

        /* Map to percentage */
        uint32_t percent = (currentVal * 100) / 4095;

        /* Check threshold and print accordingly */
        if (currentVal > THRESHOLD)
        {
            printf("[ALERT_TASK] *** HIGH ALERT! Value: %lu (%lu%%) ***\r\n",
                   currentVal, percent);
        }
        else
        {
            printf("[ALERT_TASK] Normal. Value: %lu (%lu%%)\r\n",
                   currentVal, percent);
        }

        /* Release CPU for 600ms */
        vTaskDelay(pdMS_TO_TICKS(600));
    }
}

/* USER CODE END Application */

