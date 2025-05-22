#ifndef mztTumtaMidi_h
#define mztTumtaMidi_h

#include <stdio.h>
#include "ofMain.h"
#include "ofxMidi.h"
#include "ofxGui.h"
#include "mztTumtaData.h"

class mztTumtaMidi
{
public:
	void setup();
	void updateWithData(mztTumtaData * data);
	
	ofParameterGroup parametersTum, parametersTumIntensity,parametersTumMidi, parametersTa, parametersTaIntensity, parametersTaMidi;
	ofParameter<int>
		paramTumIntensityMin,
		paramTumIntensityMax,
		paramTumVelocityMin,
		paramTumVelocityMax,
		paramTumNote,
		paramTumChannel,
		paramTumNoteDuration,
		paramTaIntensityMin,
		paramTaIntensityMax,
		paramTaVelocityMin,
		paramTaVelocityMax,
		paramTaNote,
		paramTaChannel,
		paramTaNoteDuration;
	
//	ofParameter<bool> serial;
	
private:
	ofxMidiOut midiOut;
	unsigned long long lastNoteTriggeredMillis[2];
	
	void sendMidiNoteOn(int channel, int pitch, int velocity);
	void sendMidiNoteOff(int channel, int pitch);
	
	void iniGui();
	
	bool noteOffFlag[2] = {false, false};
	bool noteOnFlag[2] = {false, false};
	unsigned long long elapsedTime[2];
};

#endif