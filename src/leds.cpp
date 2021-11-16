/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2021 Jason Skuby (mytechtoybox.com)
 */

#include "BoardConfig.h"
#include <string>
#include <map>
#include "pico/util/queue.h"

#include "AnimationStation.hpp"
#include "AnimationStorage.hpp"
#include "NeoPico.hpp"
#include "Pixel.hpp"
#include "PlayerLEDs.h"
#include "gp2040.h"
#include "leds.h"
#include "storage.h"
#include "themes.h"

using namespace std;

static vector<uint8_t> EMPTY_VECTOR;
extern void setRGBPLEDs(uint32_t *frame);

uint8_t ledCount;
PixelMatrix matrix;
NeoPico *neopico;
LEDOptions ledOptions;
AnimationStation as;
queue_t baseAnimationQueue;
queue_t buttonAnimationQueue;
queue_t animationSaveQueue;
map<string, int> buttonPositions;

inline vector<uint8_t> *getLEDPositions(string button, vector<vector<uint8_t>> *positions)
{
	int buttonPosition = buttonPositions[button];
	if (buttonPosition < 0)
		return &EMPTY_VECTOR;
	else
		return &positions->at(buttonPosition);
}

/**
 * @brief Create an LED layout using a 2x4 matrix.
 */
PixelMatrix createLedLayoutArcadeButtons(vector<vector<uint8_t>> *positions)
{
	vector<vector<Pixel>> pixels =
	{
		{
			Pixel(buttonPositions[BUTTON_LABEL_B3], GAMEPAD_MASK_B3, *getLEDPositions(BUTTON_LABEL_B3, positions)),
			Pixel(buttonPositions[BUTTON_LABEL_B1], GAMEPAD_MASK_B1, *getLEDPositions(BUTTON_LABEL_B1, positions)),
		},
		{
			Pixel(buttonPositions[BUTTON_LABEL_B4], GAMEPAD_MASK_B4, *getLEDPositions(BUTTON_LABEL_B4, positions)),
			Pixel(buttonPositions[BUTTON_LABEL_B2], GAMEPAD_MASK_B2, *getLEDPositions(BUTTON_LABEL_B2, positions)),
		},
		{
			Pixel(buttonPositions[BUTTON_LABEL_R1], GAMEPAD_MASK_R1, *getLEDPositions(BUTTON_LABEL_R1, positions)),
			Pixel(buttonPositions[BUTTON_LABEL_R2], GAMEPAD_MASK_R2, *getLEDPositions(BUTTON_LABEL_R2, positions)),
		},
		{
			Pixel(buttonPositions[BUTTON_LABEL_L1], GAMEPAD_MASK_L1, *getLEDPositions(BUTTON_LABEL_L1, positions)),
			Pixel(buttonPositions[BUTTON_LABEL_L2], GAMEPAD_MASK_L2, *getLEDPositions(BUTTON_LABEL_L2, positions)),
		},
	};

	PixelMatrix matrix;
	matrix.setup(pixels);
	return matrix;
}

/**
 * @brief Create an LED layout using a 3x8 matrix.
 */
PixelMatrix createLedLayoutArcadeHitbox(vector<vector<uint8_t>> *positions)
{
	vector<vector<Pixel>> pixels =
	{
		{
			Pixel(buttonPositions[BUTTON_LABEL_LEFT], GAMEPAD_MASK_DL, *getLEDPositions(BUTTON_LABEL_LEFT, positions)),
			NO_PIXEL,
			NO_PIXEL,
		},
		{
			Pixel(buttonPositions[BUTTON_LABEL_DOWN], GAMEPAD_MASK_DD, *getLEDPositions(BUTTON_LABEL_DOWN, positions)),
			NO_PIXEL,
			NO_PIXEL,
		},
		{
			Pixel(buttonPositions[BUTTON_LABEL_RIGHT], GAMEPAD_MASK_DR, *getLEDPositions(BUTTON_LABEL_RIGHT, positions)),
			NO_PIXEL,
			NO_PIXEL,
		},
		{
			NO_PIXEL,
			Pixel(buttonPositions[BUTTON_LABEL_UP], GAMEPAD_MASK_DU, *getLEDPositions(BUTTON_LABEL_UP, positions)),
		},
		{
			Pixel(buttonPositions[BUTTON_LABEL_B3], GAMEPAD_MASK_B3, *getLEDPositions(BUTTON_LABEL_B3, positions)),
			Pixel(buttonPositions[BUTTON_LABEL_B1], GAMEPAD_MASK_B1, *getLEDPositions(BUTTON_LABEL_B1, positions)),
			NO_PIXEL,
		},
		{
			Pixel(buttonPositions[BUTTON_LABEL_B4], GAMEPAD_MASK_B4, *getLEDPositions(BUTTON_LABEL_B4, positions)),
			Pixel(buttonPositions[BUTTON_LABEL_B2], GAMEPAD_MASK_B2, *getLEDPositions(BUTTON_LABEL_B2, positions)),
			NO_PIXEL,
		},
		{
			Pixel(buttonPositions[BUTTON_LABEL_R1], GAMEPAD_MASK_R1, *getLEDPositions(BUTTON_LABEL_R1, positions)),
			Pixel(buttonPositions[BUTTON_LABEL_R2], GAMEPAD_MASK_R2, *getLEDPositions(BUTTON_LABEL_R2, positions)),
			NO_PIXEL,
		},
		{
			Pixel(buttonPositions[BUTTON_LABEL_L1], GAMEPAD_MASK_L1, *getLEDPositions(BUTTON_LABEL_L1, positions)),
			Pixel(buttonPositions[BUTTON_LABEL_L2], GAMEPAD_MASK_L2, *getLEDPositions(BUTTON_LABEL_L2, positions)),
			NO_PIXEL,
		},
	};

	PixelMatrix matrix;
	matrix.setup(pixels);
	return matrix;
}

