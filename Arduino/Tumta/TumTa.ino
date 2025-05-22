byte message[6];
boolean isRawData = false;

int threshold; //vai ser definido por um potenciometro
int debounceTime = 150;

long previousMillis = 0;
long interval = 5;

void setup() 
{
  Serial.begin(38400);
  pinMode(A0, INPUT);
  pinMode(A7, INPUT);
  analogReference(EXTERNAL);
}

void loop() 
{
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > interval)
  {
    previousMillis = currentMillis;   

    int channel;
    threshold = analogRead(A7)/4;
    // Read ANALOG A0
    if(!isRawData)
    {
      int newValue = sendAvarageValue(0); //channel 0: average pressure
      int newDerivative = sendDerivativeValue(1, 0, newValue); //channel 1: derivative
      sendIntensityValue(2, newDerivative); //channel 2: intensity triggers
    }
    else
    {
      setMessage(0, analogRead(A0));
    }
  }

}

void setMessage(int channel, int value)
{
  // Start Header
  message[0] = 0xFF;
  message[1] = 0xFF;
  // Channel
  message[2] = byte(channel);
  // Value
  message[3] = lowByte(value);
  message[4] = highByte(value);
  // Checksum
  message[5] = (message[2] + message[3] + message[4]);
}

// Avarage

#define numReadings 3
int values[1][numReadings];
int total[1];
int index = 0;
int avrg = 0;

int avarage(int channel,int value)
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

int sendAvarageValue(int channel)
{
  int valueAvrg = avarage(channel, analogRead(A0));
  setMessage(0, valueAvrg);
  Serial.write(message, 6);
  return valueAvrg;
}

// Derivative

int valuesForDerivative[1][5];
float derivativeValues[10] = {
  0,0,0,0,0,0,0,0,0,0};

int derivative(int channel, int value)
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

int sendDerivativeValue(int channel, int fromChannel, int newValue)
{
  int derivativeValue = derivative(fromChannel, newValue);
  setMessage(channel, derivativeValue + 512);
  Serial.write(message, 6);
  return derivativeValue;
}


// Intensity

int intensity = 0;

int debouncing;
long lastDebounceTime;
int aindaNao;
int maximo;

void sendIntensityValue(int channel, int newDerivative)
{

  if (!debouncing){                    //se nao estiver no meio de uma outro pisada
    if(newDerivative > threshold){       //define a pisada quando a derivada passa de um threshold
      lastDebounceTime = millis();     //guarda na memoria o instante que aconteceu a pisada no Tum
      debouncing = true;                                      //diz que ainda esta debouncing      
    }
  }

  //Agoritmo para controlar o estado de debounce
  if ((millis() - lastDebounceTime) > debounceTime) {       //Controla quando esta debouncing. Se ja passou o tempo de debounce
    debouncing = false;                                     //apos a ultima pisada diz que ja saiu do debounce
    intensity = 0;                             //avisa que parou o tempo de debounce do Ta
    aindaNao = true;                             //reinicia o aindaNao para permitir que a proxima pisada venha e que nao role outro noteOff...
    maximo = 0;                                  //zera o valor do maximo para a proxima poder chegar...    
  } 


  //Algoritmo para deteccao da pisada
  if (debouncing) {                               //enquanto estiver no tempo do debounce
    if (aindaNao){                                //se ainda nao tiver descoberto o maximo da derivada (a intensidade da pisada)
      if (newDerivative > maximo){               //se o grafico da derivada esta crescendo
        maximo = newDerivative;                  //continue colocando no valor da derivada na variavel maximo
      }
      else {                                         //se o grafico da derivada parar de crescer
        intensity = maximo - threshold;                  //pega o ultimo valor do maximo e guarda o valor na variavel intensidade
        aindaNao = false;                         //a pisada ja rolou
      }
    }
  }

  setMessage(channel, intensity);
  Serial.write(message, 6);
}

