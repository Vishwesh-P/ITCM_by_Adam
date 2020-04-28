// Read IMU C
void readImu() {

  // Start loop timer
  unsigned long loopStartTime = micros();

  // Get a new sensor event
  accelmag.getEvent(&aEvent, &mEvent);  // Measured in m/s^2 and uTesla
  gyro.getEvent(&gEvent);               // Measured in rad/s

  // Add to statistics object
  axStats.add(aEvent.acceleration.x);
  ayStats.add(aEvent.acceleration.y);
  azStats.add(aEvent.acceleration.z);
  mxStats.add(mEvent.magnetic.x);
  myStats.add(mEvent.magnetic.y);
  mzStats.add(mEvent.magnetic.z);
  gxStats.add(gEvent.gyro.x);
  gyStats.add(gEvent.gyro.y);
  gzStats.add(gEvent.gyro.z);

//  // Write data to union
//  message.gyroFlag = 0b01100101; // Just an example

  // Place accelerometer and gyroscope in low-power (standby) mode
  accelmag.standby(1);
  gyro.standby(1);
  #ifdef DEBUG
    Serial.print(F("Added delay in IMU loop: 500 msec"));
    delay(500); //needed here ??
  #endif
  // Stop loop timer
  unsigned long loopEndTime = micros() - loopStartTime;
  Serial.print(F("readImu() executed in: ")); Serial.print(loopEndTime); Serial.println(F(" μs"));
}
