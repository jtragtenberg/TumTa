#include "mztGraphicI.h"

void mztGraphicI::setup(string name, ofVec2f newLimits)
{
	limits = newLimits;
	iniGui(name);
}

void mztGraphicI::update(vector<int> * newValue)
{
	value = newValue;
}

void mztGraphicI::draw()
{
	ofPushMatrix();
	ofTranslate(pos->x, pos->y);
	ofSetColor(fillColor);
	ofSetLineWidth(0);
   	ofBeginShape();
    ofVertex(0, size->y);
    for(unsigned x = 0; x < value->size(); x++)
    {
		float mappedValue = ofMap((*value)[x], limits.x, limits.y, 0, size->y);
        ofVertex(x, size->y - mappedValue);
    }
    ofVertex(size->x, size->y);
	ofEndShape();
	ofPopMatrix();
}

// GUI
void mztGraphicI::iniGui(string name)
{
	parameters.setName(name);
	parameters.add(pos.set("Position", ofVec2f(0, 0), ofVec2f(-ofGetWidth(), -ofGetHeight()), ofVec2f(ofGetWidth(), ofGetHeight())));
	parameters.add(size.set("Size", ofVec2f(0, 0), ofVec2f(0, 0), ofVec2f(ofGetWidth(), ofGetHeight())));
	parameters.add(fillColor.set("Color", ofColor(0, 0, 0, 255), ofColor(0, 0, 0, 0), ofColor(255, 255, 255, 255)));
}