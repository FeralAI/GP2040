#include "Rainbow.hpp"

int Rainbow::defaultCycleTime = 40;

Rainbow::Rainbow(std::vector<Pixel> pixels) : Animation(pixels) {
  this->cycleTime = Rainbow::defaultCycleTime;
  this->mode = RAINBOW;
}

void Rainbow::Animate(uint32_t (&frame)[100]) {
  if (!time_reached(this->nextRunTime)) {
    return;
  }

  for (size_t i = 0; i != pixels.size(); i++) {
    uint32_t color = AnimationStation::Wheel(this->currentFrame);
    for (size_t j = 0; j != pixels[i].positions.size(); j++) {
      frame[pixels[i].positions[j]] = color;
    }
  }

  if (reverse) {
    currentFrame--;

    if (currentFrame < 0) {
      currentFrame = 1;
      reverse = false;
      this->currentLoop++;
    }
  }
  else {
    currentFrame++;

    if (currentFrame > 255) {
      currentFrame = 254;
      reverse = true;
    }
  }

  this->nextRunTime = make_timeout_time_ms(this->cycleTime);
}
