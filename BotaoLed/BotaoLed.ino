#include "AdafruitIO_WiFi.h"

// suas credenciais Wi-Fi
#define WIFI_SSID       ""
#define WIFI_PASS       ""

// suas credenciais da Adafruit IO
#define IO_USERNAME     ""
#define IO_KEY          ""

#include "WiFi.h"
#include "AdafruitIO_WiFi.h"

// Cria objeto de conexão
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);

// Cria referência ao feed
AdafruitIO_Feed *botaoLED = io.feed("botaoled");

// Define o pino do LED
#define LED_PIN 14

void setup() {
  // Inicializa comunicação serial
  Serial.begin(115200);

  // Configura o pino do LED como saída
  pinMode(LED_PIN, OUTPUT);

  // Conecta ao Adafruit IO
  Serial.print("Conectando ao Adafruit IO");
  io.connect();

  // Registra função callback quando o feed receber dados
  botaoLED->onMessage(handleBotaoLed);

  // Aguarda conexão
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("Conectado!");
}

void loop() {
  // Mantém conexão ativa
  io.run();
}

// Função chamada sempre que houver alteração no feed
void handleBotaoLed(AdafruitIO_Data *data) {
  String valor = data->toString();
  Serial.print("Recebido do feed: ");
  Serial.println(valor);

  if (valor == "true") {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("LED LIGADO");
  } else {
    digitalWrite(LED_PIN, LOW);
    Serial.println("LED DESLIGADO");
  }
}