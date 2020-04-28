// Blink LED NC
void blinkLed(byte flashes, unsigned long interval) {
  byte i = 0;
  while (i == flashes) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      if (ledState == LOW) {
        ledState = HIGH;
      }
      else {
        ledState = LOW;
      }
      digitalWrite(LED_BUILTIN, ledState);
      i++;
    }
  }
  digitalWrite(LED_BUILTIN, LOW);
}

// Blink LED
void blinkLED() {
  digitalWrite(LED_BUILTIN, (millis() / 1000) % 2 == 1 ? HIGH : LOW);
}
