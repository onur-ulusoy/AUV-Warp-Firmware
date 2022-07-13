/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "analogdata.h"
#include "motordrive.h"
#include "monitoring.h"
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

/* USER CODE END Variables */
/* Definitions for motorDrive */
osThreadId_t motorDriveHandle;
const osThreadAttr_t motorDrive_attributes = {
  .name = "motorDrive",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for sensorRead */
osThreadId_t sensorReadHandle;
const osThreadAttr_t sensorRead_attributes = {
  .name = "sensorRead",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for monitoring */
osThreadId_t monitoringHandle;
const osThreadAttr_t monitoring_attributes = {
  .name = "monitoring",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartMotorDrive(void *argument);
void StartSensorRead(void *argument);
void StartMonitoring(void *argument);

extern void MX_USB_DEVICE_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of motorDrive */
  motorDriveHandle = osThreadNew(StartMotorDrive, NULL, &motorDrive_attributes);

  /* creation of sensorRead */
  sensorReadHandle = osThreadNew(StartSensorRead, NULL, &sensorRead_attributes);

  /* creation of monitoring */
  monitoringHandle = osThreadNew(StartMonitoring, NULL, &monitoring_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartMotorDrive */
/**
  * @brief  Function implementing the motorDrive thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartMotorDrive */
void StartMotorDrive(void *argument)
{
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN StartMotorDrive */
  /* Infinite loop */
  for(;;)
  {
    if (cmd_ready_flag == 1){
			cmd_ready_flag = 0;
			DriveMotors((WarpCommand *)&esc_command);
		}

    osDelay(10);
  }
  /* USER CODE END StartMotorDrive */
}

/* USER CODE BEGIN Header_StartSensorRead */
/**
* @brief Function implementing the sensorRead thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartSensorRead */
void StartSensorRead(void *argument)
{
  /* USER CODE BEGIN StartSensorRead */
  /* Infinite loop */
  for(;;)
  {
    AnalogDataRequest(START_CONTINUOUS);
    osDelay(1);
  }
  /* USER CODE END StartSensorRead */
}

/* USER CODE BEGIN Header_StartMonitoring */
/**
* @brief Function implementing the monitoring thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartMonitoring */
void StartMonitoring(void *argument)
{
  /* USER CODE BEGIN StartMonitoring */
  /* Infinite loop */
  for(;;)
  {
    printSensorData((Sensors*)&sensors);
    transmitSensorData(&huart1, (Sensors*)&sensors);
    osDelay(100);
  }
  /* USER CODE END StartMonitoring */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