/**
 * @brief Create an LED layout using a 2x7 matrix.
 */
PixelMatrix createLedLayoutArcadeWasd(vector<vector<uint8_t>> *positions)
{
	vector<vector<Pixel>> pixels =
	{
		{
			NO_PIXEL,
			Pixel(buttonPositions[BUTTON_LABEL_LEFT], GAMEPAD_MASK_DL, *getLEDPositions(BUTTON_LABEL_LEFT, positions)),
		},
		{
			Pixel(buttonPositions[BUTTON_LABEL_UP], GAMEPAD_MASK_DU, *getLEDPositions(BUTTON_LABEL_UP, positions)),
			Pixel(buttonPositions[BUTTON_LABEL_DOWN], GAMEPAD_MASK_DD, *getLEDPositions(BUTTON_LABEL_DOWN, positions)),
		},
		{
			NO_PIXEL,
			Pixel(buttonPositions[BUTTON_LABEL_RIGHT], GAMEPAD_MASK_DR, *getLEDPositions(BUTTON_LABEL_RIGHT, positions)),
		},
		{
			Pixel(buttonPositions[BUTTON_LABEL_B3], GAMEPAD_MASK_B3, *getLEDPositions(BUTTON_LABEL_B3, positions)),
			Pixel(buttonPositions[BUTTON_LABEL_B1], GAMEPAD_MASK_B1, *getLEDPositions(BUTTON_LABEL_B1, positions)),
		},
		{
			Pixel(buttonPositions[BUTTON_LABEL_B4], GAMEPAD_MASK_B4, *getLEDPositions(BUTTON_LABEL_B4, positions)),
			Pixel(buttonPositions[BUTTON_LABEL_B2], GAMEPAD_MASK_B2, *getLEDPositions(BUTTON_LABEL_B2, positions)),
		},
		{
			Pixel(buttonPositions[BUTTON_LABEL_R1], GAMEPAD_MASK_R1, *getLEDPositions(BUTTON_LABEL_R1, positions)),
			Pixel(buttonPositions[BUTTON_LABEL_R2], GAMEPAD_MASK_R2, *getLEDPositions(BUTTON_LABEL_R2, positions)),
		},
		{
			Pixel(buttonPositions[BUTTON_LABEL_L1], GAMEPAD_MASK_L1, *getLEDPositions(BUTTON_LABEL_L1, positions)),
			Pixel(buttonPositions[BUTTON_LABEL_L2], GAMEPAD_MASK_L2, *getLEDPositions(BUTTON_LABEL_L2, positions)),
		},
	};

	PixelMatrix matrix;
	matrix.setup(pixels);
	return matrix;
}

PixelMatrix createLedButtonLayout(LEDLayout layout, vector<vector<uint8_t>> *positions)
{
	switch (layout)
	{
		case LED_LAYOUT_ARCADE_BUTTONS:
			return createLedLayoutArcadeButtons(positions);

		case LED_LAYOUT_ARCADE_HITBOX:
			return createLedLayoutArcadeHitbox(positions);

		case LED_LAYOUT_ARCADE_WASD:
			return createLedLayoutArcadeWasd(positions);
	}
}

PixelMatrix createLedButtonLayout(LEDLayout layout, uint8_t ledsPerPixel, uint8_t ledButtonCount)
{
	vector<vector<uint8_t>> positions(ledButtonCount);
	for (int i = 0; i != ledButtonCount; i++)
	{
		positions[i].resize(ledsPerPixel);
		for (int l = 0; l != ledsPerPixel; l++)
			positions[i][l] = (i * ledsPerPixel) + l;
	}

	return createLedButtonLayout(layout, &positions);
}

