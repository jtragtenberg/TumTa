# TumTa - Instrumento Digital de Dança e Música voltado a disparar samples sonoros a partir de pisadas através de uma palmilha sensível à pressão (FSR).

## Descrição

**TumTa** é um sistema híbrido, modular e open-source, que integra:

- Hardware de detecção percussiva sensível ao toque.
- Firmware embarcado com processamento em tempo real.
- Comunicação sem fio via **XBee**.
- Software receptor/visualizador desenvolvido em **C++ com OpenFrameworks**.

O sistema foi desenvolvido para aplicações em:

- Performance musical ao vivo.
- Instalações audiovisuais interativas.
- Pesquisas em interfaces físico-digitais.

## Componentes do Projeto

### 1. Circuito eletrônico

- Projeto de circuito com sensores piezoelétricos.
- Arquivos disponíveis no repositório:
  - Esquema elétrico (.sch).
  - PCB para fabricação ou CNC (.brd).
  - Layout da caixa para **corte a laser** (.svg ou .dxf).

Todos os arquivos estão na pasta `hardware/`.

### 2. Firmware (Arduino)

- Processamento de sinais em tempo real com:
  - Derivada via diferenças finitas (float).
  - Controle **não bloqueante** com `millis()`.
  - Sistema de **debounce** ajustável.
  - Modos de envio: `DEBUG` e `LIVE`.

- Comunicação serial com módulo **XBee** para transmissão sem fio.
- Código modular e otimizado disponível na pasta `firmware/`.

### 3. Comunicação sem fio (XBee)

- Configuração clássica:
  - Transmissor: Arduino + XBee.
  - Receptor: XBee + computador.

- Baudrate: 38400.
- Comunicação robusta com protocolo de mensagem e **checksum**.

### 4. Software receptor (OpenFrameworks)

Desenvolvido em **C++ com OpenFrameworks**, o software realiza:

- Recepção e interpretação das mensagens enviadas via XBee.
- Detecção de picos ajustável, permitindo o controle da sensibilidade à intensidade.
- Mapeamento dinâmico: a intensidade controla diretamente a dinâmica dos samples gerados.
- Contorno de ruídos: filtros e thresholds ajustáveis para minimizar falsos disparos.
- Interface com `ofxGui` para ajuste em tempo real.
- Visualização gráfica dos dados recebidos.

**Integração MIDI**:

- O software converte os eventos de detecção percussiva em **mensagens MIDI**.
- Utiliza bibliotecas como `ofxMidi` para envio MIDI.
- As mensagens MIDI são enviadas em tempo real para uma **DAW** (ex.: **Ableton Live**, **Logic Pro**, **Reaper**).
- Cada batida detectada pode ser mapeada para **notas MIDI específicas**, disparando samples ou instrumentos virtuais.

## Fluxo geral do sistema

```
Sensor Piezo → Arduino → Processamento → XBee TX → ~~~ Wireless ~~~ → XBee RX → OpenFrameworks → MIDI OUT → DAW (Ableton Live)
```

## Arquivos incluídos no repositório

| Pasta            | Conteúdo                                                 |
|------------------|----------------------------------------------------------|
| `PCB/ Enclosure/ | Esquema, PCB (.brd/.sch), layout da caixa (.svg/.dxf).   |
| `Arduino/`       | Código Arduino (`TumTa.ino`).                            |
| `OpenFrameWorks/`| Projeto OpenFrameworks com envio MIDI.                   |
| `Imagens/`       | Fotos                                                    |
| `Max/`           | Software Aternativo                                      |

## Como montar o sistema

### 1. Hardware

- Construa o circuito com base no esquema elétrico.
- Corte a caixa com laser utilizando os arquivos vetoriais.
- Monte os componentes, conecte sensores, potenciômetro e switches.

### 2. Firmware

- Abra o `TumTa.ino` no Arduino IDE.
- Compile e envie para a placa Arduino.

### 3. Comunicação

- Configure os módulos **XBee**:
  - Mesma PAN ID.
  - Baudrate: 38400.
- Conecte o transmissor ao Arduino e o receptor ao computador.

### 4. Software

- Instale **OpenFrameworks** (versão 0.11 ou superior).
- Inclua `ofxMidi` para envio de mensagens MIDI.
- Abra o projeto `TumTa` na pasta `software/`.
- Ajuste:
  - Porta serial para XBee receptor.
  - Configurações MIDI (porta virtual ou física).
- Compile e execute.
- Configure a DAW (ex.: **Ableton Live**) para receber as mensagens MIDI provenientes do software.

## Parâmetros ajustáveis

| Parâmetro                | Local                            | Valor padrão |
|------------------------- |--------------------------------- |------------- |
| Threshold                | Potenciômetro (A7)               | Variável     |
| Debounce Time            | Estrutura `ChannelState`         | 150 ms       |
| Process Interval         | Global                           | 1 ms         |
| Tempo de Pisca (LED)     | Global                           | 500 ms       |
| Sensibilidade de Pico    | Interface do OpenFrameworks      | Ajustável    |
| Dinâmica de Sample/MIDI  | Mapeamento no software           | Ajustável    |
| Notas MIDI mapeadas      | Configuração no OpenFrameworks   | Ajustável    |

## Diferenciais do projeto

- Modular e expansível, com suporte para múltiplos sensores.
- Processamento híbrido, com divisão de carga entre hardware e software.
- Integração robusta com sistemas artísticos interativos e musicais.
- Arquivos completos: circuito, caixa e software.
- Envio de MIDI para **DAW** em tempo real.

