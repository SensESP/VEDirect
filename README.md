# Victron Energy VE.Direct Input Parser for SensESP

This library implements a [Victron Energy VE.Direct protocol](https://www.victronenergy.com/live/vedirect_protocol:faq) text mode input parser for
the [SensESP](https://signalk.org/SensESP/) framework.
The library allows reading VE.Direct output from any Victron device and writing the output to a Signal K server (examples included) or as NMEA 2000 messages (minor work required).

## VE.Direct Interface

The VE.Direct serial communication interface is documented in the "VE.Direct Protocol" document available at [this page](https://www.victronenergy.com/support-and-downloads/technical-information) (direct linking unfortunately not possible).

The physical interface is a four-pin JST-PH connector with pins for GND, RX, TX, and power, with RX and TX connecting to the product microcontroller UART.
It is important to notice that the [VE.Direct interface voltage level depends on the product](https://www.victronenergy.com/live/vedirect_protocol:faq#q4is_the_vedirect_interface_33_or_5v); some are 3.3V, while others are 5V.
Since the ESP32 is _not_ 5V tolerant, a logic level converter (or a voltage divider) is required for 5V devices.
Additionally, Victron Energy states that [galvanic isolation is important](https://www.victronenergy.com/live/vedirect_protocol:faq#introduction) because there is little protection on the Victron product itself.
Providing proper galvanic isolation requires some additional hardware and is out of scope for this README.

The serial bit rate is 19200 bps, with 8 data bits, no parity, and 1 stop bit.
No flow control is used.
The VE.Direct interface can operate in two modes: Text-mode and HEX-mode.
This library only implements Text-mode input.

## Minimal Hardware Configuration

Victron Energy provides inexpensive VE.Direct cables.
They have the great benefit of having molded connectors for improved durability and handling.
One such cable can be cut in half to get two pigtail cables with bare wires.
Note that the RX and TX of the cable are crossed over, so you must always check the pinout and color coding of the cable.

You can also make the cables yourself.
To make a VE.Direct cable, it is possible to crimp the JST PH connection yourself.
A more practical solution is to get a bunch of JST PH pigtail cables and splice the wires to a longer cable.
The type of cable is not important; a suitable ethernet cable or even a flat 4-wire phone cord is fine.

Connect the VE.Direct interface GND to the GND pin of the ESP32.
If your Victron Energy product is a 5V device (all MPPTs), you MUST use a logic level converter.
Connect the Victron product VE.Direct TX pin (via the logic level converter, if necessary) to ESP32 GPIO pin 17.
If you're using an [SH-ESP32](https://hatlabs.github.io/sh-esp32/), connect the TX (again, via the logic level converter, if necessary) to the SH-ESP32 I2C SCL pin.
The SCL pin has some EMC protection that might come handy.

## Using the library

### In-place builds

An example source code file for BMV device connectivity is located at `examples/vedirect-bmv.cpp`.
You can clone this repository and copy that file to `src/main.cpp` and build and upload.
More details for doing that can be found in [SensESP documentation](https://signalk.org/SensESP/pages/getting_started/). After configuring WiFi and Signal K settings, you should have your BMV data available in Signal K.

Likewise, an example source code file for interfacing MPPT solar charge controllers is located at `examples/vedirect-solar.cpp`.
IMPORTANT: MPPT charge controllers use a 5V VE.Direct interface and can NOT be directly connected to the ESP32.
A logic level converter or a voltage divider is required.

### Using SensESP/VEDirect as an imported library

The library can also be pulled in as an external dependency in your own project.
This is done by adding `SensESP/VEDirect` to your `platformio.ini` file `lib_deps` section.
An example project is located at FIXME.
