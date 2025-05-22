#include "mztTumtaMidi.h"

void mztTumtaMidi::setup()
{
	midiOut.listPorts();
	vector<string> midiPorts = midiOut.getPortList();
	midiOut.openPort(0);
	iniGui();
}

void mztTumtaMidi::updateWithData(mztTumtaData * data)
{
	// Tum
	if(data->getTumIntensityLastValue() > paramTumIntensityMin && noteOnFlag[0])
	{
		int mappedVelocity = ofMap(data->getTumIntensityLastValue(), paramTumIntensityMin, paramTumIntensityMax, paramTumVelocityMin, paramTumVelocityMin.getMax());
		mappedVelocity = ofClamp(mappedVelocity, paramTumVelocityMin, paramTumVelocityMax);
		
		sendMidiNoteOn(paramTumChannel, paramTumNote, mappedVelocity);
		
		lastNoteTriggeredMillis[0] = ofGetElapsedTimeMillis();
		
		noteOffFlag[0] = true;
		noteOnFlag[0] = false;
		
	}
	
	if(data->getTumIntensityLastValue() < paramTumIntensityMin && !noteOnFlag[0]){
		noteOnFlag[0] = true;
	}
	
	elapsedTime[0] = ofGetElapsedTimeMillis() - lastNoteTriggeredMillis[0];
	
	if(elapsedTime[0] > paramTumNoteDuration && noteOffFlag[0]) 	{
		sendMidiNoteOff(paramTumChannel, paramTumNote);
		noteOffFlag[0] = false;
	}
	
	// Ta
	if(data->getTaIntensityLastValue() > paramTaIntensityMin && noteOnFlag[1])
	{
		
		int mappedVelocity = ofMap(data->getTaIntensityLastValue(), paramTaIntensityMin,    paramTaIntensityMax, paramTaVelocityMin, paramTaVelocityMax);
		mappedVelocity = ofClamp(mappedVelocity, paramTaVelocityMin, paramTaVelocityMin.getMax());
		
		sendMidiNoteOn(paramTaChannel, paramTaNote, mappedVelocity);
		
		lastNoteTriggeredMillis[1] = ofGetElapsedTimeMillis();
		
		noteOffFlag[1] = true;
		noteOnFlag[1] = false;
		
		
	}
	
	if(data->getTaIntensityLastValue() < paramTaIntensityMin && !noteOnFlag[1]){
		noteOnFlag[1] = true;
	}
	
	elapsedTime[1] = ofGetElapsedTimeMillis() - lastNoteTriggeredMillis[1];
	
	if(elapsedTime[1] > paramTaNoteDuration && noteOffFlag[1])
	{
		sendMidiNoteOff(paramTaChannel, paramTaNote);
		noteOffFlag[1] = false;
	}
}

void mztTumtaMidi::sendMidiNoteOn(int channel, int pitch, int velocity)
{
	midiOut.sendNoteOn(channel, pitch, velocity);
}

void mztTumtaMidi::sendMidiNoteOff(int channel, int pitch)
{
	midiOut.sendNoteOff(channel, pitch, 0);
}

// GUI
void mztTumtaMidi::iniGui()
{
	// TUM
	parametersTum.setName("Tum");
	// Intensity
	parametersTumIntensity.setName("Intensity");
	parametersTumIntensity.add(paramTumIntensityMin.set("Tum Intensity Min", 0, 0, 1023));
	parametersTumIntensity.add(paramTumIntensityMax.set("Tum Intensity Max", 0, 0, 1023));
	parametersTum.add(parametersTumIntensity);
	// Midi
	parametersTumMidi.setName("Midi");
	parametersTumMidi.add(paramTumChannel.set("Tum MIDI Channel", 0, 1, 16));
	parametersTumMidi.add(paramTumNote.set("Tum MIDI Note", 0, 0, 127));
	parametersTumMidi.add(paramTumVelocityMin.set("Tum Velocity Min", 0, 0, 127));
	parametersTumMidi.add(paramTumVelocityMax.set("Tum Velocity Max", 0, 0, 127));
	parametersTumMidi.add(paramTumNoteDuration.set("Tum Note Duration", 0, 0, 1000));
	parametersTum.add(parametersTumMidi);
	
	// TA
	parametersTa.setName("Ta");
	// Intensity
	parametersTaIntensity.setName("Intensity");
	parametersTaIntensity.add(paramTaIntensityMin.set("Ta Intensity Min", 0, 0, 1023));
	parametersTaIntensity.add(paramTaIntensityMax.set("Ta Intensity Max", 0, 0, 1023));
	parametersTa.add(parametersTaIntensity);
	// Midi
	parametersTaMidi.add(paramTaChannel.set("Ta MIDI Channel", 0, 1, 16));
	parametersTaMidi.add(paramTaNote.set("Ta MIDI Note", 0, 0, 127));
	parametersTaMidi.add(paramTaVelocityMin.set("Ta Midi Velocity Min", 0, 0, 127));
	parametersTaMidi.add(paramTaVelocityMax.set("Ta Midi Velocity Max", 0, 0, 127));
	parametersTaMidi.add(paramTaNoteDuration.set("Ta Midi Note Duration", 0, 0, 1000));
	parametersTa.add(parametersTaMidi);
}