#ifndef __OscTest__mztGraphicI__
#define __OscTest__mztGraphicI__

#include <iostream>
#include "ofMain.h"
#include "ofParameterGroup.h"
#include "ofParameter.h"

class mztGraphicI
{
public:
    void setup(string name, ofVec2f newLimits);
    void update(vector<int> * newValue);
    void draw();
	
	// GUI
	ofParameterGroup parameters;
	
private:
	vector<int> * value;
	ofVec2f limits;
	
	// GUI
	void iniGui(string name);
	ofParameter<ofVec2f> pos, size;
	ofParameter<ofColor> fillColor;
};

#endif
