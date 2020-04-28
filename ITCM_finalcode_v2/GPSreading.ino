// Read GPS NC
void readGps() {

  // Start loop timer
  unsigned long loopStartTime = millis();

  bool fixFound = false;
  bool charsSeen = false;
  byte fixCounter = 0;

  // Enable GPS
  digitalWrite(GPS_EN_PIN, LOW);

  Serial.println("Beginning to listen for GPS traffic...");
  GpsSerial.begin(9600);
  blinkLed(2, 500);

  // Configure GPS
  GpsSerial.println("$PMTK220,1000*1F"); // Set NMEA port update rate to 1Hz
  blinkLed(1, 100);
  GpsSerial.println("$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28"); // Set NMEA sentence output frequencies to GGA and RMC
  blinkLed(1, 100);
  //GpsSerial.println("$PGCMD,33,1*6C"); // Enable antenna updates
  GpsSerial.println("$PGCMD,33,0*6D"); // Disable antenna updates

  // Look for GPS signal for up to 5 minutes
  while (!fixFound && millis() - loopStartTime < 5UL * 60UL * 1000UL) {
    if (GpsSerial.available()) {
      charsSeen = true;
      char c = GpsSerial.read();
#if DEBUG_GPS
      Serial.write(c); // Echo NMEA sentences to serial
#endif
      if (gps.encode(c)) {
        if ((gps.location.isValid() && gps.date.isValid() && gps.time.isValid()) &&
            (gps.location.isUpdated() && gps.date.isUpdated() && gps.time.isUpdated())) {
          fixCounter++;
#if DEBUG
          char gpsDateTime[25];
          snprintf(gpsDateTime, sizeof(gpsDateTime), "%04u-%02d-%02dT%02d:%02d:%02d,",
                   gps.date.year(), gps.date.month(), gps.date.day(),
                   gps.time.hour(), gps.time.minute(), gps.time.second());
          Serial.print(gpsDateTime);
          Serial.print(gps.location.lat(), 6);
          Serial.print(F(","));
          Serial.print(gps.location.lng(), 6);
          Serial.print(F(","));
          Serial.print(gps.satellites.value());
          Serial.print(F(","));
          Serial.println(gps.hdop.hdop(), 2);
#endif
          if (fixCounter >= maxFixCounter) {
            fixFound = true;

            // Sync RTC with GPS time
            rtc.setTime(gps.time.hour(), gps.time.minute(), gps.time.second());
            rtc.setDate(gps.date.day(), gps.date.month(), gps.date.year() - 2000);
            rtcFlag = true;
            Serial.print("RTC set: "); printDateTime();

            message.latitude = gps.location.lat() * 1000000;
            message.longitude = gps.location.lng() * 1000000;
          }
        }
      }
    }

    ISBDCallback();

    if ((millis() - loopStartTime) > 5000 && gps.charsProcessed() < 10) {
      Serial.println(F("No GPS data received: check wiring"));
      break;
    }
  }
  Serial.println(charsSeen ? fixFound ? F("A GPS fix was found!") : F("No GPS fix was found.") : F("Wiring error: No GPS data seen."));

  // Stop loop timer
  unsigned long loopEndTime = millis() - loopStartTime;
  Serial.print(F("readGps() function execution: ")); Serial.print(loopEndTime); Serial.println(F(" ms"));

  // Disable GPS
  digitalWrite(GPS_EN_PIN, HIGH);
}
