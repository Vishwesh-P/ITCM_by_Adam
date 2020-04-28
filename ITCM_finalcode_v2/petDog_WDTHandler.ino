// Pet the Watchdog Timer NC
void petDog() {
  watchdogCounter = 0;              // Clear Watchdog Timer trigger counter
  WDT->CLEAR.bit.CLEAR = 0xA5;      // Clear the Watchdog Timer and restart time-out period //REG_WDT_CLEAR = WDT_CLEAR_CLEAR_KEY;
  while (WDT->STATUS.bit.SYNCBUSY); // Await synchronization of registers between clock domains
}

// Watchdog Timer interrupt service routine
void WDT_Handler() {
  // Permit a limited number of Watchdog Timer triggers before forcing system reset.
  if (watchdogCounter < 10) {
    WDT->INTFLAG.bit.EW = 1;          // Clear the Early Warning interrupt flag //REG_WDT_INTFLAG = WDT_INTFLAG_EW;
    WDT->CLEAR.bit.CLEAR = 0xA5;      // Clear the Watchdog Timer and restart time-out period //REG_WDT_CLEAR = WDT_CLEAR_CLEAR_KEY;
    while (WDT->STATUS.bit.SYNCBUSY); // Await synchronization of registers between clock domains
  }
  else {
    while (true);                     // Force Watchdog Timer to reset the system
  }
}
