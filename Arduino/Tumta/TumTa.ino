//Receiver Code

#include "MIDIUSB.h"

#define TUM_THR_PIN A0
#define TUM_VEL_MIN_PIN A1
#define TA_THR_PIN A2
#define TA_VEL_MIN_PIN A3


bool ledState = false;

void setup() {
  Serial1.begin(38400);
  Serial.begin(38400);
  pinMode(LED_BUILTIN, OUTPUT);
}

unsigned char stringRecebida[2];
int footSelector;
int tumIntensity;
int taIntensity;

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
  updatePots();
  while (Serial1.available() > 2) {

    //charRecebido = Serial1.read();
    //Serial.println(charRecebido);
    footSelector = Serial1.read();
    Serial.print(footSelector);
    Serial.print(" ");
    if (footSelector == 255) {

      for (int i = 0; i < sizeof(stringRecebida); i++) {
        stringRecebida[i] = Serial1.read();
        Serial.print(stringRecebida[i]);
        Serial.print(" ");
      }
      tumIntensity = stringRecebida[0] + (stringRecebida[1] << 8);
      if (tumIntensity > tumIntensityMin) {
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
    }
    if (footSelector == 254) {
      for (int i = 0; i < sizeof(stringRecebida); i++) {
        stringRecebida[i] = Serial1.read();
        Serial.print(stringRecebida[i]);
        Serial.print(" ");
      }
      taIntensity = stringRecebida[0] + (stringRecebida[1] << 8);
      if (tumIntensity > tumIntensityMin) {
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

void updatePots() {
  tumIntensityMin = analogRead(TUM_THR_PIN);
  tumVelocityMin = floor(map(analogRead(TUM_VEL_MIN_PIN), 0, 1023, 0, 127));
  taIntensityMin = analogRead(TA_THR_PIN);
  taVelocityMin = floor(map(analogRead(TA_VEL_MIN_PIN), 0, 1023, 0, 127));
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

