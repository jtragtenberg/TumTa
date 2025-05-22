#ifndef __OscTest__mztLineI__
#define __OscTest__mztLineI__

#include <iostream>
#include "ofMain.h"
#include "ofParameterGroup.h"
#include "ofParameter.h"

class mztLineI
{
public:
    void setup(string name, ofVec2f newLimits);
    void update(int newValue);
    void draw();
	
	// GUI
	ofParameterGroup parameters;
	
private:
	int value;
	ofVec2f limit;
	
	// GUI
	void iniGui(string name);
	ofParameter<ofVec2f> pos, size;
	ofParameter<ofColor> fillColor;
};

#endif