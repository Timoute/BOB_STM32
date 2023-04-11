/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "motor.h"
#include "xl320.h"
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
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t UART1_rxBuffer[9]={0};
//mettre une taille de tableau de 10
uint8_t flag = 0;
uint8_t motor = 0;
uint16_t angle = 0;
uint8_t dir = 0;
uint8_t speed = 0;
uint16_t position_x = 0;
uint16_t position_y = 0;
uint16_t position_z = 0;


/**
 * @brief Fonction de rappel pour l'interruption de réception UART.
 *
 * Cette fonction est appelée par la fonction HAL_UART_IRQHandler() lorsqu'une interruption de réception UART
 * est déclenchée. Elle traite les données reçues et effectue les actions appropriées en fonction des données reçues.
 *
 * @param huart Pointeur vers la structure de gestion UART
 *
 * @return Void
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART1)
	{
		flag = 1;
		HAL_UART_Receive_IT(&huart1, UART1_rxBuffer, 9);

	}
}

/**
 * @brief Fonction de rappel pour l'interruption de période de temporisation (Timer).
 *
 * Cette fonction est appelée par la fonction HAL_TIM_IRQHandler() lorsqu'une interruption de période de temporisation
 * est déclenchée. Elle vérifie les instances des Timers pour déterminer quelle action doit être effectuée en fonction
 * de la valeur de la variable "speed".
 *
 * @param htim Pointeur vers la structure de gestion Timer
 *
 * @return Void
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM1)
	{
		if(speed == 1){
			motor_irq_cb(motor,dir);
		}
	}
	if (htim->Instance == TIM2){
		if(speed == 2){
			motor_irq_cb(motor,dir);
		}
	}
	if (htim->Instance == TIM6)
	{
		if(speed == 3){
			motor_irq_cb(motor,dir);
		}
	}
}

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
  MX_USART2_UART_Init();
  MX_USART1_UART_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */
	HAL_UART_Receive_IT(&huart1, UART1_rxBuffer, 9);
	HAL_TIM_Base_Start_IT(&htim1);
	HAL_TIM_Base_Start_IT(&htim2);
	HAL_TIM_Base_Start_IT(&htim6);

	//Servomotor
	xl320_write8(254,25,2);  //Allume LED
	//HAL_Delay(500);
	//xl320_write8(254,25,0); //Eteind LED
	HAL_Delay(500);
	xl320_write8(254,11,2); //Control Mode : -JointMode2
	xl320_write16(254,35,1023);//Torque Limit
	xl320_write8(254,24,1); //Torque Enable
	xl320_write8(254,3,3); //Set ID can only be written to if Torque Enable(24) is cleared to ‘0’(Torque OFF)
	HAL_Delay(500);
	xl320_write16(254,32,300);//Moving Speed
	xl320_write16(1,30,512); //test sm1  780 open, 480 close 415 410 max
	xl320_write16(2,30,512); //test sm2
	xl320_write16(3,30,512); //test sm3
	HAL_Delay(500);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
	{
		/**
		 * @brief Fonction de rappel pour l'interruption de réception UART.
		 *
		 * Cette fonction est appelée par la fonction HAL_UART_IRQHandler() lorsqu'une interruption de réception UART
		 * est déclenchée. Elle traite les données reçues et effectue les actions appropriées en fonction des données reçues.
		 *
		 * @param huart Pointeur vers la structure de gestion UART
		 *
		 * @return Aucun
		 */
		if(flag)
		{
			flag = 0;
			motor_reset();
			HAL_UART_Transmit(&huart1, UART1_rxBuffer,9, 100);

			if((UART1_rxBuffer[0] == 255) && (UART1_rxBuffer[1] == 255))
			{
				motor = UART1_rxBuffer[2];
				angle = (UART1_rxBuffer[3]<<8)+ UART1_rxBuffer[4];
				dir = UART1_rxBuffer[5];
				speed = UART1_rxBuffer[6];

				motor_dir(dir,motor,angle); // Direction, Mouvement
				//motor_set(angle);
				// pour rest le moteur 4 depuis le fin de course top, mettre angle = 105° et dir = 1

				servomotor_com(motor, angle, dir, speed);

				/* Direction des moteurs
				 * Motor 1 (Clamp) : 0 -> close
				 * 					 1 -> open
				 * Motor 2 (Wirst_Z) : 0 -> left
				 * 					   1 -> right
				 * Motor 3 (Wirst_X) : 0 -> down
				 * 					   1 -> up
				 * Motor 4 (Elbow) : 0 -> up
				 * 					 1 -> down
				 */


			}
			else
			{
				// Commande non valide
				// Allumer une led warning?

				// Ecrire un message d'erreur quelque part?
			}

		}


		xl320_write8(1,25,5); //Change couleur de LED //Address 1, 254 étant celle de Broadcast Yellow
		xl320_write8(2,25,5); //Cyan
		xl320_write8(3,25,5); // Purple

//		if (HAL_GPIO_ReadPin(In_Setpper_Capt_Pos_Top_GPIO_Port,In_Setpper_Capt_Pos_Top_Pin) == 0){ // Lumière rouge moteur mauvaise position.
//			xl320_write8(254,25,1);
//		}
//		if (HAL_GPIO_ReadPin(In_Setpper_Capt_Pos_Bot_GPIO_Port,In_Setpper_Capt_Pos_Bot_Pin) == 0){
//			xl320_write8(254,25,1);
//			}




		//				xl320_write16(254,30,400); // Goal Position
		//xl320_write16(1,30,492);
	  		/*		xl320_write16(1,30,820);
	  				HAL_Delay(1000);
	  				xl320_write16(2,30,200);
	  				HAL_Delay(1000);
	  				xl320_write16(3,30,250);
	  				HAL_Delay(1000);
	  				//xl320_write16(1,30,200);
	  				xl320_write8(254,25,0); //Couleur Led
	  				//xl320_write16(1,30,312);
	  //				xl320_write16(2,30,700); //GOAL Position
	  //				xl320_write16(1,30,532);
	  //				xl320_write16(3,30,450);
	  				xl320_write16(1,30,420);
	  				HAL_Delay(1000);
	  				xl320_write16(2,30,800);
	  				HAL_Delay(1000);
	  				xl320_write16(3,30,850);
		 HAL_Delay(500);
		 xl320_write8(254,25,0); //Couleur Led
		 */HAL_GPIO_TogglePin(Test_LED_GPIO_Port, Test_LED_Pin);
		 HAL_Delay(500);

		 //Demo
//		 xl320_write16(3,32,600);
//		 HAL_Delay(500);
//		 xl320_write16(3,30,1023);
//
//		 xl320_write16(2,30,820);
//		 xl320_write16(1,30,420);
//		 HAL_Delay(1000);
//		 xl320_write16(2,30,204);
//		 HAL_Delay(1000);
//		 xl320_write16(2,30,820);
//		 HAL_Delay(1000);
//		 xl320_write16(1,30,780);
//		 xl320_write16(2,30,204);
//
//		 xl320_write16(3,30,100);
//		 HAL_Delay(1000);


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

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 2;
  RCC_OscInitStruct.PLL.PLLN = 20;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
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
