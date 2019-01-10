	// TIM2 Sound on port PA1
	// TIM3 timer LED Off = 2 sec
	// TIM4 prompt current cipher
	// UART3 -> PB10 for DEBUG, speed = 38400;

	// Segment_A PA  1
	// Segment_B PC 13
	// Segment_C PC 15
	// Segment_D PA  2
	// Segment_E PA  3
	// Segment_F PA  6
	// Segment_G PA  7
	// Segment_P PC 14

#include "stm32f1xx_hal.h"
#include "rtc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

#include <string.h>
#include "cipherglot17_sm.h"
#include <stdlib.h> // rand
//**********************************************************************

RTC_TimeTypeDef TimeStruct  ;

#define END_NUMBER 995

uint8_t cipher_arr_u8[END_NUMBER] = {3,
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

uint8_t blank_u8 		= 0 ;
uint8_t prompt_u8 		= 0 ;
uint8_t error_status_u8	= 0 ;
uint8_t game_type_u8	= 3 ; // Pi or Old

uint32_t start_cipher_number_u32   = 0 ;
uint32_t current_cipher_number_u32 = 0 ;
uint32_t total_cipher_number_u32   = 0 ;

uint8_t start_numb_arr_u8[3]  = {0,0,0} ;

uint8_t stop_second_u8    = 0 ;
uint8_t stop_minutes_u8    = 0 ;
uint8_t stop_hours_u8      = 0 ;

char DataChar[100];
//**********************************************************************

void Segment_A(uint8_t);	// A	бело-зеленый
void Segment_B(uint8_t);	// B	зеленый
void Segment_C(uint8_t);	// C	бело-оранжевый
void Segment_D(uint8_t);	// D	cиний
void Segment_E(uint8_t);	// E	бело-cиний
void Segment_F(uint8_t);	// F	оранжевый
void Segment_G(uint8_t);	// G	бело-коричневый
void Segment_P(uint8_t);	// p	коричневый

void ScanRow_123A(uint8_t);
void ScanRow_456B(uint8_t);
void ScanRow_789C(uint8_t);
void ScanRow_E0FD(uint8_t);

void CipherPrint (uint8_t);
void Cipher_OK(void);
void Cipher_Error(uint32_t, uint32_t, uint32_t, uint8_t, uint8_t, uint8_t);
void Beeper (uint8_t);
void BeepError2 (void);
void BeepError3 (void);
void TheEnd (void);
void Test_Segment (void);

uint8_t ScanKeyBoard (void);
uint8_t KeyPressed (void);
uint8_t TestLED (void);
uint8_t Prompt_Status (void);
//**********************************************************************

void CipherGlot_init(void)
{
	MX_RTC_Init();
	MX_TIM2_Init();
	MX_TIM3_Init();
	MX_TIM4_Init();
	MX_USART1_UART_Init();

	HAL_TIM_Base_Start_IT(&htim3); // start TIM3 interupt

	sprintf(DataChar,"\r\n\r\nUART3 for debug Start\r\nSpeed 38400\r\n");
	HAL_UART_Transmit(&huart1, (uint8_t *)DataChar, strlen(DataChar), 100);

	// Test_Segment();
	game_type_u8 = TestLED();

	if (game_type_u8 == 3)
	{
	  start_cipher_number_u32 = 0;
	}
	else
	{
	  start_cipher_number_u32 = 5 ;
	  do
	  {
		  cipher_arr_u8[1] = rand()%10; // 0..9
		  cipher_arr_u8[2] = rand()%10;
		  cipher_arr_u8[3] = rand()%10;
		  cipher_arr_u8[4] = rand()%6 + 10; // A .. F
	  }
	  while (	(cipher_arr_u8[1] == cipher_arr_u8[2])
			 ||	(cipher_arr_u8[2] == cipher_arr_u8[3])
			 ||	(cipher_arr_u8[3] == cipher_arr_u8[1])  );
	} // end if

	total_cipher_number_u32   = start_cipher_number_u32 ;
	current_cipher_number_u32 = start_cipher_number_u32 ;

	sprintf(DataChar,"Init - Ok\r\n");
	HAL_UART_Transmit(&huart1, (uint8_t *)DataChar, strlen(DataChar), 100);
	HAL_TIM_Base_Start_IT(&htim4); // start TIM4 prompt
}
//**********************************************************************

void CipherGlot_main(void)
{
	if (total_cipher_number_u32 == start_cipher_number_u32)
	{
		TimeStruct.Hours   = 0;
		TimeStruct.Minutes = 0;
		TimeStruct.Seconds = 0;
		HAL_RTC_SetTime( &hrtc, &TimeStruct, RTC_FORMAT_BIN );
	}

	if (game_type_u8 != 3)
	{
		if ( total_cipher_number_u32 %4 == 0 )
		{
			do
			{
				cipher_arr_u8[total_cipher_number_u32] = rand()%6 + 10; // A .. F
			}
			while ( (cipher_arr_u8[total_cipher_number_u32] == cipher_arr_u8[total_cipher_number_u32 - 4]) );
		}
		else
		{
			do
			{
				cipher_arr_u8[total_cipher_number_u32] = rand()%10; // 0x00 .. 0x0F
			}
			while ( (cipher_arr_u8[total_cipher_number_u32] == cipher_arr_u8[total_cipher_number_u32 - 1]) ||
					(cipher_arr_u8[total_cipher_number_u32] == cipher_arr_u8[total_cipher_number_u32 - 2]) ||
					(cipher_arr_u8[total_cipher_number_u32] == cipher_arr_u8[total_cipher_number_u32 - 3]) ||
					(cipher_arr_u8[total_cipher_number_u32] == cipher_arr_u8[total_cipher_number_u32 - 4])  );
		}
	}	//	end if (game_type_u8 != 3)

	if ((game_type_u8 == 3) && (total_cipher_number_u32 == start_cipher_number_u32))
	{
		Beeper(1);
		CipherPrint(0x11);
		Segment_A(1);
		start_numb_arr_u8[0] = KeyPressed();
		Beeper(2);
		CipherPrint(start_numb_arr_u8[0]);
		HAL_Delay(500);

		CipherPrint(0x11);
		Segment_G(1);
		start_numb_arr_u8[1] = KeyPressed();
		Beeper(2);
		CipherPrint(start_numb_arr_u8[1]);
		HAL_Delay(500);

		CipherPrint(0x11);
		Segment_D(1);
		start_numb_arr_u8[2] = KeyPressed();
		Beeper(2);
		CipherPrint(start_numb_arr_u8[2]);
		HAL_Delay(300);
		total_cipher_number_u32 = 100*start_numb_arr_u8[0] + 10*start_numb_arr_u8[1] + start_numb_arr_u8[2];
	}

	Beeper(2);
	CipherPrint(cipher_arr_u8[total_cipher_number_u32]);
	current_cipher_number_u32 = start_cipher_number_u32;

	TIM4->CNT = 0;
	HAL_TIM_Base_Start(&htim4);
	do  // Compare
	{
		if ( KeyPressed() == cipher_arr_u8[current_cipher_number_u32])
		{
			error_status_u8 = 0;
			Cipher_OK();
			current_cipher_number_u32++;
			TIM4->CNT = 0;
			set_Prompt(0);
		}
		else
		{
			error_status_u8++;

			if (error_status_u8 < 2)
			{
				BeepError2();
				CipherPrint(cipher_arr_u8[current_cipher_number_u32]);
				current_cipher_number_u32 = start_cipher_number_u32;
			}
			else
			{
				HAL_TIM_Base_Stop(&htim4); // stop TIM4 prompt
				BeepError3();
				HAL_RTC_GetTime( &hrtc, &TimeStruct, RTC_FORMAT_BIN );
				stop_second_u8 = TimeStruct.Seconds ;
				stop_minutes_u8 = TimeStruct.Minutes ;
				stop_hours_u8   = TimeStruct.Hours ;
				Cipher_Error(start_cipher_number_u32, current_cipher_number_u32, total_cipher_number_u32, stop_hours_u8, stop_minutes_u8, stop_second_u8 );
				total_cipher_number_u32 = start_cipher_number_u32;
			}
		}
	} // do Komp
	while ((current_cipher_number_u32 <= total_cipher_number_u32 ) && ( error_status_u8 < 2 ));

	HAL_TIM_Base_Stop(&htim4); // stop TIM4 prompt
	if ( error_status_u8 == 0 ) total_cipher_number_u32++;

	if (total_cipher_number_u32 >= END_NUMBER)
	{
		TheEnd();
	}
}
//**********************************************************************

void Segment_A(uint8_t status)
{
	if (status == 0) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4,   SET);
	else             HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, RESET);
}

