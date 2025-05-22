#ifndef mztTumtaMidi_h
#define mztTumtaMidi_h

#include <stdio.h>
#include "ofMain.h"
#include "ofxMidi.h"
#include "ofxGui.h"
#include "mztTumtaData.h"

#define MIDI_TUMTA_CHANNEL 1
#define MIDI_NOTE_TUM 30
#define MIDI_NOTE_TA 31

class mztTumtaMidi
{
public:
	void setup();
	void updateWithData(mztTumtaData * data);
	
	ofParameterGroup parameters;
	ofParameter<int> paramTumIntensityMin, paramTumIntensityMax, paramTumVelocityMin, paramTumNoteDelay, paramTaIntensityMin, paramTaIntensityMax, paramTaVelocityMin, paramTaNoteDelay;
	
private:
	ofxMidiOut midiOut;
	unsigned long long lastNoteTriggeredMillis[2];
	
	void sendMidiNoteOn(int channel, int pitch, int velocity);
	void sendMidiNoteOff(int channel, int pitch);
	
	void iniGui();
};

#endif