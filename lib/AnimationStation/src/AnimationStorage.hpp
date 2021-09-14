#ifndef _CONFIG_STORAGE_H_
#define _CONFIG_STORAGE_H_

#include "Animation.hpp"
#include "AnimationStation.hpp"

class AnimationStorage
{
	public:
		void setup(AnimationStation *as);
		void save(AnimationStation as);

		uint8_t getBaseAnimation();
		void setBaseAnimation(uint8_t mode);
		uint8_t getBrightness();
		void setBrightness(uint8_t brightness);
};

static AnimationStorage AnimationStore;

#endif
