#include "StaticColor.hpp"

RGB StaticColor::defaultColor = ColorRed;

StaticColor::StaticColor(std::vector<Pixel> pixels) : Animation(pixels) {
  this->color = StaticColor::defaultColor;
}

void StaticColor::Animate(RGB (&frame)[100]) {
  for (size_t i = 0; i != pixels.size(); i++) {
    for (size_t j = 0; j != pixels[i].positions.size(); j++) {
      frame[pixels[i].positions[j]] = this->color;
    }
  }
}
