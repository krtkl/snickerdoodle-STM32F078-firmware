# snickerdoodle-STM32F078-firmware
Production firmware for the STM32F078 platform controller on the snickerdoodle embedded development system

https://krtkl.github.io/snickerdoodle-STM32F078-firmware/

## Build with Keil

???

## Build with GCC

Install the [GNU ARM Embedded toolchain](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads)

Build with:

```
make
```

**WARNING: The firmware compiled with GCC and this Makefile is not yet functional**

## DFU

To enter DFU mode, press and hold both buttons when powering on the board. All the LEDs light up.

With [dfu-util](http://dfu-util.sourceforge.net/):

Back up original firmware:
```
dfu-util -a 0 -s 0x08000000:131072 -U original.bin
```

Flash firmware:
```
dfu-util -a 0 -s 0x08000000:leave -D build/firmware.bin
```

## SWD

Alternatively, the STM32F078 can be programmed and debugged via SWD on connector J2:

|Pin| Function |
|---|------   |
|15 | SWCLK   |
|16 | SWDIO   |
|17 | nRST    |
|18 | 3.3V    |
|20 | GND     |

(from [the Snickerdoodle Book](https://static1.squarespace.com/static/55557fa8e4b0cf3d9840408d/t/57660c11f5e23128b3240a62/1466305567818/snickerdoodle-book.pdf))
