/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2021 Jason Skuby (mytechtoybox.com)
 */

#include <string>
#include "display.h"
#include "storage.h"
#include "pico/stdlib.h"
#include "OneBitDisplay.h"
#include <deque>

uint8_t ucBackBuffer[1024];
OBDISP obd;
string statusBar;
std::deque<string> history;

inline void clearScreen(int render = 0)
{
	obdFill(&obd, 0, render);
}

inline void drawHitbox(int startX, int startY, int buttonRadius, int buttonPadding, Gamepad *gamepad)
{
	const int buttonMargin = buttonPadding + (buttonRadius * 2);

	// UDLR
	obdPreciseEllipse(&obd, startX, startY, buttonRadius, buttonRadius, 1, gamepad->pressedLeft());
	obdPreciseEllipse(&obd, startX + buttonMargin, startY, buttonRadius, buttonRadius, 1, gamepad->pressedDown());
	obdPreciseEllipse(&obd, startX + (buttonMargin * 1.875), startY + (buttonMargin / 2), buttonRadius, buttonRadius, 1, gamepad->pressedRight());
	obdPreciseEllipse(&obd, startX + (buttonMargin * 2.25), startY + buttonMargin * 1.875, buttonRadius, buttonRadius, 1, gamepad->pressedUp());

	// 8-button
	obdPreciseEllipse(&obd, startX + (buttonMargin * 2.75), startY, buttonRadius, buttonRadius, 1, gamepad->pressedB3());
	obdPreciseEllipse(&obd, startX + (buttonMargin * 3.75), startY - (buttonMargin / 4), buttonRadius, buttonRadius, 1, gamepad->pressedB4());
	obdPreciseEllipse(&obd, startX + (buttonMargin * 4.75), startY - (buttonMargin / 4), buttonRadius, buttonRadius, 1, gamepad->pressedR1());
	obdPreciseEllipse(&obd, startX + (buttonMargin * 5.75), startY, buttonRadius, buttonRadius, 1, gamepad->pressedL1());

	obdPreciseEllipse(&obd, startX + (buttonMargin * 2.75), startY + buttonMargin, buttonRadius, buttonRadius, 1, gamepad->pressedB1());
	obdPreciseEllipse(&obd, startX + (buttonMargin * 3.75), startY + buttonMargin - (buttonMargin / 4), buttonRadius, buttonRadius, 1, gamepad->pressedB2());
	obdPreciseEllipse(&obd, startX + (buttonMargin * 4.75), startY + buttonMargin - (buttonMargin / 4), buttonRadius, buttonRadius, 1, gamepad->pressedR2());
	obdPreciseEllipse(&obd, startX + (buttonMargin * 5.75), startY + buttonMargin, buttonRadius, buttonRadius, 1, gamepad->pressedL2());
}

inline void drawWasdBox(int startX, int startY, int buttonRadius, int buttonPadding, Gamepad *gamepad)
{
	const int buttonMargin = buttonPadding + (buttonRadius * 2);

	// UDLR
	obdPreciseEllipse(&obd, startX, startY + buttonMargin * 0.5, buttonRadius, buttonRadius, 1, gamepad->pressedLeft());
	obdPreciseEllipse(&obd, startX + buttonMargin, startY + buttonMargin * 0.875, buttonRadius, buttonRadius, 1, gamepad->pressedDown());
	obdPreciseEllipse(&obd, startX + buttonMargin * 1.5, startY - buttonMargin * 0.125, buttonRadius, buttonRadius, 1, gamepad->pressedUp());
	obdPreciseEllipse(&obd, startX + (buttonMargin * 2), startY + buttonMargin * 1.25, buttonRadius, buttonRadius, 1, gamepad->pressedRight());

	// 8-button
	obdPreciseEllipse(&obd, startX + buttonMargin * 3.625, startY, buttonRadius, buttonRadius, 1, gamepad->pressedB3());
	obdPreciseEllipse(&obd, startX + buttonMargin * 4.625, startY - (buttonMargin / 4), buttonRadius, buttonRadius, 1, gamepad->pressedB4());
	obdPreciseEllipse(&obd, startX + buttonMargin * 5.625, startY - (buttonMargin / 4), buttonRadius, buttonRadius, 1, gamepad->pressedR1());
	obdPreciseEllipse(&obd, startX + buttonMargin * 6.625, startY, buttonRadius, buttonRadius, 1, gamepad->pressedL1());

	obdPreciseEllipse(&obd, startX + buttonMargin * 3.25, startY + buttonMargin, buttonRadius, buttonRadius, 1, gamepad->pressedB1());
	obdPreciseEllipse(&obd, startX + buttonMargin * 4.25, startY + buttonMargin - (buttonMargin / 4), buttonRadius, buttonRadius, 1, gamepad->pressedB2());
	obdPreciseEllipse(&obd, startX + buttonMargin * 5.25, startY + buttonMargin - (buttonMargin / 4), buttonRadius, buttonRadius, 1, gamepad->pressedR2());
	obdPreciseEllipse(&obd, startX + buttonMargin * 6.25, startY + buttonMargin, buttonRadius, buttonRadius, 1, gamepad->pressedL2());
}

