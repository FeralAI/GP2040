/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2021 Jason Skuby (mytechtoybox.com)
 */

#define GAMEPAD_DEBOUNCE_MILLIS 5

#include "BoardConfig.h"

#include <vector>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/bootrom.h"
#include "pico/util/queue.h"
#include "tusb.h"
#include "Common.h" //added for GP2040-X4 to use bitWrite functions

#include "rndis/rndis.h"
#include "usb_driver.h"
#include "gp2040.h"
#include "gamepad.h"
#include "leds.h"
#include "pleds.h"
#include "display.h"



uint32_t getMillis() { return to_ms_since_boot(get_absolute_time()); }

Gamepad gamepad(GAMEPAD_DEBOUNCE_MILLIS);
static InputMode inputMode;
queue_t gamepadQueue;

DisplayModule displayModule;
LEDModule ledModule;
PLEDModule pledModule(PLED_TYPE);
std::vector<GPModule*> modules =
{
	&displayModule,
	&ledModule,
	&pledModule,
};

void setup();
void loop();
void core1();
void webserver();

int main()
{
	setup();
	multicore_launch_core1(core1);

	if (inputMode == INPUT_MODE_CONFIG)
	{
		webserver();
	}
	else
	{
		while (1)
			loop();
	}

	return 0;
}

void setup()
{
	// Initialise UART - used for 4PXE serial ring sending values in circle
    uart_init(uart0, 921600);
    uart_init(uart1, 921600);

    // Set the GPIO pin mux to the UART - 0 is TX, 1 is RX
    gpio_set_function(PIN_UART0_RX, GPIO_FUNC_UART);
    gpio_set_function(PIN_UART0_TX, GPIO_FUNC_UART);
	gpio_set_function(PIN_UART1_RX, GPIO_FUNC_UART);
    gpio_set_function(PIN_UART1_TX, GPIO_FUNC_UART);
	    //uart_puts(uart0, "Hello world!"); this was from testing

	//Initilize LED Pin on Pico
	gpio_init(PIN_LED);
	gpio_set_dir(PIN_LED, GPIO_OUT);

	//Initilize inputs pins for player position and assign player number based on jumpers and auxillary buttons
	gpio_init(PIN_PLAYER_0);
	gpio_init(PIN_PLAYER_1);
	gpio_init(PIN_AUX_0);
	gpio_init(PIN_AUX_1);
	gpio_set_dir(PIN_PLAYER_0, 0); //second parameter false means input
	gpio_set_dir(PIN_PLAYER_1, 0);
	gpio_set_dir(PIN_AUX_0, 0);
	gpio_set_dir(PIN_AUX_1, 0);
	gpio_pull_up(PIN_PLAYER_0);
	gpio_pull_up(PIN_PLAYER_1);
	gpio_pull_up(PIN_AUX_0);
	gpio_pull_up(PIN_AUX_1);

	
	// Start storage before anything else
	GamepadStore.start();
	gamepad.setup();

	// Check for input mode override
	gamepad.read();
	inputMode = gamepad.options.inputMode;
	if (gamepad.pressedS2())
		inputMode = INPUT_MODE_CONFIG;
	else if (gamepad.pressedB3())
		inputMode = INPUT_MODE_HID;
	else if (gamepad.pressedB1())
		inputMode = INPUT_MODE_SWITCH;
	else if (gamepad.pressedB2())
		inputMode = INPUT_MODE_XINPUT;
	else if (gamepad.pressedF1() && gamepad.pressedUp())
		reset_usb_boot(0, 0);

	queue_init(&gamepadQueue, sizeof(Gamepad), 1);

	for (auto it = modules.begin(); it != modules.end();)
	{
		GPModule *module = (*it);
		module->setup();

		if (module->isEnabled())
			it++;
		else
			it = modules.erase(it);
	}

	bool configMode = inputMode == INPUT_MODE_CONFIG;
	if (inputMode != gamepad.options.inputMode && !configMode)
	{
		gamepad.options.inputMode = inputMode;
		gamepad.save();
	}

	initialize_driver(inputMode);
}

