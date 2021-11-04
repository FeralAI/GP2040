# GP2040

GP2040 is a game controller firmware for RP2040 microcontrollers. The goal is to provide high performance and a rich feature set across multiple platforms. GP2040 currently has the following features:

* Support for XInput, DirectInput and Nintendo Switch input modes
* Less than 1ms of input latency at the default 1000 Hz (1 ms) polling rate
* Multiple SOCD cleaning modes - Neutral, Up Priority (a.k.a. Hitbox), Second Input Priority
* Left and Right stick emulation via D-pad inputs
* Per-button RGB LED support
* PWM and RGB player indicator LED support (XInput only)
* Saves options to internal memory

## Performance

Input latency is tested using the methodology outlined at [WydD's inputlag.science website](https://inputlag.science/controller/methodology), using the default 1000 Hz (1 ms) polling rate in the firmware.

| Version | Mode | Poll Rate | Min | Max | Avg | Stdev | % on time | %1f skip | %2f skip |
| - | - | - | - | - | - | - | - | - | - |
| v0.3.1 | All | 1 ms | 0.56 ms | 1.32 ms | 0.85 ms | 0.24 ms | 95.95% | 4.05% | 0% |

Full results can be found in the [GP2040 Firmware Latency Test Results](https://docs.google.com/spreadsheets/d/1eeX0SCOYnUDZMYzt_69wDpjnB_XUtvsfvHJYxxgTj28/edit#gid=1559471406) Google Sheet.

## Installation

Prebuilt `uf2` files are available in the [Releases](https://github.com/FeralAI/GP2040/releases) section for the following boards:

* [Raspberry Pi Pico](https://github.com/FeralAI/GP2040/tree/main/configs/Pico) and other pin-compatible boards such as the Pimoroni Pico Lipo
* [Pico Fighting Board](https://github.com/FeralAI/GP2040-Config-PicoFightingBoard/)
* [Crush Counter](https://github.com/FeralAI/GP2040/tree/main/configs/CrushCounter) (formerly the [OSFRD](https://github.com/FeralAI/GP2040/tree/main/configs/OSFRD))

Several other working example configurations are located in the [configs](https://github.com/FeralAI/GP2040/tree/main/configs) folder.

The instructions will slightly vary based on your device. These instructions are for a Raspberry Pi Pico.

1. Download the latest `GP2040.uf2` file from the [Releases](https://github.com/FeralAI/GP2040/releases) section for your board (e.g. `GP2040-PiPico.uf2` for the Raspberry Pi Pico).
1. Unplug your Pico.
1. Hold the BOOTSEL button on the Pico and plug into your computer. A new removable drive named `RPI-RP2` should appear in your file explorer.
1. Drag and drop the `GP2040.uf2` file into the removable drive. This will flash the board.
1. The board is now running the GP2040 firmware and will appear as a controller on your computer.

## Buttons

GP2040 uses a generic button labeling for gamepad state, which is then converted to the appropriate input type before sending. Here are the mappings of generic buttons to each supported platform/layout:

| GP2040  | XInput | Switch  | PS3          | DirectInput  | Arcade |
| ------- | ------ | ------- | ------------ | ------------ | ------ |
| B1      | A      | B       | Cross        | 2            | K1     |
| B2      | B      | A       | Circle       | 3            | K2     |
| B3      | X      | Y       | Square       | 1            | P1     |
| B4      | Y      | X       | Triangle     | 4            | P2     |
| L1      | LB     | L       | L1           | 5            | P4     |
| R1      | RB     | R       | R1           | 6            | P3     |
| L2      | LT     | ZL      | L2           | 7            | K4     |
| R2      | RT     | ZR      | R2           | 8            | K3     |
| S1      | Back   | Minus   | Select       | 9            | Coin   |
| S2      | Start  | Plus    | Start        | 10           | Start  |
| L3      | LS     | LS      | L3           | 11           | LS     |
| R3      | RS     | RS      | R3           | 12           | RS     |
| A1      | Guide  | Home    | -            | 13           | -      |
| A2      | -      | Capture | -            | 14           | -      |

## Usage

Any button references in this documentation will use the `XInput` labels for clarity.

> NOTE: LED modes are available on the Pico Fighting Board, Crush Counter/OSFRD and custom builds only.

### Home Button

If you do not have a dedicated Home button, you can activate it via the **`BACK + START + UP`** button combination.

### Input Modes

To change the input mode, **hold one of the following buttons as the controller is plugged in (Arcade mapping in parentheses):**

* **`X (P1)`** for DirectInput/PS3
* **`A (K1)`** for Nintendo Switch
* **`B (K2)`** for XInput

Input mode is saved across power cycles.

### D-Pad Modes

You can switch between the 3 modes for the D-Pad **while the controller is in use by pressing one of the following combinations:**

* **`BACK + START + DOWN`** - D-Pad
* **`BACK + START + LEFT`** - Emulate Left Analog stick
* **`BACK + START + RIGHT`** - Emulate Right Analog stick

D-Pad mode is saved across power cycles.

### SOCD Modes

Simultaneous Opposite Cardinal Direction (SOCD) cleaning will ensure the controller doesn't send invalid directional inputs to the computer/console, like Left + Right at the same time. There are 3 modes to choose from **while the controller is in use by pressing one of the following combinations:**

* **`LS + RS + UP`** - **Up Priority mode**: Up + Down = Up, Left + Right = Neutral (Hitbox behavior)
* **`LS + RS + DOWN`** - **Neutral mode**: Up + Down = Neutral, Left + Right = Neutral
* **`LS + RS + LEFT`** - **Last Input Priority (Last Win)**: Hold Up then hold Down = Down, then release and re-press Up = Up. Applies to both axes.

SOCD mode is saved across power cycles.

### LED Brightness

You can increase brightness with `BACK + START + Y` and decrease brightness with `BACK + START + B`.

### LED Modes

Swap between LED modes using the `BACK + START + A` or `BACK + START + X`. The following modes are available (pics coming eventually):

* Off
* Static Color
* Rainbow Cycle
* Rainbow Chase
* Static Rainbow
* Super Famicom
* Xbox
* Neo Geo Classic
* Neo Geo Curved
* Neo Geo Modern
* Six Button Fighter
* Six Button Fighter+
* Guilty Gear Type-A
* Guilty Gear Type-D

## Support

If you would like to discuss features, issues or anything else related to GP2040 please [create an issue](https://github.com/FeralAI/GP2040/issues/new) or join the [OpenStick GP2040 Discord channel](https://discord.gg/ABQafnty).
