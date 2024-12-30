# sensitive-idiot

sensitive-idiot is a throwaway repository to test publishing things using MQTT
from within [zephyr].

It intends to build a x86\_64 firmware to run on the [QEMU] emulator.

## FETCH, BUILD AND FLASH

Create and enter the workspace directory:

	mkdir sensitive-workspace
	cd sensitive-workspace

Fetch the sources down to the workspace:

	west init -m git@github.com:idiot-prototypes/sensitive-idiot
	west update

Build and flash the application from the source tree:

	west build -b esp32_devkitc_wroom/esp32/procpu app/
	west build -t flash

## PREREQUISITE

### BLOBS

Download the blobs:

	west blobs west blobs fetch hal_espressif

## PATCHES

Sumbit patches at *https://github.com/yidiot/sensitive-idiot/pulls*

## BUGS

Report bugs at *https://github.com/yidiot/sensitive-idiot/issues*

## AUTHOR

Written by Gaël PORTAY *gael.portay@gmail.com*

## COPYRIGHT

Copyright (c) 2024 Gaël PORTAY

This program is licensed under the terms of the Apache License, version 2.0.

[zephyr]: https://github.com/zephyrproject-rtos/zephyr
