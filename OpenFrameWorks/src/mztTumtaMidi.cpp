#include "mztTumtaMidi.h"

void mztTumtaMidi::setup()
{
	midiOut.listPorts();
	midiOut.openPort(0);
	iniGui();
}

void mztTumtaMidi::updateWithData(mztTumtaData * data)
{
	// Tum
	if(data->getTumIntensityLastValue() > paramTumIntensityMin)
	{
		unsigned long long elapsedTime = ofGetElapsedTimeMillis() - lastNoteTriggeredMillis[0];
		
		if(elapsedTime > paramTumNoteDelay) // Delay betwee Note and Note
		{
			int mappedVelocity = ofMap(data->getTumIntensityLastValue(), paramTumIntensityMin, paramTumIntensityMax, paramTumVelocityMin, paramTumVelocityMin.getMax());
			mappedVelocity = ofClamp(mappedVelocity, paramTumVelocityMin, paramTumVelocityMin.getMax());

			sendMidiNoteOn(MIDI_TUMTA_CHANNEL, MIDI_NOTE_TUM, mappedVelocity);

			lastNoteTriggeredMillis[0] = ofGetElapsedTimeMillis();
		}
	}
	// Ta
	if(data->getTaIntensityLastValue() > paramTaIntensityMin)
	{
		unsigned long long elapsedTime = ofGetElapsedTimeMillis() - lastNoteTriggeredMillis[1];
		
		if(elapsedTime > paramTaNoteDelay)  // Delay betwee Note and Note
		{
			int mappedVelocity = ofMap(data->getTaIntensityLastValue(), paramTaIntensityMin, paramTaIntensityMax, paramTaVelocityMin, paramTaVelocityMin.getMax());
			mappedVelocity = ofClamp(mappedVelocity, paramTaVelocityMin, paramTaVelocityMin.getMax());
			
			sendMidiNoteOn(MIDI_TUMTA_CHANNEL, MIDI_NOTE_TA, mappedVelocity);

			lastNoteTriggeredMillis[1] = ofGetElapsedTimeMillis();
		}
	}
}

void mztTumtaMidi::sendMidiNoteOn(int channel, int pitch, int velocity)
{
	// TODO: Cambiar para que sea la duración de la nota?
	// No se si es necesario mandar un note off
	midiOut.sendNoteOn(channel, pitch, velocity);
}

void mztTumtaMidi::sendMidiNoteOff(int channel, int pitch)
{
	midiOut.sendNoteOff(channel, pitch);
}

// GUI
void mztTumtaMidi::iniGui()
{
	parameters.setName("Midi");
	// Tum
	parameters.add(paramTumIntensityMin.set("Tum Sensor Intensity Min", 0, 0, 1023));
	parameters.add(paramTumIntensityMax.set("Tum Sensor Intensity Max", 0, 0, 1023));
	parameters.add(paramTumVelocityMin.set("Tum Midi Velocity Min", 0, 0, 127));
	parameters.add(paramTumNoteDelay.set("Tum Midi Note Delay", 0, 0, 1000));
	// Ta
	parameters.add(paramTaIntensityMin.set("Ta Sensor Intensity Min", 0, 0, 1023));
	parameters.add(paramTaIntensityMax.set("Ta Sensor Intensity Max", 0, 0, 1023));
	parameters.add(paramTaVelocityMin.set("Ta Midi Velocity Min", 0, 0, 127));
	parameters.add(paramTaNoteDelay.set("Ta Midi Note Delay", 0, 0, 1000));
}