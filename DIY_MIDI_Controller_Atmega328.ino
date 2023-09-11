/*
  Based on Sketch built by Gustavo Silveira (aka Music Nerd)
  Modified by kenapagituh
  This code is only for Arduinos that use ATmega328 (like Uno, Mega, Nano, Beetle...)
*/

//=======================================================================================
#define off 1  // on MIDI MODE, off DEBUG MODE

#ifdef on
#include <MIDI.h>
MIDI_CREATE_DEFAULT_INSTANCE();
#endif

//=======================================================================================
// Tombol dan Potensiometer. Silahkan ubah disini.
const byte JumlahTombol = 3;                                    // Tombolnya ada berapa?
const byte PinTombol[JumlahTombol] = { 2, 3, 4 };               // Tombolnya terhubung ke pin berapa?
const byte JumlahPotensiometer = 2;                             // Potensiometernya ada berapa?
const byte PinPotensiometer[JumlahPotensiometer] = { A0, A1 };  // Potensiometernya terhubung ke pin berapa?

//=======================================================================================
// Function Tombol. Jangan diubah jika tidak perlu.
int buttonCState[JumlahTombol] = {};
int buttonPState[JumlahTombol] = {};
unsigned long lastDebounceTime[JumlahTombol] = { 0 };
unsigned long debounceDelay = 50;

//=======================================================================================
// Function Potensiometer. Jangan diubah jika tidak perlu.
int potCState[JumlahPotensiometer] = { 0 };
int potPState[JumlahPotensiometer] = { 0 };
int potVar = 0;

byte midiCState[JumlahPotensiometer] = { 0 };
byte midiPState[JumlahPotensiometer] = { 0 };

int TIMEOUT = 300;
byte varThreshold = 10;
boolean potMoving = true;
unsigned long PTime[JumlahPotensiometer] = { 0 };
unsigned long timer[JumlahPotensiometer] = { 0 };

//=======================================================================================
// MIDI
byte midiCh = 1;
byte note = 36;
byte cc = 1;

//=======================================================================================
// SETUP dan LOOP

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < JumlahTombol; i++) {
    pinMode(PinTombol[i], INPUT_PULLUP);
  }
}

void loop() {
  tombol();
  potensiometer();
}

//=======================================================================================

// Tombol
void tombol() {
  for (int i = 0; i < JumlahTombol; i++) {
    buttonCState[i] = digitalRead(PinTombol[i]);
    if ((millis() - lastDebounceTime[i]) > debounceDelay) {
      if (buttonPState[i] != buttonCState[i]) {
        lastDebounceTime[i] = millis();
        if (buttonCState[i] == LOW) {
#ifdef on
          MIDI.sendNoteOn(note + i, 127, midiCh);  // note, velocity, channel
#elif off
          Serial.print(i);
          Serial.println(": Tombol ditekan");
#endif
        } else {
#ifdef on
          MIDI.sendNoteOn(note + i, 0, midiCh);  // note, velocity, channel
#elif off
          Serial.print(i);
          Serial.println(": Tombol dilepas");
#endif
        }
        buttonPState[i] = buttonCState[i];
      }
    }
  }
}

//=======================================================================================

// Potensiometer
void potensiometer() {
  for (int i = 0; i < JumlahPotensiometer; i++) {
    potCState[i] = analogRead(PinPotensiometer[i]);
    midiCState[i] = map(potCState[i], 0, 1023, 0, 127);
    potVar = abs(potCState[i] - potPState[i]);
    if (potVar > varThreshold) {
      PTime[i] = millis();
    }
    timer[i] = millis() - PTime[i];
    if (timer[i] < TIMEOUT) {
      potMoving = true;
    } else {
      potMoving = false;
    }
    if (potMoving == true) {
      if (midiPState[i] != midiCState[i]) {
#ifdef on
        MIDI.sendControlChange(cc + i, midiCState[i], midiCh);
#elif off
        Serial.print("Pot: ");
        Serial.print(i);
        Serial.print(" ");
        Serial.println(midiCState[i]);
#endif
        potPState[i] = potCState[i];
        midiPState[i] = midiCState[i];
      }
    }
  }
}
