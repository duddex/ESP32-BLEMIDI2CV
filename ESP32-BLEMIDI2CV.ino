#include <Arduino.h>
#include <BLEMidi.h> // https://github.com/max22-/ESP32-BLE-MIDI

// constants
const int LED_BUILTIN = 2;
const int TRIGGER_PIN1 = 32;
const int TRIGGER_PIN2 = 33;
const int CV_PIN1 = 25;
const int CV_PIN2 = 26;
const int MAX_OUTPUT_VOLTAGE = 3300;
const int LOWNOTE = 48;
const int HIGHNOTE = LOWNOTE + 36; // 3 octaves 

// global variables
int pitchbendrange = 2; // range in semitones -> TODO: should be selectable for example via potentiometer
float pitchvoltage1 = 0, pitchbendvoltage1 = 0, pitchvoltage2 = 0, pitchbendvoltage2 = 0;

// ---------------------------------------------------------------------------
void onNoteOn(uint8_t channel, uint8_t note, uint8_t velocity, uint16_t timestamp) {
  if (velocity == 0) {
    onNoteOff(channel, note, velocity, timestamp);
  }

  if (note<LOWNOTE) note=LOWNOTE;
  if (note>HIGHNOTE) note=HIGHNOTE;

  switch (channel) {
    case 0: // MIDI channel 1
      digitalWrite(TRIGGER_PIN1, HIGH);
      pitchvoltage1 = (((note-LOWNOTE)/12.0)*1000.0);
      if (pitchvoltage1 + pitchbendvoltage1 < 0) {
        dacWrite(CV_PIN1, 0);
      } else {
        dacWrite(CV_PIN1, map(pitchvoltage1 + pitchbendvoltage1, 0, MAX_OUTPUT_VOLTAGE, 0, 255));
      }
      //Serial.printf("Note on: MIDI channel %d, note %d (%fmV + %fmV), velocity %d\n", channel+1, note, pitchvoltage1, pitchbendvoltage1, velocity);
      break;

    case 1: // MIDI channel 2
      digitalWrite(TRIGGER_PIN2, HIGH);
      pitchvoltage2 = (((note-LOWNOTE)/12.0)*1000.0);
      if (pitchvoltage2 + pitchbendvoltage2 < 0) {
        dacWrite(CV_PIN2, 0);
      } else {
        dacWrite(CV_PIN2, map(pitchvoltage2 + pitchbendvoltage2, 0, MAX_OUTPUT_VOLTAGE, 0, 255));
      }
      //Serial.printf("Note on: MIDI channel %d, note %d (%fmV + %fmV), velocity %d\n", channel+1, note, pitchvoltage2, pitchbendvoltage2, velocity);
      break;
    
    default:
      break;
  }
}

// ---------------------------------------------------------------------------
void onNoteOff(uint8_t channel, uint8_t note, uint8_t velocity, uint16_t timestamp) {
  //Serial.printf("Note off: channel %d, note %d\n", channel+1, note);
  switch (channel) {
    case 0: // MIDI channel 1
      digitalWrite(TRIGGER_PIN1, LOW);
      break;

    case 1: // MIDI channel 2
      digitalWrite(TRIGGER_PIN2, LOW);
      break;
    
    default:
      break;
  }
}

// ---------------------------------------------------------------------------
// TODO: not tested yet
void onPitchbend(uint8_t channel, uint16_t value, uint16_t timestamp) {
  switch (channel) {
    case 0: // MIDI channel 1
      // The range of value here is 0 to 16383. Pitch bend wheel in "neutral position" is value 8192
      pitchbendvoltage1 = (map(value, 0, 16383, pitchbendrange * -1 * 1000, pitchbendrange * 1000) * (1000.0 / 12.0)) / 1000;
  
      if (pitchvoltage1 + pitchbendvoltage1 < 0) {
        dacWrite(CV_PIN1, 0);
      } else {
        dacWrite(CV_PIN1, map(pitchvoltage1 + pitchbendvoltage1, 0, MAX_OUTPUT_VOLTAGE, 0, 255));
      }
      //Serial.printf("Pitch bend: MIDI channel %d, value %d, pitchvoltage %fmV, pitchbendvoltage %fmV\n", channel+1, value, pitchvoltage1, pitchbendvoltage1);
      break;

      case 1: // MIDI channel 2
      // The range of value here is 0 to 16383. Pitch bend wheel in "neutral position" is value 8192
      pitchbendvoltage2 = (map(value, 0, 16383, pitchbendrange * -1 * 1000, pitchbendrange * 1000) * (1000.0 / 12.0)) / 1000;
  
      if (pitchvoltage2 + pitchbendvoltage2 < 0) {
        dacWrite(CV_PIN2, 0);
      } else {
        dacWrite(CV_PIN2, map(pitchvoltage2 + pitchbendvoltage2, 0, MAX_OUTPUT_VOLTAGE, 0, 255));
      }
      //Serial.printf("Pitch bend: MIDI channel %d, value %d, pitchvoltage %fmV, pitchbendvoltage %fmV\n", channel+1, value, pitchvoltage2, pitchbendvoltage2);
      break;

    default:
      break;
  }
  
}

// ---------------------------------------------------------------------------
void blink(int number, int highdelay, int lowdelay) {
  for (int i=0; i<number; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(highdelay);
    digitalWrite(LED_BUILTIN, LOW);
    delay(lowdelay);
  }
}

// ---------------------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  //Serial.println("ESP32 BLE MIDI device started");
  BLEMidiServer.begin("ESP32 BLE MIDI device");

  BLEMidiServer.setOnConnectCallback([]() {
    //Serial.println("Connected");
    blink(10, 25, 25);
  });

  BLEMidiServer.setNoteOnCallback(onNoteOn);
  BLEMidiServer.setNoteOffCallback(onNoteOff);
  //BLEMidiServer.setPitchBendCallback(onPitchbend);

  // There are 2 x 8 bits DAC channels on the ESP32 to convert digital signals into
  // analog voltage signal outputs. These are the DAC channels: DAC1 (GPIO25), DAC2 (GPIO26)
  pinMode(CV_PIN1, ANALOG);
  pinMode(CV_PIN2, ANALOG);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(TRIGGER_PIN1, OUTPUT);
  pinMode(TRIGGER_PIN2, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(TRIGGER_PIN1, LOW);
  digitalWrite(TRIGGER_PIN2, LOW);
}

// ---------------------------------------------------------------------------
void loop() {
  // nothing to do here :-)
}
