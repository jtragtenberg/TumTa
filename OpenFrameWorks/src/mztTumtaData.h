#ifndef ____mztTumtaData__
#define ____mztTumtaData__

#define DATA_N_VALUES 1000

#include <stdio.h>

#include "ofMain.h"
#include "ofxGui.h"

class mztTumtaData
{
public:
    void setup();
	void update(int tumPressure, int tumDerivative, int tumIntensity, int tumInterval, int taPressure, int taDerivative, int taIntensity, int taInterval);
	void updateWithDataGui();
	
	// DATA - SETTERS
	// Tum
	void setTumPressure(int value);
	void setTumDerivative(int value);
	void setTumIntensity(int value);
	void setTumInterval(int value);
	// Ta
	void setTaPressure(int value);
	void setTaDerivative(int value);
	void setTaIntensity(int value);
	void setTaInterval(int value);
	
	// DATA - GETTERS
	// Pointers
	// Tum
	vector<int> * getTumPressurePtr();
	vector<int> * getTumDerivativePtr();
	vector<int> * getTumIntensityPtr();
	vector<int> * getTumIntervalPtr();
	// Ta
	vector<int> * getTaPressurePtr();
	vector<int> * getTaDerivativePtr();
	vector<int> * getTaIntensityPtr();
	vector<int> * getTaIntervalPtr();
	// Current Value
	// Tum
	int getTumPressureLastValue();
	int getTumDerivativeLastValue();
	int getTumIntensityLastValue();
	int getTumIntervalLastValue();
	// Ta
	int getTaPressureLastValue();
	int getTaDerivativeLastValue();
	int getTaIntensityLastValue();
	int getTaIntervalLastValue();
	
    // GUI
    ofParameterGroup parameters;
	ofParameter<int> paramTumPressure, paramTumDerivative, paramTumIntensity, paramTumInterval, paramTaPressure, paramTaDerivative, paramTaIntensity, paramTaInterval;
	
private:
	
	// DATA
	struct Tumta
	{
		vector<int> pressure;
		vector<int> derivative;
		vector<int> intensity;
		vector<int> interval;
	};
	Tumta tum, ta;
	
	void iniData();

	void setValueToVector(int newValue, vector<int> * vec);
	void setValueToVector(float newValue, vector<float> * vec);
	void setValueToVector(bool newValue, vector<bool> * vec);
	void setInterval(vector<int> * intervalVector);
	
    
    // GUI
	void iniGui();
    
};

#endif
