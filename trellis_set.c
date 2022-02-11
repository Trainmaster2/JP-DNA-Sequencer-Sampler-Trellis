/*
 * trellis_set.c
 *
 *  Created on: Feb 9, 2022
 *      Author: Donald
 */

#include "trellis_set.h"
#include <stdlib.h>
#include <string.h>

// Public Functions
struct TrellisSet* TrellisSet_New(uint8_t index1, uint8_t index2)
{
	if (index1 < 0 || index1 > 7 || index2 < 0 || index2 > 7 || index1 == index2)
		return NULL;
	struct TrellisSet* set = malloc(sizeof(struct TrellisSet));
	set->board1 = TrellisBoard_New(index1);
	set->board2 = TrellisBoard_New(index2);
	set->leds = 0x00000000;
	set->btns = 0x00000000;
	set->prevBtns = 0x00000000;
	set->newBtns = 0x00000000;
	set->oldBtns = 0x00000000;
	return set;
}
struct TrellisSet* TrellisSet_Copy(struct TrellisSet* set)
{
	if (set != NULL && set->board1 != NULL && set->board2 != NULL)
	{
		struct TrellisSet* newSet = malloc(sizeof(struct TrellisSet));
		memcpy(newSet, set, sizeof(struct TrellisSet));
		newSet->board1 = TrellisBoard_Copy(set->board1);
		newSet->board2 = TrellisBoard_Copy(set->board2);
		return newSet;
	}
	return NULL;
}
void TrellisSet_Delete(struct TrellisSet* set)
{
	if (set != NULL)
	{
		if (set->board1 != NULL)
		{
			free(set->board1);
			set->board1 = NULL;
		}
		if (set->board2 != NULL)
		{
			free(set->board2);
			set->board2 = NULL;
		}
		free(set);
		set = NULL;
	}
}
void TrellisSet_Init(struct TrellisSet* set)
{
	if (set != NULL && set->board1 != NULL && set->board2 != NULL)
	{
		__Separate_Bitfields(set->leds, &(set->board1->leds), &(set->board2->leds));
		set->btns = __Collate_Bitfields(set->board1->btns, set->board2->btns);
		set->prevBtns = __Collate_Bitfields(set->board1->prevBtns, set->board2->prevBtns);
		set->newBtns = __Collate_Bitfields(set->board1->newBtns, set->board2->newBtns);
		set->oldBtns = __Collate_Bitfields(set->board1->oldBtns, set->board2->oldBtns);
		TrellisBoard_Init(set->board1);
		TrellisBoard_Init(set->board2);
	}
}
uint32_t TrellisSet_SetLEDs(struct TrellisSet* set, uint32_t ledMask)
{
	if (set != NULL && set->board1 != NULL && set->board2 != NULL)
	{
		set->leds |= ledMask;
		__Separate_Bitfields(set->leds, &(set->board1->leds), &(set->board2->leds));
		return set->leds;
	}
	return 0;
}
uint32_t TrellisSet_ClrLEDs(struct TrellisSet* set, uint32_t ledMask)
{
	if (set != NULL && set->board1 != NULL && set->board2 != NULL)
	{
		set->leds &= ~ledMask;
		__Separate_Bitfields(set->leds, &(set->board1->leds), &(set->board2->leds));
		return set->leds;
	}
	return 0;
}
uint32_t TrellisSet_TglLEDs(struct TrellisSet* set, uint32_t ledMask)
{
	if (set != NULL && set->board1 != NULL && set->board2 != NULL)
	{
		set->leds ^= ledMask;
		__Separate_Bitfields(set->leds, &(set->board1->leds), &(set->board2->leds));
		return set->leds;
	}
	return 0;
}
void TrellisSet_SendLEDs(struct TrellisSet* set)
{
	if (set != NULL && set->board1 != NULL && set->board2 != NULL)
	{
		TrellisBoard_SendLEDs(set->board1);
		TrellisBoard_SendLEDs(set->board2);
	}
}
uint32_t TrellisSet_ReadBtns(struct TrellisSet* set)
{
	if (set != NULL && set->board1 != NULL && set->board2 != NULL)
	{
		TrellisBoard_ReadBtns(set->board1);
		TrellisBoard_ReadBtns(set->board2);
		set->prevBtns = set->btns;
		set->btns = __Collate_Bitfields(set->board1->btns, set->board2->btns);
		set->newBtns = set->btns & ~(set->prevBtns);
		set->oldBtns = set->btns & set->prevBtns;
		return set->btns;
	}
	return 0;
}
uint32_t TrellisSet_NonReadBtns(struct TrellisSet* set)
{
	if (set != NULL && set->board1 != NULL && set->board2 != NULL)
	{
		TrellisBoard_NonReadBtns(set->board1);
		TrellisBoard_NonReadBtns(set->board2);
		set->prevBtns = set->btns;
		set->btns = 0x0000;
		set->newBtns = 0x0000;
		set->oldBtns = 0x0000;
	}
	return 0;
}
void TrellisSet_SetBlinking(struct TrellisSet* set, uint8_t freq)
{
	if (set != NULL && set->board1 != NULL && set->board2 != NULL)
	{
		if (freq < 0)
			freq = 0;
		else if (freq > 3)
			freq = 3;
		TrellisBoard_SetBlinking(set->board1, freq);
		TrellisBoard_SetBlinking(set->board2, freq);
	}
}
void TrellisSet_SetBrightness(struct TrellisSet* set, uint8_t brighness)
{
	if (set != NULL && set->board1 != NULL && set->board2 != NULL)
	{
		if (brighness < 1)
			brighness = 1;
		else if (brighness > 16)
			brighness = 16;
		TrellisBoard_SetBrightness(set->board1, brighness);
		TrellisBoard_SetBrightness(set->board2, brighness);
	}
}

// Private Functions
uint32_t __Collate_Bitfields(uint16_t field1, uint16_t field2)
{
	uint32_t combinedField = 0x00000000;
	// Currently set up to assume boards are side-by-side
	combinedField |= field1 & 0xF;
	combinedField |= (field2 & 0xF) << 4;
	combinedField |= (field1 & 0xF0) << 4;
	combinedField |= (field2 & 0xF0) << 8;
	combinedField |= (field1 & 0xF00) << 8;
	combinedField |= (field2 & 0xF00) << 12;
	combinedField |= (field1 & 0xF000) << 12;
	combinedField |= (field2 & 0xF000) << 16;
	return combinedField;
}
void __Separate_Bitfields(uint32_t combinedField, uint16_t* field1, uint16_t* field2)
{
	*field1 = 0x0000;
	*field2 = 0x0000;
	// Currently set up to assume boards are side-by-side
	*field1 |= combinedField & 0xF;
	*field2 |= (combinedField >> 4) & 0xF;
	*field1 |= (combinedField >> 4) & 0xF0;
	*field2 |= (combinedField >> 8) & 0xF0;
	*field1 |= (combinedField >> 8) & 0xF00;
	*field2 |= (combinedField >> 12) & 0xF00;
	*field1 |= (combinedField >> 12) & 0xF000;
	*field2 |= (combinedField >> 16) & 0xF000;
}
