#ifndef LCD1602_FC113_SM_H_
#define LCD1602_FC113_SM_H_

#include "stm32f1xx_hal.h"

// Segment_A PA  1
// Segment_B PC 13
// Segment_C PC 15
// Segment_D PA  2
// Segment_E PA  3
// Segment_F PA  6
// Segment_G PA  7
// Segment_P PC 14

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

void Test_Segment(void);

void CipherPrint (uint32_t);
void Beeper(uint32_t);
void TheEnd(void);

void Cipher_OK(void);
void Cipher_Error(uint32_t, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);

uint32_t ScanKeyBoard(void);
uint32_t KeyPressed(void);
uint32_t TestLED(void);
uint8_t Prompt_Status (void);
void Set_Prompt_Status(uint8_t new_prompt_u8);

#endif
