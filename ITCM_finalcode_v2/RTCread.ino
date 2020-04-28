// Read RTC NC
void readRtc() {

  // Start loop timer
  unsigned long loopStartTime = micros();

  // Get UNIX Epoch time
  unixtime = rtc.getEpoch();

  // Write data to union
  message.unixtime = unixtime;
  Serial.print(F("Epoch time: ")); Serial.println(unixtime);

  // Stop loop timer
  unsigned long loopEndTime = micros() - loopStartTime;
  Serial.print(F("readRtc() function execution: ")); Serial.print(loopEndTime); Serial.println(F(" μs"));
}

// RTC alarm interrupt service routine
void alarmMatch() {
  alarmFlag = true; // Set alarm flag
}
