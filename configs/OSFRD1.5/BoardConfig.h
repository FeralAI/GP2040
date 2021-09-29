/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2021 Jason Skuby (mytechtoybox.com)
 */

#ifndef OSFRD_BOARD_CONFIG_H_
#define OSFRD_BOARD_CONFIG_H_

#include <vector>
#include <GamepadEnums.h>
#include <GamepadState.h>
#include "Pixel.hpp"

#define PIN_DPAD_UP     20
#define PIN_DPAD_DOWN   8
#define PIN_DPAD_LEFT   1
#define PIN_DPAD_RIGHT  14
#define PIN_BUTTON_B1   18
#define PIN_BUTTON_B2   17
#define PIN_BUTTON_B3   13
#define PIN_BUTTON_B4   9
#define PIN_BUTTON_L1   12
#define PIN_BUTTON_R1   10
#define PIN_BUTTON_L2   19
#define PIN_BUTTON_R2   16
#define PIN_BUTTON_S1   3
#define PIN_BUTTON_S2   0
#define PIN_BUTTON_L3   6
#define PIN_BUTTON_R3   7
#define PIN_BUTTON_A1   4
#define PIN_BUTTON_A2   5

#define BOARD_LEDS_PIN 2
#define LED_BRIGHTNESS_MAXIMUM 100
#define LED_BRIGHTNESS_STEPS 5

#define LEDS_DPAD_LEFT   0
#define LEDS_DPAD_DOWN   1
#define LEDS_DPAD_RIGHT  2
#define LEDS_DPAD_UP     3
#define LEDS_BUTTON_03   4
#define LEDS_BUTTON_04   5
#define LEDS_BUTTON_06   6
#define LEDS_BUTTON_05   7
#define LEDS_BUTTON_01   8
#define LEDS_BUTTON_02   9
#define LEDS_BUTTON_07   11
#define LEDS_BUTTON_08   10

#define DEFAULT_SOCD_MODE SOCD_MODE_NEUTRAL

const static std::vector<Pixel> pixels =
{
	{ .index = LEDS_DPAD_LEFT,  .mask = GAMEPAD_MASK_DL, .positions = { LEDS_DPAD_LEFT } },
	{ .index = LEDS_DPAD_DOWN,  .mask = GAMEPAD_MASK_DD, .positions = { LEDS_DPAD_DOWN } },
	{ .index = LEDS_DPAD_RIGHT, .mask = GAMEPAD_MASK_DR, .positions = { LEDS_DPAD_RIGHT } },
	{ .index = LEDS_DPAD_UP,    .mask = GAMEPAD_MASK_DU, .positions = { LEDS_DPAD_UP } },
	{ .index = LEDS_BUTTON_03,  .mask = GAMEPAD_MASK_B3, .positions = { LEDS_BUTTON_03 } },
	{ .index = LEDS_BUTTON_04,  .mask = GAMEPAD_MASK_B4, .positions = { LEDS_BUTTON_04 } },
	{ .index = LEDS_BUTTON_06,  .mask = GAMEPAD_MASK_R1, .positions = { LEDS_BUTTON_06 } },
	{ .index = LEDS_BUTTON_05,  .mask = GAMEPAD_MASK_L1, .positions = { LEDS_BUTTON_05 } },
	{ .index = LEDS_BUTTON_01,  .mask = GAMEPAD_MASK_B1, .positions = { LEDS_BUTTON_01 } },
	{ .index = LEDS_BUTTON_02,  .mask = GAMEPAD_MASK_B2, .positions = { LEDS_BUTTON_02 } },
	{ .index = LEDS_BUTTON_08,  .mask = GAMEPAD_MASK_R2, .positions = { LEDS_BUTTON_08 } },
	{ .index = LEDS_BUTTON_07,  .mask = GAMEPAD_MASK_L2, .positions = { LEDS_BUTTON_07 } },
};

#endif