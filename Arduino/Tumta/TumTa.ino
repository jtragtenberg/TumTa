//Joao Tragtenberg - 4/07/2015
//TumTa 0.2.0 - Arduino - Para rodar com o arquivo de Max 1.0.7 e com a placa 0.2.6
//

byte TUMBYTE = 0xA0;
byte TABYTE = 0xB0;
byte EXECUCAOBYTE = 0xC0;
byte CALIBRACAOBYTE = 0xC1;
byte STARTBYTE = 0xF0;
byte ENDBYTE = 0xFF;
byte SINALSEGATIVOBYTE = 0x80;
byte THRESHOLDTUMBYTE = 0xD0;
byte THRESHOLDTABYTE = 0xD1;
byte DEBOUNCETIMEBYTE = 0xE0;
byte CONFIRMACAOBYTE = 0x90;

int tumPin = A0;
int taPin = A1;
int led1Pin = 9; //led que avista o estado, se est' em standby ou nao
int led2Pin = 10; //led do aperto de mao, que diz se o programa deu um aperto de mao ou nao
int butaoPin = 6; //botao liga e desliga modo standby

boolean butao = LOW;

byte calibration[10] = {
  0,0,0,0,0,0,0,0,0,0};

byte serialIn = 0;
byte modo = 0;
byte checksum = 0;

float valorTum[10] = {
  0,0,0,0,0,0,0,0,0,0};
float valorTa[10] = {
  0,0,0,0,0,0,0,0,0,0};

float derivadaTum[10] = {
  0,0,0,0,0,0,0,0,0,0};
float derivadaTa[10] = {
  0,0,0,0,0,0,0,0,0,0};

long lastDebounceTime[2] = {               //define o tempo em que ocorreu a ultima pisada no tum [0] e no ta [1]
  0,0};

boolean debouncing[2] = {                  //define se esta ou nao no intervalo de tempo do debouce apos uma pisada no tum [0] e no ta [1]
  false,false};

float intensidade[2] = {                   //guarda a intensidade da pisada
  0,0};

boolean aindaNao[2] = {                   //variavel pra ajudar a descobrir o maximo na pisada.
  true,true};

float maximo[2] = {                       //variavel para acumular o maximo da derivada
  0,0};

float threshold[2] = {
  20,20};             //threshold do tum e do ta
float debounceTime = 200;                 //se der merda ja inicia com esses valores, mas o codigo so vai comecar se receber do max a calibracao


void setup (){
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  pinMode(butaoPin, INPUT_PULLUP);  
  Serial.begin(57600);
  analogReference(EXTERNAL);
  while(true){                                 //mantem o programa em suspenso ate que receba as informacoes de calibracao (threshold e debounceTime) do Max. A sintaxe da mensagem tem que ser:
    if (Serial.read() == STARTBYTE) {               //STARTBYTE THRESHOLDTUMBYTE  threshold_do_tum  THRESHOLDTABYTE  threshold_do_ta DEBOUNCETIMEBYTE valor_de_0_a_127 EXECUCAOBYTEouCALIBRACAOBYTE
      if(Serial.available() >= 7){                  //  0xF0         0xD0             0 - 127           0xD1              0 - 127          0xE0            0 - 127             0xC0 ou 0xC1
        int i;                                 //o codigo vai esperar pelo STARTBYTE e so entao vai receber 4 informacoes de calibracao no array calibration[]
        for (i=0;i<7;i++) {                    
          calibration[i] = Serial.read();
        }
        if (calibration[0] == THRESHOLDTUMBYTE && calibration[2] == THRESHOLDTABYTE && calibration[4] == DEBOUNCETIMEBYTE){        //se o primeiro e o terceiro forem os bytes de Threshold e debounceTime
          threshold[0] = calibration[1];                              //threshold do Tum (Intensidade Minima)
          threshold[1] = calibration[3];                              //threshold do Ta
          debounceTime = map(calibration[5],0,127,0,1000);             //debounceTime mapeados de 0 a 1000
          modo = calibration[6];                                      //e do ultimo modo usado (execucao ou calibracao)
          break  ;                                                      //so assim ele sai do loop.
        }                                                             
      }
    }
    Serial.write(CONFIRMACAOBYTE); //manda um byte de confirmacao para o max que responde ao arduino os dados de calibracao
    digitalWrite(led2Pin, HIGH); //mantem o led do aperto de mao piscando ate dar o aperto de mao (programa do max mandar as informacoes de calibracao)
    delay(500); //aproveita pra dar o delay no envio de CONFIRAMACAOBYTEs, que so manda a cada 1 segundo um byte pro max...
    digitalWrite(led2Pin, LOW);
    delay(500);
  }
}


