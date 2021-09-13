#include "StaticColor.hpp"

uint32_t StaticColor::defaultColor = 255;

StaticColor::StaticColor(std::vector<Pixel> pixels) : Animation(pixels) {
  this->color = StaticColor::defaultColor;
  this->mode = STATIC;
}

void StaticColor::Animate(uint32_t (&frame)[100]) {
  uint32_t color = this->color * AnimationStation::GetBrightnessX();
  for (size_t i = 0; i != pixels.size(); i++) {
    for (size_t j = 0; j != pixels[i].positions.size(); j++) {
      frame[pixels[i].positions[j]] = color;
    }
  }
}
