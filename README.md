THIS IS A WORK IN PROGRESS!
IT DOES NOT WORK RIGHT NOW
******************************************************
Lee Maskell's 4PXE Firmware (fork of FeralAI's GP2040)
******************************************************

This project provides a 4 player XInput solution for arcade cabinets using a windows computer.

Features:
* High compatibility with modern PC games (Emulates Xbox 360 controller, BUT DOES NOT WORK ON ANY XBOX)
* Players 1 to 4 stay in the correct order (without any software or configuration on computer)
* Extremely low latency (up to 3ms in addition to 1ms of existing GP2040 firmware - unverified)
* Compatible with most raspberry pi arcade setups as well (although almost any dinput encoders are adequate with a pi)

******************************************************
JOYSTICK/BUTTON GPIO PINOUT

![image](https://user-images.githubusercontent.com/29419798/161194361-953cf7bf-aeda-4b18-8b6f-2e04e53bc69e.png)

******************************************************
This project uses 4 Raspberry Pi foundation RP2040 microcontrollers connected to arcade controllers and to eachother with a serial bus. Each one connects to your computer via USB and gets assigned a player number by the computer. the computer lets each controller know what number it was assigned. The inputs to each RP2040 get passed along using serial communication to whichever RP2040 was assigned the corresponding player number.

If the RP2040 receiving Player 1 buttons is assigned as player 2 by the computer, and the one wired up to player 2 buttons is assigned player 1 by the computer:
-Player 1 button inputs get send to the RP2040 in player 2 position which was assigned as player 1 by computer
-Player 2 button inputs get sent to the RP2040 in player 1 position which was assigned as player 2 by computer




Acknowledgement:
This is a fork of FeralAI's GP2040 firmware for a single encoder for fightsticks. This project provides the best fightstick solution I am aware of in terms of both cost AND performance. The scope of my work here is specifically the addition of the 4 player xinput management scheme for arcade cabinets. Thank you FeralAI! 

GP2040 project:
https://github.com/FeralAI/GP2040
