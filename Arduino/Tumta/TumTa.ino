#include "TimerOne.h"
#include "TimerThree.h"
#include "MIDIUSB.h"

// Tum
#define TUM_PIN A0
#define TUM_CHANNEL 0

//Threshold Pot
#define THRESHOLD_PIN A5

const int LED_PIN = LED_BUILTIN;  // the pin with a LED

int tumPressure = 0;
int tumIntensity = 0;

int threshold = 100;
int debounceTime = 30;//80;
int midiNoteTum = 60;
int midiChannelTum = 0;

//mapeamento
int intensityMax = 700;
int midiVelocityMin = 20;
int midiVelocityMax = 127;
unsigned long noteDuration = 80;


void setup() {
  Serial.begin(57600);
  pinMode(LED_PIN, OUTPUT);
  pinMode(TUM_PIN, INPUT);
  pinMode(THRESHOLD_PIN, INPUT);


  Timer1.initialize(300); //500us
  Timer1.attachInterrupt(readTum);

  Timer3.initialize(5000); //5ms
  Timer3.attachInterrupt(sendMessage);
}

void loop() {

}

int c = 0; //zera contador
bool noteOnFlag = false;
unsigned long noteTime;


void readTum() {
  //leitura mais lenta do potenciometro que define o threshold
  c++;
  if (c > 1000) { //a cada 500 leituras do sensor ele lê uma vez o pot do threshold
    threshold = analogRead(THRESHOLD_PIN);
    c = 0;
  }

  //leitura do sensor do Tum
  tumPressure = analogRead(TUM_PIN);

  //algoritmo de detecção de pisada
  tumIntensity = getIntensity(TUM_CHANNEL, tumPressure);


  if (tumIntensity > 0 && !noteOnFlag) {
    noteTime = millis();
    //Serial.println("Sending note on");
    int midiVelocityTum = truncamento(mapeamento(tumIntensity, threshold, intensityMax, midiVelocityMin, midiVelocityMax), midiVelocityMin, midiVelocityMax);
    noteOn(midiChannelTum, midiNoteTum, midiVelocityTum);   // Channel 0, middle C, normal velocity
    MidiUSB.flush();
    noteOnFlag = true;
  }
  //if ((millis() - noteTime > noteDuration) && noteOnFlag) {
  if (tumIntensity <= 0 && noteOnFlag) {
    //Serial.println("Sending note off");
    noteOff(midiChannelTum, midiNoteTum, 0);  // Channel 0, middle C, normal velocity
    MidiUSB.flush();
    noteOnFlag = false;
  }

}

void sendMessage() {
  Serial.print(1023);
  Serial.print(" ");
  Serial.print(threshold);
  Serial.print(" ");
  Serial.print(tumPressure);
  Serial.print(" ");
  Serial.println(tumIntensity);
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



// Intensity


int intensity[2] = {0, 0};
int debouncing[2];
long lastDebounceTime[2];
int aindaNao[2];
int maximo[2];

int getIntensity(int channel, int newDerivative)
{


  if (newDerivative > threshold) {     //define a pisada quando a derivada passa de um threshold
    lastDebounceTime[channel] = millis();     //guarda na memoria o instante que aconteceu a pisada no Tum
    debouncing[channel] = true;                                      //diz que ainda esta debouncing
  }

  //Agoritmo para controlar o estado de debounce
  if ((millis() - lastDebounceTime[channel]) > debounceTime) {       //Controla quando esta debouncing. Se ja passou o tempo de debounce
    debouncing[channel] = false;                                     //apos a ultima pisada diz que ja saiu do debounce
    intensity[channel] = 0;                             //avisa que parou o tempo de debounce do Ta
    aindaNao[channel] = true;                             //reinicia o aindaNao para permitir que a proxima pisada venha e que nao role outro noteOff...
    maximo[channel] = 0;                                  //zera o valor do maximo para a proxima poder chegar...
  }


  //Algoritmo para deteccao da pisada
  if (debouncing[channel]) {                               //enquanto estiver no tempo do debounce
    if (aindaNao[channel]) {                               //se ainda nao tiver descoberto o maximo da derivada (a intensidade da pisada)
      if (newDerivative > maximo[channel]) {              //se o grafico da derivada esta crescendo
        maximo[channel] = newDerivative;                  //continue colocando no valor da derivada na variavel maximo
      }
      else {                                         //se o grafico da derivada parar de crescer
        intensity[channel] = maximo[channel];                   //pega o ultimo valor do maximo e guarda o valor na variavel intensidade
        aindaNao[channel] = false;                         //a pisada ja rolou
      }
    }
    
  }
  return intensity[channel];
}


void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

int mapeamento(float inputVal, float inputMin, float inputMax, float outputMin, float outputMax){ 
  float m = (outputMax - outputMin)/(inputMax - inputMin);
  return round(outputMin + m*(inputVal - inputMin)); //equacao da reta a partir de dois pontos
}

int truncamento(int inputVal, int minVal, int maxVal){
  if (inputVal > maxVal) return maxVal;
  else if (inputVal < minVal) return minVal;  
  else return inputVal;
}

