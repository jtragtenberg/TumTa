#include "mztTumtaSerial.h"

void mztTumtaSerial::setup()
{

	
	iniSerial(0);
	isPackageReady = false;
	contPackage = 0;
	serialActive = true;
	iniGui();
}

void mztTumtaSerial::update()
{
	checkGuiSerialDeviceName();
}

void mztTumtaSerial::updateSerialValues()
{
	if(paramSerialOn)
	{
		emptyValues();
		int serialRead;
		while((serialRead = serial.readByte()) >= 0)
		{
			checkByteAndUpdateVectors(serialRead);
		}
	}
}

void mztTumtaSerial::checkByteAndUpdateVectors(int value)
{
	setPackage(value);
	
	if(isPackageReady)
	{
		if(debugMessage)
		{
			// TUM
			tum.pressure.push_back(package[0] + (package[1] * 255));
			tum.derivative.push_back(package[4] + (package[5] * 255));
			tum.intensity.push_back(package[8] + (package[9] * 255));
			// TA
			ta.pressure.push_back(package[2] + (package[3] * 255));
			ta.derivative.push_back(package[6] + (package[7] * 255));
			ta.intensity.push_back(package[10] + (package[11] * 255));
		}
		else
		{
			// TUM
			tum.pressure.push_back(0);
			tum.derivative.push_back(0);
			tum.intensity.push_back(package[0] + (package[1] * 255));
			// TA
			ta.pressure.push_back(0);
			ta.derivative.push_back(0);
			ta.intensity.push_back(package[2] + (package[3] * 255));
		}
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
			unsigned char localDebuggingChecksum = 0;
			for(int i = 0; i < 12; i++)
			{
				localDebuggingChecksum += package[i];
			}
			if(packageChecksum == localDebuggingChecksum)
			{
				isPackageReady = true;
			}
			contPackage = 0;
			break;
	}
}

void mztTumtaSerial::emptyValues()
{
	// TUM
	tum.pressure.clear();
	tum.derivative.clear();
	tum.intensity.clear();
	// TA
	ta.pressure.clear();
	ta.derivative.clear();
	ta.intensity.clear();
}

// SERIAL
void mztTumtaSerial::iniSerial(int serialDeviceId = 0)
{
	if(serial.isInitialized())
	{
		serial.close();
	}
	serial.setup(serialDeviceId, SERIAL_BAUDS);
	paramSerialDeviceNameSelected = serialDeviceId;
	serial.flush();
}

void mztTumtaSerial::setGuiSerialDeviceName()
{
	paramSerialDeviceName.empty();
	vector<ofSerialDeviceInfo> serialDeviceList = serial.getDeviceList();
	for(int i = 0; i < serialDeviceList.size(); i++)
	{
		ofParameter<bool> serialDevice;
		serialDevice.set(serialDeviceList[i].getDeviceName(), false);
		paramSerialDeviceName.push_back(serialDevice);
	}
}

void mztTumtaSerial::checkGuiSerialDeviceName()
{
	paramSerialDeviceName[paramSerialDeviceNameSelected] = true;
	for(int i = 0; i < paramSerialDeviceName.size(); i++)
	{
		if(paramSerialDeviceName[i] && paramSerialDeviceNameSelected != i)
		{
			paramSerialDeviceName[paramSerialDeviceNameSelected] = false;
			paramSerialDeviceNameSelected = i;
			paramSerialDeviceName[i] = true;
			iniSerial(paramSerialDeviceNameSelected);
		}
	}
}

// GUI
void mztTumtaSerial::iniGui()
{
	parameters.setName("Serial");
	parameters.add(paramSerialOn.set("Active", true));
	// Serial Port Names
	parameterGroupSerialPort.setName("Serial Ports");
	setGuiSerialDeviceName();
	for(int i = 0; i < paramSerialDeviceName.size(); i++)
	{
		parameterGroupSerialPort.add(paramSerialDeviceName[i]);
		if(i == paramSerialDeviceNameSelected)
		{
			paramSerialDeviceName[paramSerialDeviceNameSelected] = true;
		}
	}
	parameters.add(parameterGroupSerialPort);
}