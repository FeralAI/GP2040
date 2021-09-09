/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2021 Jason Skuby (mytechtoybox.com)
 */

#include "FlashPROM.h"

uint8_t FlashPROM::cache[EEPROM_SIZE_BYTES] = { };
spin_lock_t *FlashPROM::flashLock;

FlashPROM::FlashPROM()
{
	flashLock = spin_lock_instance(spin_lock_claim_unused(true));
}

void FlashPROM::start()
{
	memcpy(&cache, (uint8_t *)EEPROM_ADDRESS_START, EEPROM_SIZE_BYTES);
}

void FlashPROM::commit()
{
	while (is_spin_locked(flashLock));
	uint32_t interrupts = spin_lock_blocking(flashLock);

	flash_range_erase((intptr_t)EEPROM_ADDRESS_START - (intptr_t)XIP_BASE, EEPROM_SIZE_BYTES);
	flash_range_program((intptr_t)EEPROM_ADDRESS_START - (intptr_t)XIP_BASE, cache, EEPROM_SIZE_BYTES);

	spin_unlock(flashLock, interrupts);
}
