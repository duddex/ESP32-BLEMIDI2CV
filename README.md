# ESP32-BLEMIDI2CV

This is a module for converting MIDI messages to control voltage (CV) with an ESP32 using the ESP32-BLE-MIDI library from Maxime ANDRÉ (https://github.com/max22-/ESP32-BLE-MIDI).

There are 2 x 8 bits DAC channels on the ESP32 to convert digital signals into analog voltage signal outputs. These are the DAC channels: DAC1 (GPIO25), DAC2 (GPIO26). The MIDI data is received via Bluetooth and will be converted into control voltage for controlling analog synthesizers.

Note on and note off information is sent to PIN32 and PIN33. Note information, velocity information and modwheel information is converted to CV and sent to PIN25 and PIN26.

At the moment the ESP32-BLEMIDI2CV module supports two modes which can be selected via potentiometer connected to PIN 34.

## Mode 1
* Note on/Note off is sent to PIN32 (Note on: HIGH, Note off, LOW)
* Note information is converted to control voltage and sent to PIN25
* Velocity information is converted to control voltage and sent to PIN26

## Mode 2
* Note on/Note off is sent to PIN32 (Note on: HIGH, Note off, LOW)
* Note information is converted to control voltage and sent to PIN25
* Modwheel information is converted to control voltage and sent to PIN26

Turn the potentiometer to the left for mode 1. Turn it to the right for mode 2.

In mode 1 and mode 2 the module listens to all MIDI channels.
