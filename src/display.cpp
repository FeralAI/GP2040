#include "display.h"
#include "pico/stdlib.h"
#include "OneBitDisplay.h"

uint8_t ucBackBuffer[1024];
OBDISP obd;

void DisplayModule::setup()
{
	enabled = I2C_SDA_PIN != -1 && I2C_SCL_PIN != -1;
	if (enabled)
	{
		obdI2CInit(&obd,
			DISPLAY_SIZE,
			DISPLAY_I2C_ADDR,
			DISPLAY_FLIP,
			DISPLAY_INVERT,
			DISPLAY_USEWIRE,
			I2C_SDA_PIN,
			I2C_SCL_PIN,
			I2C_BLOCK,
			-1,
			I2C_SPEED);

		sleep_ms(100);
		obdSetContrast(&obd, 0xFF);
		obdFill(&obd, 0, 1); // Clear the screen
		obdSetBackBuffer(&obd, ucBackBuffer);
	}
}

void DisplayModule::loop()
{

}

void DisplayModule::process(Gamepad *gamepad)
{

}
