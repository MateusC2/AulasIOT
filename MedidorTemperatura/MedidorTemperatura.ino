#include <math.h>
#include "AdafruitIO_WiFi.h"
#include "NewPing.h"

//Configurações da rede WIFI
#define WIFI_SSID ""
#define WIFI_PASS ""

//Autenticação Adafruit IO
#define IO_USERNAME ""
#define IO_KEY ""

AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);


#define pinNTC 34
#define pinLed 14

#define BUZZER_PIN 27
#define LED_ALARME 13
#define BOTAO_FISICO 26
#define TRIG_PIN 12
#define ECHO_PIN 14

//Configuração do ultrassonico
#define MAX_DISTANCE 100
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);


//Controle de envio de dados
float temp_atual = 0;
float temp_anterior = -1;

//Variavel / ponteiro para referenciar o feed temperatura
// AdafruitIO_Feed *temperatura = io.feed("temperatura");
AdafruitIO_Feed *botaoalarme = io.feed("botaoalarme");



// --- Constantes do NTC ---
const float Rfixo = 10000.0;     // Resistor fixo do divisor de tensão (ohms)
const float Beta = 3950.0;       // Constante Beta do NTC (fornecida pelo fabricante)
const float R0 = 10000.0;        // Resistência nominal do NTC a 25°C (ohms)
const float T0_kelvin = 298.15;  // 25°C em Kelvin
const float Vcc = 3.3;           // Tensão de alimentação do divisor (ESP32 = 3,3V)

// Variaveis de controle
bool alarmeAtivo = false;
unsigned int distancia = 0;
const int LIMITE_DISTANCIA = 15;



void setup() {
  // pinMode(pinNTC, INPUT);
  // pinMode(pinLed, OUTPUT);

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_ALARME, OUTPUT);
  pinMode(BOTAO_FISICO, INPUT);

  Serial.begin(115200);

  while (!Serial)
    ;
  Serial.print("Conectando ao Adafruit IO");
  io.connect();

  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // Serial.println();
  // Serial.println(io.statusText());

  // //Configuração do callback, quando o feed receber(atualizar) um valor
  // temperatura->onMessage(handleTemperatura);
  botaoalarme->onMessage(handleAlarme);
  // //Registra a função de callback
  // //Ela será chamada sempre que o feed receber um novo dado

  Serial.print("Solicitando o estado incial do alarme: ");
  botaoalarme->get();

  delay(1000);
}

void loop() {

  // testeLed();
  // testeBuzzer();
  // testeBotao(BOTAO_FISICO);
  // Serial.print(F("Distancia Lida: "));
  // Serial.println(sonar.ping_cm());
  // delay(500);

  //Manter a conexão com o Adafruit IO ativa
  io.run();

  // publicacao();  //Chamada da função publish

  // // Serial.print("Temperatura Analogica: ");
  // // Serial.println(analogRead(pinNTC));

  // delay(3000);

  //Leitura do botão fisico
  if (digitalRead(BOTAO_FISICO) == 1) {
    delay(200);  // debounce simples
    alarmeAtivo = !alarmeAtivo;

    botaoalarme->save(String(alarmeAtivo ? "true" : "false"));
    Serial.println(alarmeAtivo ? F("Alarme ARMADO pelo botao fisico") : F("Alarme DESARMADO pelo botao fisico"));
  }

  distancia = sonar.ping_cm();
  Serial.print("Distancia lida: ");
  Serial.println(distancia);
  Serial.print("cm");

  //Ativação ou Desativação do alarme
  if(alarmeAtivo && distancia > 0 && distancia < LIMITE_DISTANCIA){
    ativarAlerta();
  }
  else{
    desligarAlerta();
  }
}
