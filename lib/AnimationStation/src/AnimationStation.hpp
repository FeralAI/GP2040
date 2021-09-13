#ifndef _ANIMATION_STATION_H_
#define _ANIMATION_STATION_H_

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "hardware/clocks.h"

#include "NeoPico.hpp"
#include "Animation.hpp"

#define BRIGHTNESS_MAXIMUM  100
#define BRIGHTNESS_STEPS 5
#define BRIGHTNESS_STEP_SIZE (BRIGHTNESS_MAXIMUM / BRIGHTNESS_STEPS)

typedef enum
{
  HOTKEY_LEDS_NONE,
	HOTKEY_LEDS_ANIMATION_UP,
	HOTKEY_LEDS_ANIMATION_DOWN,
	HOTKEY_LEDS_PARAMETER_UP,
	HOTKEY_LEDS_PARAMETER_DOWN,
	HOTKEY_LEDS_BRIGHTNESS_UP,
	HOTKEY_LEDS_BRIGHTNESS_DOWN
} AnimationHotkey;

static const RGB ColorBlack     = RGB(  0,   0,   0);
static const RGB ColorWhite     = RGB(255, 255, 255);
static const RGB ColorRed       = RGB(255,   0,   0);
static const RGB ColorOrange    = RGB(255, 128,   0);
static const RGB ColorYellow    = RGB(255, 255,   0);
static const RGB ColorLimeGreen = RGB(128, 255,   0);
static const RGB ColorGreen     = RGB(  0, 255,   0);
static const RGB ColorSeafoam   = RGB(  0, 255, 128);
static const RGB ColorAqua      = RGB(  0, 255, 255);
static const RGB ColorSkyBlue   = RGB(  0, 128, 255);
static const RGB ColorBlue      = RGB(  0,   0, 255);
static const RGB ColorPurple    = RGB(128,   0, 255);
static const RGB ColorPink      = RGB(255,   0, 255);
static const RGB ColorMagenta   = RGB(255,   0, 128);

class AnimationStation
{
public:
  AnimationStation(std::vector<Pixel> pixels);

  void Animate();
  void HandleEvent(AnimationHotkey action);
  void SetAnimation(AnimationMode mode);
  void SetStaticColor();
  void SetRainbow();
  void SetChase();
  void SetStaticColorNeoGeo();
  void Clear();
  void ChangeAnimation();
  void GetAdjustedFrame(uint32_t *frameValue);

  static float GetBrightnessX();
  static uint8_t GetBrightness();
  static void SetBrightness(uint8_t brightness);
  static void DecreaseBrightness();
  static void IncreaseBrightness();
  static RGB Wheel(uint8_t pos);

  std::vector<Animation*> animations;
  static absolute_time_t nextAnimationChange;
  static absolute_time_t nextBrightnessChange;
  RGB frame[100];

protected:
  static uint8_t brightness;
  static float brightnessX;
  std::vector<Pixel> pixels;
};

#endif
