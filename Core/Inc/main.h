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
#define LED_D2_Pin GPIO_PIN_6
#define LED_D2_GPIO_Port GPIOA
#define LED_D3_Pin GPIO_PIN_7
#define LED_D3_GPIO_Port GPIOA
#define BT_ENABLE_Pin GPIO_PIN_5
#define BT_ENABLE_GPIO_Port GPIOC
#define BT_RST_Pin GPIO_PIN_11
#define BT_RST_GPIO_Port GPIOB
#define BT_NORMAL_Pin GPIO_PIN_12
#define BT_NORMAL_GPIO_Port GPIOB
#define BT_PILA_Pin GPIO_PIN_13
#define BT_PILA_GPIO_Port GPIOB
#define BT_SINUS_Pin GPIO_PIN_14
#define BT_SINUS_GPIO_Port GPIOB
#define BT_PACE_Pin GPIO_PIN_15
#define BT_PACE_GPIO_Port GPIOB
#define LED_NORMAL_Pin GPIO_PIN_6
#define LED_NORMAL_GPIO_Port GPIOC
#define LED_MEANDR_Pin GPIO_PIN_7
#define LED_MEANDR_GPIO_Port GPIOC
#define LED_PILA_Pin GPIO_PIN_8
#define LED_PILA_GPIO_Port GPIOC
#define BT_LSTAT_Pin GPIO_PIN_8
#define BT_LSTAT_GPIO_Port GPIOA
#define BT_TX_Pin GPIO_PIN_9
#define BT_TX_GPIO_Port GPIOA
#define BT_RX_Pin GPIO_PIN_10
#define BT_RX_GPIO_Port GPIOA
#define BT_BOOT_Pin GPIO_PIN_15
#define BT_BOOT_GPIO_Port GPIOA
#define LED_SINUS_Pin GPIO_PIN_11
#define LED_SINUS_GPIO_Port GPIOC
#define BT_MEANDR_Pin GPIO_PIN_12
#define BT_MEANDR_GPIO_Port GPIOC
#define REO_OUT0_Pin GPIO_PIN_2
#define REO_OUT0_GPIO_Port GPIOD
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
