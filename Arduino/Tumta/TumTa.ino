// Tum
#define TUM_PIN A0
#define TUM_CHANNEL 0
// Ta
#define TA_PIN A1
#define TA_CHANNEL 1

//Mode Switch
#define MODE_SWITCH_PIN 12

//Threshold Pot
#define THRESHOLD_PIN A7

//Standby Switch
#define STANDBY_SWITCH_PIN 2

//Led
#define LED_PIN 6


bool isDebugging = false;
byte debugMessage[15];
byte liveMessage[7];

int threshold = 20;
int debounceTime = 150;

void setup()
{
  Serial.begin(38400);
  pinMode(TUM_PIN, INPUT);
  pinMode(TA_PIN, INPUT);
  pinMode(MODE_SWITCH_PIN, INPUT);
  pinMode(THRESHOLD_PIN, INPUT);
  pinMode(STANDBY_SWITCH_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  analogReference(EXTERNAL);

  digitalWrite(LED_PIN, HIGH);
}

boolean standby = false;

void loop()
{
  standby = !digitalRead(STANDBY_SWITCH_PIN);
  if (standby) {
    digitalWrite(LED_PIN, LOW);
  }
  else {
    digitalWrite(LED_PIN, HIGH);
    //isDebugging = digitalRead(MODE_SWITCH_PIN);


    // Tum
    int tumPressure = getAvarage(TUM_CHANNEL, analogRead(TUM_PIN));
    int tumDerivative = getDerivative(TUM_CHANNEL, tumPressure);
    int tumIntensity = getIntensity(TUM_CHANNEL, tumDerivative);
    // Ta
    int taPressure = getAvarage(TA_CHANNEL, analogRead(TA_PIN));
    int taDerivative = getDerivative(TA_CHANNEL, taPressure);
    int taIntensity = getIntensity(TA_CHANNEL, taDerivative);
    // Message
    if(isDebugging)
    {
      setDebugMessage(tumPressure, taPressure, tumDerivative, taDerivative, tumIntensity, taIntensity); 
      Serial.write(debugMessage, sizeof(debugMessage));
    }
    else
    {
      setLiveMessage(tumIntensity, taIntensity); 
      Serial.write(liveMessage, sizeof(liveMessage));
    }
    delay(5);
  }
}

// MESSAGE
void setDebugMessage(int tumPressure, int taPressure, int tumDerivative, int taDerivative, int tumIntensity, int taIntensity)
{
  // Start Header
  debugMessage[0] = 0xFF;
  debugMessage[1] = 0xFF;
  // Pressure Tum
  debugMessage[2] = lowByte(tumPressure);
  debugMessage[3] = highByte(tumPressure);
  // Pressure Ta
  debugMessage[4] = lowByte(taPressure);
  debugMessage[5] = highByte(taPressure);
  // Derivative Tum
  debugMessage[6] = lowByte(tumDerivative);
  debugMessage[7] = highByte(tumDerivative);
  // Derivative Ta
  debugMessage[8] = lowByte(taDerivative);
  debugMessage[9] = highByte(taDerivative);
  // Derivative Tum
  debugMessage[10] = lowByte(tumIntensity);
  debugMessage[11] = highByte(tumIntensity);
  // Derivative Ta
  debugMessage[12] = lowByte(taIntensity);
  debugMessage[13] = highByte(taIntensity);
  // Checksum
  debugMessage[14] = 0;
  for(int i = 2; i < 14; i++)
  {
    debugMessage[14] += debugMessage[i];
  }
}

void setLiveMessage(int tumIntensity, int taIntensity)
{
  // Start Header
  liveMessage[0] = 0xFE;
  liveMessage[1] = 0xFE;
  // Intensity Tum
  liveMessage[2] = lowByte(tumIntensity);
  liveMessage[3] = highByte(tumIntensity);
  // Intensity Ta
  liveMessage[4] = lowByte(taIntensity);
  liveMessage[5] = highByte(taIntensity);
  // Checksum
  liveMessage[6] = 0;
  for(int i = 2; i < 6; i++)
  {
    liveMessage[6] += liveMessage[i];
  }
}

// Avarage

#define numReadings 5
int values[2][numReadings];
int total[2];
int index = 0;
int avrg = 0;

int getAvarage(int channel, int value)
{
  total[channel] = total[channel] - values[channel][index];
  values[channel][index] = value;

  total[channel] = total[channel] + values[channel][index];
  index = index + 1;

  if (index >= numReadings)
  {
    index = 0;
  }
  avrg = (int) total[channel] / numReadings;
  return avrg;
}

// Derivative

int valuesForDerivative[2][5];
float derivativeValues[10] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

int getDerivative(int channel, int value)
{
  // Derivada
  valuesForDerivative[channel][4] = valuesForDerivative[channel][3];
  valuesForDerivative[channel][3] = valuesForDerivative[channel][2];
  valuesForDerivative[channel][2] = valuesForDerivative[channel][1];
  valuesForDerivative[channel][1] = valuesForDerivative[channel][0];
  valuesForDerivative[channel][0] = value;

  derivativeValues[0] = 2.08 * valuesForDerivative[channel][0] - 4 * valuesForDerivative[channel][1] + 3 * valuesForDerivative[channel][2] - 1.33 * valuesForDerivative[channel][3] + 0.25 * valuesForDerivative[channel][4];
  //faz o calculo da derivada com coeficientes de diferenca finita (http://en.wikipedia.org/wiki/Finite_difference_coefficients)
  return derivativeValues[0];
}

// Intensity

bool isAboveThreshold = false;
int intensity = 0;
int antDerivative;
int derivativeMaxValue;

int getIntensity(int channel, int newDerivative)
{
  intensity = 0;
  derivativeMaxValue = (derivativeMaxValue < newDerivative) ? newDerivative : derivativeMaxValue;
  if (antDerivative > newDerivative && newDerivative > threshold)
  {
    intensity = derivativeMaxValue;
    derivativeMaxValue = 0;
  }
  antDerivative = newDerivative;

  return intensity;
}


