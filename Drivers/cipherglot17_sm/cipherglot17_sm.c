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

uint8_t blank_u8 = 0 ;
uint8_t prompt_u8 = 0 ;

uint32_t Error          = 0 ;
uint32_t Start_Number   = 0 ;
uint32_t Start_Numb[3]  = {0,0,0} ;
uint32_t Total_Number   = 0 ;
uint32_t Current_Number = 0 ;

uint32_t StopSeconds    = 0 ;
uint32_t StopMinutes    = 0 ;
uint32_t StopHours      = 0 ;
uint32_t TypeOfGame		= 3 ; // Pi or Old
char DataChar[100];
//**********************************************************************

void Segment_A(uint32_t);	// A	бело-зеленый
void Segment_B(uint32_t);	// B	зеленый
void Segment_C(uint32_t);	// C	бело-оранжевый
void Segment_D(uint32_t);	// D	cиний
void Segment_E(uint32_t);	// E	бело-cиний
void Segment_F(uint32_t);	// F	оранжевый
void Segment_G(uint32_t);	// G	бело-коричневый
void Segment_P(uint32_t);	// p	коричневый

void ScanRow_123A(uint32_t);
void ScanRow_456B(uint32_t);
void ScanRow_789C(uint32_t);
void ScanRow_E0FD(uint32_t);

void CipherPrint (uint32_t);
void Cipher_OK(void);
void Cipher_Error(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);
void Beeper(uint32_t);
void TheEnd(void);
void Test_Segment(void);

uint32_t ScanKeyBoard(void);
uint32_t KeyPressed(void);
uint32_t TestLED(void);
uint8_t Prompt_Status (void);
//**********************************************************************

void CipherGlot_init(void)
{
	MX_RTC_Init();
	MX_TIM2_Init();
	MX_TIM3_Init();
	MX_TIM4_Init();
	MX_USART3_UART_Init();

	HAL_TIM_Base_Start_IT(&htim3); // start TIM3 interupt

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
	HAL_TIM_Base_Start_IT(&htim4); // start TIM4 prompt
}
//**********************************************************************

void CipherGlot_main(void)
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
	Current_Number = Start_Number;

	HAL_TIM_Base_Start(&htim4);
	do  // Compare
	{
		if ( KeyPressed() == Cipher[Current_Number])
		{
			Error = 0;
			Cipher_OK();
			Current_Number++;
			TIM4->CNT = 0;
			set_Prompt(0);
		}
		else
		{
			HAL_TIM_Base_Stop(&htim4); // stop TIM4 prompt
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

	HAL_TIM_Base_Stop(&htim4); // stop TIM4 prompt
	if ( Error == 0 ) Total_Number++;

	if (Total_Number >= END_NUMBER)
	{
		TheEnd();
	}
}
//**********************************************************************

void Segment_A(uint32_t status)
{
	if (status == 0) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4,   SET);
	else             HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, RESET);
}

void Segment_B(uint32_t status)
{
	if (status == 0) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1,   SET);
	else         	 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, RESET);
}

void Segment_C(uint32_t status)
{
	if (status == 0) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15,   SET);
	else      		 HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, RESET);
}

void Segment_D(uint32_t status)
{
	if (status == 0) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2,   SET);
	else      		 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, RESET);
}

void Segment_E(uint32_t status)
{
	if (status == 0) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3,   SET);
	else             HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, RESET);
}

void Segment_F(uint32_t status)
{
	if (status == 0) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6,   SET);
	else             HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, RESET);
}

void Segment_G(uint32_t status)
{
	if (status == 0) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7,   SET);
	else             HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, RESET);
}

void Segment_P(uint32_t status)
{
	if (status == 0) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14,   SET);
	else         	 HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, RESET);
}
//**********************************************************************


void ScanRow_123A(uint32_t status)
{
	if (status == 0) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8,  RESET);
	else			 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8,    SET);
}

void ScanRow_456B(uint32_t status)
{
	if (status == 0) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9,  RESET);
	else			 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9,    SET);
}

void ScanRow_789C(uint32_t status)
{
	if (status == 0) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10,  RESET);
	else			 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10,    SET);
}

void ScanRow_E0FD(uint32_t status)
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

void CipherPrint (uint32_t num)
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



