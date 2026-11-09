# "BeeTract" - ESP32 – Reprodutor Automático de Áudio com Sensor LDR

Sistema embarcado desenvolvido com ESP32 para reprodução automática de arquivos de áudio de acordo com a luminosidade do ambiente.

O dispositivo utiliza um sensor LDR para identificar se é dia ou noite. Quando detecta luminosidade correspondente ao período diurno, reproduz um arquivo WAV armazenado na memória flash (SPIFFS). Após a reprodução, ou quando identifica ausência de luminosidade, entra em modo Deep Sleep por 10 minutos antes de realizar uma nova verificação.

## Funcionalidades

- **Detecção de luminosidade:** utiliza um sensor LDR para identificar condições de dia e noite.
- **Reprodução automática:** reproduz o áudio quando as condições de luminosidade são adequadas.
- **Armazenamento interno:** utiliza SPIFFS para armazenar o arquivo de áudio.
- **Economia de energia:** utiliza Deep Sleep para reduzir o consumo energético.
- **Reprodução PCM:** utiliza o DAC interno do ESP32 para gerar o sinal de áudio.
- **Fade-out:** aplica uma transição suave ao final da reprodução para reduzir ruídos e estalos.

## Hardware utilizado

| Componente | Conexão ESP32 |
|---|---|
| Sensor LDR (saída digital) | GPIO 27 |
| Saída de áudio DAC | GPIO 25 |
| Alimentação | 3.3V / GND |

### Observações sobre o hardware

- O GPIO 27 recebe o sinal digital do módulo LDR.
- O GPIO 25 utiliza o DAC interno do ESP32 para gerar o sinal de áudio.
- A saída DAC deve ser conectada a um circuito amplificador compatível antes de alimentar um alto-falante.
- O sensor LDR deve fornecer um sinal digital compatível com 3.3V.

**Importante:** o código foi desenvolvido para o ESP32 clássico, que possui DAC interno nos GPIOs 25 e 26. Outros modelos, como ESP32-S3, ESP32-C3 e ESP32-S2, não oferecem a mesma compatibilidade com `dacWrite()`.

## Funcionamento

1. O ESP32 inicializa e configura a comunicação serial.
2. O sensor LDR realiza a leitura da luminosidade.
3. Se detectar noite, o dispositivo não reproduz o áudio e entra em Deep Sleep.
4. Se detectar dia, verifica a disponibilidade do sistema de arquivos SPIFFS.
5. Localiza e abre o arquivo `/test.wav`.
6. Reproduz o áudio pelo DAC interno, utilizando uma taxa de amostragem de 16 kHz.
7. Aplica um Fade-out ao término da reprodução.
8. Fecha o arquivo e desmonta o SPIFFS.
9. Entra em Deep Sleep por 10 minutos.
10. Reinicia automaticamente e repete o processo.

## Configurações

As principais configurações podem ser alteradas diretamente no código:

| Parâmetro | Valor | Descrição |
|---|---|---|
| `nome_arquivo` | `/test.wav` | Caminho do arquivo de áudio |
| `dacPin` | 25 | GPIO utilizado pelo DAC |
| `ldrPin` | 27 | GPIO do sensor LDR |
| `sampleRate` | 16000 | Taxa de amostragem |
| `headerSize` | 44 | Tamanho do cabeçalho WAV ignorado |
| `INTERVALO_US` | 10 minutos | Intervalo entre verificações |
| `LDR_DIA` | LOW | Nível lógico correspondente ao dia |

### Ajuste do sensor LDR

A configuração atual considera:

- LOW: Dia.
- HIGH: Noite.

Caso o módulo LDR utilizado apresente comportamento inverso, basta alterar:

`const int LDR_DIA = LOW;`

Para:

`const int LDR_DIA = HIGH;`

### Intervalo de Deep Sleep

O intervalo está configurado para 10 minutos:

`const uint64_t INTERVALO_US = 10ULL * 60ULL * 1000000ULL;`

O valor pode ser alterado conforme a necessidade.

## Arquivo de áudio

O projeto utiliza um arquivo chamado `test.wav`, armazenado na memória SPIFFS.

Caminho esperado:

`/test.wav`

Características esperadas do áudio:

- Formato WAV.
- PCM sem compressão.
- 8 bits por amostra.
- Mono.
- Taxa de amostragem de 11.025 Hz. (Se posto em 16.000 Hz o audio fica ligeiramente mais rapido.)
- Cabeçalho WAV de 44 bytes.

**Atenção:** o código atual não interpreta o cabeçalho WAV. Ele simplesmente ignora os primeiros 44 bytes e reproduz o restante como amostras PCM de 8 bits. Portanto, o arquivo precisa respeitar essas características.

## Monitoramento Serial

O projeto disponibiliza mensagens de depuração pela porta serial, utilizando baud rate de 115200.

As mensagens permitem acompanhar:

- Inicialização do ESP32.
- Leitura do sensor LDR.
- Detecção de dia ou noite.
- Montagem do SPIFFS.
- Localização do arquivo de áudio.
- Início e término da reprodução.
- Entrada no modo Deep Sleep.

## Objetivo do projeto

Desenvolver um dispositivo autônomo de reprodução de áudio, capaz de operar com baixo consumo energético e sem necessidade de intervenção humana, utilizando a luminosidade ambiente como condição para execução.

---

**Tecnologias:** Arduino Framework, ESP32, SPIFFS, DAC, LDR e ESP32 Deep Sleep.