
#include <Arduino.h>
#include "SPIFFS.h"
#include "esp_sleep.h"

// CONFIGURAÇÕES
const char* nome_arquivo = "/test.wav";

const int dacPin = 25;
const int ldrPin = 27;

const int sampleRate = 16000;
const int headerSize = 44;

const uint64_t INTERVALO_US = 10ULL * 60ULL * 1000000ULL;

// CONFIGURAÇÃO DO LDR
// LOW = DIA / HIGH = NOITE
const int LDR_DIA = LOW;

File audioFile;

void dormir() {
  Serial.println("[SLEEP] Dormindo por 5 minutos...");
  Serial.flush();

  esp_sleep_enable_timer_wakeup(INTERVALO_US);
  esp_deep_sleep_start();
}

bool estaDeDia() {
  int leitura = digitalRead(ldrPin);

  Serial.printf("[LDR] Leitura: %s\n",
                leitura == HIGH ? "HIGH" : "LOW");

  return leitura == LDR_DIA;
}

void tocarAudio() {
  audioFile.seek(headerSize);

  Serial.println("[AUDIO] Reprodução iniciada.");

  while (audioFile.available()) {
    dacWrite(dacPin, audioFile.read());
    delayMicroseconds(1000000 / sampleRate);
  }

  // Fade-out suave
  for (int v = 128; v >= 0; v--) {
    dacWrite(dacPin, v);
    delayMicroseconds(200);
  }

  Serial.println("[AUDIO] Fim.");
}

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println("\n=== ACORDEI ===");

  pinMode(ldrPin, INPUT);

  // Verifica luminosidade
  if (!estaDeDia()) {
    Serial.println("[LDR] Noite detectada. Não vou reproduzir.");
    dormir();
  }

  Serial.println("[LDR] Dia detectado. Verificando áudio...");

  if (!SPIFFS.begin(true)) {
    Serial.println("[ERRO] Falha ao montar SPIFFS.");
    dormir();
  }

  if (!SPIFFS.exists(nome_arquivo)) {
    Serial.printf("[ERRO] Arquivo %s não encontrado!\n",
                  nome_arquivo);
    dormir();
  }

  audioFile = SPIFFS.open(nome_arquivo);

  if (!audioFile) {
    Serial.println("[ERRO] Não foi possível abrir o arquivo.");
    dormir();
  }

  tocarAudio();

  audioFile.close();
  SPIFFS.end();

  dormir();
}

void loop() {
}
