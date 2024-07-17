/*
 * main.c
 *
 *  Created on: Jul 14, 2024
 *      Author: daniel
 */

#include "main.h"

TIM_HandleTypeDef htim10;
TIM_HandleTypeDef htim11;
TIM_HandleTypeDef htim12;

TIM_OC_InitTypeDef *sConfig_nonConst12;
TIM_OC_InitTypeDef *sConfig_nonConst10;
TIM_OC_InitTypeDef *sConfig_nonConst11;



TIM_Base_InitTypeDef Timer_Init;
UART_HandleTypeDef huart6;

uint32_t ccr_content;

char msg[100];

int main() {
	// Init HAL
	  HAL_Init();
	  memset(&htim12, 0, sizeof(htim12));
	  memset(&htim10, 0, sizeof(htim10));
	  memset(&htim11, 0, sizeof(htim11));


	  /***********************************************************************
	   ***************             Configure Clock             ***************
	   ***********************************************************************/
	  SystemClock_Config();


	  /***********************************************************************
	   ***************           Configure BTN GPIO           ****************
	   ***********************************************************************/
	  BTN_GPIO_Init();


	  /***********************************************************************
	   ***************             Configure UART6             ***************
	   ***********************************************************************/


	  memset(&huart6, 0, sizeof(huart6));
	  UART6_Init();

	  memset(msg, 0, strlen(msg));
	  sprintf(msg, "\e[1;1H\e[2J");
	  if(HAL_UART_Transmit(&huart6, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY) != HAL_OK) {
		Error_Handler();
	  }
	  while(HAL_GPIO_ReadPin(GPIOJ, GPIO_PIN_1) == GPIO_PIN_SET);
	  memset(msg, 0, strlen(msg));
	  sprintf(msg, "\nUse the Timer Output Compare Modes to produce the following frequencies:\n");
	  if(HAL_UART_Transmit(&huart6, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY) != HAL_OK) {
		Error_Handler();
	  }
	  memset(msg, 0, strlen(msg));
	  sprintf(msg, "------------------------------------------------------------------\r\n");
	  if(HAL_UART_Transmit(&huart6, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY) != HAL_OK) {
		Error_Handler();
	  }
	  memset(msg, 0, strlen(msg));
	  sprintf(msg, "\t500Hz -> TIM12_Channel2\tLogic_Analyzer CH0\n\t1Khz  -> TIM10_Channel1\tLogic_Analyzer CH1\n\t2KHz  -> TIM11_Channel1\tLogic_Analyzer CH2\n\t4Khz  -> TIM12_Channel1\tLogic_Analyzer CH3\n");
	  if(HAL_UART_Transmit(&huart6, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY) != HAL_OK) {
		Error_Handler();
	  }

	  memset(msg, 0, strlen(msg));
	  sprintf(msg, "==================================================================\r\n");
	  if(HAL_UART_Transmit(&huart6, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY) != HAL_OK) {
		Error_Handler();
	  }

	  memset(msg, 0, strlen(msg));
	  sprintf(msg, "Press the PWR/START button to begin...\n");
	  if(HAL_UART_Transmit(&huart6, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY) != HAL_OK) {
		Error_Handler();
	  }

	  /***********************************************************************
	   ***************             Initialize Timer            ***************
	   ***********************************************************************/
		// Start Timer Peripheral Clock
		__HAL_RCC_TIM10_CLK_ENABLE();
		__HAL_RCC_TIM11_CLK_ENABLE();
		__HAL_RCC_TIM12_CLK_ENABLE();
	  TIM_TypeDef *timerBaseAddr;
	  uint32_t Channel;
	  memset(&Timer_Init, 0, sizeof(Timer_Init));
	  Timer_Init.CounterMode = TIM_COUNTERMODE_UP;
	  Timer_Init.Prescaler = 1;
	  Timer_Init.Period = 0xFFFFFFFF;

	  /*
	   * Configure TIM Output Compare structure and clear
	   */
	  memset(&sConfig_nonConst12, 0, sizeof(sConfig_nonConst12));
	  memset(&sConfig_nonConst10, 0, sizeof(sConfig_nonConst10));
	  memset(&sConfig_nonConst11, 0, sizeof(sConfig_nonConst11));
	  sConfig_nonConst12->OCMode = TIM_OCMODE_TOGGLE;
	  sConfig_nonConst12->OCPolarity = TIM_OCPOLARITY_HIGH;
	  sConfig_nonConst10->OCMode = TIM_OCMODE_TOGGLE;
	  sConfig_nonConst10->OCPolarity = TIM_OCPOLARITY_HIGH;
	  sConfig_nonConst11->OCMode = TIM_OCMODE_TOGGLE;
	  sConfig_nonConst11->OCPolarity = TIM_OCPOLARITY_HIGH;

	  for(int i = 0; i < 4; i++){
		  if(i == 0) {
			  timerBaseAddr = TIM12;
			  Channel = TIM_CHANNEL_2;
			  sConfig_nonConst12->Pulse = 25000;
			  Timer_Config(timerBaseAddr, sConfig_nonConst12, Channel, i);

		  }
		  else if( i == 1 ) {
			  timerBaseAddr = TIM10;
			  Channel = TIM_CHANNEL_1;
			  sConfig_nonConst10->Pulse = 12500;
			  Timer_Config(timerBaseAddr, sConfig_nonConst10, Channel, i);
		  }
		  else if( i == 2 ) {
			  timerBaseAddr = TIM11;
			  Channel = TIM_CHANNEL_1;
			  sConfig_nonConst11->Pulse = 6250;
			  Timer_Config(timerBaseAddr, sConfig_nonConst11, Channel, i);
		  }
		  else {
			  timerBaseAddr = TIM12;
			  Channel = TIM_CHANNEL_1;
			  sConfig_nonConst12->Pulse = 3125;
			  Timer_Config(timerBaseAddr, sConfig_nonConst12, Channel, i);
		  }

	  }
	  uint16_t waitVar = 0;
	  while(waitVar < 60000) {
		  waitVar++;
	  }
	  while(waitVar > 0) {
		  waitVar--;
	  }
	  while(waitVar < 60000) {
		  waitVar++;
	  }
	  while(waitVar > 0) {
		  waitVar--;
	  }


	  while(HAL_GPIO_ReadPin(GPIOJ, GPIO_PIN_1) == GPIO_PIN_SET);

	  /***********************************************************************
	   ***************               Start Timer               ***************
	   ***********************************************************************/
	  if (HAL_TIM_OC_Start_IT(&htim12, TIM_CHANNEL_2) != HAL_OK) {
		  Error_Handler();
	  }

	  if (HAL_TIM_OC_Start_IT(&htim10, TIM_CHANNEL_1) != HAL_OK) {
		  Error_Handler();
	  }

	  if (HAL_TIM_OC_Start_IT(&htim11, TIM_CHANNEL_1) != HAL_OK) {
		  Error_Handler();
	  }

	  if (HAL_TIM_OC_Start_IT(&htim12, TIM_CHANNEL_1) != HAL_OK) {
		  Error_Handler();
	  }

	  while(1);

	return 0;
}