void Segment_B(uint8_t status)
{
	if (status == 0) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1,   SET);
	else         	 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, RESET);
}

void Segment_C(uint8_t status)
{
	if (status == 0) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15,   SET);
	else      		 HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, RESET);
}

void Segment_D(uint8_t status)
{
	if (status == 0) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2,   SET);
	else      		 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, RESET);
}

void Segment_E(uint8_t status)
{
	if (status == 0) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3,   SET);
	else             HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, RESET);
}

void Segment_F(uint8_t status)
{
	if (status == 0) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6,   SET);
	else             HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, RESET);
}

void Segment_G(uint8_t status)
{
	if (status == 0) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7,   SET);
	else             HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, RESET);
}

void Segment_P(uint8_t status)
{
	if (status == 0) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14,   SET);
	else         	 HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, RESET);
}
//**********************************************************************


void ScanRow_123A(uint8_t status)
{
	if (status == 0) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8,  RESET);
	else			 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8,    SET);
}

void ScanRow_456B(uint8_t status)
{
	if (status == 0) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9,  RESET);
	else			 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9,    SET);
}

void ScanRow_789C(uint8_t status)
{
	if (status == 0) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10,  RESET);
	else			 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10,    SET);
}

void ScanRow_E0FD(uint8_t status)
{
	if (status == 0) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11,  RESET);
	else			 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11,    SET);
}
//**********************************************************************