inline void drawArcadeStick(int startX, int startY, int buttonRadius, int buttonPadding, Gamepad *gamepad)
{
	const int buttonMargin = buttonPadding + (buttonRadius * 2);

	// UDLR
	obdPreciseEllipse(&obd, startX, startY + buttonMargin / 2, buttonRadius, buttonRadius, 1, gamepad->pressedLeft());
	obdPreciseEllipse(&obd, startX + (buttonMargin * 0.875), startY - (buttonMargin / 4), buttonRadius, buttonRadius, 1, gamepad->pressedUp());
	obdPreciseEllipse(&obd, startX + (buttonMargin * 0.875), startY + buttonMargin * 1.25, buttonRadius, buttonRadius, 1, gamepad->pressedDown());
	obdPreciseEllipse(&obd, startX + (buttonMargin * 1.625), startY + buttonMargin / 2, buttonRadius, buttonRadius, 1, gamepad->pressedRight());

	// 8-button
	obdPreciseEllipse(&obd, startX + buttonMargin * 3.125, startY, buttonRadius, buttonRadius, 1, gamepad->pressedB3());
	obdPreciseEllipse(&obd, startX + buttonMargin * 4.125, startY - (buttonMargin / 4), buttonRadius, buttonRadius, 1, gamepad->pressedB4());
	obdPreciseEllipse(&obd, startX + buttonMargin * 5.125, startY - (buttonMargin / 4), buttonRadius, buttonRadius, 1, gamepad->pressedR1());
	obdPreciseEllipse(&obd, startX + buttonMargin * 6.125, startY, buttonRadius, buttonRadius, 1, gamepad->pressedL1());

	obdPreciseEllipse(&obd, startX + buttonMargin * 2.875, startY + buttonMargin, buttonRadius, buttonRadius, 1, gamepad->pressedB1());
	obdPreciseEllipse(&obd, startX + buttonMargin * 3.875, startY + buttonMargin - (buttonMargin / 4), buttonRadius, buttonRadius, 1, gamepad->pressedB2());
	obdPreciseEllipse(&obd, startX + buttonMargin * 4.875, startY + buttonMargin - (buttonMargin / 4), buttonRadius, buttonRadius, 1, gamepad->pressedR2());
	obdPreciseEllipse(&obd, startX + buttonMargin * 5.875, startY + buttonMargin, buttonRadius, buttonRadius, 1, gamepad->pressedL2());
}

inline void drawStatusBar()
{
	obdWriteString(&obd, 0, 0, 0, (char *)statusBar.c_str(), FONT_6x8, 0, 0);
}

inline void drawHistory()
{
	std::string ret;

	for (auto it = history.crbegin(); it != history.crend(); ++it) {
		if (ret.size() < 22) {
			std::string newRet = ret;
			if (!newRet.empty())
				newRet = " " + newRet;
		
			newRet = *it + newRet;

			if (newRet.size() < 22) {
				ret = newRet;
			}
			else {
				break;
			}
  		}
	}

	obdWriteString(&obd, 0, 0, 1, (char *)ret.c_str(), FONT_6x8, 0, 0);
}

