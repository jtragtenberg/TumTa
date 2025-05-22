#include "mztTumtaData.h"

void mztTumtaData::setup()
{
	iniData();
    iniGui();
}

void mztTumtaData::update(int tumPressure, int tumDerivative, int tumIntensity, int tumInterval, int taPressure, int taDerivative, int taIntensity, int taInterval)
{
	// Tum
	setValueToVector(tumPressure, &tum.pressure);
	setValueToVector(tumDerivative, &tum.derivative);
	setValueToVector(tumIntensity, &tum.intensity);
	setValueToVector(tumInterval, &tum.interval);
	// Ta
	setValueToVector(taPressure, &ta.pressure);
	setValueToVector(taDerivative, &ta.derivative);
	setValueToVector(taIntensity, &ta.intensity);
	setValueToVector(taInterval, &ta.interval);
}

void mztTumtaData::updateWithDataGui()
{
	update(paramTumPressure, paramTumDerivative, paramTumIntensity, paramTumInterval, paramTaPressure, paramTaDerivative, paramTaIntensity, paramTaInterval);
}


// DATA
void mztTumtaData::iniData()
{
	for(int i = 0; i < DATA_N_VALUES; i++)
	{
		// Tum
		tum.pressure.push_back(0);
		tum.derivative.push_back(0);
		tum.intensity.push_back(0);
		tum.interval.push_back(0);
		// Ta
		ta.pressure.push_back(0);
		ta.derivative.push_back(0);
		ta.intensity.push_back(0);
		ta.interval.push_back(0);
	}
}

// SETTERS
// Tum
void mztTumtaData::setTumPressure(int value)
{
	setValueToVector(value, &tum.pressure);
}

void mztTumtaData::setTumDerivative(int value)
{
	setValueToVector(value, &tum.derivative);
}

void mztTumtaData::setTumIntensity(int value)
{
	setValueToVector(value, &tum.intensity);
}

void mztTumtaData::setTumInterval(int value)
{
	setValueToVector(value, &tum.interval);
}

// Ta
void mztTumtaData::setTaPressure(int value)
{
	setValueToVector(value, &ta.pressure);
}

void mztTumtaData::setTaDerivative(int value)
{
	setValueToVector(value, &ta.derivative);
}

void mztTumtaData::setTaIntensity(int value)
{
	setValueToVector(value, &ta.intensity);
}

void mztTumtaData::setTaInterval(int value)
{
	setValueToVector(value, &ta.interval);
}

void mztTumtaData::setValueToVector(int newValue, vector<int> * vec)
{
	vec->push_back(newValue);
	
	if(vec->size() > DATA_N_VALUES)
	{
		vec->erase(vec->begin());
	}
}

void mztTumtaData::setValueToVector(float newValue, vector<float> * vec)
{
	vec->push_back(newValue);
	
	if(vec->size() > DATA_N_VALUES)
	{
		vec->erase(vec->begin());
	}
}

void mztTumtaData::setValueToVector(bool newValue, vector<bool> * vec)
{
	vec->push_back(newValue);
	
	if(vec->size() > DATA_N_VALUES)
	{
		vec->erase(vec->begin());
	}
}

// DATA - GETTERS
// Pointers
// Tum
vector<int> * mztTumtaData::getTumPressurePtr()
{
	return &tum.pressure;
}

vector<int> * mztTumtaData::getTumDerivativePtr()
{
	return &tum.derivative;
}

vector<int> * mztTumtaData::getTumIntensityPtr()
{
	return &tum.intensity;
}

vector<int> * mztTumtaData::getTumIntervalPtr()
{
	return &tum.interval;
}

// Ta
vector<int> * mztTumtaData::getTaPressurePtr()
{
	return &ta.pressure;
}

vector<int> * mztTumtaData::getTaDerivativePtr()
{
	return &ta.derivative;
}

vector<int> * mztTumtaData::getTaIntensityPtr()
{
	return &ta.intensity;
}

vector<int> * mztTumtaData::getTaIntervalPtr()
{
	return &ta.interval;
}

// Current Value
// Tum
int mztTumtaData::getTumPressureLastValue()
{
	return tum.pressure[DATA_N_VALUES-1];
}

int mztTumtaData::getTumDerivativeLastValue()
{
	return tum.derivative[DATA_N_VALUES-1];
}

int mztTumtaData::getTumIntensityLastValue()
{
	return tum.intensity[DATA_N_VALUES-1];
}
// Ta
int mztTumtaData::getTaPressureLastValue()
{
	return ta.pressure[DATA_N_VALUES-1];
}

int mztTumtaData::getTaDerivativeLastValue()
{
	return ta.derivative[DATA_N_VALUES-1];
}

int mztTumtaData::getTaIntensityLastValue()
{
	return ta.intensity[DATA_N_VALUES-1];
}

// GUI

void mztTumtaData::iniGui()
{
    parameters.setName("Data");
	// Tum
	parameters.add(paramTumPressure.set("Tum Pressure", 0, 0, 1023));
  	parameters.add(paramTumDerivative.set("Tum Derivative", 0, 0, 1023));
   	parameters.add(paramTumIntensity.set("Tum Intensity", 0, 0, 1023));
   	parameters.add(paramTumInterval.set("Tum Interval", 0, 0, 1023));
	// Ta
	parameters.add(paramTaPressure.set("Ta Pressure", 0, 0, 1023));
	parameters.add(paramTaDerivative.set("Ta Derivative", 0, 0, 1023));
	parameters.add(paramTaIntensity.set("Ta Intensity", 0, 0, 1023));
	parameters.add(paramTaInterval.set("Ta Interval", 0, 0, 1023));
}