void loop(){
  digitalWrite(led1Pin, HIGH);
  butao = digitalRead(butaoPin);
  if (!butao) digitalWrite(led2Pin, LOW);
  if (butao) {
  digitalWrite(led2Pin, HIGH);
  if (Serial.available() > 0){
    serialIn = Serial.read();
    if (serialIn == EXECUCAOBYTE || serialIn == CALIBRACAOBYTE) {
      modo = serialIn;                                  //recebe na variavel serialIn o modo de operacao, que pode ser o byte do
      delay(50);
    }                                                   //EXECUCAOBYTE ou CALIBRACAOBYTE
    if (serialIn == THRESHOLDTUMBYTE){
      threshold[0] = Serial.read();                        //threshold do Tum (ja vem de 0 a 127)      
      delay(50);
    }
    if (serialIn == THRESHOLDTABYTE){
      threshold[1] = Serial.read();                        //threshold do Ta 
      delay(50);
    }
    if (serialIn == DEBOUNCETIMEBYTE){
      debounceTime = map(Serial.read(),0,127,0,1000);    //mapeia o valor que vem de 0-127 para um debounceTime de 0 a 400
      delay(50);
    }
  }


  //TUM
  valorTum[4] = valorTum[3];
  valorTum[3] = valorTum[2];
  valorTum[2] = valorTum[1];
  valorTum[1] = valorTum[0];             //carrega os valores anterior na memoria
  valorTum[0] = analogRead(tumPin);      // faz a leitura do Tum na porta A0

  //derivadaTum[1] = derivadaTum[0]; //carrega o valor anterior da derivada na memoria
  derivadaTum[0] = 2.08*valorTum[0] - 4*valorTum[1] + 3*valorTum[2] - 1.33*valorTum[3] + 0.25*valorTum[4]; //faz o calculo da derivada com coeficientes de diferenca finita (http://en.wikipedia.org/wiki/Finite_difference_coefficients)

  //segundaDerivadaTum[0] = derivadaTum[0] - derivadaTum[1]; //faz o calculo da segunda derivada

  if (!debouncing[0]){                    //se nao estiver no meio de uma outro pisada
    if(derivadaTum[0] > threshold[0]){       //define a pisada quando a derivada passa de um threshold
      lastDebounceTime[0] = millis();     //guarda na memoria o instante que aconteceu a pisada no Tum
      debouncing[0] = true;                                      //diz que ainda esta debouncing      
    }
  }

  //Agoritmo para controlar o estado de debounce
  if ((millis() - lastDebounceTime[0]) > debounceTime) {       //Controla quando esta debouncing. Se ja passou o tempo de debounce
    debouncing[0] = false;                                     //apos a ultima pisada diz que ja saiu do debounce
    intensidade[0] = 0;                             //avisa que parou o tempo de debounce do Ta
    aindaNao[0] = true;                             //reinicia o aindaNao para permitir que a proxima pisada venha e que nao role outro noteOff...
    maximo[0] = 0;                                  //zera o valor do maximo para a proxima poder chegar...    
  } 


  //Algoritmo para deteccao da pisada
  if (debouncing[0]) {                               //enquanto estiver no tempo do debounce
    if (aindaNao[0]){                                //se ainda nao tiver descoberto o maximo d  a derivada (a intensidade da pisada)
      if (derivadaTum[0] > maximo[0]){               //se o grafico da derivada esta crescendo
        maximo[0] = derivadaTum[0];                  //continue colocando no valor da derivada na variavel maximo
      }
      else {                                         //se o grafico da derivada parar de crescer
        intensidade[0] = maximo[0];                  //pega o ultimo valor do maximo e guarda o valor na variavel intensidade
        //===============================================  MODO EXECUCAO  ==============================================================
        if(modo == EXECUCAOBYTE) {                   //se estiver no modo execucao
          serialExec(TUMBYTE, intensidade[0]);       //ja manda um noteOn com a intensidade da pisada
        }
        //==============================================================================================================================
        aindaNao[0] = false;                         //a pisada ja rolou
      }
    }
  }



  //TA
  valorTa[4] = valorTa[3];
  valorTa[3] = valorTa[2];
  valorTa[2] = valorTa[1];
  valorTa[1] = valorTa[0];                         //carrega o valor anterior na memoria
  valorTa[0] = analogRead(taPin);                  // faz a leitura do Ta na porta A1

  //  derivadaTa[1] = derivadaTa[0]; //carrega o valor anterior da derivada na memoria
  derivadaTa[0] = 2.08*valorTa[0] - 4*valorTa[1] + 3*valorTa[2] - 1.33*valorTa[3] + 0.25*valorTa[4];    //faz o calculo da derivada com coeficientes de diferenca finita

  //  segundaDerivadaTa[0] = derivadaTa[0] - derivadaTa[1]; //faz o calculo da segunda derivada

  if (!debouncing[1]){                                              //se nao estiver no meio de outra pisada
    if(derivadaTa[0] > threshold[1]){                                  //define a pisada quando a derivada passar de um limite
      lastDebounceTime[1] = millis();                               //guarda na memoria o instante que aconteceu a pisada no Ta
      debouncing[1] = true;                                           //ainda esta debouncing no Ta
    }
  }

  if ((millis() - lastDebounceTime[1]) > debounceTime) {            //se ja passou o tempo de debounce apos a ultima pisada no Ta
    debouncing[1] = false;                                          //diz que ja saiu do debounce do Ta
    intensidade[1] = 0;                        //avisa que parou o tempo de debounce do Ta
    aindaNao[1] = true;                       //reinicia o aindaNao para permitir que a proxima pisada venha e que nao mande outro noteOff...
    maximo[1] = 0;                            //zera o valor do maximo para a proxima poder chegar...    
  } 

  if (debouncing[1]) {                                //enquanto estiver no tempo do debounce
    if (aindaNao[1]){                                 //se ainda nao tiver descoberto o maximo da derivada (a intensidade da pisada)
      if (derivadaTa[0] > maximo[1]){                 //se o grafico da derivada esta crescendo
        maximo[1] = derivadaTa[0];                    //continue colocando no valor da derivada na variavel maximo
      }
      else {                                          //se o grafico da derivada parar de crescer
        intensidade[1] = maximo[1];                   //pega o ultimo valor do maximo e guarda o valor na variavel intensidade
        //===============================================  MODO EXECUCAO  ==============================================================
        if(modo == EXECUCAOBYTE) {                    //se estiver no modo execucao
          serialExec(TABYTE, intensidade[1]);         //ja manda um noteOn com a intensidade da pisada
        }
        //==============================================================================================================================
        aindaNao[1] = false;                          //a pisada ja rolou
      }
    }
  }


  //==============================================  MODO CALIBRACAO  =============================================================
  if (modo == CALIBRACAOBYTE) {
    serialCalib(valorTum[0], derivadaTum[0], intensidade[0], valorTa[0], derivadaTa[0], intensidade[1]);
  }
  //==============================================================================================================================




  delay(3);
}
}


