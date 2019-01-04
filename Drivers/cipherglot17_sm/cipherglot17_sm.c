#include <string.h>
#include "cipherglot17_sm.h"
#include <stdlib.h> // rand

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim4;
extern UART_HandleTypeDef huart3;

volatile uint32_t Blank = 0 ;
volatile uint8_t prompt_u8 = 0 ;
extern uint32_t current_cipher_u32;
char DataChar_sm[100];

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
		Blank = 0;
		Key1 = ScanKeyBoard();
		HAL_Delay(20);
		Blank = 0;
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
	Set_Prompt_Status(0);
	HAL_TIM_Base_Start(&htim4);

	do
	{
		if (Prompt_Status() == 1 )
		{
			sprintf(DataChar_sm,"prompt: %d\r\n", (int)current_cipher_u32);
			HAL_UART_Transmit(&huart3, (uint8_t *)DataChar_sm, strlen(DataChar_sm), 100);
			CipherPrint(current_cipher_u32);
			HAL_Delay(200);
			CipherPrint(0x11);
			Set_Prompt_Status(0);
			TIM4->CNT = 0;
		}

		if (Blank == 1)
		{
			CipherPrint(0x11) ; // blank
			Blank = 0;
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

	HAL_TIM_Base_Stop(&htim4); // stop TIM4 prompt

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
	KeyPressed();
	Beeper(2);					HAL_Delay(300);

	Beeper(1);
	CipherPrint(CurNumb/100);	HAL_Delay(500);
	CipherPrint(0x11);			HAL_Delay(100);

	Beeper(1);
	CurNumb = CurNumb%100;
	CipherPrint(CurNumb/10);	HAL_Delay(500);
	CipherPrint(0x11);			HAL_Delay(100);

	Beeper(1);
	CurNumb = CurNumb%10;
	CipherPrint(CurNumb);		HAL_Delay(500);
	CipherPrint(0x11);			HAL_Delay(500);


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

void Set_Prompt_Status(uint8_t new_prompt_u8)
{
	prompt_u8 = new_prompt_u8;
}
//**********************************************************************
