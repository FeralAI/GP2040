/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2021 Jason Skuby (mytechtoybox.com)
 */

#include "FlashPROM.h"
#include "pico/multicore.h"

uint8_t FlashPROM::cache[EEPROM_SIZE_BYTES] = { };
spin_lock_t *FlashPROM::flashLock;
absolute_time_t FlashPROM::nextWriteTime = 0;

FlashPROM::FlashPROM()
{
	flashLock = spin_lock_instance(spin_lock_claim_unused(true));
}

void FlashPROM::start()
{
	memcpy(&cache, (uint8_t *)EEPROM_ADDRESS_START, EEPROM_SIZE_BYTES);
}

/* We don't have an actual EEPROM, so we need to be extra careful about minimizing writes. Instead
	of writing when a commit is requested, we update a time to actually commit. That way, if we receive multiple requests
	to commit in that timeframe, we'll hold off until the user is done sending changes. */
void FlashPROM::commit() {
	FlashPROM::nextWriteTime = make_timeout_time_ms(500);
}

/* This should be run every loop. It looks to see if it's time to actually commit to flash */
void FlashPROM::checkCommit() {
	if (FlashPROM::nextWriteTime == 0 || !time_reached(FlashPROM::nextWriteTime)) {
		return;
	}

	this->writeToFlash();
}

void FlashPROM::writeToFlash()
{
	multicore_lockout_start_blocking();
	while (is_spin_locked(flashLock));
	uint32_t interrupts = spin_lock_blocking(flashLock);

	FlashPROM::nextWriteTime = 0;

	flash_range_erase((intptr_t)EEPROM_ADDRESS_START - (intptr_t)XIP_BASE, EEPROM_SIZE_BYTES);
	flash_range_program((intptr_t)EEPROM_ADDRESS_START - (intptr_t)XIP_BASE, cache, EEPROM_SIZE_BYTES);

	spin_unlock(flashLock, interrupts);
	multicore_lockout_end_blocking();
}
