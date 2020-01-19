/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#define LED_SEGMENT_P_Pin GPIO_PIN_14
#define LED_SEGMENT_P_GPIO_Port GPIOC
#define LED_SEGMENT_C_Pin GPIO_PIN_15
#define LED_SEGMENT_C_GPIO_Port GPIOC
#define LED_SEGMENT_B_Pin GPIO_PIN_1
#define LED_SEGMENT_B_GPIO_Port GPIOA
#define LED_SEGMENT_D_Pin GPIO_PIN_2
#define LED_SEGMENT_D_GPIO_Port GPIOA
#define LED_SEGMENT_E_Pin GPIO_PIN_3
#define LED_SEGMENT_E_GPIO_Port GPIOA
#define LED_SEGMENT_A_Pin GPIO_PIN_4
#define LED_SEGMENT_A_GPIO_Port GPIOA
#define LED_SEGMENT_F_Pin GPIO_PIN_6
#define LED_SEGMENT_F_GPIO_Port GPIOA
#define LED_SEGMENT_G_Pin GPIO_PIN_7
#define LED_SEGMENT_G_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
