# RF433ANY-M5Cardputer

RF433ANY-M5Cardputer is designed for the M5Cardputer platform, enabling RF433 signal decoding and analysis. It leverages the RF433any library to provide seamless integration of RF433 communication capabilities into M5Stack ecosystem projects.

## Installation

To use RF433ANY-M5Cardputer, download this repository, open the .ino file with Arduino IDE and upload it to your M5Cardputer.

## Features

- Decodes RF433 signals received by an Arduino board connected to a RF433MHz Receiver.
- Supports auto-synchronization protocols including Tri-bit, Tri-bit Inverted, and Manchester.
- Flexible decoding without pre-knowledge about signal timings.
- Outputs timing information in a format ready for use with other RF433 libraries, such as RF433recv.

## Usage

1. Connect your RF433 hardware to the M5Cardputer device and power it on.
2. Upon receiving RF433 signals, the code decodes the signals and outputs timing information both to the serial interface and the M5Cardputer's LCD display.

## Credits

RF433ANY-M5Cardputer is based on the original RF433ANY library by [Sébastien Millet](https://github.com/sebmillet/RF433any), adapted and optimized for the M5Stack ecosystem.
