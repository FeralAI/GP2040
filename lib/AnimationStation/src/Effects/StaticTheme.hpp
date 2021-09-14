#ifndef STATIC_THEME_H_
#define STATIC_THEME_H_

#include <vector>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../Animation.hpp"

class StaticTheme : public Animation {
public:
  StaticTheme(std::vector<Pixel> pixels, std::vector<RGB> theme, std::vector<uint32_t> masks, RGB defaultColor = ColorWhite);

  void Animate(RGB (&frame)[100]);
  std::vector<uint32_t> masks;
protected:
  std::vector<RGB> theme;
  RGB defaultColor;
};

#endif