void loop()
{
	

	
	static void *report;
	static const uint16_t reportSize = gamepad.getReportSize();
	static const uint32_t intervalMS = 1;
	static uint32_t nextRuntime = 0;
	static uint8_t featureData[32] = { };
	static Gamepad snapshot;

	//Variables added for GP2040-X4 UART communication features
	static uint32_t nextSerial = 5000; //holds time of next serial read and write
	static const uint32_t serialInterval = 1; //how often does serial read and write in ms
	const size_t serialLen = 12; //Serial frame length (Should match number of bytes in serial buffer arrays) 

	// Player number to be set based on PIN_PLAYER_0 and PIN_PLAYER_1
	bool physicalPlayer0 = 0; //pin reading of first GPIO assign ping
	bool physicalPlayer1 = 0; //pin reading of second GPIO assign ping
	int physicalPlayer = 0; //pins 0 and 1 solved to get player 1, 2, 3, or 4
	int computerPlayer = 0; //This is going to be assigned based on xinput position assigned by computer (From player LED status)

	//Serial data holds for 4PXE
	static uint8_t serialOut[12] = {}; //Stores test values to send on UART0
	static uint8_t serialIn[12] = {}; //Buffer for received values on UART1

	//Determine Physical Player Position of Encoder
	//Determined by combination of permanent assignment pins
	//in addition to 2 or 4 player mode
	physicalPlayer0 = !gpio_get(PIN_PLAYER_0); //inverted because pull-up is used
	physicalPlayer1 = !gpio_get(PIN_PLAYER_1);
	if(!physicalPlayer0 && !physicalPlayer1) physicalPlayer = 1;
	if(!physicalPlayer0 && physicalPlayer1) physicalPlayer = 2;
	if(physicalPlayer0 && !physicalPlayer1) physicalPlayer = 3;
	if(physicalPlayer0 && physicalPlayer1) physicalPlayer = 4;



	if (getMillis() - nextRuntime < 0)
		return;

	gamepad.read();
#if GAMEPAD_DEBOUNCE_MILLIS > 0
	gamepad.debounce();
#endif
	gamepad.hotkey();
	gamepad.process();
	report = gamepad.getReport();
	send_report(report, reportSize);

	memset(featureData, 0, sizeof(featureData));
	receive_report(featureData);
	if (featureData[0])
		queue_try_add(&pledModule.featureQueue, featureData);

	tud_task();

	if (queue_is_empty(&gamepadQueue))
	{
		memcpy(&snapshot, &gamepad, sizeof(Gamepad));
		queue_try_add(&gamepadQueue, &snapshot);
	}

	nextRuntime = getMillis() + intervalMS;

	//SERIAL SEND AND RECEIVE FOR GP2040-X4///////////
	if (getMillis() > nextSerial){
	uart_read_blocking(uart1, serialIn, serialLen); //THIS IS BREAKING USB FUNCTIONS
	uart_write_blocking(uart0, serialOut, serialLen); //write on interval to avoid flooding buffers
	nextSerial = getMillis() + serialInterval;
	}

	//Steps to execute based on physical player location for GP2040-X4
	if (physicalPlayer == 1) {
	gamepad.pressedLeft() ? serialOut[0] = 1 : serialOut[0] = 0; // read left button input to serial out
	gpio_put(PIN_LED, serialOut[0]);
	}

	if (physicalPlayer == 2){
	serialOut[0] = serialIn[0];
	}

	if (physicalPlayer == 3){
	serialOut[0] = serialIn[0];
	}

	if (physicalPlayer == 4){
	gpio_put(PIN_LED, serialIn[0]);
	}

}

void core1()
{
	multicore_lockout_victim_init();

	while (1)
	{
		static Gamepad snapshot;

		if (queue_try_remove(&gamepadQueue, &snapshot))
		{
			for (auto module : modules)
				module->process(&snapshot);
		}

		for (auto module : modules)
			module->loop();
	}
}

void webserver()
{
	static Gamepad snapshot;

	rndis_init();
	while (1)
	{
		gamepad.read();
#if GAMEPAD_DEBOUNCE_MILLIS > 0
		gamepad.debounce();
#endif
		gamepad.hotkey();
		gamepad.process();

		if (queue_is_empty(&gamepadQueue))
		{
			memcpy(&snapshot, &gamepad, sizeof(Gamepad));
			queue_try_add(&gamepadQueue, &snapshot);
		}

		rndis_task();
	}
}
