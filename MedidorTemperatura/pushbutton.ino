void testeBotao(int pinoBotao) {
  Serial.print(F("Estado Botão: "));
  Serial.println(digitalRead(pinoBotao));
  delay(500);
}
