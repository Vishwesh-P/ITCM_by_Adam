// Print RTC date and time NC
void printDateTime() {
  char datetimeBuffer[20];
  snprintf(datetimeBuffer, sizeof(datetimeBuffer), "%04u-%02d-%02dT%02d:%02d:%02d",
           rtc.getYear() + 2000, rtc.getMonth(), rtc.getDay(),
           rtc.getHours(), rtc.getMinutes(), rtc.getSeconds());
  Serial.println(datetimeBuffer);
}

// Print RTC alarm
void printAlarm() {
  char alarmBuffer[20];
  snprintf(alarmBuffer, sizeof(alarmBuffer), "%04u-%02d-%02dT%02d:%02d:%02d",
           rtc.getAlarmYear() + 2000, rtc.getAlarmMonth(), rtc.getAlarmDay(),
           rtc.getAlarmHours(), rtc.getAlarmMinutes(), rtc.getAlarmSeconds());
  Serial.println(alarmBuffer);
}
