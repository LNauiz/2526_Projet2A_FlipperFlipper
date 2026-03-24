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
#include "adc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SPI1_CS_Pin GPIO_PIN_6
#define SPI1_CS_Port GPIOB

uint8_t tx[16] = { 0 };     // dummy bytes to clock the slave
uint8_t rx[16] = { 0 };     // buffer for sensor data
uint16_t sensor[8];		  // nombre de capteurs
int score = 0; // Initialisation du compteur à 0
int deja_compte = 0; // Empêchera un effet rebond "flag"

#define CS_LOW()  HAL_GPIO_WritePin(SPI1_CS_Port, SPI1_CS_Pin, GPIO_PIN_RESET)
#define CS_HIGH() HAL_GPIO_WritePin(SPI1_CS_Port, SPI1_CS_Pin, GPIO_PIN_SET)

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
int __io_putchar(int ch) {
	HAL_UART_Transmit(&huart2, (uint8_t*) &ch, 1, HAL_MAX_DELAY);
	return ch;
}

void read_and_decode_sensors(void) {
    for (int i = 0; i < 8; i++) {
        uint8_t tx_buff[3];
        uint8_t rx_buff[3];

        // Construction de l'octet de commande pour MCP3208
        // Byte 0: Start bit (0x04)
        // Byte 1: Single/Diff + Channel D2, D1, D0
        // Pour MCP3208: Start=1, SGL=1, D2-D0=channel

#define START_BIT 0x04
#define SINGLE_BIT 0x02
#define D2_POS 0x01

        uint8_t d2 = ((i >> 2) & D2_POS);
        tx_buff[0] = START_BIT | SINGLE_BIT | d2; // Start bit + SGL + D2
        tx_buff[1] = (i << 6) & 0xC0;          // D1 + D0 + dummies
        tx_buff[2] = 0x00;                     // Dummy byte

        CS_LOW();
        // On envoie 3 octets pour recevoir la réponse d'un canal
        HAL_SPI_TransmitReceive(&hspi1, tx_buff, rx_buff, 3, 10);
        CS_HIGH();

        // Reconstruction sur 12 bits (les données chevauchent les octets 1 et 2)
        // MCP3208 : Les 4 derniers bits de rx[1] et tout rx[2]
        sensor[i] = ((rx_buff[1] & 0x0F) << 8) | rx_buff[2];

        // Petit délai pour laisser l'ADC respirer (optionnel selon vitesse SPI)
        // for(int k=0; k<100; k++);
    }
}

void print_sensors(void) {
	char buffer[100];
	int len = snprintf(buffer, sizeof(buffer),
			"S: %4d %4d %4d %4d %4d %4d %4d %4d\r\n", sensor[0], sensor[1],
			sensor[2], sensor[3], sensor[4], sensor[5], sensor[6], sensor[7]);
	//HAL_UART_Transmit(&huart2, (uint8_t*) buffer, len, HAL_MAX_DELAY);
	if (sensor[0]<1000 && deja_compte == 0){
		score+=50;
		deja_compte=1; //On bloque le comptage
		len = snprintf(buffer, sizeof(buffer), "Score: %d\n\r", score);
		HAL_UART_Transmit(&huart2, (uint8_t*) buffer, len, HAL_MAX_DELAY);
	}
	if (sensor[0]>1000){
		deja_compte=0; //On réautorise le comptage
	}
	if (sensor[1]<1000 && deja_compte == 0){
		score+=50;
		deja_compte=1; //On bloque le comptage
		len = snprintf(buffer, sizeof(buffer), "Score: %d\n\r", score);
		HAL_UART_Transmit(&huart2, (uint8_t*) buffer, len, HAL_MAX_DELAY);
	}
	if (sensor[1]>1000){
		deja_compte=0; //On réautorise le comptage
	}
	if (sensor[2]<1000 && deja_compte == 0){
		score+=50;
		deja_compte=1; //On bloque le comptage
		len = snprintf(buffer, sizeof(buffer), "Score: %d\n\r", score);
		HAL_UART_Transmit(&huart2, (uint8_t*) buffer, len, HAL_MAX_DELAY);
	}
	if (sensor[2]>1000){
		deja_compte=0; //On réautorise le comptage
	}
	if (sensor[3]<1000 && deja_compte == 0){
		score+=50;
		deja_compte=1; //On bloque le comptage
		len = snprintf(buffer, sizeof(buffer), "Score: %d\n\r", score);
		HAL_UART_Transmit(&huart2, (uint8_t*) buffer, len, HAL_MAX_DELAY);
	}
	if (sensor[3]>1000){
		deja_compte=0; //On réautorise le comptage
	}
	if (sensor[4]<1000 && deja_compte == 0){
		score+=50;
		deja_compte=1; //On bloque le comptage
		len = snprintf(buffer, sizeof(buffer), "Score: %d\n\r", score);
		HAL_UART_Transmit(&huart2, (uint8_t*) buffer, len, HAL_MAX_DELAY);
	}
	if (sensor[4]>1000){
		deja_compte=0; //On réautorise le comptage
	}
	if (sensor[5]<1000 && deja_compte == 0){
		score+=50;
		deja_compte=1; //On bloque le comptage
		len = snprintf(buffer, sizeof(buffer), "Score: %d\n\r", score);
		HAL_UART_Transmit(&huart2, (uint8_t*) buffer, len, HAL_MAX_DELAY);
	}
	if (sensor[5]>1000){
		deja_compte=0; //On réautorise le comptage
	}
	if (sensor[6]<1000 && deja_compte == 0){
		score+=50;
		deja_compte=1; //On bloque le comptage
		len = snprintf(buffer, sizeof(buffer), "Score: %d\n\r", score);
		HAL_UART_Transmit(&huart2, (uint8_t*) buffer, len, HAL_MAX_DELAY);
	}
	if (sensor[6]>1000){
		deja_compte=0; //On réautorise le comptage
	}
	if (sensor[7]<1000 && deja_compte == 0){
		score+=50;
		deja_compte=1; //On bloque le comptage
		len = snprintf(buffer, sizeof(buffer), "Score: %d\n\r", score);
		HAL_UART_Transmit(&huart2, (uint8_t*) buffer, len, HAL_MAX_DELAY);
	}
	if (sensor[7]>1000){
		deja_compte=0; //On réautorise le comptage
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
  MX_SPI2_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  MX_ADC1_Init();
  MX_TIM4_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
	CS_HIGH();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

	while (1) {
		read_and_decode_sensors();
		print_sensors();
		HAL_Delay(10); //besoin ajustement (balle à 6m/s sur 3,4mm)

	}
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
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
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6) {
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
}
  /* USER CODE END Error_Handler_Debug */


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