//manda informacoes seriais no modo Execucao, mandando apenas a intensidade da pisada com o Tum ou do Ta quando eles forem pisados.
void serialExec(byte tumOuTa, int intensidade){
  if (modo == EXECUCAOBYTE) {
    Serial.write(STARTBYTE);                           //manda o starbyte do TumTa, que eh 0xF0
    Serial.write(tumOuTa);                        //manda o startbyte do Tum (0xA0) ou do Ta (0xB0)
    if (intensidade > 127) intensidade = 127;
    if (intensidade < 0) intensidade = 0;  
    Serial.write(intensidade);                   //manda a intensidade da pisada e zero quando n tiver nada
    checksum = STARTBYTE + tumOuTa + intensidade;
    checksum = checksum & B01111111;
    Serial.write(checksum);
    Serial.write(ENDBYTE);                         //manda o EndByte do TumTa, que eh 0xFF    
  }
}


//manda informacoes seriais no modo Calibracao, mandando todas as informacoes de pressao, derivada e pisada detectada do Tum e do Ta o tempo todo.
void serialCalib(int pTum, int dpTum, int iTum, int pTa, int dpTa, int iTa){
  if (modo == CALIBRACAOBYTE){
    Serial.write(STARTBYTE);                                     //manda o starbyte do TumTa, que eh 0xF0
    checksum = sendValueAsTwo7bitBytes(pTum);               //manda o valor da pressao do Tum e armazena o valor dos bytes no checksum
    checksum += sendNegativeAsTwo7bitBytes(dpTum);          //manda o valor da derivada do Tum e armazena o valor dos bytes no checksum
    if (iTum > 127) iTum = 127;
    if (iTum < 0) iTum = 0;                                 //garante que o valor da intensidade esteja em um byte de 7 bits
    Serial.write(iTum);                                     //manda o velocity midi da pisada e zero quando n tiver nada
    checksum += sendValueAsTwo7bitBytes(pTa);               //manda o valor da pressao do Ta e armazena o valor dos bytes no checksum
    checksum += sendNegativeAsTwo7bitBytes(dpTa);           //manda o valor da derivada do Ta e armazena o valor dos bytes no checksumt
    if (iTa > 127) iTa = 127;
    if (iTa < 0) iTa = 0;                                   //garante que o valor da intensidade esteja em um byte de 7 bits
    Serial.write(iTa);                                      //manda a intensidade da pisada e zero quando n tiver nada
    checksum += STARTBYTE + iTum + iTa;
    checksum = checksum & B01111111;                        //trunca o valor do checksum pra 7 bits
    Serial.write(checksum);
    Serial.write(ENDBYTE);                                     //manda o EndByte do TumTa, que eh 0xFF
  }
}



