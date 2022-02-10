/*
 * trellis_set.h
 *
 *  Created on: Feb 9, 2022
 *      Author: Donald
 */

#ifndef INC_TRELLIS_SET_H_
#define INC_TRELLIS_SET_H_

#include "stm32g4xx_hal.h"
#include "trellis_board.h"

struct TrellisSet {
	struct TrellisBoard* board1;
	struct TrellisBoard* board2;
	uint32_t leds;
	uint32_t btns;
	uint32_t prevBtns;
	uint32_t newBtns;
	uint32_t oldBtns;
};

// Public Functions
struct TrellisSet* TrellisSet_New(uint8_t index1, uint8_t index2); // Only accepts 0-7 (returns NULL if out of bounds)
struct TrellisSet* TrellisSet_Copy(struct TrellisSet* set);
void TrellisSet_Delete(struct TrellisSet* set);
void TrellisSet_Init(struct TrellisSet* set);
uint32_t TrellisSet_SetLEDs(struct TrellisSet* set, uint32_t ledMask);
uint32_t TrellisSet_ClrLEDs(struct TrellisSet* set, uint32_t ledMask);
uint32_t TrellisSet_TglLEDs(struct TrellisSet* set, uint32_t ledMask);
void TrellisSet_SendLEDs(struct TrellisSet* set);
uint32_t TrellisSet_ReadBtns(struct TrellisSet* set);
uint32_t TrellisSet_NonReadBtns(struct TrellisSet* set);
void TrellisSet_SetBlinking(struct TrellisSet* set, uint8_t freq); // Only accepts 0-3 [0Hz, 2Hz, 1Hz, 0.5Hz] (rounds if out of bounds)
void TrellisSet_SetBrightness(struct TrellisSet* set, uint8_t brighness); // Only accepts 1-16 (rounds if out of bounds)

// Private Functions
uint32_t __Collate_Bitfields(uint16_t field1, uint16_t field2);
void __Separate_Bitfields(uint32_t combinedField, uint16_t* field1, uint16_t* field2);

#endif /* INC_TRELLIS_SET_H_ */
