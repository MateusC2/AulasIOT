// Função chamda automaticamente quando o feed receber atualizações
void handleTemperatura(AdafruitIO_Data *data) {
  float temp = data->toFloat();

  Serial.print(F("Temperatura do feed: "));
  Serial.print(temp, 2);
  Serial.println(F("°C"));

  //Logica para acender ou não o led
  if (temp <= 0) {
    digitalWrite(pinLed, HIGH);
  } else {
    digitalWrite(pinLed, LOW);
  }
}