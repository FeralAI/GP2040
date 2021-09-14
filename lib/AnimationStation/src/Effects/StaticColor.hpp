#ifndef _STATIC_COLOR_H_
#define _STATIC_COLOR_H_

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "../Animation.hpp"

class StaticColor : public Animation {
public:
  StaticColor(std::vector<Pixel> pixels);

  void Animate(RGB (&frame)[100]);

  static void SetDefaultColor(RGB color) {
    StaticColor::defaultColor = color;
  }
protected:
  static RGB defaultColor;
  RGB color;
};

#endif
