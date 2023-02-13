/*
  Based on Sketch built by Gustavo Silveira (aka Music Nerd)
  Modified by kenapagituh
  This code is only for Arduinos that use ATmega328 (like Uno, Mega, Nano, Beetle...)
*/

//=======================================================================================
// #include <MIDI.h>
// MIDI_CREATE_DEFAULT_INSTANCE();

//=======================================================================================
// Tombol
const byte JumlahTombol = 3;
const byte PinTombol[JumlahTombol] = {2, 3, 4};

int buttonCState[JumlahTombol] = {};
int buttonPState[JumlahTombol] = {};

unsigned long lastDebounceTime[JumlahTombol] = {0};
unsigned long debounceDelay = 50;

//=======================================================================================
// Potensiometer
const byte JumlahPotensiometer = 2;
const byte PinPotensiometer[JumlahPotensiometer] = {A0, A1};

int potCState[JumlahPotensiometer] = {0};
int potPState[JumlahPotensiometer] = {0};
int potVar = 0;

byte midiCState[JumlahPotensiometer] = {0};
byte midiPState[JumlahPotensiometer] = {0};

int TIMEOUT = 300;
byte varThreshold = 10;
boolean potMoving = true;
unsigned long PTime[JumlahPotensiometer] = {0};
unsigned long timer[JumlahPotensiometer] = {0};
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

//          MIDI.sendNoteOn(note + i, 127, midiCh); // note, velocity, channel

          Serial.print(i);
          Serial.println(": Tombol ditekan");
        } else {
          
//          MIDI.sendNoteOn(note + i, 0, midiCh); // note, velocity, channel

          Serial.print(i);
          Serial.println(": Tombol dilepas");
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
    }
    else {
      potMoving = false;
    }

    if (potMoving == true) {
      if (midiPState[i] != midiCState[i]) {

//        MIDI.sendControlChange(cc + i, midiCState[i], midiCh);
        Serial.print("Pot: ");
        Serial.print(i);
        Serial.print(" ");
        Serial.println(midiCState[i]);
        //Serial.print("  ");
        
        potPState[i] = potCState[i];
        midiPState[i] = midiCState[i];
      }
    }
  }
}
