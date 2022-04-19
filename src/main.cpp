/*
 * SPDX-License-Identifier: MIT
 * SPDX-FileCopyrightText: Copyright (c) 2021 Jason Skuby (mytechtoybox.com)
 */

#define GAMEPAD_DEBOUNCE_MILLIS 5

#define UART_ID uart1 //This is the ID for UART1 interupt for receiving serial

#include "BoardConfig.h"

#include <vector>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/bootrom.h"
#include "pico/util/queue.h"
#include "tusb.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//added common.h for GP2040-X4////////////////////////////////////////////////////////////////////////////////////////////
#include "Common.h" 
#include "hardware/uart.h"
#include "hardware/irq.h"

#include "rndis/rndis.h"
#include "usb_driver.h"
#include "gp2040.h"
#include "gamepad.h"
#include "leds.h"
#include "pleds.h"
#include "display.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//values to transmitted and received for GP2040-4X 4 player functionality/////////////////////////////////////////////////
const size_t serialLen = 2; //Serial storage array size and serial frame length (RP2040 has a 16 byte FIFO UART buffer, so 16 is max)
static uint8_t serialOut[serialLen] = {0}; //Data Prepared to send on UART0
static uint8_t serialIn[serialLen] = {0}; //Buffer for received values on UART1

uint32_t getMillis() { return to_ms_since_boot(get_absolute_time()); }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//microsecond time stamp used for GP2040-4X Serial Timing/////////////////////////////////////////////////////////////////
uint32_t getMicros() { return to_us_since_boot(get_absolute_time()); } 

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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//UART data received interupt - added for GP2040-X4 serial communication//////////////////////////////////////////////////
void on_uart_rx() {
    uart_read_blocking(uart1, serialIn, serialLen);
}

void setup()
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Initialise UART - used for 4PXE serial ring sending values in circle///////////////////////////////////////////////
	// Set the GPIO pin mux to the UART - 0 is TX, 1 is RX
    uart_init(uart0, 9600); //MAX 921600 tested working with 1 byte
    uart_init(uart1, 9600);
    gpio_set_function(PIN_UART0_RX, GPIO_FUNC_UART);
    gpio_set_function(PIN_UART0_TX, GPIO_FUNC_UART);
	gpio_set_function(PIN_UART1_RX, GPIO_FUNC_UART);
    gpio_set_function(PIN_UART1_TX, GPIO_FUNC_UART);
	uart_set_hw_flow(uart0, false, false);
	uart_set_hw_flow(uart1, false, false);
	uart_set_format(uart0, 8, 1, UART_PARITY_NONE);
	uart_set_format(uart1, 8, 1, UART_PARITY_NONE);
	uart_set_fifo_enabled(uart0, true);
	uart_set_fifo_enabled(uart1, true);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//SERIAL RECEIVE INTERUPT FOR GP2040-X4///////////////////////////////////////////////////////////////////////////////
	int UART_IRQ = UART_ID == uart0 ? UART0_IRQ : UART1_IRQ; //This is written this way because I borrowed from example//
    irq_set_exclusive_handler(UART_IRQ, on_uart_rx);
    irq_set_enabled(UART_IRQ, true);
    uart_set_irq_enables(uart1, true, false);    // Now enable the UART to send interrupts - RX only

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Initilize LED Pin on Pico///////////////////////////////////////////////////////////////////////////////////////////
	gpio_init(PIN_LED);
	gpio_set_dir(PIN_LED, GPIO_OUT);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Initilize inputs pins for player position and assign player number based on jumpers and auxillary buttons///////////
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

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Variables added for GP2040-X4 UART communication features///////////////////////////////////////////////////////////
	static uint32_t nextSerial = 0; //holds minimum time before next serial write
	const uint32_t serialInterval = 500; //minimum time in us between serial transmissions to give time for receiving side to clear buffer

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Variables for player numbers for GP2040-X4//////////////////////////////////////////////////////////////////////////
	int physicalPlayer = 0; //pins 0 and 1 solved to get player 1, 2, 3, or 4
	int assignedPlayer = 0; //The pysical player position gets assigned desired player number based on 2 or 4 player mode (3124 or 1234)
	int computerPlayer = 0; //This is going to be assigned based on xinput position assigned by computer (From player LED status)
	int orderPlayers = 0; //This is the setting for 2 or 4 player mode (3124 or 1234). 0 = 4 player (1234), 1 = 2 Player (3124)

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Determine Physical Player Position of Encoder //////////////////////////////////////////////////////////////////////
	//Determined by combination of permanent assignment pins
	if(gpio_get(PIN_PLAYER_0) && gpio_get(PIN_PLAYER_1)) physicalPlayer = 1;
	if(gpio_get(PIN_PLAYER_0) && !gpio_get(PIN_PLAYER_1)) physicalPlayer = 2;
	if(!gpio_get(PIN_PLAYER_0) && gpio_get(PIN_PLAYER_1)) physicalPlayer = 3;
	if(!gpio_get(PIN_PLAYER_0) && !gpio_get(PIN_PLAYER_1)) physicalPlayer = 4;

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

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//SERIAL SEND FOR GP2040-X4///////////////////////////////////////////////////////////////////////////////////////////
	if (getMicros() > nextSerial){
		uart_write_blocking(uart0, serialOut, serialLen); //write on interval to avoid flooding buffers
		nextSerial = getMicros() + serialInterval;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Steps to execute based on physical player location for GP2040-X4////////////////////////////////////////////////////
	if (physicalPlayer == 1) {
	gamepad.pressedLeft() ? bitWrite(serialOut[0],0,1) : bitWrite(serialOut[0],0,0); // read left button input to serial out byte 0 bit 0
	gamepad.pressedLeft() ? bitWrite(serialOut[0],0,1) : bitWrite(serialOut[0],0,0); // read left button input to serial out byte 0 bit 0
	gpio_put(PIN_LED, bitRead(serialOut[0], 0));
	}
	if (physicalPlayer == 2){
	serialOut[0] = serialIn[1];
	(gamepad.pressedLeft() || bitRead(serialIn[1],0)) ? gpio_put(PIN_LED,1) : gpio_put(PIN_LED,0); // button check to see if running
	}
	if (physicalPlayer == 3){
	serialOut[0] = serialIn[0];
	(gamepad.pressedLeft() || bitRead(serialIn[0],0)) ? gpio_put(PIN_LED,1) : gpio_put(PIN_LED,0); // button check to see if running
	}
	if (physicalPlayer == 4){
	(gamepad.pressedLeft() || bitRead(serialIn[0],0)) ? gpio_put(PIN_LED,1) : gpio_put(PIN_LED,0); // button check to see if running
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
