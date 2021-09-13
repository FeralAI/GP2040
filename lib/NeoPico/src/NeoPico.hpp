#ifndef _NEO_PICO_H_
#define _NEO_PICO_H_

#include "ws2812.pio.h"
#include <vector>

struct Pixel
{
  uint8_t index;                  // The pixel index
  uint32_t mask;                  // Used to detect per-pixel lighting
  std::vector<uint8_t> positions; // The actual LED indexes on the chain
};

class NeoPico
{
public:
  NeoPico(int ledPin, std::vector<Pixel> pixels);
  void Show();
  void Clear();
  // void SetPixel(int pixel, uint32_t color);
  void SetFrame(uint32_t newFrame[100]);
private:
  void PutPixel(uint32_t pixel_grb);
  std::vector<Pixel> pixels;
  PIO pio = pio0;
  int numPixels = 0;
  uint32_t frame[100];
};

#endif
