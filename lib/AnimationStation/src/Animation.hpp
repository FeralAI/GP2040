#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "NeoPico.hpp"

#define ANIMATION_MODE_COUNT 4

typedef enum
{
  STATIC,
  RAINBOW,
  CHASE,
  STATIC_NEOGEO,
} AnimationMode;

struct RGB
{
  RGB()
    : r(0), g(0), b(0) { }

  RGB(uint8_t r, uint8_t g, uint8_t b)
    : r(r), g(g), b(b) { }

  uint8_t r;
  uint8_t g;
  uint8_t b;

  inline uint32_t value(float brightnessX = 1) {
    return ((uint32_t)(r * brightnessX) << 8)
         | ((uint32_t)(g * brightnessX) << 16)
         | (uint32_t)(b * brightnessX);
  }
};

class Animation {
public:
  Animation(std::vector<Pixel> pixels);

  virtual void Animate(RGB (&frame)[100]) = 0;
  AnimationMode mode;

  bool isComplete();
protected:
  std::vector<Pixel> pixels;
  int currentLoop = 0;
  int totalLoops;
  bool baseAnimation = true;
};

#endif
