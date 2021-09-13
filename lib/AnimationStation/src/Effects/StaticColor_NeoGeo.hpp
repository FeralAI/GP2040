#ifndef STATIC_COLOR_NEO_GEO_H_
#define STATIC_COLOR_NEO_GEO_H_

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "../Animation.hpp"
#include "../AnimationStation.hpp"

class StaticColor_NeoGeo : public Animation {
public:
  StaticColor_NeoGeo(std::vector<Pixel> pixels);

  static void setThemeMasks(const uint32_t masks[4]);

  void Animate(RGB (&frame)[100]);
private:
  static const uint16_t maskCount = 4;
  static uint32_t masks[maskCount];
};

#endif
