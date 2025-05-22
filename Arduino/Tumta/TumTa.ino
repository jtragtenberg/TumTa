//Receiver Code

#include "MIDIUSB.h"

bool ledState = false;

void setup() {
  Serial1.begin(38400);
  Serial.begin(38400);
  pinMode(LED_BUILTIN, OUTPUT);
}

unsigned char stringRecebida[2];
unsigned char stringRecebidaDebug[13];
unsigned char modeSelector[2];
unsigned char footSelector;
bool modoDebug = false;
bool modoExec = false;

int tumPressure;
int taPressure;
int tumDerivative;
int taDerivative;
int tumIntensity;
int taIntensity;
int checksum;

//Configurações MIDI
int tumMidiChannel = 0;
int tumMidiNote = 31;
int tumMidiVelocity;
int taMidiChannel = 0;
int taMidiNote = 32;
int taMidiVelocity;
int midiNoteDuration = 500; //duração da nota em ms

//configurações de Mapeamento
int tumIntensityMin = 20;
int tumIntensityMax = 1023;
int tumVelocityMin = 20;
int tumVelocityMax = 127;

int taIntensityMin = 20;
int taIntensityMax = 1023;
int taVelocityMin = 20;
int taVelocityMax = 127;

double tumTriggerTime;
double taTriggerTime;
bool umTumOff = true;
bool umTaOff = true;

void loop() {
  if (Serial1.available()) {
    modeSelector[0] = Serial1.read();
    Serial.println(modeSelector[0]);
    if (modeSelector[0] == 255 || modeSelector[0] == 254) {
      modeSelector[1] = Serial1.read();
      Serial.print("ANDOU ");
      Serial.println(modeSelector[0]);
      if (modeSelector[1] == 255 || modeSelector[1] == 254) {
        Serial.print("ANDOU DENOVO ");
        Serial.print(modeSelector[0]);
        Serial.print(" ");
        Serial.println(modeSelector[1]);
      }
    }
  }
  /*
    modeSelector[0] = Serial1.read();
    //modeSelector[1] = Serial1.read();
    Serial.print("MODO: ");
    Serial.println(modeSelector[0]);
    if (modeSelector[0] == 255 && modeSelector[1] == 255) {
    modoDebug = true;
    modoExec = false;
    }
    else if (modeSelector[0] == 254) {
    modoDebug = false;
    modoExec = true;
    footSelector = modeSelector[1];
    }
    else {
    modoDebug = false;
    modoExec = false;
    modeSelector[0] = modeSelector[1];
    modeSelector[1] = Serial1.read();
    }


    }

    if (modoDebug) {
    while (Serial1.available() > 12) {
    for (int i = 0; i < sizeof(stringRecebida); i++) {
      stringRecebida[i] = Serial1.read();
    }
    }
    for (int i = 0; i < sizeof(stringRecebidaDebug); i++) {
    stringRecebidaDebug[i] = Serial1.read();
    }
    tumPressure = stringRecebidaDebug[0] + (stringRecebidaDebug[1] << 8);
    taPressure = stringRecebidaDebug[2] + (stringRecebidaDebug[3] << 8);
    tumDerivative = stringRecebidaDebug[4] + (stringRecebidaDebug[5] << 8);
    taDerivative = stringRecebidaDebug[6] + (stringRecebidaDebug[7] << 8);
    tumIntensity = stringRecebidaDebug[8] + (stringRecebidaDebug[9] << 8);
    taIntensity = stringRecebidaDebug[10] + (stringRecebidaDebug[11] << 8);
    checksum = stringRecebidaDebug[12];

    Serial.print(tumPressure);
    Serial.print(" ");
    Serial.print(tumDerivative);
    Serial.print(" ");
    Serial.print(tumIntensity);
    Serial.print(" ");

    Serial.print(taPressure);
    Serial.print(" ");
    Serial.print(taDerivative);
    Serial.print(" ");
    Serial.print(taIntensity);
    Serial.print(" ");

    }
    if (modoExec) {
    while ( Serial1.available() > 1) {
    if (footSelector == 255) { //se for uma nota tum
      for (int i = 0; i < sizeof(stringRecebida); i++) {
        stringRecebida[i] = Serial1.read();
      }
      tumIntensity = stringRecebida[0] + (stringRecebida[1] << 8);

      Serial.print("Tum - intensity:");
      Serial.print(tumIntensity);
      umTumOff = true; //reinicia o flag pra so dar um noteoff
      tumMidiVelocity = mapeamento(tumIntensity, tumIntensityMin, tumIntensityMax, tumVelocityMin, tumVelocityMax);
      Serial.print(" velocity:");
      Serial.println(tumMidiVelocity);
      noteOn(tumMidiChannel, tumMidiNote, tumMidiVelocity);
      MidiUSB.flush();
      tumTriggerTime = millis();
    }
    else if (footSelector = 254) {
      for (int i = 0; i < sizeof(stringRecebida); i++) {
        stringRecebida[i] = Serial1.read();
      }
      taIntensity = stringRecebida[0] + (stringRecebida[1] << 8);

      Serial.print("Ta - intensity:");
      Serial.print(taIntensity);
      umTaOff = true; //reinicia o flag pra so dar um noteoff
      taMidiVelocity = mapeamento(taIntensity, taIntensityMin, taIntensityMax, taVelocityMin, taVelocityMax);
      Serial.print(" velocity:");
      Serial.println(taMidiVelocity);
      noteOn(taMidiChannel, taMidiNote, taMidiVelocity);
      MidiUSB.flush();
      taTriggerTime = millis();
    }
    }
    if ((millis() - tumTriggerTime) > midiNoteDuration && umTumOff) {
    noteOff(tumMidiChannel, tumMidiNote, 0);
    MidiUSB.flush();
    umTumOff = false;

    }
    if ((millis() - taTriggerTime) > midiNoteDuration && umTaOff) {
    noteOff(taMidiChannel, taMidiNote, 0);
    MidiUSB.flush();
    umTaOff = false;
    }
    }
  */
}

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

int mapeamento(int intensity, int intensityMin, int intensityMax, int velocityMin, int velocityMax) {
  int velocity = floor(map(intensity, intensityMin, intensityMax, velocityMin, velocityMax));
  if (velocity > velocityMax) velocity = velocityMax;
  if (velocity < velocityMin) velocity = velocityMin;
  return velocity;
}