void setStatusBar(Gamepad *gamepad)
{
	// Limit to 21 chars with 6x8 font for now
	statusBar.clear();

	switch (gamepad->options.inputMode)
	{
		case INPUT_MODE_HID:    statusBar += "DINPUT"; break;
		case INPUT_MODE_SWITCH: statusBar += "SWITCH"; break;
		case INPUT_MODE_XINPUT: statusBar += "XINPUT"; break;
		case INPUT_MODE_CONFIG: statusBar += "CONFIG"; break;
	}

	switch (gamepad->options.dpadMode)
	{

		case DPAD_MODE_DIGITAL:      statusBar += "         DPAD"; break;
		case DPAD_MODE_LEFT_ANALOG:  statusBar += "         LEFT"; break;
		case DPAD_MODE_RIGHT_ANALOG: statusBar += "        RIGHT"; break;
	}

	switch (gamepad->options.socdMode)
	{
		case SOCD_MODE_NEUTRAL:               statusBar += "-N"; break;
		case SOCD_MODE_UP_PRIORITY:           statusBar += "-U"; break;
		case SOCD_MODE_SECOND_INPUT_PRIORITY: statusBar += "-L"; break;
	}
}

void setHistory(Gamepad *gamepad)
{
	std::deque<string> pressed;

	if (gamepad->pressedUp()) {
		pressed.push_back("U");	
	}

	if (gamepad->pressedDown()) {
		pressed.push_back("D");	
	}
	
	if (gamepad->pressedLeft()) {
		pressed.push_back("L");	
	}

	if (gamepad->pressedRight()) {
		pressed.push_back("R");	
	}

	if (gamepad->pressedB1()) {
		pressed.push_back("1K");	
	}	

	if (gamepad->pressedB2()) {
		pressed.push_back("2K");	
	}	
	
	if (gamepad->pressedR2()) {
		pressed.push_back("3K");	
	}	
	
	if (gamepad->pressedL2()) {
		pressed.push_back("4K");	
	}	

	if (gamepad->pressedB3()) {
		pressed.push_back("1P");	
	}	

	if (gamepad->pressedB4()) {
		pressed.push_back("2P");	
	}	
	
	if (gamepad->pressedR1()) {
		pressed.push_back("3P");	
	}	
	
	if (gamepad->pressedL1()) {
		pressed.push_back("4P");	
	}		

	if (pressed.size() > 0) {
	    std::string newInput;
	    for(const auto &s : pressed) {
	        if(!newInput.empty())
	            newInput += "+";
	        newInput += s;
	    }

		if (history.empty() || history.back() != newInput) {
			history.push_back(newInput);
		}
	}

	if (history.size() > 10) {
		history.pop_front();
	}
}

void DisplayModule::setup()
{
	BoardOptions options = getBoardOptions();
	enabled = options.hasI2CDisplay && options.i2cSDAPin != -1 && options.i2cSCLPin != -1;
	if (enabled)
	{
		obdI2CInit(&obd,
			options.displaySize,
			options.displayI2CAddress,
			options.displayFlip,
			options.displayInvert,
			DISPLAY_USEWIRE,
			options.i2cSDAPin,
			options.i2cSCLPin,
			options.i2cBlock == 0 ? i2c0 : i2c1,
			-1,
			options.i2cSpeed);

		obdSetContrast(&obd, 0xFF);
		obdSetBackBuffer(&obd, ucBackBuffer);
		clearScreen(1);
	}
}

void DisplayModule::loop()
{
	// All screen updates should be handled in process() as they need to display ASAP
}

void DisplayModule::process(Gamepad *gamepad)
{
	BoardOptions options = getBoardOptions();

	clearScreen();

	setStatusBar(gamepad);

	drawStatusBar();

	// Display history only for 128x32 displays for now
	if (options.displaySize == 2) {
		setHistory(gamepad);
		drawHistory();
	}
	else {	
		switch (BUTTON_LAYOUT)
		{
			case BUTTON_LAYOUT_ARCADE:
				drawArcadeStick(8, 28, 8, 2, gamepad);
				break;

			case BUTTON_LAYOUT_HITBOX:
				drawHitbox(8, 20, 8, 2, gamepad);
				break;

			case BUTTON_LAYOUT_WASD:
				drawWasdBox(8, 28, 7, 3, gamepad);
				break;
		}
	}

	obdDumpBuffer(&obd, NULL);
}
