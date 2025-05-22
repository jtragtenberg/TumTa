#include "ofApp.h"

void ofApp::setup()
{
    iniSetup();
	iniData();
	iniSerial();
	iniMidi();
	iniGraphic();
	iniGui();
}

void ofApp::update()
{
	updateSerial();
	updateData();
	updateMidi();
	updateGraphic();
}

void ofApp::draw()
{
	ofBackground(255);
	drawGraphic();
	ofDrawBitmapStringHighlight(ofToString(ofGetFrameRate()), 0, 10);
	if(showGui)
	{
		gui.draw();
		ofShowCursor();
	}
	else
	{
		ofHideCursor();
	}
}

void ofApp::keyPressed(int key)
{
	switch(key)
	{
		// GUI
		case OF_KEY_TAB:
			showGui = !showGui;
			break;
		case 's':
			gui.saveToFile("settings.xml");
			break;
		case 'l':
			gui.loadFromFile("settings.xml");
			break;
	}
}

// ---
void ofApp::iniSetup()
{
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
}

// DATA
void ofApp::iniData()
{
	data.setup();
}

void ofApp::updateData()
{
	if(isDebugWithoutSensorsEnabled)
	{
		data.updateWithDataGui();
	}
	else
	{
		for(int i = 0; i < serial.getTumPressure().size(); i++)
		{
			data.update(serial.getTumPressure()[i], serial.getTumDerivative()[i], serial.getTumIntensity()[i], serial.getTaPressure()[i], serial.getTaDerivative()[i], serial.getTaIntensity()[i]);
		}
	}
}

//SERIAL
void ofApp::iniSerial()
{
	serial.setup();
}

void ofApp::updateSerial()
{
	serial.update();
	serial.updateSerialValues();
}

// MIDI
void ofApp::iniMidi()
{
	midi.setup();
}

void ofApp::updateMidi()
{
	midi.updateWithData(&data);
}

// GRAPHIC
void ofApp::iniGraphic()
{
	// Tum
	graphicI[0].setup("Tum Pressure", ofVec2f(data.paramTumPressure.getMin(), data.paramTumPressure.getMax()));
	graphicI[1].setup("Tum Derivative", ofVec2f(data.paramTumDerivative.getMin(), data.paramTumDerivative.getMax()));
	graphicI[2].setup("Tum Intensity", ofVec2f(data.paramTumIntensity.getMin(), data.paramTumIntensity.getMax()));
	graphicI[3].setup("Tum Interval", ofVec2f(data.paramTumInterval.getMin(), data.paramTumInterval.getMax()));
	// Ta
	graphicI[4].setup("Ta Pressure", ofVec2f(data.paramTaPressure.getMin(), data.paramTaPressure.getMax()));
	graphicI[5].setup("Ta Derivative", ofVec2f(data.paramTaDerivative.getMin(), data.paramTaDerivative.getMax()));
	graphicI[6].setup("Ta Intensity", ofVec2f(data.paramTaIntensity.getMin(), data.paramTaIntensity.getMax()));
	graphicI[7].setup("Ta Interval", ofVec2f(data.paramTaInterval.getMin(), data.paramTaInterval.getMax()));
	// MIDI Tum
	lineI[0].setup("Tum Sensor Intensity Min", ofVec2f(midi.paramTumIntensityMin.getMin(), midi.paramTumIntensityMin.getMax()));
	lineI[1].setup("Tum Sensor Intensity Max", ofVec2f(midi.paramTumIntensityMax.getMin(), midi.paramTumIntensityMax.getMax()));
	lineI[2].setup("Tum Midi Velocity Min", ofVec2f(midi.paramTumVelocityMin.getMin(), midi.paramTumVelocityMin.getMax()));
	lineI[3].setup("Tum Midi Velocity Max", ofVec2f(midi.paramTumVelocityMax.getMin(), midi.paramTumVelocityMin.getMax()));
	// MIDI Ta
	lineI[4].setup("Ta Sensor Intensity Min", ofVec2f(midi.paramTaIntensityMin.getMin(), midi.paramTaIntensityMin.getMax()));
	lineI[5].setup("Ta Sensor Intensity Max", ofVec2f(midi.paramTaIntensityMax.getMin(), midi.paramTaIntensityMax.getMax()));
	lineI[6].setup("Ta Midi Velocity Min", ofVec2f(midi.paramTaVelocityMin.getMin(), midi.paramTaVelocityMin.getMax()));
	lineI[7].setup("Ta Midi Velocity Max", ofVec2f(midi.paramTaVelocityMax.getMin(), midi.paramTaVelocityMin.getMax()));
	
}

