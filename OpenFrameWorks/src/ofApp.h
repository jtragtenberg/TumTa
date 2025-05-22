#pragma once

#include "ofMain.h"
#include "mztGraphicI.h"
#include "mztLineI.h"
#include "mztTumtaData.h"
#include "mztTumtaSerial.h"
#include "mztTumtaMidi.h"

class ofApp : public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);

    // ---
	void iniSetup();
	
	// DATA
	mztTumtaData data;
	void iniData();
	void updateData();
	
	// SERIAL
	mztTumtaSerial serial;
	void iniSerial();
	void updateSerial();
	
	// MIDI
	mztTumtaMidi midi;
	void iniMidi();
	void updateMidi();
	
	// GRAPHIC
	mztGraphicI graphicI[8];
	mztLineI lineI[8];
	void iniGraphic();
	void updateGraphic();
	void drawGraphic();

	// GUI
	bool showGui;
	ofxPanel gui;
	ofParameterGroup
		parameters,
		parametersLive,
		parametersSerial,
		parametersDebug;
	ofParameter<bool> isDebugWithoutSensorsEnabled;
	void iniGui();
};
