// --- Função para calcular temperatura com média de leituras ---
float lerTemperaturaNTC(int pino, int numLeituras) {
  long somaLeituras = 0;

  // Faz várias leituras para reduzir ruído
  for (int i = 0; i < numLeituras; i++) {
    somaLeituras += analogRead(pino);
    delay(5);  // pequeno atraso para estabilizar
  }

  // Calcula média das leituras
  float leituraMedia = somaLeituras / (float)numLeituras;

  // Converte para tensão real (ESP32: ADC de 12 bits = 0 a 4095)
  float Vout = leituraMedia * (Vcc / 4095.0);

  // Calcula resistência do NTC pelo divisor de tensão
  float Rntc = Rfixo * ((Vcc / Vout) - 1.0);

  // Aplica equação Beta para calcular temperatura em Kelvin
  float tempK = 1.0 / ((1.0 / T0_kelvin) + (1.0 / Beta) * log(Rntc / R0));

  // Retorna temperatura em Celsius
  return tempK - 273.15;
}

void publicacao() {
  float temp_atual = lerTemperaturaNTC(pinNTC, 10);

  //Verificando alteração na temperatura
  if (temp_atual == temp_anterior) 
    return;

  // Exibe temperatura no Serial Monitor com duas casas decimais
  Serial.print("Temperatura enviada: ");
  Serial.print(temp_atual, 2);
  Serial.println("°C");

  // Envia valor para o feed "temperatura" no Adafruit IO
  temperatura->save(temp_atual);

  // Atualiza último valor enviado
  temp_anterior = temp_atual;

  
}