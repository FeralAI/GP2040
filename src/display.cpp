/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2021 Jason Skuby (mytechtoybox.com)
 */

#include <string>
#include "display.h"
#include "storage.h"
#include "pico/stdlib.h"
#include "OneBitDisplay.h"
#include "bitmaps.h"

uint8_t ucBackBuffer[1024];
OBDISP obd;
string statusBar;

inline void clearScreen(int render = 0)
{
	obdFill(&obd, 0, render);
}

inline void drawDiamond(int cx, int cy, int size, uint8_t colour, uint8_t filled)
{
	if (filled) {
		int i;
		for (i = 0; i < size; i++) {
			obdDrawLine(&obd, cx - i, cy - size + i, cx + i, cy - size + i, colour, 0);
			obdDrawLine(&obd, cx - i, cy + size - i, cx + i, cy + size - i, colour, 0);
		}
		obdDrawLine(&obd, cx - size, cy, cx + size, cy, colour, 0); // Fill in the middle
	}
	obdDrawLine(&obd, cx - size, cy, cx, cy - size, colour, 0);
	obdDrawLine(&obd, cx, cy - size, cx + size, cy, colour, 0);
	obdDrawLine(&obd, cx + size, cy, cx, cy + size, colour, 0);
	obdDrawLine(&obd, cx, cy + size, cx - size, cy, colour, 0);
}

inline void drawHitbox(int startX, int startY, int buttonRadius, int buttonPadding, Gamepad *gamepad)
{
	const int buttonMargin = buttonPadding + (buttonRadius * 2);

	// Hitbox
	obdPreciseEllipse(&obd, startX, startY, buttonRadius, buttonRadius, 1, gamepad->pressedLeft());
	obdPreciseEllipse(&obd, startX + buttonMargin, startY, buttonRadius, buttonRadius, 1, gamepad->pressedDown());
	obdPreciseEllipse(&obd, startX + (buttonMargin * 1.875), startY + (buttonMargin / 2), buttonRadius, buttonRadius, 1, gamepad->pressedRight());
	obdPreciseEllipse(&obd, startX + (buttonMargin * 2.25), startY + buttonMargin * 1.875, buttonRadius, buttonRadius, 1, gamepad->pressedUp());
}

inline void drawWasdBox(int startX, int startY, int buttonRadius, int buttonPadding, Gamepad *gamepad)
{
	const int buttonMargin = buttonPadding + (buttonRadius * 2);

	// WASD
	obdPreciseEllipse(&obd, startX, startY + buttonMargin * 0.5, buttonRadius, buttonRadius, 1, gamepad->pressedLeft());
	obdPreciseEllipse(&obd, startX + buttonMargin, startY + buttonMargin * 0.875, buttonRadius, buttonRadius, 1, gamepad->pressedDown());
	obdPreciseEllipse(&obd, startX + buttonMargin * 1.5, startY - buttonMargin * 0.125, buttonRadius, buttonRadius, 1, gamepad->pressedUp());
	obdPreciseEllipse(&obd, startX + (buttonMargin * 2), startY + buttonMargin * 1.25, buttonRadius, buttonRadius, 1, gamepad->pressedRight());
}

inline void drawUDLR(int startX, int startY, int buttonRadius, int buttonPadding, Gamepad *gamepad)
{
	const int buttonMargin = buttonPadding + (buttonRadius * 2);

	// UDLR
	obdPreciseEllipse(&obd, startX, startY + buttonMargin / 2, buttonRadius, buttonRadius, 1, gamepad->pressedLeft());
	obdPreciseEllipse(&obd, startX + (buttonMargin * 0.875), startY - (buttonMargin / 4), buttonRadius, buttonRadius, 1, gamepad->pressedUp());
	obdPreciseEllipse(&obd, startX + (buttonMargin * 0.875), startY + buttonMargin * 1.25, buttonRadius, buttonRadius, 1, gamepad->pressedDown());
	obdPreciseEllipse(&obd, startX + (buttonMargin * 1.625), startY + buttonMargin / 2, buttonRadius, buttonRadius, 1, gamepad->pressedRight());
}

