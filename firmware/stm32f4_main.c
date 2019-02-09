
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"
#include "dma.h"
#include "i2c.h"
#include "i2s.h"
#include "tim.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "oscillator.h"
#include "mixer.h"
#include "wavetables.h"
#include "envelope.h"
#include "biquad.h"
#include "defines.h"
#include "sequencer.h"
#include "cs43l22.h"
#include <math.h>
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
Oscillator osc1, osc2;
Mixer mixer;
Envelope envelope;
Biquad bq;
Sequencer seq;
int16_t data_a[2];
int16_t data_b[2];
int16_t* data_ptr = data_a;
float cutoff = 1000;
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

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
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_I2S3_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

  CS43L22Init(hi2c1);
  CS43L2Start();

  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET);

  OscInit(&osc1, 80, SINE_TABLE_UINT16);
  OscInit(&osc2, 320, SAWTOOTH_TABLE_UINT16);
  MixerSetGain(&mixer, 0.3f, 0.1f);
  EnvelopeInit(&envelope, 0.2f, 0.2f, UINT16_MAX / 2, 0.4f);
  BiquadSet(&bq, bq_type_lowpass, 800.0 / SAMPLE_RATE, 0.707, 0.0);

  Sequencer_Init(&seq, 100, 3);
  Sequencer_SetPulseWidth(&seq, 0.5f);
  Sequencer_SetStepValue(&seq, 0, 70);
  Sequencer_SetStepValue(&seq, 1, 100);
  Sequencer_SetStepValue(&seq, 2, 60);
  Sequencer_SetStepValue(&seq, 3, 620);
  Sequencer_SetStepValue(&seq, 4, 690);
  Sequencer_SetStepValue(&seq, 5, 400);
  Sequencer_SetStepValue(&seq, 6, 450);
  Sequencer_SetStepValue(&seq, 7, 70);
  Sequencer_SetStepValue(&seq, 8, 100);

  HAL_I2S_Transmit_DMA(&hi2s3, (uint16_t*)data_ptr, 2);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  uint8_t previous_button_state_3 = 0;
  uint8_t previous_button_state_5 = 0;
  while (1)
  {

  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
	  if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_5) && previous_button_state_5 == 0) {
		  EnvelopeRelease(&envelope);
		  previous_button_state_5 = 1;
	  }

	  if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_5) == 0 && previous_button_state_5 == 1) {
		  OscSetFreq(&osc1, 100);
		  OscSetFreq(&osc2, 100);

		  EnvelopeTrigger(&envelope);
		  previous_button_state_5 = 0;
	  }

	  if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3) && previous_button_state_3 == 0) {
		  EnvelopeRelease(&envelope);
		  previous_button_state_3 = 1;
	  }

	  if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3) == 0 && previous_button_state_3 == 1) {
		  OscSetFreq(&osc1, 70);
		  OscSetFreq(&osc2, 70);
		  EnvelopeTrigger(&envelope);
		  previous_button_state_3 = 0;
	  }
  }
  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2S;
  PeriphClkInitStruct.PLLI2S.PLLI2SN = 50;
  PeriphClkInitStruct.PLLI2S.PLLI2SR = 2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

uint16_t prev_seq = 0;


void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s) {
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_SET);
    uint16_t seq_val = Sequencer_Get(&seq);
	if (seq_val > 0 && seq_val != prev_seq) {
	     //EnvelopeTrigger(&envelope);
	 //    EnvelopeRelease(&envelope);
	     //OscSetFreq(&osc1, seq_val);
	}

	prev_seq = seq_val;
	uint16_t val = MixerMix(&mixer, OscGetValue(&osc1), OscGetValue(&osc2));
	val = EnvelopeProcess(&envelope, val);
	val = BiquadProcess(&bq, val / (float) UINT16_MAX) * UINT16_MAX;

	int16_t val_signed = ((uint32_t) val) - (1 << 15);
	data_ptr[0] = val_signed / 2;
	data_ptr[1] = val_signed / 2;
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)	{
	if (htim->Instance != TIM2) {
		return;
	}

	//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_SET);
	uint16_t val = MixerMix(&mixer, OscGetValue(&osc1), OscGetValue(&osc2));
	//val = EnvelopeProcess(&envelope, val);
	//val = BiquadProcess(&bq, val / (float) UINT16_MAX) * UINT16_MAX;

	int16_t val_signed = ((uint32_t) val) - (1 << 15);
	data_ptr[0] = val_signed / 2;
	data_ptr[1] = val_signed / 2;
	//HAL_I2S_Transmit(&hi2s3, (uint16_t*)data_ptr, 2, 10);
	//HAL_I2S_Transmit_DMA(&hi2s3, (uint16_t*)data_ptr, 2);
	if (data_ptr == data_a) {
		data_ptr = data_b;
	} else {
		data_ptr = data_a;
	}
	//HAL_DAC_SetValue(&hdac, DAC1_CHANNEL_1, DAC_ALIGN_8B_R, val);
	//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET);

	//HAL_I2S_Transmit_IT(&hi2s3, &val, 1);

}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
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
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
