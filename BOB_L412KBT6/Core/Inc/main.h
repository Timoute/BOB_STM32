/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32l4xx_hal.h"

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
#define Servo_TX_Pin GPIO_PIN_2
#define Servo_TX_GPIO_Port GPIOA
#define Test_LED_Pin GPIO_PIN_3
#define Test_LED_GPIO_Port GPIOA
#define Stepper_Dir_4_Pin GPIO_PIN_4
#define Stepper_Dir_4_GPIO_Port GPIOA
#define Setpper_Rot_5_Pin GPIO_PIN_5
#define Setpper_Rot_5_GPIO_Port GPIOA
#define Stepper_Dir_5_Pin GPIO_PIN_6
#define Stepper_Dir_5_GPIO_Port GPIOA
#define Setpper_Rot_6_Pin GPIO_PIN_7
#define Setpper_Rot_6_GPIO_Port GPIOA
#define Stepper_Dir_6_Pin GPIO_PIN_0
#define Stepper_Dir_6_GPIO_Port GPIOB
#define Setpper_Rot_4_Pin GPIO_PIN_1
#define Setpper_Rot_4_GPIO_Port GPIOB
#define In_Setpper_Capt_Pos_Bot_Pin GPIO_PIN_8
#define In_Setpper_Capt_Pos_Bot_GPIO_Port GPIOA
#define In_Setpper_Capt_Pos_Top_Pin GPIO_PIN_9
#define In_Setpper_Capt_Pos_Top_GPIO_Port GPIOA
#define In_Setpper_Capt_Pos_3_Pin GPIO_PIN_10
#define In_Setpper_Capt_Pos_3_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