uint8_t setupButtonPositions()
{
	buttonPositions.emplace(BUTTON_LABEL_UP, ledOptions.indexUp);
	buttonPositions.emplace(BUTTON_LABEL_DOWN, ledOptions.indexDown);
	buttonPositions.emplace(BUTTON_LABEL_LEFT, ledOptions.indexLeft);
	buttonPositions.emplace(BUTTON_LABEL_RIGHT, ledOptions.indexRight);
	buttonPositions.emplace(BUTTON_LABEL_B1, ledOptions.indexB1);
	buttonPositions.emplace(BUTTON_LABEL_B2, ledOptions.indexB2);
	buttonPositions.emplace(BUTTON_LABEL_B3, ledOptions.indexB3);
	buttonPositions.emplace(BUTTON_LABEL_B4, ledOptions.indexB4);
	buttonPositions.emplace(BUTTON_LABEL_L1, ledOptions.indexL1);
	buttonPositions.emplace(BUTTON_LABEL_R1, ledOptions.indexR1);
	buttonPositions.emplace(BUTTON_LABEL_L2, ledOptions.indexL2);
	buttonPositions.emplace(BUTTON_LABEL_R2, ledOptions.indexR2);
	buttonPositions.emplace(BUTTON_LABEL_S1, ledOptions.indexS1);
	buttonPositions.emplace(BUTTON_LABEL_S2, ledOptions.indexS2);
	buttonPositions.emplace(BUTTON_LABEL_L3, ledOptions.indexL3);
	buttonPositions.emplace(BUTTON_LABEL_R3, ledOptions.indexR3);
	buttonPositions.emplace(BUTTON_LABEL_A1, ledOptions.indexA1);
	buttonPositions.emplace(BUTTON_LABEL_A2, ledOptions.indexA2);

	uint8_t buttonCount = 0;
	for (auto const buttonPosition : buttonPositions)
	{
		if (buttonPosition.second != -1)
			buttonCount++;
	}

	return buttonCount;
}

void LEDModule::setup()
{
	ledOptions = getLEDOptions();
	if (!ledOptions.useUserDefinedLEDs)
	{
		ledOptions.dataPin = BOARD_LEDS_PIN;
		ledOptions.ledFormat = LED_FORMAT;
		ledOptions.ledLayout = LED_LAYOUT;
		ledOptions.ledsPerButton = LEDS_PER_PIXEL;
		ledOptions.brightnessMaximum = LED_BRIGHTNESS_MAXIMUM;
		ledOptions.brightnessSteps = LED_BRIGHTNESS_STEPS;
		ledOptions.indexUp = LEDS_DPAD_UP;
		ledOptions.indexDown = LEDS_DPAD_DOWN;
		ledOptions.indexLeft = LEDS_DPAD_LEFT;
		ledOptions.indexRight = LEDS_DPAD_RIGHT;
		ledOptions.indexB1 = LEDS_BUTTON_B1;
		ledOptions.indexB2 = LEDS_BUTTON_B2;
		ledOptions.indexB3 = LEDS_BUTTON_B3;
		ledOptions.indexB4 = LEDS_BUTTON_B4;
		ledOptions.indexL1 = LEDS_BUTTON_L1;
		ledOptions.indexR1 = LEDS_BUTTON_R1;
		ledOptions.indexL2 = LEDS_BUTTON_L2;
		ledOptions.indexR2 = LEDS_BUTTON_R2;
		ledOptions.indexS1 = LEDS_BUTTON_S1;
		ledOptions.indexS2 = LEDS_BUTTON_S2;
		ledOptions.indexL3 = LEDS_BUTTON_L3;
		ledOptions.indexR3 = LEDS_BUTTON_R3;
		ledOptions.indexA1 = LEDS_BUTTON_A1;
		ledOptions.indexA2 = LEDS_BUTTON_A2;
	}

	queue_init(&baseAnimationQueue, sizeof(AnimationHotkey), 1);
	queue_init(&buttonAnimationQueue, sizeof(uint32_t), 1);
	queue_init(&animationSaveQueue, sizeof(int), 1);

	uint8_t buttonCount = setupButtonPositions();
	matrix = createLedButtonLayout(ledOptions.ledLayout, ledOptions.ledsPerButton, buttonCount);
	ledCount = matrix.getLedCount();
	if (PLED_TYPE == PLED_TYPE_RGB && PLED_COUNT > 0)
		ledCount += PLED_COUNT;
	neopico = new NeoPico(ledOptions.dataPin, ledCount, ledOptions.ledFormat);

	as.SetMatrix(matrix);
	AnimationStore.setup(&as);

	StaticTheme::AddTheme(themeStaticRainbow);
	StaticTheme::AddTheme(themeGuiltyGearTypeA);
	StaticTheme::AddTheme(themeGuiltyGearTypeD);
	StaticTheme::AddTheme(themeGuiltyGearCustom);
	StaticTheme::AddTheme(themeNeoGeo);
	StaticTheme::AddTheme(themeNeoGeoCurved);
	StaticTheme::AddTheme(themeNeoGeoModern);
	StaticTheme::AddTheme(themeSixButtonFighter);
	StaticTheme::AddTheme(themeSixButtonFighterPlus);
	StaticTheme::AddTheme(themeSuperFamicom);
	StaticTheme::AddTheme(themeXbox);
}