void Test_Segment(void)
{
	Segment_A(0);
	Segment_B(0);
	Segment_C(0);
	Segment_P(0);
	Segment_D(0);
	Segment_G(0);
	Segment_F(0);
	Segment_E(0);

	Segment_A(1);
	HAL_Delay(500);
	Segment_A(0);

	Segment_B(1);
	HAL_Delay(500);
	Segment_B(0);

	Segment_C(1);
	HAL_Delay(500);
	Segment_C(0);

	Segment_D(1);
	HAL_Delay(500);
	Segment_D(0);

	Segment_E(1);
	HAL_Delay(500);
	Segment_E(0);

	Segment_F(1);
	HAL_Delay(500);
	Segment_F(0);

	Segment_G(1);
	HAL_Delay(500);
	Segment_G(0);

	Segment_P(1);
	HAL_Delay(500);
	Segment_P(0);
}
//**********************************************************************

void CipherPrint (uint8_t num)
{
//	-A-A-
//	F---B
//	-G-G-
//	E---C
//	-D-D-
//	----- P
	TIM3->CNT = 0; //
	if (num==0x00) // "0"
		{
			Segment_A(1);
		Segment_F(1);Segment_B(1);
			Segment_G(0);
		Segment_E(1);Segment_C(1);
			Segment_D(1);
					Segment_P(0);
		}

	if (num==0x01)	// "1"
		{
		Segment_A(0);
	Segment_F(0);Segment_B(1);
		Segment_G(0);
	Segment_E(0);Segment_C(1);
		Segment_D(0);
				Segment_P(0);
		}

	if (num==0x02)		// "2"
		{
		Segment_A(1);
	Segment_F(0);Segment_B(1);
		Segment_G(1);
	Segment_E(1);Segment_C(0);
		Segment_D(1);
				Segment_P(0);
		}

	if (num==0x03)		// "3"
		{
		Segment_A(1);
	Segment_F(0);Segment_B(1);
		Segment_G(1);
	Segment_E(0);Segment_C(1);
		Segment_D(1);
				Segment_P(0);
		}

	if (num==0x04)		// "4"
		{
		Segment_A(0);
	Segment_F(1);Segment_B(1);
		Segment_G(1);
	Segment_E(0);Segment_C(1);
		Segment_D(0);
				Segment_P(0);
		}

	if (num==0x05)		// "5"
		{
		Segment_A(1);
	Segment_F(1);Segment_B(0);
		Segment_G(1);
	Segment_E(0);Segment_C(1);
		Segment_D(1);
				Segment_P(0);
		}

	if (num==0x06)		// "6"
		{
		Segment_A(1);
	Segment_F(1);Segment_B(0);
		Segment_G(1);
	Segment_E(1);Segment_C(1);
		Segment_D(1);
				Segment_P(0);
		}

	if (num==0x07)		// "7"
		{
		Segment_A(1);
	Segment_F(0);Segment_B(1);
		Segment_G(0);
	Segment_E(0);Segment_C(1);
		Segment_D(0);
				Segment_P(0);
		}

	if (num==0x08)		// "8"
		{
		Segment_A(1);
	Segment_F(1);Segment_B(1);
		Segment_G(1);
	Segment_E(1);Segment_C(1);
		Segment_D(1);
				Segment_P(0);
		}

	if (num==0x09)		// "9"
		{
		Segment_A(1);
	Segment_F(1);Segment_B(1);
		Segment_G(1);
	Segment_E(0);Segment_C(1);
		Segment_D(1);
				Segment_P(0);
		}

	if (num==0x0A)		// "A"
		{
		Segment_A(1);
	Segment_F(1);Segment_B(1);
		Segment_G(1);
	Segment_E(1);Segment_C(1);
		Segment_D(0);
				Segment_P(0);
		}

	if (num==0x0B)		// "b"
		{
		Segment_A(0);
	Segment_F(1);Segment_B(0);
		Segment_G(1);
	Segment_E(1);Segment_C(1);
		Segment_D(1);
				Segment_P(0);
		}

	if (num==0x0C)		// "C"
		{
		Segment_A(1);
	Segment_F(1);Segment_B(0);
		Segment_G(0);
	Segment_E(1);Segment_C(0);
		Segment_D(1);
				Segment_P(0);
		}

	if (num==0x0D)		// "d"
		{
		Segment_A(0);
	Segment_F(0);Segment_B(1);
		Segment_G(1);
	Segment_E(1);Segment_C(1);
		Segment_D(1);
				Segment_P(0);
		}

	if (num==0x0E)		// "E"
		{
		Segment_A(1);
	Segment_F(1);Segment_B(0);
		Segment_G(1);
	Segment_E(1);Segment_C(0);
		Segment_D(1);
				Segment_P(0);
		}

	if (num==0x0F)		// "F"
		{
		Segment_A(1);
	Segment_F(1);Segment_B(0);
		Segment_G(1);
	Segment_E(1);Segment_C(0);
		Segment_D(0);
				Segment_P(0);
		}

	if (num==0x10)		// "point"
		{
		Segment_A(0);
	Segment_F(0);Segment_B(0);
		Segment_G(0);
	Segment_E(0);Segment_C(0);
		Segment_D(0);
				Segment_P(1);
		}

	if (num==0x11)		// "blank"
		{
		Segment_A(0);
	Segment_F(0);Segment_B(0);
		Segment_G(0);
	Segment_E(0);Segment_C(0);
		Segment_D(0);
				Segment_P(0);
		}

	if (num==0x12)		// "error-1"
		{
		Segment_A(0);
	Segment_F(1);Segment_B(1);
		Segment_G(0);
	Segment_E(1);Segment_C(1);
		Segment_D(0);
				Segment_P(0);
		}

	if (num==0x13)		// "error-2"
		{
		Segment_A(1);
	Segment_F(0);Segment_B(0);
		Segment_G(1);
	Segment_E(0);Segment_C(0);
		Segment_D(1);
				Segment_P(0);
		}

	if (num==0x14)		// "L"
		{
		Segment_A(0);
	Segment_F(1);Segment_B(0);
		Segment_G(0);
	Segment_E(1);Segment_C(0);
		Segment_D(1);
				Segment_P(0);
		}

	if (num==0x15)		// "o"
		{
		Segment_A(0);
	Segment_F(0);Segment_B(0);
		Segment_G(1);
	Segment_E(1);Segment_C(1);
		Segment_D(1);
				Segment_P(0);
		}

	if (num==0x16)		// "u"
		{
		Segment_A(0);
	Segment_F(0);Segment_B(0);
		Segment_G(0);
	Segment_E(1);Segment_C(1);
		Segment_D(1);
				Segment_P(0);
		}

	if (num==0x17)		// "H"
		{
		Segment_A(0);
	Segment_F(1);Segment_B(1);
		Segment_G(1);
	Segment_E(1);Segment_C(1);
		Segment_D(0);
				Segment_P(0);
		}

	if (num==0x18)		// "i"
		{
		Segment_A(0);
	Segment_F(0);Segment_B(0);
		Segment_G(0);
	Segment_E(0);Segment_C(1);
		Segment_D(0);
				Segment_P(0);
		}


	if (num==0x21)		// "-1"
		{
		Segment_A(0);
	Segment_F(0);Segment_B(0);
		Segment_G(0);
	Segment_E(0);Segment_C(0);
		Segment_D(1);
				Segment_P(0);
		}
	if (num==0x22)		// "-2"
		{
		Segment_A(0);
	Segment_F(0);Segment_B(0);
		Segment_G(1);
	Segment_E(0);Segment_C(0);
		Segment_D(0);
				Segment_P(0);
		}
	if (num==0x23)		// "-3"
		{
		Segment_A(1);
	Segment_F(0);Segment_B(0);
		Segment_G(0);
	Segment_E(0);Segment_C(0);
		Segment_D(0);
				Segment_P(0);
		}
}
//**********************************************************************