void SystemClock_Config(void) {
	RCC_OscInitTypeDef  oscInit;
	RCC_ClkInitTypeDef  clkInit;

	oscInit.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_LSE;
	oscInit.HSEState = RCC_HSE_BYPASS;
	oscInit.LSEState = RCC_LSE_ON;
	oscInit.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	oscInit.PLL.PLLM = 19;
	oscInit.PLL.PLLN = 152;
	oscInit.PLL.PLLP = RCC_PLLP_DIV2;
	oscInit.PLL.PLLState = RCC_PLL_ON;
	if(HAL_RCC_OscConfig(&oscInit) != HAL_OK) {
		Error_Handler();
	}

	clkInit.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	clkInit.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	clkInit.AHBCLKDivider = RCC_SYSCLK_DIV2;
	clkInit.APB1CLKDivider = RCC_HCLK_DIV1;
	clkInit.APB2CLKDivider = RCC_HCLK_DIV1;
	if( HAL_RCC_ClockConfig(&clkInit, FLASH_ACR_LATENCY_1WS) != HAL_OK) {
		Error_Handler();
	}

	__HAL_RCC_HSI_DISABLE(); // Turn off the HSI to save power now

	// Reconfigure Systick now to work withe the new System Clock Frequency
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

}

void Timer_Config(TIM_TypeDef *Instance, TIM_OC_InitTypeDef *sConfig_nonConst, uint32_t Channel, int i) {

	if(i == 0 || i == 3) {
		htim12.Instance = Instance;
		htim12.Init = Timer_Init;
		if(HAL_TIM_OC_Init(&htim12) != HAL_OK) {
			Error_Handler();
		};
		  // Configure the Channel for the Input Capture Timer
			if(HAL_TIM_OC_ConfigChannel(&htim12, sConfig_nonConst, Channel) != HAL_OK) {
				Error_Handler();
			};
	}
	else if(i == 1) {
		htim10.Instance = Instance;
		htim10.Init = Timer_Init;
		if(HAL_TIM_OC_Init(&htim10) != HAL_OK) {
			Error_Handler();
		};
		  // Configure the Channel for the Input Capture Timer
			if(HAL_TIM_OC_ConfigChannel(&htim10, sConfig_nonConst, Channel) != HAL_OK) {
				Error_Handler();
			};
	}
	else {
		htim11.Instance = Instance;
		htim11.Init = Timer_Init;
		if(HAL_TIM_OC_Init(&htim11) != HAL_OK) {
			Error_Handler();
		};
		  // Configure the Channel for the Input Capture Timer
			if(HAL_TIM_OC_ConfigChannel(&htim11, sConfig_nonConst, Channel) != HAL_OK) {
				Error_Handler();
			};
	}
}