void LEDModule::process(Gamepad *gamepad)
{
	// We use queue_try_add here because if core1 explodes everywhere, we don't
	// care. It's not as important as handling inputs.

	AnimationHotkey action = animationHotkeys(gamepad);
	if (action != HOTKEY_LEDS_NONE)
		queue_try_add(&baseAnimationQueue, &action);

	uint32_t buttonState = gamepad->state.dpad << 16 | gamepad->state.buttons;
	queue_try_add(&buttonAnimationQueue, &buttonState);
}

void LEDModule::loop()
{
	if (ledOptions.dataPin < 0 || !time_reached(this->nextRunTime))
		return;

	AnimationHotkey action;
	uint32_t buttonState;

	if (queue_try_remove(&baseAnimationQueue, &action))
	{
		as.HandleEvent(action);
		queue_try_add(&animationSaveQueue, 0);
	}

	if (queue_try_remove(&buttonAnimationQueue, &buttonState))
	{
		vector<Pixel> pressed;

		for (auto row : matrix.pixels)
		{
			for (auto pixel : row)
			{
				if (buttonState & pixel.mask)
					pressed.push_back(pixel);
			}
		}

		if (pressed.size() > 0)
			as.HandlePressed(pressed);
		else
			as.ClearPressed();
	}

	as.Animate();
	as.ApplyBrightness(frame);
	if (PLED_TYPE == PLED_TYPE_RGB)
		setRGBPLEDs(frame);
	neopico->SetFrame(frame);
	neopico->Show();

	this->nextRunTime = make_timeout_time_ms(LEDModule::intervalMS);
	trySave();
}

void LEDModule::trySave()
{
	static int saveValue = 0;

	if (queue_try_remove(&animationSaveQueue, &saveValue))
		AnimationStore.save();
}

AnimationHotkey animationHotkeys(Gamepad *gamepad)
{
	AnimationHotkey action = HOTKEY_LEDS_NONE;

	if (gamepad->pressedF1())
	{
		if (gamepad->pressedB3())
		{
			action = HOTKEY_LEDS_ANIMATION_UP;
			gamepad->state.buttons &= ~(GAMEPAD_MASK_B3 | gamepad->f1Mask);
		}
		else if (gamepad->pressedB1())
		{
			action = HOTKEY_LEDS_ANIMATION_DOWN;
			gamepad->state.buttons &= ~(GAMEPAD_MASK_B1 | gamepad->f1Mask);
		}
		else if (gamepad->pressedB4())
		{
			action = HOTKEY_LEDS_BRIGHTNESS_UP;
			gamepad->state.buttons &= ~(GAMEPAD_MASK_B4 | gamepad->f1Mask);
		}
		else if (gamepad->pressedB2())
		{
			action = HOTKEY_LEDS_BRIGHTNESS_DOWN;
			gamepad->state.buttons &= ~(GAMEPAD_MASK_B2 | gamepad->f1Mask);
		}
		else if (gamepad->pressedR1())
		{
			action = HOTKEY_LEDS_PARAMETER_UP;
			gamepad->state.buttons &= ~(GAMEPAD_MASK_R1 | gamepad->f1Mask);
		}
		else if (gamepad->pressedR2())
		{
			action = HOTKEY_LEDS_PARAMETER_DOWN;
			gamepad->state.buttons &= ~(GAMEPAD_MASK_R2 | gamepad->f1Mask);
		}
		else if (gamepad->pressedL1())
		{
			action = HOTKEY_LEDS_PRESS_PARAMETER_UP;
			gamepad->state.buttons &= ~(GAMEPAD_MASK_L1 | gamepad->f1Mask);
		}
		else if (gamepad->pressedL2())
		{
			action = HOTKEY_LEDS_PRESS_PARAMETER_DOWN;
			gamepad->state.buttons &= ~(GAMEPAD_MASK_L2 | gamepad->f1Mask);
		}
	}

	return action;
}
