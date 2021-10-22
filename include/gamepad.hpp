#ifndef _GAMEPAD_H_
#define _GAMEPAD_H_

#include "BoardConfig.h"
#include "pico/stdlib.h"
#include <MPGS.h>
#include "storage.h"

struct GamepadButtonMapping {
	GamepadButtonMapping(uint8_t p, uint16_t bm) : pin(p), pinMask((1 << p)), buttonMask(bm) { }

	const uint8_t pin;
	const uint32_t pinMask;
	const uint16_t buttonMask;
};

class Gamepad : public MPGS {
public:
	Gamepad(int debounceMS = 5, GamepadStorage *storage = &GamepadStore)
		: MPGS(debounceMS, storage) { }

	void setup();
	void read();

	inline bool __attribute__((always_inline)) pressedF1() {
#ifdef PIN_SETTINGS
		return state.aux & (1 << 0);
#else
		return MPGS::pressedF1();
#endif
	}

	GamepadButtonMapping *mapDpadUp;
	GamepadButtonMapping *mapDpadDown;
	GamepadButtonMapping *mapDpadLeft;
	GamepadButtonMapping *mapDpadRight;
	GamepadButtonMapping *mapButtonB1;
	GamepadButtonMapping *mapButtonB2;
	GamepadButtonMapping *mapButtonB3;
	GamepadButtonMapping *mapButtonB4;
	GamepadButtonMapping *mapButtonL1;
	GamepadButtonMapping *mapButtonR1;
	GamepadButtonMapping *mapButtonL2;
	GamepadButtonMapping *mapButtonR2;
	GamepadButtonMapping *mapButtonS1;
	GamepadButtonMapping *mapButtonS2;
	GamepadButtonMapping *mapButtonL3;
	GamepadButtonMapping *mapButtonR3;
	GamepadButtonMapping *mapButtonA1;
	GamepadButtonMapping *mapButtonA2;
};

#endif