uint8_t TestLED(void)
{
	uint8_t start_key_u8 = 0;

	CipherPrint(0x10);
	Beeper(2);
	start_key_u8 = KeyPressed();
	Beeper(1);
	srand(start_key_u8);
	CipherPrint(start_key_u8);
	HAL_Delay(500);
	for (uint32_t i=0; i<=start_key_u8 ; i++)
	{
		Beeper(1);
		CipherPrint(i);
		HAL_Delay(300);
	}
	CipherPrint(0x11); // blank
	rand(); // проcто так
	HAL_Delay(1000);
	return start_key_u8;
}
//**********************************************************************

uint8_t KeyPressed(void)
{
	uint8_t key1_u8;
	uint8_t key2_u8;

	do // KeyPresseed
	{
		blank_u8 = 0;
		key1_u8 = ScanKeyBoard();
		HAL_Delay(20);
		blank_u8 = 0;
		key2_u8 = ScanKeyBoard();
	}
	while (key1_u8 != key2_u8);

	return key1_u8;
}
//**********************************************************************

uint8_t ScanKeyBoard(void)
{
	uint8_t keyboard_u8;

	TIM4->CNT = 0;
	set_Prompt(0);

	do
	{
		if (Prompt_Status() == 1 )
		{
			sprintf(DataChar,"prompt: %d\r\n", (int)cipher_arr_u8[current_cipher_number_u32]);
			HAL_UART_Transmit(&huart1, (uint8_t *)DataChar, strlen(DataChar), 100);
			CipherPrint(cipher_arr_u8[current_cipher_number_u32]);
			HAL_Delay(200);
			CipherPrint(0x11);
			set_Prompt(0);
			TIM4->CNT = 0;
		}

		if (blank_u8 == 1)
		{
			CipherPrint(0x11) ; // blank
			blank_u8 = 0;
		}
		rand();
		keyboard_u8 = 0xFF;

		ScanRow_123A(1);
		if (GPIOB->IDR & GPIO_IDR_IDR5) keyboard_u8 =0x01;
		if (GPIOB->IDR & GPIO_IDR_IDR3) keyboard_u8 =0x02;
		if (GPIOB->IDR & GPIO_IDR_IDR4) keyboard_u8 =0x03;
		if (GPIOB->IDR & GPIO_IDR_IDR8) keyboard_u8 =0x0A;
		ScanRow_123A(0);

		ScanRow_456B(1);
		if (GPIOB->IDR & GPIO_IDR_IDR5) keyboard_u8 =0x04;
		if (GPIOB->IDR & GPIO_IDR_IDR3) keyboard_u8 =0x05;
		if (GPIOB->IDR & GPIO_IDR_IDR4) keyboard_u8 =0x06;
		if (GPIOB->IDR & GPIO_IDR_IDR8) keyboard_u8 =0x0B;
		ScanRow_456B(0);

		ScanRow_789C(1);
		if (GPIOB->IDR & GPIO_IDR_IDR5) keyboard_u8 =0x07;
		if (GPIOB->IDR & GPIO_IDR_IDR3) keyboard_u8 =0x08;
		if (GPIOB->IDR & GPIO_IDR_IDR4) keyboard_u8 =0x09;
		if (GPIOB->IDR & GPIO_IDR_IDR8) keyboard_u8 =0x0C;
		ScanRow_789C(0);

		ScanRow_E0FD(1);
		if (GPIOB->IDR & GPIO_IDR_IDR5) keyboard_u8 =0x0E;
		if (GPIOB->IDR & GPIO_IDR_IDR3) keyboard_u8 =0x00;
		if (GPIOB->IDR & GPIO_IDR_IDR4) keyboard_u8 =0x0F;
		if (GPIOB->IDR & GPIO_IDR_IDR8) keyboard_u8 =0x0D;
		ScanRow_E0FD(0);
		}
	while (keyboard_u8 == 0xFF);
	return keyboard_u8;
}
//**********************************************************************