inline void drawArcadeStick(int startX, int startY, int buttonRadius, int buttonPadding, Gamepad *gamepad)
{
	const int buttonMargin = buttonPadding + (buttonRadius * 2);

	// Stick
	obdPreciseEllipse(&obd, startX + (buttonMargin / 2), startY + (buttonMargin / 2), buttonRadius * 1.25, buttonRadius * 1.25, 1, 0);
	
	if (gamepad->pressedUp()) {
		if (gamepad->pressedLeft()) {
			obdPreciseEllipse(&obd, startX + (buttonMargin / 5), startY + (buttonMargin / 5), buttonRadius, buttonRadius, 1, 1);
		} else if (gamepad->pressedRight()) {
			obdPreciseEllipse(&obd, startX + (buttonMargin * 0.875), startY + (buttonMargin / 5), buttonRadius, buttonRadius, 1, 1);
		} else {
			obdPreciseEllipse(&obd, startX + (buttonMargin / 2), startY, buttonRadius, buttonRadius, 1, 1);
		}
	} else if (gamepad->pressedDown()) {
		if (gamepad->pressedLeft()) {
			obdPreciseEllipse(&obd, startX + (buttonMargin / 5), startY + (buttonMargin * 0.875), buttonRadius, buttonRadius, 1, 1);
		} else if (gamepad->pressedRight()) {
			obdPreciseEllipse(&obd, startX + (buttonMargin * 0.875), startY + (buttonMargin * 0.875), buttonRadius, buttonRadius, 1, 1);
		} else {
			obdPreciseEllipse(&obd, startX + buttonMargin / 2, startY + buttonMargin, buttonRadius, buttonRadius, 1, 1);
		}
	} else if (gamepad->pressedLeft()) {
		obdPreciseEllipse(&obd, startX, startY + buttonMargin / 2, buttonRadius, buttonRadius, 1, 1);
	} else if (gamepad->pressedRight()) {
		obdPreciseEllipse(&obd, startX + buttonMargin, startY + buttonMargin / 2, buttonRadius, buttonRadius, 1, 1);
	} else {
		obdPreciseEllipse(&obd, startX + buttonMargin / 2, startY + buttonMargin / 2, buttonRadius, buttonRadius, 1, 1);
	}
}

inline void drawMAMEA(int startX, int startY, int buttonSize, int buttonPadding, Gamepad *gamepad)
{
	const int buttonMargin = buttonPadding + buttonSize;

	// MAME
	obdRectangle(&obd, startX, startY + buttonMargin, startX + buttonSize, startY + buttonSize + buttonMargin, 1, gamepad->pressedLeft());
	obdRectangle(&obd, startX + buttonMargin, startY + buttonMargin, startX + buttonSize + buttonMargin, startY + buttonSize + buttonMargin, 1, gamepad->pressedDown());
	obdRectangle(&obd, startX + buttonMargin, startY, startX + buttonSize + buttonMargin, startY + buttonSize, 1, gamepad->pressedUp());
	obdRectangle(&obd, startX + buttonMargin * 2, startY + buttonMargin, startX + buttonSize + buttonMargin * 2, startY + buttonSize + buttonMargin, 1, gamepad->pressedRight());
}

inline void drawMAMEB(int startX, int startY, int buttonSize, int buttonPadding, Gamepad *gamepad)
{
	const int buttonMargin = buttonPadding + buttonSize;

	// 6-button MAME Style
	obdRectangle(&obd, startX, startY, startX + buttonSize, startY + buttonSize, 1, gamepad->pressedB3());
	obdRectangle(&obd, startX + buttonMargin, startY, startX + buttonSize + buttonMargin, startY + buttonSize, 1, gamepad->pressedB4());
	obdRectangle(&obd, startX + buttonMargin * 2, startY, startX + buttonSize + buttonMargin * 2, startY + buttonSize, 1, gamepad->pressedR1());

	obdRectangle(&obd, startX, startY + buttonMargin, startX + buttonSize, startY + buttonMargin + buttonSize, 1, gamepad->pressedB1());
	obdRectangle(&obd, startX + buttonMargin, startY + buttonMargin, startX + buttonSize + buttonMargin, startY + buttonMargin + buttonSize, 1, gamepad->pressedB2());
	obdRectangle(&obd, startX + buttonMargin * 2, startY + buttonMargin, startX + buttonSize + buttonMargin * 2, startY + buttonMargin + buttonSize, 1, gamepad->pressedR2());

}