void UART6_Init() {
	// Start USART6 Clock
	__HAL_RCC_USART6_CLK_ENABLE();

	// Initialize USART6 Handle
	huart6.Instance = USART6;
	huart6.Init.BaudRate = 115200;
	huart6.Init.WordLength = UART_WORDLENGTH_8B;
	huart6.Init.StopBits = UART_STOPBITS_1;
	huart6.Init.Mode = UART_MODE_TX;
	huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	if(HAL_UART_Init(&huart6) != HAL_OK) {
		Error_Handler();
	}
}

void BTN_GPIO_Init(void) {
	__HAL_RCC_GPIOJ_CLK_ENABLE();
	GPIO_InitTypeDef gpioJ_Init;
	memset(&gpioJ_Init, 0, sizeof(gpioJ_Init));
	gpioJ_Init.Pin = GPIO_PIN_1;
	gpioJ_Init.Mode = GPIO_MODE_INPUT;
	gpioJ_Init.Pull = GPIO_PULLUP;
	gpioJ_Init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOJ, &gpioJ_Init);

}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	  memset(msg, 0, strlen(msg));
	  sprintf(msg, "0\n");
	  if(HAL_UART_Transmit(&huart6, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY) != HAL_OK) {
		Error_Handler();
	  }
}


void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim) {
	if(htim->Instance == TIM10) {
		ccr_content = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1, ccr_content + 12500);
	}
	else if(htim->Instance == TIM11) {
		ccr_content = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1, ccr_content + 6250);
	}
	else {
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) {
			ccr_content = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
			__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1, ccr_content + 3125);
		}
		else {
			ccr_content = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
			__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2, ccr_content + 25000);
		}
	}
}


void Error_Handler(void) {
	while(1);
}

