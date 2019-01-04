
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
#include "stm32f1xx_hal.h"
#include "rtc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include <stdlib.h> // rand
	#include <string.h>
	#include "cipherglot17_sm.h"
	// TIM2 Sound on port PA1
	// TIM3 timer LED Off = 2 sec
	// TIM4 prompt current cipher
	// UART3 -> PB10 for DEBUG, speed = 38400;
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

#define END_NUMBER 995
volatile uint32_t current_cipher_u32 = 0 ;

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	uint32_t Cipher[END_NUMBER] = {3,
		1,4,1,5,9,2,6,5,3,5,8,9,7,9,3,2,3,8,4,6,2,6,4,3,3,8,3,2,7,9,5,0,2,8,8,4,1,9,7,1,6,9,3,9,9,3,7,5,1,0,
		5,8,2,0,9,7,4,9,4,4,5,9,2,3,0,7,8,1,6,4,0,6,2,8,6,2,0,8,9,9,8,6,2,8,0,3,4,8,2,5,3,4,2,1,1,7,0,6,7,9,
		8,2,1,4,8,0,8,6,5,1,3,2,8,2,3,0,6,6,4,7,0,9,3,8,4,4,6,0,9,5,5,0,5,8,2,2,3,1,7,2,5,3,5,9,4,0,8,1,2,8,
		4,8,1,1,1,7,4,5,0,2,8,4,1,0,2,7,0,1,9,3,8,5,2,1,1,0,5,5,5,9,6,4,4,6,2,2,9,4,8,9,5,4,9,3,0,3,8,1,9,6
		//		  44288 10975 66593 34461 28475  64823 37867 83165 27120 19091
		//		  45648 56692 34603 48610 45432  66482 13393 60726 02491 41273
		//		  72458 70066 06315 58817 48815  20920 96282 92540 91715 36436
		//		  78925 90360 01133 05305 48820  46652 13841 46951 94151 16094
		//		  33057 27036 57595 91953 09218  61173 81932 61179 31051 18548
		//		  07446 23799 62749 56735 18857  52724 89122 79381 83011 94912
		//		  98336 73362 44065 66430 86021  39494 63952 24737 19070 21798
		//		  60943 70277 05392 17176 29317  67523 84674 81846 76694 05132
		//		  00056 81271 45263 56082 77857  71342 75778 96091 73637 17872
		//		  14684 40901 22495 34301 46549  58537 10507 92279 68925 89235
		//		  42019 95611 21290 21960 86403  44181 59813 62977 47713 09960
		//		  51870 72113 49999 99837 29780  49951 05973 17328 16096 31859
		//		  50244 59455 34690 83026 42522  30825 33446 85035 26193 11881
		//		  71010 00313 78387 52886 58753  32083 81420 61717 76691 47303
		//		  59825 34904 28755 46873 11595  62863 88235 37875 93751 95778
		//		  18577 80532 17122 68066 13001  92787 66111 95909 21642 01989
	};

	uint32_t Error          = 0 ;
	uint32_t Start_Number   = 0 ;
	uint32_t Start_Numb[3]  = {0,0,0} ;
	uint32_t Total_Number   = 0 ;
	uint32_t Current_Number = 0 ;
	RTC_TimeTypeDef TimeStruct  ;
	uint32_t StopSeconds    = 0 ;
	uint32_t StopMinutes    = 0 ;
	uint32_t StopHours      = 0 ;
	uint32_t TypeOfGame		= 3 ; // Pi or Old
	char DataChar[100];

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
  MX_RTC_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start_IT(&htim3); // start TIM3 interupt
	HAL_TIM_Base_Start_IT(&htim4); // start TIM4 prompt

	sprintf(DataChar,"\r\n\r\nUART3 for debug Start\r\nSpeed 38400\r\n");
	HAL_UART_Transmit(&huart3, (uint8_t *)DataChar, strlen(DataChar), 100);

	// Test_Segment();
	TypeOfGame = TestLED();

   if (TypeOfGame == 3)
   {
	  Start_Number = 0;
   }
   else
   {
	  Start_Number = 5 ;
	  do
	  {
		  Cipher[1] = rand()%10; // 0..9
		  Cipher[2] = rand()%10;
		  Cipher[3] = rand()%10;
		  Cipher[4] = rand()%6 + 10; // A .. F
	  }
	  while (	(Cipher[1] == Cipher[2])
			 ||	(Cipher[2] == Cipher[3])
			 ||	(Cipher[3] == Cipher[1])  );
   } // end if

   Total_Number   = Start_Number ;
   Current_Number = Start_Number ;
	sprintf(DataChar,"Init - Ok\r\n");
	HAL_UART_Transmit(&huart3, (uint8_t *)DataChar, strlen(DataChar), 100);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	if (Total_Number == Start_Number)
		{
			TimeStruct.Hours   = 0;
			TimeStruct.Minutes = 0;
			TimeStruct.Seconds = 0;
			HAL_RTC_SetTime( &hrtc, &TimeStruct, RTC_FORMAT_BIN );
		}

	if (TypeOfGame != 3)
	{
		if ( Total_Number %4 == 0 )
		{
			do
			{
				Cipher[Total_Number] = rand()%6 + 10; // A .. F
			}
			while ( (Cipher[Total_Number] == Cipher[Total_Number - 4]) );
		}
		else
		{
			do
			{
				Cipher[Total_Number] = rand()%10; // 0x00 .. 0x0F
			}
			while ( (Cipher[Total_Number] == Cipher[Total_Number - 1]) ||
					(Cipher[Total_Number] == Cipher[Total_Number - 2]) ||
					(Cipher[Total_Number] == Cipher[Total_Number - 3]) ||
					(Cipher[Total_Number] == Cipher[Total_Number - 4])  );
		}
	}	//	end if (TypeOfGame != 3)

	if ((TypeOfGame == 3) && (Total_Number == Start_Number))
	{
		Beeper(1);
		CipherPrint(0x11);
		Segment_A(1);
		Start_Numb[0] = KeyPressed();
		Beeper(2);
		CipherPrint(Start_Numb[0]);
		HAL_Delay(500);

		CipherPrint(0x11);
		Segment_G(1);
		Start_Numb[1] = KeyPressed();
		Beeper(2);
		CipherPrint(Start_Numb[1]);
		HAL_Delay(500);

		CipherPrint(0x11);
		Segment_D(1);
		Start_Numb[2] = KeyPressed();
		Beeper(2);
		CipherPrint(Start_Numb[2]);
		HAL_Delay(300);
		Total_Number = 100*Start_Numb[0] + 10*Start_Numb[1] + Start_Numb[2];
	}

	Beeper(2);
	CipherPrint(Cipher[Total_Number]);
	Current_Number = Start_Number ;

	do  // Komp
	{
		current_cipher_u32 = Cipher[Current_Number];
		if ( KeyPressed() == Cipher[Current_Number])
		{
			Error = 0;
			Cipher_OK();
			Current_Number++;
			TIM4->CNT = 0;
			Set_Prompt_Status(0);
		}
		else
		{
			Error = 1;
			HAL_RTC_GetTime( &hrtc, &TimeStruct, RTC_FORMAT_BIN );
			StopSeconds = TimeStruct.Seconds ;
			StopMinutes = TimeStruct.Minutes ;
			StopHours   = TimeStruct.Hours ;
			Cipher_Error(Start_Number,Current_Number,Total_Number, StopHours, StopMinutes, StopSeconds );
			Total_Number = Start_Number;
			}
		} // do Komp
	while ((Current_Number <= Total_Number ) && ( Error == 0 ));

	if ( Error == 0 ) Total_Number++;

	if (Total_Number >= END_NUMBER)
	{
		TheEnd();
	}

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

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
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