inline void drawMixBox(int startX, int startY, int buttonRadius, int buttonPadding, Gamepad *gamepad)
{
	const int buttonMargin = buttonPadding + (buttonRadius * 2);

	// MixBox
	drawDiamond(startX, startY, buttonRadius, 1, gamepad->pressedLeft());
	drawDiamond(startX + buttonMargin / 2, startY + buttonMargin / 2, buttonRadius, 1, gamepad->pressedDown());
	drawDiamond(startX + buttonMargin, startY, buttonRadius, 1, gamepad->pressedUp());
	drawDiamond(startX + buttonMargin, startY + buttonMargin, buttonRadius, 1, gamepad->pressedRight());
}

inline void drawDancepadA(int startX, int startY, int buttonSize, int buttonPadding, Gamepad *gamepad)
{
	const int buttonMargin = buttonPadding + buttonSize;

	// MAME
	obdRectangle(&obd, startX, startY + buttonMargin, startX + buttonSize, startY + buttonSize + buttonMargin, 1, gamepad->pressedLeft());
	obdRectangle(&obd, startX + buttonMargin, startY + buttonMargin * 2, startX + buttonSize + buttonMargin, startY + buttonSize + buttonMargin * 2, 1, gamepad->pressedDown());
	obdRectangle(&obd, startX + buttonMargin, startY, startX + buttonSize + buttonMargin, startY + buttonSize, 1, gamepad->pressedUp());
	obdRectangle(&obd, startX + buttonMargin * 2, startY + buttonMargin, startX + buttonSize + buttonMargin * 2, startY + buttonSize + buttonMargin, 1, gamepad->pressedRight());
}

inline void drawDancepadB(int startX, int startY, int buttonSize, int buttonPadding, Gamepad *gamepad)
{
	const int buttonMargin = buttonPadding + buttonSize;

	// MAME
	obdRectangle(&obd, startX, startY, startX + buttonSize, startY + buttonSize, 1, gamepad->pressedB2()); // Up/Left
	obdRectangle(&obd, startX, startY + buttonMargin * 2, startX + buttonSize, startY + buttonSize + buttonMargin * 2, 1, gamepad->pressedB4()); // Down/Left
	obdRectangle(&obd, startX + buttonMargin * 2, startY, startX + buttonSize + buttonMargin * 2, startY + buttonSize, 1, gamepad->pressedB1()); // Up/Right
	obdRectangle(&obd, startX + buttonMargin * 2, startY + buttonMargin * 2, startX + buttonSize + buttonMargin * 2, startY + buttonSize + buttonMargin * 2, 1, gamepad->pressedB3()); // Down/Right
}

inline void drawVewlix(int startX, int startY, int buttonRadius, int buttonPadding, Gamepad *gamepad)
{
	const int buttonMargin = buttonPadding + (buttonRadius * 2);

	// 8-button Vewlix
	obdPreciseEllipse(&obd, startX + (buttonMargin * 2.75), startY + (buttonMargin * 0.2), buttonRadius, buttonRadius, 1, gamepad->pressedB3());
	obdPreciseEllipse(&obd, startX + (buttonMargin * 3.75), startY - (buttonMargin / 4), buttonRadius, buttonRadius, 1, gamepad->pressedB4());
	obdPreciseEllipse(&obd, startX + (buttonMargin * 4.75), startY - (buttonMargin / 4), buttonRadius, buttonRadius, 1, gamepad->pressedR1());
	obdPreciseEllipse(&obd, startX + (buttonMargin * 5.75), startY - (buttonMargin / 4), buttonRadius, buttonRadius, 1, gamepad->pressedL1());

	obdPreciseEllipse(&obd, startX + (buttonMargin * 2.75) - (buttonMargin / 3), startY + buttonMargin + (buttonMargin * 0.2), buttonRadius, buttonRadius, 1, gamepad->pressedB1());
	obdPreciseEllipse(&obd, startX + (buttonMargin * 3.75) - (buttonMargin / 3), startY + buttonMargin - (buttonMargin / 4), buttonRadius, buttonRadius, 1, gamepad->pressedB2());
	obdPreciseEllipse(&obd, startX + (buttonMargin * 4.75) - (buttonMargin / 3), startY + buttonMargin - (buttonMargin / 4), buttonRadius, buttonRadius, 1, gamepad->pressedR2());
	obdPreciseEllipse(&obd, startX + (buttonMargin * 5.75) - (buttonMargin / 3), startY + buttonMargin - (buttonMargin / 4), buttonRadius, buttonRadius, 1, gamepad->pressedL2());
}

