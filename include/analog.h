/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2021 Jason Skuby (mytechtoybox.com)
 */

#pragma once

#include <stdint.h>

#ifndef PIN_ANALOG_LX
#define PIN_ANALOG_LX -1
#endif
#ifndef PIN_ANALOG_LY
#define PIN_ANALOG_LY -1
#endif
#ifndef PIN_ANALOG_RX
#define PIN_ANALOG_RX -1
#endif
#ifndef PIN_ANALOG_RY
#define PIN_ANALOG_RY -1
#endif

#define ADC_MAX ((1 << 12) - 1)

inline static uint16_t scaleValue(uint16_t value, uint16_t fromMin, uint16_t fromMax, uint16_t toMin, uint16_t toMax)
{
	return (value - fromMin) * (toMax - toMin) / (fromMax - fromMin) + toMin;
}

inline static uint16_t scaleInternalADCValue(uint16_t value)
{
	return value * UINT16_MAX / ADC_MAX;
}
