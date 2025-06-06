/*
	Tumta - Package
	[0] Pressure Tum DataLow
	[1] Pressure Tum DataHigh
	[2] Pressure Ta DataLow
	[3] Pressure Ta DataHigh
	[4] Derivative Tum DataLow
	[5] Derivative Tum DataHigh
	[6] Derivative Ta DataLow
	[7] Derivative Ta DataHigh
	[8] Intensity Tum DataLow
	[9] Intensity Tum DataHigh
	[10] Intensity Ta DataLow
	[11] Intensity Ta DataHigh
*/
 
 
#ifndef __OscTest__mztTumtaSerial__
#define __OscTest__mztTumtaSerial__

#include "ofMain.h"

#define SERIAL_BAUDS 38400

class mztTumtaSerial
{
public:
	void setup();
	void update();
	void updateSerialValues();
	void checkByteAndUpdateVectors(int serialRead);
	void close();
	
	// GETTERS
	vector<int> getTumPressure();
	vector<int> getTumDerivative();
	vector<int> getTumIntensity();
	vector<int> getTaPressure();
	vector<int> getTaDerivative();
	vector<int> getTaIntensity();
	
	// GUI
	ofParameterGroup parameters, parameterGroupSerialPort;
	ofParameter<bool> paramSerialOn;
	vector<ofParameter<bool> > paramSerialDeviceName;

private:
	// DATA
	struct Tumta
	{
		vector<int> pressure;
		vector<int> derivative;
		vector<int> intensity;
	};
	Tumta tum, ta; // Vector of serial Values from this loop
	
	bool debugMessage;
	
	void emptyValues();
	
	// Serial
	ofSerial serial;
	void iniSerial(int serialDeviceId);
	void setGuiSerialDeviceName();
	void checkGuiSerialDeviceName();
	int paramSerialDeviceNameSelected;
	
	int package[12], contPackage, packageChecksum;
	bool isPackageReady;
	void setPackage(int newChar);
	bool serialActive;
	
	// Gui
	void iniGui();
};

#endif