inline void drawSega2p(int startX, int startY, int buttonRadius, int buttonPadding, Gamepad *gamepad)
{
	const int buttonMargin = buttonPadding + (buttonRadius * 2);

	// 8-button Sega2P
	obdPreciseEllipse(&obd, startX + (buttonMargin * 2.75), startY + (buttonMargin / 3), buttonRadius, buttonRadius, 1, gamepad->pressedB3());
	obdPreciseEllipse(&obd, startX + (buttonMargin * 3.75), startY - (buttonMargin / 4), buttonRadius, buttonRadius, 1, gamepad->pressedB4());
	obdPreciseEllipse(&obd, startX + (buttonMargin * 4.75), startY - (buttonMargin / 4), buttonRadius, buttonRadius, 1, gamepad->pressedR1());
	obdPreciseEllipse(&obd, startX + (buttonMargin * 5.75), startY, buttonRadius, buttonRadius, 1, gamepad->pressedL1());

	obdPreciseEllipse(&obd, startX + (buttonMargin * 2.75), startY + buttonMargin + (buttonMargin / 3), buttonRadius, buttonRadius, 1, gamepad->pressedB1());
	obdPreciseEllipse(&obd, startX + (buttonMargin * 3.75), startY + buttonMargin - (buttonMargin / 4), buttonRadius, buttonRadius, 1, gamepad->pressedB2());
	obdPreciseEllipse(&obd, startX + (buttonMargin * 4.75), startY + buttonMargin - (buttonMargin / 4), buttonRadius, buttonRadius, 1, gamepad->pressedR2());
	obdPreciseEllipse(&obd, startX + (buttonMargin * 5.75), startY + buttonMargin, buttonRadius, buttonRadius, 1, gamepad->pressedL2());
}

inline void drawNoir8(int startX, int startY, int buttonRadius, int buttonPadding, Gamepad *gamepad)
{
	const int buttonMargin = buttonPadding + (buttonRadius * 2);

	// 8-button Noir8
	obdPreciseEllipse(&obd, startX + (buttonMargin * 2.75), startY + (buttonMargin / 3.5), buttonRadius, buttonRadius, 1, gamepad->pressedB3());
	obdPreciseEllipse(&obd, startX + (buttonMargin * 3.75), startY - (buttonMargin / 4), buttonRadius, buttonRadius, 1, gamepad->pressedB4());
	obdPreciseEllipse(&obd, startX + (buttonMargin * 4.75), startY - (buttonMargin / 4), buttonRadius, buttonRadius, 1, gamepad->pressedR1());
	obdPreciseEllipse(&obd, startX + (buttonMargin * 5.75), startY, buttonRadius, buttonRadius, 1, gamepad->pressedL1());

	obdPreciseEllipse(&obd, startX + (buttonMargin * 2.75), startY + buttonMargin + (buttonMargin / 3.5), buttonRadius, buttonRadius, 1, gamepad->pressedB1());
	obdPreciseEllipse(&obd, startX + (buttonMargin * 3.75), startY + buttonMargin - (buttonMargin / 4), buttonRadius, buttonRadius, 1, gamepad->pressedB2());
	obdPreciseEllipse(&obd, startX + (buttonMargin * 4.75), startY + buttonMargin - (buttonMargin / 4), buttonRadius, buttonRadius, 1, gamepad->pressedR2());
	obdPreciseEllipse(&obd, startX + (buttonMargin * 5.75), startY + buttonMargin, buttonRadius, buttonRadius, 1, gamepad->pressedL2());
}

