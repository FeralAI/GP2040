#include "StaticTheme.hpp"

StaticTheme::StaticTheme(std::vector<Pixel> pixels, std::vector<RGB> theme, std::vector<uint32_t> masks, RGB defaultColor)
  : Animation(pixels), masks(masks), theme(theme), defaultColor(defaultColor) { }

void StaticTheme::Animate(RGB (&frame)[100]) {
  for (size_t i = 0; i != pixels.size(); i++) {
    bool isTheme = false;

    for (size_t m = 0; m != masks.size(); m++) {
      if (pixels[i].mask & StaticTheme::masks[m]) {
        isTheme = true;
        for (size_t j = 0; j != pixels[i].positions.size(); j++) {
          frame[pixels[i].positions[j]] = theme[m];
        }
      }

      if (isTheme)
        break;
    }

    if (!isTheme) {
      for (size_t j = 0; j != pixels[i].positions.size(); j++) {
        frame[pixels[i].positions[j]] = ColorWhite;
      }
    }
  }
}
