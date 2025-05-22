#include "mztTumtaSerial.h"

void mztTumtaSerial::setup()
{
	serial.setup(0, SERIAL_BAUDS);
	serial.flush();
	isPackageReady = false;
	contPackage = 0;
}

void mztTumtaSerial::updateSerialValues()
{
	emptyValues();
	int serialRead;
	while((serialRead = serial.readByte()) >= 0)
	{
		checkByteAndUpdateVectors(serialRead);
	}
}

void mztTumtaSerial::checkByteAndUpdateVectors(int value)
{
	setPackage(value);
	
	if(isPackageReady && debugMessage)
	{
		// TUM
		tum.pressure.push_back(package[0] + (package[1] * 255));
		tum.derivative.push_back(package[4] + (package[5] * 255));
		tum.intensity.push_back(package[8] + (package[9] * 255));
//		setInterval(&tum.interval);
		// TA
		ta.pressure.push_back(package[2] + (package[3] * 255));
		ta.derivative.push_back(package[6] + (package[7] * 255));
		ta.intensity.push_back(package[10] + (package[11] * 255));
//		setInterval(&ta.interval);
	}
	else
	{
//		// TUM
//		tum.pressure.push_back(0);
//		tum.derivative.push_back(0);
//		tum.intensity.push_back(package[0] + (package[1] * 255));
//		setInterval(&tum.interval);
//		// TA
//		ta.pressure.push_back(0);
//		ta.derivative.push_back(0);
//		ta.intensity.push_back(package[2] + (package[3] * 255));
//		setInterval(&ta.interval);
	}
}

void mztTumtaSerial::close()
{
	serial.close();
}

// GETTERS
vector<int> mztTumtaSerial::getTumPressure()
{
	return tum.pressure;
}

vector<int> mztTumtaSerial::getTumDerivative()
{
	return tum.derivative;
}

vector<int> mztTumtaSerial::getTumIntensity()
{
	return tum.intensity;
}

//vector<int> mztTumtaSerial::getTumInterval()
//{
//	return tum.interval;
//}

vector<int> mztTumtaSerial::getTaPressure()
{
	return ta.pressure;
}

vector<int> mztTumtaSerial::getTaDerivative()
{
	return ta.derivative;
}

vector<int> mztTumtaSerial::getTaIntensity()
{
	return ta.intensity;
}

//vector<int> mztTumtaSerial::getTaInterval()
//{
//	return ta.interval;
//}
//
//void mztTumtaSerial::setInterval(vector<int> * intervalVector)
//{
//	int newValue;
//	switch (intervalVector->size())
//	{
//		case 0:
//			newValue = ofGetElapsedTimeMillis();
//			break;
//		case 1:
//			newValue = (*intervalVector)[0] - ofGetElapsedTimeMillis();
//			break;
//		default:
//			newValue = (*intervalVector)[intervalVector->size() - 1] - (*intervalVector)[intervalVector->size() - 2];
//			break;
//	}
//	
//	intervalVector->push_back(newValue);
//}

void mztTumtaSerial::setPackage(int newChar)
{
	switch (contPackage)
	{
		case 0:
			isPackageReady = false;
			if(newChar == 255)
			{
				contPackage++;
				debugMessage = true;
			}
			else if(newChar == 254)
			{
				contPackage++;
				debugMessage = false;
			}
			else
			{
				contPackage = 0;
			}
			break;
		case 1:
			if(newChar == 255 && debugMessage)
			{
				contPackage++;
				debugMessage = true;
			}
			else if(newChar == 254 && !debugMessage)
			{
				contPackage++;
				debugMessage = false;
			}
			else
			{
				contPackage = 0;
				debugMessage = false;
			}
			break;
		case 2:
			package[0] = newChar;  // [0] Pressure Tum DataLow or [0] Intensity Tum DataLow
			contPackage++;
			break;
		case 3:
			package[1] = newChar; // [1] Pressure Tum DataHigh or [1] Intensity Tum DataHigh
			contPackage++;
			break;
		case 4:
			package[2] = newChar; // [2] Pressure Ta DataLow or [2] Intensity Ta DataLow
			contPackage++;
			break;
		case 5:
			package[3] = newChar; // [3] Pressure Ta DataHigh or [3] Intensity Ta DataLow
			contPackage++;
			break;
		case 6:
			if(!debugMessage)
			{
				packageChecksum = newChar; // Live Message Checksum
				unsigned char localChecksum = 0;
				for(int i = 0; i < 4; i++)
				{
					localChecksum += package[i];
				}
				if(packageChecksum == localChecksum)
				{
					isPackageReady = true;
				}
				contPackage = 0;
			}
			else
			{
				package[4] = newChar; // [4] Derivative Tum DataLow
				contPackage++;
			}
			break;
		case 7:
			package[5] = newChar; // [5] Derivative Tum DataHigh
			contPackage++;
			break;
		case 8:
			package[6] = newChar; // [6] Derivative Ta DataLow
			contPackage++;
			break;
		case 9:
			package[7] = newChar; // [7] Derivative Ta DataHigh
			contPackage++;
			break;
		case 10:
			package[8] = newChar; // [8] Intensity Tum DataLow
			contPackage++;
			break;
		case 11:
			package[9] = newChar; // [9] Intensity Tum DataHigh
			contPackage++;
			break;
		case 12:
			package[10] = newChar; // [10] Intensity Ta DataLow
			contPackage++;
			break;
		case 13:
			package[11] = newChar; // [11] Intensity Ta DataHigh
			contPackage++;
			break;
		case 14:
			packageChecksum = newChar; // Debug Message Checksum
			unsigned char localChecksum = 0;
			for(int i = 0; i < 12; i++)
			{
				localChecksum += package[i];
			}
			if(packageChecksum == localChecksum)
			{
				isPackageReady = true;
			}
			contPackage = 0;
			break;
	}
}

void mztTumtaSerial::emptyValues()
{
	tum.pressure.clear();
	tum.derivative.clear();
	tum.intensity.clear();
//	tum.interval.clear();
	
	ta.pressure.clear();
	ta.derivative.clear();
	ta.intensity.clear();
//	ta.interval.clear();
}