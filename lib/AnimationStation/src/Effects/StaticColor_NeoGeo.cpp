#include "StaticColor_NeoGeo.hpp"

static RGB theme[] = {
  ColorRed,
  ColorYellow,
  ColorGreen,
  ColorBlue,
};

uint32_t StaticColor_NeoGeo::masks[maskCount] = { };

StaticColor_NeoGeo::StaticColor_NeoGeo(std::vector<Pixel> pixels) : Animation(pixels) {
  this->mode = STATIC_NEOGEO;
}

void StaticColor_NeoGeo::setThemeMasks(const uint32_t masks[4]) {
  memcpy(StaticColor_NeoGeo::masks, masks, sizeof(StaticColor_NeoGeo::masks));
}

void StaticColor_NeoGeo::Animate(RGB (&frame)[100]) {
  for (size_t i = 0; i != pixels.size(); i++) {
    bool isTheme = false;

    for (size_t m = 0; m != StaticColor_NeoGeo::maskCount; m++) {
      if (pixels[i].mask & StaticColor_NeoGeo::masks[m]) {
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
