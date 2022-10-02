# ESP32-BLEMIDI2CV
Converting MIDI messages to control voltage (CV) with an ESP32 using the ESP32-BLE-MIDI library
from Maxime ANDRÉ (https://github.com/max22-/ESP32-BLE-MIDI)

There are 2 x 8 bits DAC channels on the ESP32 to convert digital signals into analog voltage signal outputs.
These are the DAC channels: DAC1 (GPIO25), DAC2 (GPIO26). This code receives MIDI data an converts the information
into control voltage for controlling analog synthesisers.

Note on and note off information is sent to PIN32. Note information, velocity information and modwheel information is converted to CV and sent to PIN25 and PIN26.

At the moment the ESP32-BLEMIDI2CV code supports two modes

## Mode 1
* Note on/Note off is sent to PIN32 (Note on: HIGH, Note off, LOW)
* Note information is converted to control voltage and sent to PIN25
* Velocity information is converted to control voltage and sent to PIN26

## Mode 2
* Note on/Note off is sent to PIN32 (Note on: HIGH, Note off, LOW)
* Note information is converted to control voltage and sent to PIN25
* Modwheel information is converted to control voltage and sent to PIN26
