#include "mztLineI.h"

void mztLineI::setup(string name, ofVec2f newLimits)
{
	limit = newLimits;
	iniGui(name);
}

void mztLineI::update(int newValue)
{
	value = newValue;
}

void mztLineI::draw()
{
	ofPushMatrix();
	ofTranslate(pos->x, pos->y);
	ofSetColor(fillColor);
	int mappedValue = (int) ofMap(value, limit.x, limit.y, 0, size->y);
	ofLine(0, size->y - mappedValue, size->x, size->y - mappedValue);
	ofPopMatrix();
}

// GUI
void mztLineI::iniGui(string name)
{
	parameters.setName(name);
	parameters.add(pos.set("Position", ofVec2f(0, 0), ofVec2f(0, 0), ofVec2f(ofGetWidth(), ofGetHeight())));
	parameters.add(size.set("Size X", ofVec2f(0, 0), ofVec2f(0, 0), ofVec2f(ofGetWidth(), ofGetHeight())));
	parameters.add(fillColor.set("Color", ofColor(0, 0, 0, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
}