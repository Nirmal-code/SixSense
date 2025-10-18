/**
 ******************************************************************************
 * @file    pheripheral.cpp
 * @brief   Peripheral functions.
 ******************************************************************************
 */
#ifdef STM_BUILD
#include "fatfs.h"
#include "hardware_interface/system/peripheral.h"

static void SystemClock_Config();
static void MX_USART3_UART_Init();
static void MX_SAI_1_Init();
static void MX_SPI1_Init();

static void MX_GPIO_Init();

UART_HandleTypeDef huart3;
SPI_HandleTypeDef SD_SPI_HANDLE;

#define SD_CS_Pin GPIO_PIN_4
#define SD_CS_GPIO_Port GPIOA

static SAI_HandleTypeDef hsai_BlockA1;



void setupPeripherals() {

  // STM32F7xx HAL library initialization. This sets up the microcontroller's
  // peripherals using the Hardware Abstraction Layer (HAL).
  HAL_Init();

  // Configure the system clock to 216 MHz
  SystemClock_Config();

  // Set buffer to flush immediately.
  setvbuf(stdout, NULL, _IONBF, 0);

  // Initialize Board Support Language (BSP) for LED3
  BSP_LED_Init(LED3);

  // Set up and intial UART pherrials.
  MX_USART3_UART_Init();

  // Set up SAI_1
  MX_SAI_1_Init();

  // Set up logging sd card and FATFS
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_FATFS_Init();

}

static void SystemClock_Config() {
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 8;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
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
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_GPIO_Init() {
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SD_CS_GPIO_Port, SD_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : SD_CS_Pin */
  GPIO_InitStruct.Pin = SD_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SD_CS_GPIO_Port, &GPIO_InitStruct);
}

static void MX_SPI1_Init()
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  SD_SPI_HANDLE.Instance = SPI1;
  SD_SPI_HANDLE.Init.Mode = SPI_MODE_MASTER;
  SD_SPI_HANDLE.Init.Direction = SPI_DIRECTION_2LINES;
  SD_SPI_HANDLE.Init.DataSize = SPI_DATASIZE_8BIT;
  SD_SPI_HANDLE.Init.CLKPolarity = SPI_POLARITY_LOW;
  SD_SPI_HANDLE.Init.CLKPhase = SPI_PHASE_1EDGE;
  SD_SPI_HANDLE.Init.NSS = SPI_NSS_SOFT;
  SD_SPI_HANDLE.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
  SD_SPI_HANDLE.Init.FirstBit = SPI_FIRSTBIT_MSB;
  SD_SPI_HANDLE.Init.TIMode = SPI_TIMODE_DISABLE;
  SD_SPI_HANDLE.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  SD_SPI_HANDLE.Init.CRCPolynomial = 7;
  SD_SPI_HANDLE.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  SD_SPI_HANDLE.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&SD_SPI_HANDLE) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

static void MX_SAI_1_Init()
{
  hsai_BlockA1.Instance = SAI1_Block_A;
  hsai_BlockA1.Init.AudioMode = SAI_MODEMASTER_RX;
  hsai_BlockA1.Init.Synchro = SAI_ASYNCHRONOUS;
  hsai_BlockA1.Init.OutputDrive = SAI_OUTPUTDRIVE_DISABLE;
  hsai_BlockA1.Init.NoDivider = SAI_MASTERDIVIDER_ENABLE;
  hsai_BlockA1.Init.FIFOThreshold = SAI_FIFOTHRESHOLD_EMPTY;
  hsai_BlockA1.Init.AudioFrequency = SAI_AUDIO_FREQUENCY_48K;
  hsai_BlockA1.Init.SynchroExt = SAI_SYNCEXT_DISABLE;
  hsai_BlockA1.Init.MonoStereoMode = SAI_MONOMODE;
  hsai_BlockA1.Init.CompandingMode = SAI_NOCOMPANDING;
  if (HAL_SAI_InitProtocol(&hsai_BlockA1, SAI_I2S_STANDARD, SAI_PROTOCOL_DATASIZE_24BIT, 2) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_USART3_UART_Init() {
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

  if (HAL_UART_Init(&huart3) != HAL_OK) {
    Error_Handler();
  }
}

void Error_Handler() {
  // Turn LED3 on.
  BSP_LED_On(LED3);
  while (1) {
  }
}

SAI_HandleTypeDef* getSAI_Handle()
{
  return &hsai_BlockA1;
}

#endif
