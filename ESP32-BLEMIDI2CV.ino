#include <Arduino.h>
#include <BLEMidi.h> // https://github.com/max22-/ESP32-BLE-MIDI

// constants
const int LED_BUILTIN = 2;
const int TRIGGER_PIN1 = 32;
const int TRIGGER_PIN2 = 33;
const int POT_PIN = 34;
const int CV_PIN1 = 25;
const int CV_PIN2 = 26;
const int MAX_OUTPUT_VOLTAGE = 3300;
const int NUMBER_OF_MODES = 2;
const int LOWNOTE = 48;
const int HIGHNOTE = LOWNOTE + 36; // 3 octaves 

// global variables
int potvalue, oldpotvalue;
int modevalue, oldmodevalue;
int pitchbendrange = 2; // range in semitones -> TODO: should be selectable via potentiometer (like "mode")
float pitchvoltage = 0, pitchbendvoltage = 0;

// ---------------------------------------------------------------------------
void onNoteOn(uint8_t channel, uint8_t note, uint8_t velocity, uint16_t timestamp) {
  if (note<LOWNOTE) note=LOWNOTE;
  if (note>HIGHNOTE) note=HIGHNOTE;

  pitchvoltage = (((note-LOWNOTE)/12.0)*1000.0);
  if (pitchvoltage + pitchbendvoltage < 0) {
    dacWrite(CV_PIN1, 0);
  } else {
    dacWrite(CV_PIN1, map(pitchvoltage + pitchbendvoltage, 0, MAX_OUTPUT_VOLTAGE, 0, 255));
  }

  if (modevalue == 1) { // mode 1: CV_PIN2 is used to send velocity value
    dacWrite(CV_PIN2, map(velocity, 0, 127, 0, 255));
  }

  Serial.printf("Note on: MIDI channel %d, note %d (%fmV + %fmV), velocity %d\n", channel+1, note, pitchvoltage, pitchbendvoltage, velocity);
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(TRIGGER_PIN1, HIGH);
}

// ---------------------------------------------------------------------------
void onNoteOff(uint8_t channel, uint8_t note, uint8_t velocity, uint16_t timestamp) {
  Serial.printf("Note off: channel %d, note %d, velocity\n", channel+1, note, velocity);
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(TRIGGER_PIN1, LOW);
}

// ---------------------------------------------------------------------------
void onControlChange(uint8_t channel, uint8_t controller, uint8_t value, uint16_t timestamp) {
  // controller 1: modwheel
  // mode 2: CV_PIN2 is used to send modwheel value
  if ((controller == 1) && (modevalue == 2)) { 
    Serial.printf("Modwheel change: MIDI channel %d, value %d\n", channel+1, value);
    dacWrite(CV_PIN2, map(value, 0, 127, 0, 255));
  }
}

// ---------------------------------------------------------------------------
void onPitchbend(uint8_t channel, uint16_t value, uint16_t timestamp) {
  // The range of value here is 0 to 16383. Pitch bend wheel in "neutral position" is value 8192
  pitchbendvoltage = (map(value, 0, 16383, pitchbendrange * -1 * 1000, pitchbendrange * 1000) * (1000.0 / 12.0)) / 1000;
  Serial.printf("Pitch bend: MIDI channel %d, value %d, pitchvoltage %fmV, pitchbendvoltage %fmV\n", channel+1, value, pitchvoltage, pitchbendvoltage);
  if (pitchvoltage + pitchbendvoltage < 0) {
    dacWrite(CV_PIN1, 0);
  } else {
    dacWrite(CV_PIN1, map(pitchvoltage + pitchbendvoltage, 0, MAX_OUTPUT_VOLTAGE, 0, 255));
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
  Serial.println("ESP32 BLE MIDI device started");
  BLEMidiServer.begin("ESP32 BLE MIDI device");

  BLEMidiServer.setOnConnectCallback([]() {
    Serial.println("Connected");
    blink(10, 25, 25);
  });

  BLEMidiServer.setNoteOnCallback(onNoteOn);
  BLEMidiServer.setNoteOffCallback(onNoteOff);
  BLEMidiServer.setControlChangeCallback(onControlChange);
  BLEMidiServer.setPitchBendCallback(onPitchbend);

  // There are 2 x 8 bits DAC channels on the ESP32 to convert digital signals into
  // analog voltage signal outputs. These are the DAC channels: DAC1 (GPIO25), DAC2 (GPIO26)
  pinMode(CV_PIN1, ANALOG);
  pinMode(CV_PIN2, ANALOG);

  // Potentiometer for changing the mode
  pinMode(POT_PIN, INPUT);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(TRIGGER_PIN1, OUTPUT);
  pinMode(TRIGGER_PIN2, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(TRIGGER_PIN1, LOW);
  digitalWrite(TRIGGER_PIN2, LOW);
}

// ---------------------------------------------------------------------------
void loop() {
  potvalue = analogRead(POT_PIN);
  if (oldpotvalue != potvalue) {
    oldpotvalue = potvalue;
    modevalue = map(potvalue, 0, 4095, 1, NUMBER_OF_MODES);
    if (oldmodevalue != modevalue) {
      Serial.printf("Mode change: %d\n", modevalue);
      blink(modevalue, 200, 100);
      oldmodevalue = modevalue;
    }    
  }
}
