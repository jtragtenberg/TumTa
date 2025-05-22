// ---- Configurações de pinos ----
#define TUM_PIN A0
#define TUM_CHANNEL 0
#define TA_PIN A1
#define TA_CHANNEL 1
#define MODE_SWITCH_PIN 2
#define THRESHOLD_PIN A7
#define STANDBY_SWITCH_PIN 12
#define LED_PIN 6

// ---- Parâmetros gerais ----
const int numReadings = 5;
int threshold = 20;

// ---- Estrutura de dados para cada canal ----
typedef struct {
  int debounceTime = 150;
  int derivative = 0;
  int intensity = 0;
  int maxDerivative = 0;
  bool debouncing = false;
  unsigned long lastDebounceTime = 0;
  bool waitingForPeak = true;
  int derivativeWindow[5] = {0};
} ChannelState;

ChannelState channels[2];

// ---- Mensagens ----
byte debugMessage[15];
byte liveMessage[7];

// ---- Variáveis globais ----
bool isDebugging = false;
bool standby = false;
unsigned long tempoStandby = 0;
const int tempoBlink = 500;
bool blinkFlag = false;

// ---- Funções auxiliares ----
float getDerivative(ChannelState *ch, int value) {
  for (int i = 4; i > 0; i--) {
    ch->derivativeWindow[i] = ch->derivativeWindow[i - 1];
  }
  ch->derivativeWindow[0] = value;

  // Derivada com coeficientes de diferença finita
  float derivative = 2.08 * ch->derivativeWindow[0] - 4.0 * ch->derivativeWindow[1] 
                   + 3.0 * ch->derivativeWindow[2] - 1.33 * ch->derivativeWindow[3] 
                   + 0.25 * ch->derivativeWindow[4];
  return derivative;
}

int computeIntensity(ChannelState *ch, int newDerivative, int threshold) {
  unsigned long now = millis();

  if (!ch->debouncing && newDerivative > threshold) {
    ch->lastDebounceTime = now;
    ch->debouncing = true;
  }

  if ((now - ch->lastDebounceTime) > ch->debounceTime) {
    ch->debouncing = false;
    ch->intensity = 0;
    ch->waitingForPeak = true;
    ch->maxDerivative = 0;
    digitalWrite(LED_PIN, LOW);
  }

  if (ch->debouncing && ch->waitingForPeak) {
    if (newDerivative > ch->maxDerivative) {
      ch->maxDerivative = newDerivative;
    } else {
      ch->intensity = ch->maxDerivative;
      ch->waitingForPeak = false;
      digitalWrite(LED_PIN, HIGH);
      Serial.write(ch->intensity / 10);
    }
  }
  return ch->intensity;
}

void setDebugMessage(int tumPressure, int taPressure, int tumDerivative, int taDerivative, int tumIntensity, int taIntensity) {
  debugMessage[0] = 0xFF;
  debugMessage[1] = 0xFF;
  debugMessage[2] = lowByte(tumPressure);
  debugMessage[3] = highByte(tumPressure);
  debugMessage[4] = lowByte(taPressure);
  debugMessage[5] = highByte(taPressure);
  debugMessage[6] = lowByte(tumDerivative);
  debugMessage[7] = highByte(tumDerivative);
  debugMessage[8] = lowByte(taDerivative);
  debugMessage[9] = highByte(taDerivative);
  debugMessage[10] = lowByte(tumIntensity);
  debugMessage[11] = highByte(tumIntensity);
  debugMessage[12] = lowByte(taIntensity);
  debugMessage[13] = highByte(taIntensity);

  debugMessage[14] = 0;
  for (int i = 2; i < 14; i++) {
    debugMessage[14] += debugMessage[i];
  }

  Serial.write(debugMessage, sizeof(debugMessage));
}

void setLiveMessage(int tumIntensity, int taIntensity) {
  liveMessage[0] = 0xFE;
  liveMessage[1] = 0xFE;
  liveMessage[2] = lowByte(tumIntensity);
  liveMessage[3] = highByte(tumIntensity);
  liveMessage[4] = lowByte(taIntensity);
  liveMessage[5] = highByte(taIntensity);

  liveMessage[6] = 0;
  for (int i = 2; i < 6; i++) {
    liveMessage[6] += liveMessage[i];
  }

  Serial.write(liveMessage, sizeof(liveMessage));
}

// ---- Setup ----
void setup() {
  Serial.begin(38400);

  pinMode(TUM_PIN, INPUT);
  pinMode(TA_PIN, INPUT);
  pinMode(MODE_SWITCH_PIN, INPUT);
  pinMode(THRESHOLD_PIN, INPUT);
  pinMode(STANDBY_SWITCH_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
}

// ---- Loop ----
void loop() {
  standby = digitalRead(STANDBY_SWITCH_PIN);
  threshold = analogRead(THRESHOLD_PIN);

  unsigned long now = millis();

  if (standby) {
    if (now - tempoStandby > tempoBlink) {
      digitalWrite(LED_PIN, blinkFlag ? LOW : HIGH);
      tempoStandby = now;
      blinkFlag = !blinkFlag;
    }
  } else {
    isDebugging = digitalRead(MODE_SWITCH_PIN);

    int tumValue = analogRead(TUM_PIN);
    int taValue = analogRead(TA_PIN);

    float tumDerivative = getDerivative(&channels[TUM_CHANNEL], tumValue);
    float taDerivative = getDerivative(&channels[TA_CHANNEL], taValue);

    int tumIntensity = computeIntensity(&channels[TUM_CHANNEL], tumDerivative, threshold);
    int taIntensity = computeIntensity(&channels[TA_CHANNEL], taDerivative, threshold);

    if (isDebugging) {
      setDebugMessage(tumValue, taValue, (int)tumDerivative, (int)taDerivative, tumIntensity, taIntensity);
    } else {
      setLiveMessage(tumIntensity, taIntensity);
    }

    delayMicroseconds(1000);
  }
}
