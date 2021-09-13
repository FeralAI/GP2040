#ifndef _STATIC_COLOR_H_
#define _STATIC_COLOR_H_

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "../Animation.hpp"
#include "../AnimationStation.hpp"

class StaticColor : public Animation {
public:
  StaticColor(std::vector<Pixel> pixels);

  void Animate(uint32_t (&frame)[100]);

  static void SetDefaultColor(uint32_t color) {
    StaticColor::defaultColor = color;
  }
protected:
  static uint32_t defaultColor;
  uint32_t color;
};

#endif
