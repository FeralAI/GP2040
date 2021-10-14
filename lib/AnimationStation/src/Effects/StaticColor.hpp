#ifndef _STATIC_COLOR_H_
#define _STATIC_COLOR_H_

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "../Animation.hpp"

class StaticColor : public Animation {
public:
  StaticColor(PixelMatrix &matrix, int colorIndex = defaultColorIndex);
  StaticColor(PixelMatrix &matrix, std::vector<Pixel> pixels, int colorIndex = defaultColorIndex);
  ~StaticColor() {};

  void Animate(RGB (&frame)[100]);
  void ParameterUp();
  void ParameterDown();

  static void SetDefaultColorIndex(int colorIndex) {
    defaultColorIndex = colorIndex;
  }

protected:
  static int defaultColorIndex;
  int colorIndex;
};

#endif