int sendValueAsTwo7bitBytes(int value)    //Permite escrever um numero de 0 a 16383 usando dois Bytes de 7 bits. O primeiro Byte (LSB)
{                                         //representa as 7 primeiras casas binarias e o segundo (MSB) representa as outras 7 casas binarias
  if (value > 16383) value = 16383;       //garante que se o valor nao ultrapasse 14 bits
  if (value < 0) value = 0;               //garante que o valor seja positivo
  byte LSB = value & B01111111;    
  byte MSB = value >> 7 & B01111111;    
  Serial.write(LSB);                      //Least Significant Byte
  Serial.write(MSB);                      //Most Significant Byte
  return LSB + MSB;                       //Manda a soma dos dois Bytes para o checksum
}




int sendNegativeAsTwo7bitBytes(int value)     //Faz com que um numero de -127 a 127 possa ser escrito com 2 bytes de 7 digitos, 
{                                             //onde o primeiro byte (LSB) eh o sinal positivo (0) ou negativo (B10000000)
  byte LSB = 0;
  byte MSB = 0;
  if (value < 0) {
    value = abs(value);
    if (value > 127) value = 127;
    LSB = SINALSEGATIVOBYTE;                               //0x80 eh o valor que escolhi para representar o valor negativo
    MSB = value;
    Serial.write(LSB);                        //Least Significant Byte representara o sinal negativo
    Serial.write(MSB);  
  }
  else {
    if (value > 127) value = 127;
    LSB = 0;                                 //como o numero eh positivo n tem o sinal de negativo
    MSB = value;
    Serial.write(LSB);
    Serial.write(MSB);
  }
  return LSB + MSB;                          //Manda a soma dos Bytes pra o checksum
}











