
// Calculate statistics and clear objects C
void calculateStatistics() {
  int gxStdev,gyStdev,gzStdev;

  // Write data to union
  message.voltage = batteryStats.average() * 1000;
  message.axMean = axStats.average() * 100;
  message.ayMean = ayStats.average() * 100;
  message.azMean = azStats.average() * 100;
  message.axStdev = axStats.pop_stdev() * 100;
  message.ayStdev = ayStats.pop_stdev() * 100;
  message.azStdev = azStats.pop_stdev() * 100;
  message.mxMean = mxStats.average() * 100;
  message.myMean = myStats.average() * 100;
  message.mzMean = mzStats.average() * 100;
  //message.mxStdev = mxStats.pop_stdev() * 100;
  //message.myStdev = myStats.pop_stdev() * 100;
  //message.mzStdev = mzStats.pop_stdev() * 100;
  //message.gxMean = gxStats.average() * 100;
  //message.gyMean = gyStats.average() * 100;
  //message.gzMean = gzStats.average() * 100;
  gxStdev = gxStats.pop_stdev() * 180/PI;
  gyStdev = gyStats.pop_stdev() * 180/PI;
  gzStdev = gzStats.pop_stdev() * 180/PI;

  bool bit_gy[8] = {0,0,0,0,0,0,0,0};
  if (abs(gxStdev)>WarningValue) { bit_gy[7] = 1; }
  if (abs(gyStdev)>WarningValue) { bit_gy[6] = 1; } 
  if (abs(gzStdev)>WarningValue) { bit_gy[5] = 1; } 
  if (abs(gxStdev)>ErrorValue) { bit_gy[4] = 1; }
  if (abs(gyStdev)>ErrorValue) { bit_gy[3] = 1; } 
  if (abs(gzStdev)>ErrorValue) { bit_gy[2] = 1; }

  //if (abs(gxStdev)>ErrorValue) { bit_gy[1] = 1; } // extra bits available
  //if (abs(gxStdev)>ErrorValue) { bit_gy[0] = 1; } // extra bits available
  
  byte flag_g = 0;
  for (int i=0; i<8; i++) {
    flag_g |= bit_gy[i] << i;
  }
  message.gyroFlag = flag_g;

  // Clear statistics objects
  axStats.clear();
  ayStats.clear();
  azStats.clear();
  mxStats.clear();
  myStats.clear();
  mzStats.clear();
  gxStats.clear();
  gyStats.clear();
  gzStats.clear();
  batteryStats.clear();
}
