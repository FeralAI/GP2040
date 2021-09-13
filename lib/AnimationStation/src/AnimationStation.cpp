/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * Modified by Jonathan Barket - 2021
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <algorithm>
#include "AnimationStation.hpp"

uint8_t AnimationStation::brightness = 0;
float AnimationStation::brightnessX = 0;
absolute_time_t AnimationStation::nextAnimationChange = 0;
absolute_time_t AnimationStation::nextBrightnessChange = 0;

AnimationStation::AnimationStation(std::vector<Pixel> pixels) : pixels(pixels) {
  AnimationStation::SetBrightness(1);
}

void AnimationStation::HandleEvent(AnimationHotkey action) {
  if (action == HOTKEY_LEDS_BRIGHTNESS_UP) {
    AnimationStation::IncreaseBrightness();
  }

  if (action == HOTKEY_LEDS_BRIGHTNESS_DOWN) {
    AnimationStation::DecreaseBrightness();
  }

  if (action == HOTKEY_LEDS_ANIMATION_UP) {
    this->ChangeAnimation();
  }

  if (action == HOTKEY_LEDS_ANIMATION_DOWN) {
    this->ChangeAnimation();
  }
}

void AnimationStation::ChangeAnimation() {
  if (!time_reached(AnimationStation::nextAnimationChange)) {
    return;
  }

  if (this->animations.size() > 0) {
    SetAnimation((AnimationMode)((this->animations.at(0)->mode + 1) % ANIMATION_MODE_COUNT));
    this->animations.erase(this->animations.begin());
  }

  AnimationStation::nextAnimationChange = make_timeout_time_ms(250);
}

void AnimationStation::SetAnimation(AnimationMode mode) {
  switch (mode)
  {
    case RAINBOW:
      this->animations.push_back(new Rainbow(pixels));
      break;

    case CHASE:
      this->animations.push_back(new Chase(pixels));
      break;

    case STATIC_NEOGEO:
      this->animations.push_back(new StaticColor_NeoGeo(pixels));
      break;

    case STATIC:
    default:
      this->animations.push_back(new StaticColor(pixels));
      break;
  }
}

void AnimationStation::Animate() {
  if (this->animations.size() == 0) {
    this->Clear();
    return;
  }

  for (auto &element : this->animations) {
    /* non-base animations (eg. button presses) only run
      a certain number of times. before we animate, we need
      to verify that it isn't already complete */

    if (element->isComplete()) {
      this->animations.erase(std::remove(this->animations.begin(), this->animations.end(), element), this->animations.end());
    }
    else {
      element->Animate(frame);
    }
  }
}

void AnimationStation::Clear() {
  memset(frame, 0, sizeof(frame));
}

float AnimationStation::GetBrightnessX() {
  return AnimationStation::brightnessX;
}

uint8_t AnimationStation::GetBrightness() {
  return AnimationStation::brightness;
}

void AnimationStation::GetAdjustedFrame(uint32_t *frameValue) {
  for (int i = 0; i < 100; i++)
    frameValue[i] = this->frame[i].value(brightnessX);
}

void AnimationStation::SetBrightness(uint8_t brightness) {
  AnimationStation::brightness = (brightness > BRIGHTNESS_STEPS) ? BRIGHTNESS_STEPS : brightness;
  AnimationStation::brightnessX = (AnimationStation::brightness * BRIGHTNESS_STEP_SIZE) / 255.0F;

  if (AnimationStation::brightnessX > 1)
    AnimationStation::brightnessX = 1;
  else if (AnimationStation::brightnessX < 0)
    AnimationStation::brightnessX = 0;
}

void AnimationStation::DecreaseBrightness() {
  if (!time_reached(AnimationStation::nextBrightnessChange))
    return;

  if (AnimationStation::brightness > 0)
    AnimationStation::SetBrightness(--AnimationStation::brightness);

  AnimationStation::nextBrightnessChange = make_timeout_time_ms(250);
}

void AnimationStation::IncreaseBrightness() {
  if (!time_reached(AnimationStation::nextBrightnessChange))
    return;

  if (AnimationStation::brightness < BRIGHTNESS_STEP_SIZE)
    AnimationStation::SetBrightness(++AnimationStation::brightness);
  else if (AnimationStation::brightness > BRIGHTNESS_STEP_SIZE)
    AnimationStation::SetBrightness(BRIGHTNESS_STEPS);

  AnimationStation::nextBrightnessChange = make_timeout_time_ms(250);
}
