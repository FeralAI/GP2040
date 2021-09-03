/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2021 Jason Skuby (mytechtoybox.com)
 */

#ifndef USB_DRIVER_H_
#define USB_DRIVER_H_

#include "GamepadDescriptors.h"

extern InputMode current_input_mode;

#ifdef __cplusplus
extern "C"
{
#endif

	void initialize_driver(void);
	void set_report(uint8_t *report, uint8_t report_size);
	void send_report(uint8_t *report, uint8_t report_size);

#ifdef __cplusplus
}
#endif

#endif
