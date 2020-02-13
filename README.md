# PM game for GlobalLogic Starter Kit
## Prerequisites
- Linux PC with Ubuntu 16.04+
- GlobalLogic Starter Kit
## Preparing
1. Update APT caches and install common tools and libraries
```bash
sudo apt update
sudo apt install -y git vim gcc-arm-none-eabi libnewlib-arm-none-eabi minicom build-essential cmake libusb-1.0-0-dev
```
2. Install Linux [stlink tools](https://github.com/texane/stlink) using this [manual](https://github.com/texane/stlink/blob/master/doc/compiling.md)
3. Create work directory and add it to bash environment as a variable for fast access
```bash
mkdir ~/development
echo "export WORKDIR=~/development" >> ~/.bashrc
source ~/.bashrc
```
## Download game
1. Clone project
```bash
cd $WORKDIR
git clone https://github.com/astarianka/pm-game
cd pm-game
```
2. Init sub-modules
```bash
git submodule update --init
```
## Building and flashing firmware
1. Build firmware
```bash
make -j$(nproc)
```
2. Flash firmware to device
```bash
make flash
```

**Push SWT2 on-board button and enjoy the game :)**
## Overview
The aim of the game is to catch as many black squares obstacles as you can during 30 sec round. The game finishes if time is up or if you catch ' x ' obstacle (bomb) instead of target obstacle.
##  Control
Use SWT2 on-board button to start a new round.
In order to control PM use the following on-board buttons:
 * SWT1 - Right
 * SWT2 - Back to start point
 * SWT3 - Left
 * SWT4 - Up
 * SWT5 - Down
## Score
There are two numbers in the right part of display: one in the top represents **score**, another at the bottom represents how much **time** (in sec) is left.
When the score is more than 0 on-board green LED is on. When game is over red LED is turned on.
Each cached target obstacle (black square) gives you 1 point.
At the end of the round you can see your **round score** under "SCORE" section and **record score** of the game under "BEST" section.
Record score is stored permanently in EEPROM memory of the board, so it is kept even after hard reset.
>Note: In order to reset the record score value to 0 push on-board user button (A0) before starting the next round (while screen shows "GAME OVER", "SCORE" and "BEST").