void ofApp::updateGraphic()
{
	// Tum
	graphicI[0].update(data.getTumPressurePtr());
	graphicI[1].update(data.getTumDerivativePtr());
	graphicI[2].update(data.getTumIntensityPtr());
	graphicI[3].update(data.getTumIntervalPtr());
	// Ta
	graphicI[4].update(data.getTaPressurePtr());
	graphicI[5].update(data.getTaDerivativePtr());
	graphicI[6].update(data.getTaIntensityPtr());
	graphicI[7].update(data.getTaIntervalPtr());
	// MIDI Tum
	lineI[0].update(midi.paramTumIntensityMin);
	lineI[1].update(midi.paramTumIntensityMax);
	lineI[2].update(midi.paramTumVelocityMin);
	lineI[3].update(midi.paramTumVelocityMax);
	// MIDI Ta
	lineI[4].update(midi.paramTaIntensityMin);
	lineI[5].update(midi.paramTaIntensityMax);
	lineI[6].update(midi.paramTaVelocityMin);
	lineI[7].update(midi.paramTaVelocityMax);
}

void ofApp::drawGraphic()
{
	// Tum
	graphicI[0].draw();
	graphicI[1].draw();
	graphicI[2].draw();
	graphicI[3].draw();
	// Ta
	graphicI[4].draw();
	graphicI[5].draw();
	graphicI[6].draw();
	graphicI[7].draw();
	// MIDI Tum
	lineI[0].draw();
	lineI[1].draw();
	lineI[2].draw();
	lineI[3].draw();
	// MIDI Ta
	lineI[4].draw();
	lineI[5].draw();
	lineI[6].draw();
	lineI[7].draw();
}

// GUI
void ofApp::iniGui()
{
	showGui = false;
	
	// LIVE
	parametersLive.setName("Calibracao");
	parametersLive.add(midi.parametersTum);
	parametersLive.add(midi.parametersTa);

	// SERIAL
	parametersSerial.setName("Serial");
	parametersSerial.add(serial.parameters);
	
	// DEBUG
	parametersDebug.setName("Debug");
	parametersDebug.add(isDebugWithoutSensorsEnabled.set("Debug (no Sensors)", true));
	parametersDebug.add(data.parameters);
	parametersDebug.add(midi.parametersTum);
	parametersDebug.add(midi.parametersTa);
	// Tum
	parametersDebug.add(graphicI[0].parameters);
	parametersDebug.add(graphicI[1].parameters);
	parametersDebug.add(graphicI[2].parameters);
	parametersDebug.add(graphicI[3].parameters);
	// Ta
	parametersDebug.add(graphicI[4].parameters);
	parametersDebug.add(graphicI[5].parameters);
	parametersDebug.add(graphicI[6].parameters);
	parametersDebug.add(graphicI[7].parameters);
	// Midi Tum
	parametersDebug.add(lineI[0].parameters);
	parametersDebug.add(lineI[1].parameters);
	parametersDebug.add(lineI[2].parameters);
	parametersDebug.add(lineI[3].parameters);
	// Midi Ta
	parametersDebug.add(lineI[4].parameters);
	parametersDebug.add(lineI[5].parameters);
	parametersDebug.add(lineI[6].parameters);
	parametersDebug.add(lineI[7].parameters);

	parameters.setName("TumTa");
	parameters.add(parametersLive);
	parameters.add(parametersSerial);
	parameters.add(parametersDebug);
	
	gui.setup(parameters);
	gui.loadFromFile("settings.xml");
	gui.minimizeAll();
}