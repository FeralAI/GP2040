/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2021 Jason Skuby (mytechtoybox.com)
 */

#ifndef PICO_BOARD_CONFIG_H_
#define PICO_BOARD_CONFIG_H_

#include <GamepadEnums.h>

#define PIN_DPAD_UP     2
#define PIN_DPAD_DOWN   3
#define PIN_DPAD_RIGHT  5
#define PIN_DPAD_LEFT   4
#define PIN_BUTTON_B1   9
#define PIN_BUTTON_B2   10
#define PIN_BUTTON_R2   15
#define PIN_BUTTON_L2   12
#define PIN_BUTTON_B3   13
#define PIN_BUTTON_B4   17
#define PIN_BUTTON_R1   11
#define PIN_BUTTON_L1   16
#define PIN_BUTTON_S1   8
#define PIN_BUTTON_S2   7

// Not connected on mine
#define PIN_BUTTON_L3   18
#define PIN_BUTTON_R3   19
#define PIN_BUTTON_A1   20
#define PIN_BUTTON_A2   21

#define DEFAULT_SOCD_MODE SOCD_MODE_NEUTRAL
#define BUTTON_LAYOUT BUTTON_LAYOUT_ARCADE

#endif