inline void drawCapcom(int startX, int startY, int buttonRadius, int buttonPadding, Gamepad *gamepad)
{
	const int buttonMargin = buttonPadding + (buttonRadius * 2);

	// 8-button Capcom
	obdPreciseEllipse(&obd, startX + buttonMargin * 3.25, startY, buttonRadius, buttonRadius, 1, gamepad->pressedB3());
	obdPreciseEllipse(&obd, startX + buttonMargin * 4.25, startY, buttonRadius, buttonRadius, 1, gamepad->pressedB4());
	obdPreciseEllipse(&obd, startX + buttonMargin * 5.25, startY, buttonRadius, buttonRadius, 1, gamepad->pressedR1());
	obdPreciseEllipse(&obd, startX + buttonMargin * 6.25, startY, buttonRadius, buttonRadius, 1, gamepad->pressedL1());

	obdPreciseEllipse(&obd, startX + buttonMargin * 3.25, startY + buttonMargin, buttonRadius, buttonRadius, 1, gamepad->pressedB1());
	obdPreciseEllipse(&obd, startX + buttonMargin * 4.25, startY + buttonMargin, buttonRadius, buttonRadius, 1, gamepad->pressedB2());
	obdPreciseEllipse(&obd, startX + buttonMargin * 5.25, startY + buttonMargin, buttonRadius, buttonRadius, 1, gamepad->pressedR2());
	obdPreciseEllipse(&obd, startX + buttonMargin * 6.25, startY + buttonMargin, buttonRadius, buttonRadius, 1, gamepad->pressedL2());
}

inline void drawCapcom6(int startX, int startY, int buttonRadius, int buttonPadding, Gamepad *gamepad)
{
	const int buttonMargin = buttonPadding + (buttonRadius * 2);

	// 6-button Capcom
	obdPreciseEllipse(&obd, startX + buttonMargin * 3.25, startY, buttonRadius, buttonRadius, 1, gamepad->pressedB3());
	obdPreciseEllipse(&obd, startX + buttonMargin * 4.25, startY, buttonRadius, buttonRadius, 1, gamepad->pressedB4());
	obdPreciseEllipse(&obd, startX + buttonMargin * 5.25, startY, buttonRadius, buttonRadius, 1, gamepad->pressedR1());

	obdPreciseEllipse(&obd, startX + buttonMargin * 3.25, startY + buttonMargin, buttonRadius, buttonRadius, 1, gamepad->pressedB1());
	obdPreciseEllipse(&obd, startX + buttonMargin * 4.25, startY + buttonMargin, buttonRadius, buttonRadius, 1, gamepad->pressedB2());
	obdPreciseEllipse(&obd, startX + buttonMargin * 5.25, startY + buttonMargin, buttonRadius, buttonRadius, 1, gamepad->pressedR2());
}

