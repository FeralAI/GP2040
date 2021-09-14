/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2021 Jason Skuby (mytechtoybox.com)
 */

#include <GamepadStorage.h>
#include "FlashPROM.h"
#include "BoardConfig.h"

static void getStorageValue(int index, void *data, uint16_t size)
{
	uint8_t buffer[size] = { };
	for (int i = 0; i < size; i++)
		EEPROM.get(index + i, buffer[i]);

	memcpy(data, buffer, size);
}

static void setStorageValue(int index, void *data, uint16_t size)
{
	uint8_t buffer[size] = { };
	memcpy(buffer, data, size);
	for (int i = 0; i < size; i++)
		EEPROM.set(index + i, buffer[i]);
}

/* Gamepad stuffs */

void GamepadStorage::start()
{
	EEPROM.start();
}

void GamepadStorage::save()
{
	EEPROM.commit();
}

void GamepadStorage::get(int index, void *data, uint16_t size)
{
	getStorageValue(index, data, size);
}

void GamepadStorage::set(int index, void *data, uint16_t size)
{
	setStorageValue(index, data, size);
}

/* Animation stuffs */

#ifdef BOARD_LEDS_PIN

#include "AnimationStorage.hpp"
#include "AnimationStation/src/Effects/StaticColor.hpp"

#define STORAGE_LEDS_BRIGHTNESS_INDEX (STORAGE_FIRST_AVAILBLE_INDEX)              // 1 byte
#define STORAGE_LEDS_BASE_ANIMATION_MODE_INDEX (STORAGE_FIRST_AVAILBLE_INDEX + 1) // 1 byte

uint8_t AnimationStorage::getMode()
{
	uint8_t mode = 0;
	getStorageValue(STORAGE_LEDS_BASE_ANIMATION_MODE_INDEX, &mode, sizeof(uint8_t));
	return mode;
}

void AnimationStorage::setMode(uint8_t mode)
{
	setStorageValue(STORAGE_LEDS_BASE_ANIMATION_MODE_INDEX, &mode, sizeof(uint8_t));
}

uint8_t AnimationStorage::getBrightness()
{
	uint8_t brightness = LEDS_BRIGHTNESS;
	getStorageValue(STORAGE_LEDS_BRIGHTNESS_INDEX, &brightness, sizeof(uint8_t));
	return brightness;
}

void AnimationStorage::setBrightness(uint8_t brightness)
{
	setStorageValue(STORAGE_LEDS_BRIGHTNESS_INDEX, &brightness, sizeof(uint8_t));
}

void AnimationStorage::setup(AnimationStation *as)
{
	this->as = as;
	AnimationStation::SetBrightness(this->getBrightness());
	as->SetMode(getMode());
	configureAnimations(as);
}

void AnimationStorage::save()
{
	bool dirty = false;

	uint8_t brightness = as->GetBrightness();
	if (brightness != getBrightness())
	{
		setBrightness(brightness);
		dirty = true;
	}

	uint8_t mode = as->GetMode();
	if (mode != getMode())
	{
		setMode(mode);
		dirty = true;
	}

	if (dirty)
		EEPROM.commit();
}

#endif
