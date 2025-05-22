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

const unsigned long processInterval = 1; // intervalo mínimo entre processamentos (ms)
unsigned long lastProcessTime = 0;

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

  float derivative = 2.08 * ch->derivativeWindow[0] 
                   - 4.0 * ch->derivativeWindow[1] 
                   + 3.0 * ch->derivativeWindow[2] 
                   - 1.33 * ch->derivativeWindow[3] 
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
      ch->maxDerivative = ne
