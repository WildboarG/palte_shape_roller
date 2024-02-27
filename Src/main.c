/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "dma.h"
#include "spi.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32f4xx.h"
#include "sys.h"
#include "delay.h"
#include "brd_cfg.h"
#include "M8266HostIf.h"
#include "M8266WIFI_ops.h"
#include "M8266WIFIDrv.h"
#include "math.h"
#include "errcode.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define BUFFER_SIZE 720
#define TD 2
#define ctrl 1
uint8_t buffer[TD][BUFFER_SIZE];
uint8_t cur = 1;
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

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	uint8_t success=0;
	uint8_t init_status = 0;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
uint8_t wifi_init(void);
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_SPI1_Init();
  MX_ADC3_Init();
  /* USER CODE BEGIN 2 */
   delay_Init();
   M8266HostIf_Init();	 //wifi模块硬件初始�?????
  if (init_status != 0){
    wifi_init_err();
  }
	success = M8266WIFI_Module_Init_Via_SPI();
  if (success != 1){
      wifi_spi_err();
      while(1);
  }
	if(wifi_init()==0){
    wifi_workmode_err();
    while(1);
  }  //wifi模块工作模式初始化，报头报尾初始�?????
	
	HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED5_GPIO_Port, LED5_Pin, GPIO_PIN_SET);
    HAL_ADC_Start_DMA(&hadc3, (uint32_t *)buffer[0], BUFFER_SIZE);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
uint8_t wifi_init(void)  //wifi初始化函�?????
{
	u8 ssid[9]={"HYGS_2G"};
	u8 password[10]={"HYGS3305"};
  u8 con;
	M8266WIFI_SPI_Set_Opmode(1, 0, NULL);  //设定WIFI工作在STA模式   1=STA 2=AP 3=STA+AP
	M8266WIFI_SPI_STA_Connect_Ap(ssid,password,0,0,NULL);
	M8266WIFI_SPI_Config_STA_StaticIpAddr("192.168.0.111","192.168.0.1","255.255.255.0",0,NULL);
	
	con = M8266WIFI_SPI_Setup_Connection(2,4321,"192.168.0.1",4321,ctrl,30,NULL);//创建连接1=TCP clint，WiFi模块端口4321，WiFi模块地址192.168.0.200
	//con = M8266WIFI_SPI_Setup_Connection(0,4321,"192.168.0.110",4321,ctrl,30,NULL); //udp 0=UDP
  if(con==0){
   return 0;
  }
  return 1;
}
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
    if (cur == 1) {
       // HAL_ADC_Stop_DMA(hadc);  // Stop ongoing DMA
        HAL_ADC_Start_DMA(&hadc3, (uint32_t*)buffer[1], BUFFER_SIZE);  // Start DMA with the second buffer
        M8266WIFI_SPI_Send_Data(buffer[0],720,ctrl,NULL);
		cur=2;
    }
    else {
       // HAL_ADC_Stop_DMA(hadc);
        HAL_ADC_Start_DMA(&hadc3, (uint32_t*)buffer[0], BUFFER_SIZE);
        M8266WIFI_SPI_Send_Data(buffer[1],720,ctrl,NULL);
        cur=1;
    }
	//send_allowed = 1;
}
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
