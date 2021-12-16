/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

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
#define CONV1_SW_Pin GPIO_PIN_13
#define CONV1_SW_GPIO_Port GPIOC
#define ESC1_A_Pin GPIO_PIN_0
#define ESC1_A_GPIO_Port GPIOA
#define ESC2_A_Pin GPIO_PIN_1
#define ESC2_A_GPIO_Port GPIOA
#define ESC3_A_Pin GPIO_PIN_2
#define ESC3_A_GPIO_Port GPIOA
#define ESC4_A_Pin GPIO_PIN_3
#define ESC4_A_GPIO_Port GPIOA
#define ESC5_A_Pin GPIO_PIN_4
#define ESC5_A_GPIO_Port GPIOA
#define ESC6_A_Pin GPIO_PIN_5
#define ESC6_A_GPIO_Port GPIOA
#define ESC7_A_Pin GPIO_PIN_6
#define ESC7_A_GPIO_Port GPIOA
#define ESC8_A_Pin GPIO_PIN_7
#define ESC8_A_GPIO_Port GPIOA
#define BATT_V_Pin GPIO_PIN_0
#define BATT_V_GPIO_Port GPIOB
#define BATT_A_Pin GPIO_PIN_1
#define BATT_A_GPIO_Port GPIOB
#define CONV2_SW_Pin GPIO_PIN_2
#define CONV2_SW_GPIO_Port GPIOB
#define CONV1_V_Pin GPIO_PIN_13
#define CONV1_V_GPIO_Port GPIOB
#define CONV1_A_Pin GPIO_PIN_14
#define CONV1_A_GPIO_Port GPIOB
#define CONV2_V_Pin GPIO_PIN_15
#define CONV2_V_GPIO_Port GPIOB
#define CONV2_A_Pin GPIO_PIN_8
#define CONV2_A_GPIO_Port GPIOA
#define ESC1_PWM_Pin GPIO_PIN_15
#define ESC1_PWM_GPIO_Port GPIOA
#define ESC2_PWM_Pin GPIO_PIN_3
#define ESC2_PWM_GPIO_Port GPIOB
#define ESC3_PWM_Pin GPIO_PIN_4
#define ESC3_PWM_GPIO_Port GPIOB
#define ESC4_PWM_Pin GPIO_PIN_5
#define ESC4_PWM_GPIO_Port GPIOB
#define ESC5_PWM_Pin GPIO_PIN_6
#define ESC5_PWM_GPIO_Port GPIOB
#define ESC6_PWM_Pin GPIO_PIN_7
#define ESC6_PWM_GPIO_Port GPIOB
#define ESC7_PWM_Pin GPIO_PIN_8
#define ESC7_PWM_GPIO_Port GPIOB
#define ESC8_PWM_Pin GPIO_PIN_9
#define ESC8_PWM_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
