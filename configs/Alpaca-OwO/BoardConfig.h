/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2021-2022 Jason Skuby (mytechtoybox.com)
 * SPDX-FileCopyrightText: Copyright (c) 2022 Project Alpaca (project-alpaca.github.io)
 */

#ifndef _ALPACA_OWO_CONFIG_H_
#define _ALPACA_OWO_CONFIG_H_

#include <GamepadEnums.h>
#include "enums.h"

#define PIN_DPAD_DOWN   0
#define PIN_DPAD_UP     1
#define PIN_DPAD_LEFT   2
#define PIN_DPAD_RIGHT  3
#define PIN_BUTTON_A1   4
#define PIN_BUTTON_S1   5
#define PIN_BUTTON_S2   6
#define PIN_BUTTON_B3   7
#define PIN_BUTTON_B4   8
#define PIN_BUTTON_R1   9
#define PIN_BUTTON_L1   10
#define PIN_BUTTON_B1   11
#define PIN_BUTTON_B2   12
#define PIN_BUTTON_R2   13
#define PIN_BUTTON_L2   14
#define PIN_BUTTON_A2   15
#define PIN_BUTTON_L3   22
#define PIN_BUTTON_R3   23

#define DEFAULT_SOCD_MODE SOCD_MODE_NEUTRAL
#define BUTTON_LAYOUT BUTTON_LAYOUT_HITBOX

#define HAS_I2C_DISPLAY 1
#define I2C_SDA_PIN 24
#define I2C_SCL_PIN 25
#define I2C_BLOCK i2c0
#define I2C_SPEED 400000

#endif // _ALPACA_OWO_CONFIG_H_