void Beeper(uint8_t type)
{
	TIM2->CNT = 0;
	if (type == 1)
	{
		TIM2->ARR  = 1000;
		TIM2->CCR1 =  500;
		HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
		HAL_Delay(20);
		HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_1);
	}
	if (type == 2)
	{
		TIM2->ARR  = 2000;
		TIM2->CCR1 = 1000;
		HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
		HAL_Delay(80);
		HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_1);
	}
}
//**********************************************************************

void TheEnd(void)
{
	Beeper(2);
	Beeper(1);
	Beeper(2);
	CipherPrint(0x09);
}
//**********************************************************************

void Cipher_OK(void)
{
	Beeper(1);
	CipherPrint(0x10); // point
	HAL_Delay(200);
	CipherPrint(0x11); // blank
}
//**********************************************************************

void Cipher_Error(uint32_t StartNumb, uint32_t CurNumb, uint32_t MaxNumb, uint8_t _StopHour_u8, uint8_t _StopMin_u8, uint8_t _StopSec_u8)
{
	// ERROR

	//  COUT Current Number
		//	KeyPressed();
		//	Beeper(2);					HAL_Delay(300);
		//
		//	Beeper(1);
		//	CipherPrint(CurNumb/100);	HAL_Delay(500);
		//	CipherPrint(0x11);			HAL_Delay(100);
		//
		//	Beeper(1);
		//	CurNumb = CurNumb%100;
		//	CipherPrint(CurNumb/10);	HAL_Delay(500);
		//	CipherPrint(0x11);			HAL_Delay(100);
		//
		//	Beeper(1);
		//	CurNumb = CurNumb%10;
		//	CipherPrint(CurNumb);		HAL_Delay(500);
		//	CipherPrint(0x11);			HAL_Delay(500);


	//  COUT Total Number
	KeyPressed();
	//Beeper(2);					HAL_Delay(300);
	Beeper(2);					HAL_Delay(300);

	MaxNumb = MaxNumb - StartNumb;
	Beeper(1);
	CipherPrint(MaxNumb/100);	HAL_Delay(500);
	CipherPrint(0x11);			HAL_Delay(100);

	Beeper(1);
	MaxNumb = MaxNumb%100;
	CipherPrint(MaxNumb/10);	HAL_Delay(500);
	CipherPrint(0x11);			HAL_Delay(100);

	Beeper(1);
	MaxNumb = MaxNumb%10;
	CipherPrint(MaxNumb);		HAL_Delay(500);
	CipherPrint(0x11);			HAL_Delay(500);

	// COUT TIME
	KeyPressed();
	//Beeper(2);					HAL_Delay(300);
	Beeper(2);					HAL_Delay(300);
	Beeper(2);					HAL_Delay(300);

	Beeper(1);
	CipherPrint(_StopHour_u8 % 10);	HAL_Delay(500);
	CipherPrint(0x11);				HAL_Delay(100);

	Beeper(1);
	CipherPrint(_StopMin_u8 / 10);	HAL_Delay(500);
	CipherPrint(0x11);				HAL_Delay(100);

	Beeper(1);
	CipherPrint(_StopMin_u8 % 10);	HAL_Delay(500);
	CipherPrint(0x11);				HAL_Delay(500);

	KeyPressed();
	Beeper(1);
	HAL_Delay(1000);
}
//**********************************************************************

uint8_t Prompt_Status(void)
{
	return prompt_u8;
}
//**********************************************************************

void set_Prompt(uint8_t new_prompt_u8)
{
	prompt_u8 = new_prompt_u8;
}
//**********************************************************************

void set_Blank(uint8_t new_blank_u8)
{
	blank_u8 = new_blank_u8;
}
//**********************************************************************

void BeepError2(void)
{
	Beeper(2);	CipherPrint(0x12);	HAL_Delay(300);
	Beeper(2);	CipherPrint(0x13);	HAL_Delay(300);
	Beeper(2);	CipherPrint(0x11);	HAL_Delay(500);
}
//**********************************************************************

void BeepError3(void)
{
	Beeper(2);	CipherPrint(0x12);	HAL_Delay(300);
	Beeper(2);	CipherPrint(0x13);	HAL_Delay(300);
	Beeper(2);	CipherPrint(0x12);	HAL_Delay(300);
	Beeper(2);	CipherPrint(0x13);	HAL_Delay(300);
	Beeper(2);	CipherPrint(0x12);	HAL_Delay(300);
	Beeper(2);	CipherPrint(0x11);	HAL_Delay(1000);
	CipherPrint(0x10);
}
//**********************************************************************

//**********************************************************************
