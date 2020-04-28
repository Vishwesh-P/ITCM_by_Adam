// Print statistics NC
void printStatistics() {
  Serial.println(F("-------------------------------------------------------------------------"));
  Serial.println(F("Statistics"));
  Serial.println(F("-------------------------------------------------------------------------"));
  Serial.println(F("Voltage:"));
  Serial.print(F("Samples: ")); Serial.print(batteryStats.count());
  Serial.print(F("\tMin: "));   Serial.print(batteryStats.minimum());
  Serial.print(F("\tMax: ")); Serial.print(batteryStats.maximum());
  Serial.print(F("\tMean: ")); Serial.print(batteryStats.average());
  Serial.print(F("\tSD: ")); Serial.println(batteryStats.unbiased_stdev());
  Serial.println(F("ax:"));
  Serial.print(F("Samples: ")); Serial.print(axStats.count());
  Serial.print(F("\tMin: "));   Serial.print(axStats.minimum());
  Serial.print(F("\tMax: ")); Serial.print(axStats.maximum());
  Serial.print(F("\tMean: ")); Serial.print(axStats.average());
  Serial.print(F("\tSD: ")); Serial.println(axStats.unbiased_stdev());
  Serial.println(F("ay:"));
  Serial.print(F("Samples: ")); Serial.print(ayStats.count());
  Serial.print(F("\tMin: "));   Serial.print(ayStats.minimum());
  Serial.print(F("\tMax: ")); Serial.print(ayStats.maximum());
  Serial.print(F("\tMean: ")); Serial.print(ayStats.average());
  Serial.print(F("\tSD: ")); Serial.println(ayStats.unbiased_stdev());
  Serial.println(F("az:"));
  Serial.print(F("Samples: ")); Serial.print(azStats.count());
  Serial.print(F("\tMin: "));   Serial.print(azStats.minimum());
  Serial.print(F("\tMax: ")); Serial.print(azStats.maximum());
  Serial.print(F("\tMean: ")); Serial.print(azStats.average());
  Serial.print(F("\tSD: ")); Serial.println(azStats.unbiased_stdev());
  Serial.println(F("mx:"));
  Serial.print(F("Samples: ")); Serial.print(mxStats.count());
  Serial.print(F("\tMin: "));   Serial.print(mxStats.minimum());
  Serial.print(F("\tMax: ")); Serial.print(mxStats.maximum());
  Serial.print(F("\tMean: ")); Serial.print(mxStats.average());
  Serial.print(F("\tSD: ")); Serial.println(mxStats.unbiased_stdev());
  Serial.println(F("my:"));
  Serial.print(F("Samples: ")); Serial.print(myStats.count());
  Serial.print(F("\tMin: "));   Serial.print(myStats.minimum());
  Serial.print(F("\tMax: ")); Serial.print(myStats.maximum());
  Serial.print(F("\tMean: ")); Serial.print(myStats.average());
  Serial.print(F("\tSD: ")); Serial.println(myStats.unbiased_stdev());
  Serial.println(F("mz:"));
  Serial.print(F("Samples: ")); Serial.print(mzStats.count());
  Serial.print(F("\tMin: "));   Serial.print(mzStats.minimum());
  Serial.print(F("\tMax: ")); Serial.print(mzStats.maximum());
  Serial.print(F("\tMean: ")); Serial.print(mzStats.average());
  Serial.print(F("\tSD: ")); Serial.println(mzStats.unbiased_stdev());
  Serial.println(F("gx:"));
  Serial.print(F("Samples: ")); Serial.print(gxStats.count());
  Serial.print(F("\tMin: "));   Serial.print(gxStats.minimum());
  Serial.print(F("\tMax: ")); Serial.print(gxStats.maximum());
  Serial.print(F("\tMean: ")); Serial.print(gxStats.average());
  Serial.print(F("\tSD: ")); Serial.println(gxStats.unbiased_stdev());
  Serial.println(F("gy:"));
  Serial.print(F("Samples: ")); Serial.print(gyStats.count());
  Serial.print(F("\tMin: "));   Serial.print(gyStats.minimum());
  Serial.print(F("\tMax: ")); Serial.print(gyStats.maximum());
  Serial.print(F("\tMean: ")); Serial.print(gyStats.average());
  Serial.print(F("\tSD: ")); Serial.println(gyStats.unbiased_stdev());
  Serial.println(F("gz:"));
  Serial.print(F("Samples: ")); Serial.print(gzStats.count());
  Serial.print(F("\tMin: "));   Serial.print(gzStats.minimum());
  Serial.print(F("\tMax: ")); Serial.print(gzStats.maximum());
  Serial.print(F("\tMean: ")); Serial.print(gzStats.average());
  Serial.print(F("\tSD: ")); Serial.println(gzStats.unbiased_stdev());

}

