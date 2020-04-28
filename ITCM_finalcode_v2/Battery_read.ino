// Measure battery voltage from 100/100 kOhm voltage divider NC
void readBattery() {

  // Start loop timer
  unsigned long loopStartTime = millis();
  float voltage = 0.0;

  // Average measurements
  for (int i = 0; i < 10; ++i) {
    voltage += analogRead(VBAT_PIN);
    delay(1);
  }
  voltage /= 10;    // Average measurements
  voltage *= 2;     // Divided by 2, so multiply back
  voltage *= 3.3;   // Multiply by 3.3V reference voltage
  voltage /= 4096;  // Convert to voltage

  // Add to statistics object
  batteryStats.add(voltage);

  // Stop loop timer
  unsigned long loopEndTime = millis() - loopStartTime;
  //Serial.print("readBattery() function execution: "); Serial.print(loopEndTime); Serial.println(F(" ms"));
}