inline void drawHitboxButtons(int startX, int startY, int buttonRadius, int buttonPadding, Gamepad *gamepad)
{
	const int buttonMargin = buttonPadding + (buttonRadius * 2);

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

inline void drawWasdButtons(int startX, int startY, int buttonRadius, int buttonPadding, Gamepad *gamepad)
{
	const int buttonMargin = buttonPadding + (buttonRadius * 2);

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

inline void drawArcadeButtons(int startX, int startY, int buttonRadius, int buttonPadding, Gamepad *gamepad)
{
	const int buttonMargin = buttonPadding + (buttonRadius * 2);

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

inline void drawSplashScreen(int splashMode, int splashSpeed)
{
    int mils = getMillis();
    switch (splashMode)
	{
		case STATICSPLASH: // Default, display static or custom image
            if ((sizeof(splashCustom) / sizeof(*splashCustom)) > 0) {
                obdDrawSprite(&obd, (uint8_t *)splashCustom, 128, 64, 16, 0, 0, 1);
            } else {
			    obdDrawSprite(&obd, (uint8_t *)splashImage, 128, 64, 16, 0, 0, 1);
            }
			break;
		case CLOSEIN: // Close-in. Animate the GP2040 logo
			obdDrawSprite(&obd, (uint8_t *)bootLogoTop, 43, 39, 6, 43, std::min<int>((mils / splashSpeed) - 39, 0), 1);
			obdDrawSprite(&obd, (uint8_t *)bootLogoBottom, 80, 21, 10, 24, std::max<int>(64 - (mils / (splashSpeed * 2)), 44), 1);
			break;
        case CLOSEINCUSTOM: // Close-in on custom image or delayed close-in if custom image does not exist
            if ((sizeof(splashCustom) / sizeof(*splashCustom)) > 0) {
               obdDrawSprite(&obd, (uint8_t *)splashCustom, 128, 64, 16, 0, 0, 1);
            }
            if (mils > 2500) {
                int milss = mils - 2500;
                obdRectangle(&obd, 0, 0, 127, 1 + (milss / splashSpeed), 0, 1);
                obdRectangle(&obd, 0, 63, 127, 62 - (milss / (splashSpeed * 2)), 0, 1);
                obdDrawSprite(&obd, (uint8_t *)bootLogoTop, 43, 39, 6, 43, std::min<int>((milss / splashSpeed) - 39, 0), 1);
                obdDrawSprite(&obd, (uint8_t *)bootLogoBottom, 80, 21, 10, 24, std::max<int>(64 - (milss / (splashSpeed * 2)), 44), 1);
            }
            break;
	}
}

inline void drawStatusBar()
{
	obdWriteString(&obd, 0, 0, 0, (char *)statusBar.c_str(), FONT_6x8, 0, 0);
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
	// Check timer for splash
	if (getMillis() < 7500 && SPLASH_MODE != NOSPLASH) {
		clearScreen();
		drawSplashScreen(SPLASH_MODE, 90);
	} else {
		clearScreen();

		setStatusBar(gamepad);

		drawStatusBar();

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

			case BUTTON_LAYOUT_UDLR:
				drawUDLR(8, 28, 8, 2, gamepad);
				break;

			case BUTTON_LAYOUT_MIXBOX:
				drawMixBox(18, 28, 5, 2, gamepad);
				break;

			case BUTTON_LAYOUT_MAMEA:
				drawMAMEA(8, 28, 10, 1, gamepad);
				//drawWasdBox(55, 28, 7, 3, gamepad);
				break;

			case BUTTON_LAYOUT_DANCEPADA:
				drawDancepadA(39, 12, 15, 2, gamepad);
				break;
		}

		switch (BUTTON_LAYOUT_RIGHT)
		{
			case BUTTON_LAYOUT_ARCADEB:
				drawArcadeButtons(8, 28, 8, 2, gamepad);
				break;
			case BUTTON_LAYOUT_HITBOXB:
				drawHitboxButtons(8, 20, 8, 2, gamepad);
				break;
			case BUTTON_LAYOUT_WASDB:
				drawWasdButtons(8, 28, 7, 3, gamepad);
				break;
			case BUTTON_LAYOUT_VEWLIX:
				drawVewlix(8, 28, 8, 2, gamepad);
				break;
			case BUTTON_LAYOUT_CAPCOM:
				drawCapcom(6, 28, 8, 2, gamepad);
				break;
			case BUTTON_LAYOUT_CAPCOM6:
				drawCapcom6(16, 28, 8, 2, gamepad);
				break;
			case BUTTON_LAYOUT_SEGA2P:
				drawSega2p(8, 28, 8, 2, gamepad);
				break;
			case BUTTON_LAYOUT_NOIR8:
				drawNoir8(8, 28, 8, 2, gamepad);
				break;
			case BUTTON_LAYOUT_DANCEPADB:
				drawDancepadB(39, 12, 15, 2, gamepad);
				break;
			case BUTTON_LAYOUT_MAMEB:
				drawMAMEB(68, 28, 10, 1, gamepad);
				break;
		}
	}

	obdDumpBuffer(&obd, NULL);
}
