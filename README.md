# ESP32-BLEMIDI2CV

This is a module for converting MIDI messages to control voltage (CV) with an ESP32 using the ESP32-BLE-MIDI library from Maxime ANDRÉ (https://github.com/max22-/ESP32-BLE-MIDI).

There are 2 x 8 bits DAC channels on the ESP32 to convert digital signals into analog voltage signal outputs. These are the DAC channels: DAC1 (GPIO25), DAC2 (GPIO26). The MIDI data is received via Bluetooth and will be converted into control voltage for controlling analog synthesizers. Note on and note off information is sent to PIN32 (MIDI channel 1) and PIN33 (MIDI channel 2). Note information is converted to CV and sent to PIN25 (MIDI channel 1) and PIN26 (MIDI channel 2).
