# Development

The project is built using the PlatformIO VS Code plugin along with the [Wiz-IO Raspberry Pi Pico](https://github.com/Wiz-IO/wizio-pico) platform package, using the baremetal (Pico SDK) configuration. There is an external dependency on the [MPG](https://github.com/FeralAI/MPG) C++ gamepad library for handling input state, providing extra features like Left/Right stick emulation and SOCD cleaning, and converting the generic gamepad state to the appropriate USB report.

There are two simple options for building GP2040 for your board. You can either edit an existing board definition, or create your own and configure PlatformIO to build it.

## Board Configuration

Configurations for each type of board/build are contained in separate folders inside `configs`.

### Existing Configuration

Once you have the project loaded into PlatformIO, edit the `config/Pico/BoardConfig.h` file to map your GPIO pins. Then from the VS Code status bar, use the PlatformIO environment selector to choose `env:raspberry-pi-pico`.

### New Configuration

1. Create a new folder in `configs` for your board, e.g. `configs/NewBoard`.
1. Create `configs/NewBoard/BoardConfig.h` and add your pin configuration and options. A `BoardConfig.h` file can be as basic as some pin definitions:

    ```clike
    // BoardConfig.h

    #define PIN_DPAD_UP     2
    #define PIN_DPAD_DOWN   3
    #define PIN_DPAD_LEFT   4
    #define PIN_DPAD_RIGHT  5
    #define PIN_BUTTON_B1   6
    #define PIN_BUTTON_B2   7
    #define PIN_BUTTON_B3   8
    #define PIN_BUTTON_B4   9
    #define PIN_BUTTON_L1   10
    #define PIN_BUTTON_R1   11
    #define PIN_BUTTON_L2   26
    #define PIN_BUTTON_R2   27
    #define PIN_BUTTON_S1   16
    #define PIN_BUTTON_S2   17
    #define PIN_BUTTON_L3   18
    #define PIN_BUTTON_R3   19
    #define PIN_BUTTON_A1   20
    #define PIN_BUTTON_A2   21

    #define DEFAULT_SOCD_MODE SOCD_MODE_NEUTRAL
    ```

1. Create `configs/NewBoard/env.ini` using the following template:

    ```clike
    [env:new-board]
    upload_port = .pio/build/new-board/
    build_flags =
        ${env.build_flags}
        -I configs/NewBoard/
    ```

    a. If you're not using a Pico or bare RP2040, check the `include/pico/config_autogen.h` file to see if there is a define for your board. If so, add or update the `-D BOARD_...` option in `build_flags`, for example if using the SparkFun Pro Micro RP2040:

    ```clike
    [env:sparkfun-pro-micro]
    upload_port = .pio/build/sparkfun-pro-micro/
    build_flags =
        ${env.build_flags}
        -D BOARD_SPARKFUN_MICRO_RP2040
        -I configs/SparkFunProMicro/
    ```

Select your environment from the VS Code status bar menu. You may need to restart VS Code in order for PlatformIO to pick up on the `env.ini` changes.

## LED Support

If your board has WS2812 (or similar) LEDs, they can be configured in the `BoardConfig.h` file. The `AnimationStation` local library uses the `Pixel` struct to map buttons to LEDs:

```clike
struct Pixel {
  uint8_t index;                  // The pixel index
  uint32_t mask;                  // Used to detect per-pixel lighting
  std::vector<uint8_t> positions; // The actual LED indexes on the chain
}
```

This allows an arbitrary number of LEDs to be grouped and treated as a single LED for theming and animation purposes.

### LED Options

The following options are available for use in your `BoardConfig.h` file:

| Name             | Description                  | Required? |
| ---------------- | ---------------------------- | --------- |
| **BOARD_LEDS_PIN** | Data PIN for your LED strand | Yes       |
| **LED_LAYOUT** | Defines the layout of your LED buttons as a `Pixel` matrix. Available options are: `LED_LAYOUT_ARCADE_BUTTONS`, `LED_LAYOUT_ARCADE_HITBOX`, `LED_LAYOUT_ARCADE_WASD` | Yes |
| **LED_FORMAT** | `LED_FORMAT_GRB` | No, default value `LED_FORMAT_GRB` |
| **LEDS_PER_PIXEL** | 1 | Yes |
| **LED_BRIGHTNESS_MAX** | Max brightness value, `uint8_t` 0-255 | Yes |
| **LED_BRIGHTNESS_STEPS** | The number of brightness steps when using the up/down hotkey | Yes |
| **LEDS_RAINBOW_CYCLE_TIME** | For `RAINBOW`, this sets how long (in ms) it takes to cycle from one color step to the next | No |
| **LEDS_CHASE_CYCLE_TIME** | For `CHASE`, this sets how long (in ms) it takes to move from one pixel to the next | No |
| **LEDS_STATIC_COLOR_COLOR** | For `STATIC`, this sets the static color. This is an `RGB` struct which can be found in `AnimationStation/src/Animation.hpp`. Can be custom or one of these predefined values: `ColorBlack`, `ColorWhite`, `ColorRed`, `ColorOrange`, `ColorYellow`, `ColorLimeGreen`, `ColorGreen`, `ColorSeafoam`, `ColorAqua`, `ColorSkyBlue`, `ColorBlue`, `ColorPurple`, `ColorPink`, `ColorMagenta` | No |

### LED Example

A full pixel setup in your `BoardConfig.h` file:

```clike
#include "enums.h"
#include "NeoPico.hpp"
#include "Animation.hpp"

#define BOARD_LEDS_PIN 22

#define LED_BRIGHTNESS_MAXIMUM 100
#define LED_BRIGHTNESS_STEPS 5
#define LED_FORMAT LED_FORMAT_GRB
#define LED_LAYOUT LED_LAYOUT_ARCADE_HITBOX
#define LEDS_PER_PIXEL 2

#define LEDS_RAINBOW_CYCLE_TIME 100
#define LEDS_CHASE_CYCLE_TIME 50
#define LEDS_STATIC_COLOR_COLOR ColorRed // Could also use: RGB(255, 0, 0)

#define LEDS_DPAD_LEFT   0
#define LEDS_DPAD_DOWN   1
#define LEDS_DPAD_RIGHT  2
#define LEDS_DPAD_UP     3
#define LEDS_BUTTON_B3   4
#define LEDS_BUTTON_B4   5
#define LEDS_BUTTON_R1   6
#define LEDS_BUTTON_L1   7
#define LEDS_BUTTON_B1   8
#define LEDS_BUTTON_B2   9
#define LEDS_BUTTON_R2   10
#define LEDS_BUTTON_L2   11
```

## Generating Binaries

You should now be able to build or upload the project to your RP2040 board from the Build and Upload status bar icons. You can also open the PlatformIO tab and select the actions to execute for a particular environment. Output folders are defined in the `platformio.ini` file and should default to a path under `.pio/build/${env:NAME}`.
