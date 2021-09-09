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
#include <pico/mutex.h>

#define EEPROM_SIZE_BYTES    4096       // Reserve 4k of flash memory (ensure this value is divisible by 256)
#define EEPROM_ADDRESS_START 0x101FF000 // The arduino-pico EEPROM lib starts here, so we'll do the same

class FlashPROM
{
	public:
		FlashPROM();
		void start();
		void commit();

		template<typename T>
		T &get(uint16_t const index, T &value)
		{
			mutex_enter_blocking(&flashMutex);
			if (index < EEPROM_SIZE_BYTES)
				memcpy(&value, &cache[index], sizeof(T));

			mutex_exit(&flashMutex);
			return value;
		}

		template<typename T>
		void set(uint16_t const index, const T &value)
		{
			mutex_enter_blocking(&flashMutex);
			uint8_t size = sizeof(T);
			if ((index + size) <= EEPROM_SIZE_BYTES)
				memcpy(&cache[index], &value, sizeof(T));

			mutex_exit(&flashMutex);
		}

	private:
		static uint8_t cache[EEPROM_SIZE_BYTES];
		static mutex_t flashMutex;
};

static FlashPROM EEPROM;

#endif
