# ESP32-BLEMIDI2CV
Converting MIDI messages to control voltage with an ESP32 using the ESP32-BLE-MIDI library
from Maxime André (https://github.com/max22-/ESP32-BLE-MIDI)

There are 2 x 8 bits DAC channels on the ESP32 to convert digital signals into analog voltage signal outputs.
These are the DAC channels: DAC1 (GPIO25), DAC2 (GPIO26). This code receives MIDI data an converts the information
into control voltage for controlling analog synthesisers.