uint32_t TestLED(void)
{
	uint32_t StartKey = 0;

	CipherPrint(0x10);
	Beeper(2);
	StartKey = KeyPressed();
	Beeper(1);
	srand(StartKey);
	CipherPrint(StartKey);
	HAL_Delay(500);
	for (uint32_t i=0; i<=StartKey ; i++)
	{
		Beeper(1);
		CipherPrint(i);
		HAL_Delay(300);
	}
	CipherPrint(0x11); // blank
	rand(); // проcто так
	HAL_Delay(1000);
	return StartKey;
}
//**********************************************************************

uint32_t KeyPressed(void)
{
	uint32_t Key1;
	uint32_t Key2;

	do // KeyPresseed
	{
		blank_u8 = 0;
		Key1 = ScanKeyBoard();
		HAL_Delay(20);
		blank_u8 = 0;
		Key2 = ScanKeyBoard();
	}
	while (Key1 != Key2);

	return Key1;
}
//**********************************************************************

uint32_t ScanKeyBoard(void)
{
	uint32_t key_board;

	TIM4->CNT = 0;
	set_Prompt(0);

	do
	{
		if (Prompt_Status() == 1 )
		{
			sprintf(DataChar,"prompt: %d\r\n", (int)Cipher[Current_Number]);
			HAL_UART_Transmit(&huart3, (uint8_t *)DataChar, strlen(DataChar), 100);
			CipherPrint(Cipher[Current_Number]);
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
		key_board=0xFF;

		ScanRow_123A(1);
		if (GPIOB->IDR & GPIO_IDR_IDR5) key_board=0x01;
		if (GPIOB->IDR & GPIO_IDR_IDR6) key_board=0x02;
		if (GPIOB->IDR & GPIO_IDR_IDR7) key_board=0x03;
		if (GPIOB->IDR & GPIO_IDR_IDR8) key_board=0x0A;
		ScanRow_123A(0);

		ScanRow_456B(1);
		if (GPIOB->IDR & GPIO_IDR_IDR5) key_board=0x04;
		if (GPIOB->IDR & GPIO_IDR_IDR6) key_board=0x05;
		if (GPIOB->IDR & GPIO_IDR_IDR7) key_board=0x06;
		if (GPIOB->IDR & GPIO_IDR_IDR8) key_board=0x0B;
		ScanRow_456B(0);

		ScanRow_789C(1);
		if (GPIOB->IDR & GPIO_IDR_IDR5) key_board=0x07;
		if (GPIOB->IDR & GPIO_IDR_IDR6) key_board=0x08;
		if (GPIOB->IDR & GPIO_IDR_IDR7) key_board=0x09;
		if (GPIOB->IDR & GPIO_IDR_IDR8) key_board=0x0C;
		ScanRow_789C(0);

		ScanRow_E0FD(1);
		if (GPIOB->IDR & GPIO_IDR_IDR5) key_board=0x0E;
		if (GPIOB->IDR & GPIO_IDR_IDR6) key_board=0x00;
		if (GPIOB->IDR & GPIO_IDR_IDR7) key_board=0x0F;
		if (GPIOB->IDR & GPIO_IDR_IDR8) key_board=0x0D;
		ScanRow_E0FD(0);
		}
	while (key_board == 0xFF);
	return key_board;
}
//**********************************************************************

void Beeper(uint32_t type)
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

void Cipher_Error(uint32_t StartNumb, uint32_t CurNumb, uint32_t MaxNumb, uint32_t StopHour, uint32_t StopMin, uint32_t StopSec)
{
	// ERROR
	Beeper(2);	CipherPrint(0x12);	HAL_Delay(300);
	Beeper(2);	CipherPrint(0x13);	HAL_Delay(300);
	Beeper(2);	CipherPrint(0x12);	HAL_Delay(300);
	Beeper(2);	CipherPrint(0x13);	HAL_Delay(300);
	Beeper(2);	CipherPrint(0x12);	HAL_Delay(300);
	Beeper(2);	CipherPrint(0x11);	HAL_Delay(1000);
	CipherPrint(0x10);
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
	Beeper(2);					HAL_Delay(300);
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
	Beeper(2);					HAL_Delay(300);
	Beeper(2);					HAL_Delay(300);
	Beeper(2);					HAL_Delay(300);

	Beeper(1);
	CipherPrint(StopHour%10);	HAL_Delay(500);
	CipherPrint(0x11);			HAL_Delay(100);

	Beeper(1);
	CipherPrint(StopMin/10);	HAL_Delay(500);
	CipherPrint(0x11);			HAL_Delay(100);

	Beeper(1);
	CipherPrint(StopMin%10);	HAL_Delay(500);
	CipherPrint(0x11);			HAL_Delay(500);

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

