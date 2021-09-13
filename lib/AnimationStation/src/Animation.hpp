#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "NeoPico.hpp"

typedef enum
{
	STATIC,
	RAINBOW,
	CHASE,
} AnimationMode;

class Animation {
public:
  Animation(std::vector<Pixel> pixels);

  virtual void Animate(uint32_t (&frame)[100]) = 0;
  AnimationMode mode;

  bool isComplete();
protected:
  std::vector<Pixel> pixels;
  int currentLoop = 0;
  int totalLoops;
  bool baseAnimation = true;
};

#endif
