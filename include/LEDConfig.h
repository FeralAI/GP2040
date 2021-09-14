#include <vector>
#include <MPG.h>
#include "AnimationStation.hpp"

#ifndef LEDS_DPAD_LEFT
#define LEDS_DPAD_LEFT -1
#endif

#ifndef LEDS_DPAD_DOWN
#define LEDS_DPAD_DOWN  -1
#endif

#ifndef LEDS_DPAD_RIGHT
#define LEDS_DPAD_RIGHT -1
#endif

#ifndef LEDS_DPAD_UP
#define LEDS_DPAD_UP    -1
#endif

#ifndef LEDS_BUTTON_01
#define LEDS_BUTTON_01  -1
#endif

#ifndef LEDS_BUTTON_02
#define LEDS_BUTTON_02  -1
#endif

#ifndef LEDS_BUTTON_03
#define LEDS_BUTTON_03  -1
#endif

#ifndef LEDS_BUTTON_04
#define LEDS_BUTTON_04  -1
#endif

#ifndef LEDS_BUTTON_05
#define LEDS_BUTTON_05  -1
#endif

#ifndef LEDS_BUTTON_06
#define LEDS_BUTTON_06  -1
#endif

#ifndef LEDS_BUTTON_07
#define LEDS_BUTTON_07  -1
#endif

#ifndef LEDS_BUTTON_08
#define LEDS_BUTTON_08  -1
#endif

#ifndef LEDS_BUTTON_09
#define LEDS_BUTTON_09  -1
#endif

#ifndef LEDS_BUTTON_10
#define LEDS_BUTTON_10  -1
#endif

#ifndef LEDS_BUTTON_11
#define LEDS_BUTTON_11  -1
#endif

#ifndef LEDS_BUTTON_12
#define LEDS_BUTTON_12  -1
#endif

#ifndef LEDS_BUTTON_13
#define LEDS_BUTTON_13  -1
#endif

#ifndef LEDS_BUTTON_14
#define LEDS_BUTTON_14  -1
#endif

#ifndef LEDS_BRIGHTNESS
#define LEDS_BRIGHTNESS 75
#endif

#ifndef LEDS_BASE_ANIMATION
#define LEDS_BASE_ANIMATION RAINBOW
#endif

#ifndef LEDS_BASE_ANIMATION_FIRST_PIXEL
#define LEDS_BASE_ANIMATION_FIRST_PIXEL 0
#endif

#ifndef LEDS_BASE_ANIMATION_LAST_PIXEL
#define LEDS_BASE_ANIMATION_LAST_PIXEL 11
#endif

#ifndef LEDS_RAINBOW_CYCLE_TIME
#define LEDS_RAINBOW_CYCLE_TIME 40
#endif

#ifndef LEDS_CHASE_CYCLE_TIME
#define LEDS_CHASE_CYCLE_TIME 85
#endif

#ifndef LEDS_STATIC_COLOR_COLOR
#define LEDS_STATIC_COLOR_COLOR ColorRed
#endif

extern const std::vector<Pixel> pixels;

static StaticTheme themeNeoGeo(
	pixels,
	{
		{ GAMEPAD_MASK_UP,    ColorWhite },
		{ GAMEPAD_MASK_DOWN,  ColorWhite },
		{ GAMEPAD_MASK_LEFT,  ColorWhite },
		{ GAMEPAD_MASK_RIGHT, ColorWhite },
		{ GAMEPAD_MASK_B3,    ColorRed },
		{ GAMEPAD_MASK_B4,    ColorYellow },
		{ GAMEPAD_MASK_R1,    ColorGreen },
		{ GAMEPAD_MASK_L1,    ColorBlue },
	}
);

static StaticTheme themeNeoGeoCurved(
	pixels,
	{
		{ GAMEPAD_MASK_UP,    ColorWhite },
		{ GAMEPAD_MASK_DOWN,  ColorWhite },
		{ GAMEPAD_MASK_LEFT,  ColorWhite },
		{ GAMEPAD_MASK_RIGHT, ColorWhite },
		{ GAMEPAD_MASK_B1,    ColorRed },
		{ GAMEPAD_MASK_B3,    ColorYellow },
		{ GAMEPAD_MASK_B4,    ColorGreen },
		{ GAMEPAD_MASK_R1,    ColorBlue },
	}
);

static void configureAnimations(AnimationStation *as) {
	as->SetStaticColor(LEDS_STATIC_COLOR_COLOR);
	as->AddAnimation(&themeNeoGeo);
	as->AddAnimation(&themeNeoGeoCurved);
}

static AnimationHotkey animationHotkeys(MPG gamepad)
{
	AnimationHotkey action = HOTKEY_LEDS_NONE;

	if (gamepad.pressedF1())
	{
		if (gamepad.pressedB3())
		{
			action = HOTKEY_LEDS_ANIMATION_UP;
			gamepad.state.buttons &= ~(GAMEPAD_MASK_B3 | GAMEPAD_MASK_S1 | GAMEPAD_MASK_S2);
		}
		else if (gamepad.pressedB1())
		{
			action = HOTKEY_LEDS_ANIMATION_DOWN;
			gamepad.state.buttons &= ~(GAMEPAD_MASK_B1 | GAMEPAD_MASK_S1 | GAMEPAD_MASK_S2);
		}
		else if (gamepad.pressedB4())
		{
			action = HOTKEY_LEDS_BRIGHTNESS_UP;
			gamepad.state.buttons &= ~(GAMEPAD_MASK_B4 | GAMEPAD_MASK_S1 | GAMEPAD_MASK_S2);
		}
		else if (gamepad.pressedB2())
		{
			action = HOTKEY_LEDS_BRIGHTNESS_DOWN;
			gamepad.state.buttons &= ~(GAMEPAD_MASK_B2 | GAMEPAD_MASK_S1 | GAMEPAD_MASK_S2);
		}
		else if (gamepad.pressedR1())
		{
			action = HOTKEY_LEDS_PARAMETER_UP;
			gamepad.state.buttons &= ~(GAMEPAD_MASK_R1 | GAMEPAD_MASK_S1 | GAMEPAD_MASK_S2);
		}
		else if (gamepad.pressedR2())
		{
			action = HOTKEY_LEDS_PARAMETER_DOWN;
			gamepad.state.buttons &= ~(GAMEPAD_MASK_R2 | GAMEPAD_MASK_S1 | GAMEPAD_MASK_S2);
		}
	}

	return action;
}
