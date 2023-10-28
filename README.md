# Motor Controller Firmware

## Getting started

This project contains the STM32CubeG4 repo. This is an SDK for the
microcontroller. This is also a git submodule, so when cloning this repo make
sure to run `git submodule update --init --recursive`.

To build the code, run `make all` or just `make`. To flash the board over a
debugger, run `make flash`.

The HAL makes all of its modules available in `stm32g4xx_hal.h`. To enable or
disable modules, comment out the relevant lines in `src/stm32g4xx_hal.h`. You
will have to do this to fit the code into flash.
