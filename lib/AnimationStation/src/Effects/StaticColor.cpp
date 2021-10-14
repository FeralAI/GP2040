#include "StaticColor.hpp"

int StaticColor::defaultColorIndex = 2;

StaticColor::StaticColor(PixelMatrix &matrix, int colorIndex) : Animation(matrix), colorIndex(colorIndex) {
}

StaticColor::StaticColor(PixelMatrix &matrix, std::vector<Pixel> pixels, int colorIndex) : Animation(matrix), colorIndex(colorIndex) {
  this->pixels = pixels;
  this->filtered = true;
}

void StaticColor::Animate(RGB (&frame)[100]) {
  for (size_t r = 0; r != matrix->pixels.size(); r++) {
    for (size_t c = 0; c != matrix->pixels[r].size(); c++) {
      if (matrix->pixels[r][c].index == NO_PIXEL.index || this->notInFilter(matrix->pixels[r][c]))
        continue;

      for (size_t p = 0; p != matrix->pixels[r][c].positions.size(); p++) {
        frame[matrix->pixels[r][c].positions[p]] = colors[colorIndex];
      }
    }
  }
}

void StaticColor::ParameterUp() {
  if (this->colorIndex < colors.size() - 1)
  {
    this->colorIndex++;
  }
  else {
    this->colorIndex = 0;
  }
}

void StaticColor::ParameterDown() {
  if (this->colorIndex > 0) {
    this->colorIndex--;
  }
  else {
    this->colorIndex = colors.size() - 1;
  }
}
