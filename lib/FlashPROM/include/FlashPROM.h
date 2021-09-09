/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2021 Jason Skuby (mytechtoybox.com)
 */

#ifndef FLASHPROM_H_
#define FLASHPROM_H_

#include <stdint.h>
#include <string.h>
#include <hardware/flash.h>
#include <hardware/sync.h>
#include "pico/lock_core.h"

#define EEPROM_SIZE_BYTES    4096       // Reserve 4k of flash memory (ensure this value is divisible by 256)
#define EEPROM_ADDRESS_START 0x101FF000 // The arduino-pico EEPROM lib starts here, so we'll do the same

class FlashPROM
{
	public:
		FlashPROM();

		static spin_lock_t *flashLock;

		void start();
		void commit();
		void checkCommit();

		template<typename T>
		T &get(uint16_t const index, T &value)
		{
			while (is_spin_locked(flashLock));
			uint32_t interrupts = spin_lock_blocking(flashLock);

			if (index < EEPROM_SIZE_BYTES)
				memcpy(&value, &cache[index], sizeof(T));

			spin_unlock(flashLock, interrupts);
			return value;
		}

		template<typename T>
		void set(uint16_t const index, const T &value)
		{
			while (is_spin_locked(flashLock));
			uint32_t interrupts = spin_lock_blocking(flashLock);

			uint8_t size = sizeof(T);
			if ((index + size) <= EEPROM_SIZE_BYTES)
				memcpy(&cache[index], &value, sizeof(T));

			spin_unlock(flashLock, interrupts);
		}

	  static absolute_time_t nextWriteTime;
	private:
		void writeToFlash();
		static uint8_t cache[EEPROM_SIZE_BYTES];

};

static FlashPROM EEPROM;

#endif