// Print union/structure
void printUnion() {
  Serial.println(F("-----------------------------------"));
  Serial.println(F("Union/structure"));
  Serial.println(F("-----------------------------------"));
  Serial.print(F("unixtime:\t\t")); Serial.println(message.unixtime);
  //Serial.print(F("temperature:\t\t")); Serial.println(message.temperature);
  //Serial.print(F("pitch:\t\t\t")); Serial.println(message.pitch);
  //Serial.print(F("roll:\t\t\t")); Serial.println(message.roll);
  //Serial.print(F("heading:\t\t")); Serial.println(message.heading);
  Serial.print(F("axMean:\t\t\t")); Serial.println(message.axMean);
  Serial.print(F("ayMean:\t\t\t")); Serial.println(message.ayMean);
  Serial.print(F("azMean:\t\t\t")); Serial.println(message.azMean);
  Serial.print(F("axStdev:\t\t")); Serial.println(message.axStdev);
  Serial.print(F("ayStdev:\t\t")); Serial.println(message.ayStdev);
  Serial.print(F("azStdev:\t\t")); Serial.println(message.azStdev);
  Serial.print(F("mxMean:\t\t\t")); Serial.println(message.mxMean);
  Serial.print(F("myMean:\t\t\t")); Serial.println(message.myMean);
  Serial.print(F("mzMean:\t\t\t")); Serial.println(message.mzMean);
  Serial.print(F("gyroFlag:\t\t")); Serial.println(message.gyroFlag);
  Serial.print(F("latitude:\t\t")); Serial.println(message.latitude);
  Serial.print(F("longitude:\t\t")); Serial.println(message.longitude);
  //Serial.print(F("satellites:\t\t")); Serial.println(message.satellites);
  //Serial.print(F("hdop:\t\t\t")); Serial.println(message.hdop);
  Serial.print(F("voltage:\t\t")); Serial.println(message.voltage);
  Serial.print(F("transmitDuration:\t")); Serial.println(message.transmitDuration);
  Serial.print(F("messageCounter:\t\t")); Serial.println(message.messageCounter);
  Serial.println(F("-----------------------------------"));
}

// Print contents of union/structure
void printUnionBinary() {
  Serial.println(F("Union/structure "));
  Serial.println(F("-----------------------------------"));
  Serial.println(F("Byte\tHex\tBinary"));
  for (unsigned int i = 0; i < sizeof(message); ++i) {
    Serial.print(i);
    Serial.print("\t");
    Serial.print(message.bytes[i], HEX);
    Serial.print("\t");
    Serial.println(message.bytes[i], BIN);
  }
  Serial.println(F("-----------------------------------"));
}

// Print contents of transmiff buffer array
void printTransmitBuffer() {
  Serial.println(F("Transmit buffer"));
  Serial.println(F("-----------------------------------"));
  Serial.println(F("Byte\tHex\tBinary"));
  for (unsigned int i = 0; i < sizeof(transmitBuffer); i++) {
    Serial.print(i);
    Serial.print(F("\t"));
    Serial.print(transmitBuffer[i], HEX);
    Serial.print("\t");
    Serial.println(transmitBuffer[i], BIN);
  }
}
