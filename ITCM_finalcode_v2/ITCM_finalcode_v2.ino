/*
  Title:    Cryologger - Ice-Tethered Current Meter
  Date:     April 6, 2020 
  Author:   Adam Garbo
  
  Description:
  - An ice tethered current meter intended for deployment in Arctic Bay, Nunavut.
  - Current measurements are conducted every 30 minutes for 2 minutes at 1-second intervals.
  - Data is transmitted via the Iridium satellite network every 4 hours.
  
  Components:
  - Adafruit Feather M0 Proto
  - Adafruit Ultimate GPS Featherwing
  - Rock Seven RockBLOCK 9603
  - Maxtena M1621HCT-P-SMA Iridium antenna
  - Adafruit Precision NXP 9-DOF Breakout Board
  - Adafruit Lithium Ion Battery Pack - 3.7V 6600mAh
  
  Comments:
  - Code is ready for testing once IMU functionality is added. Updated: April 28,2020
*/

// Libraries
#include <Adafruit_Sensor.h>      // https://github.com/adafruit/Adafruit_Sensor
#include <Adafruit_FXAS21002C.h>  // https://github.com/adafruit/Adafruit_FXAS21002C
#include <Adafruit_FXOS8700.h>    // https://github.com/adafruit/Adafruit_FXOS8700
#include <Arduino.h>              // https://github.com/arduino/ArduinoCore-samd (required before wiring_private.h)
#include <ArduinoLowPower.h>      // https://github.com/arduino-libraries/ArduinoLowPower
#include <IridiumSBD.h>           // https://github.com/PaulZC/IridiumSBD
#include <math.h>                 // https://www.nongnu.org/avr-libc/user-manual/group__avr__math.html
#include <RTCZero.h>              // https://github.com/arduino-libraries/RTCZero
#include <LSM303.h>               // https://github.com/pololu/lsm303-arduino
#include <Statistic.h>            // https://github.com/RobTillaart/Arduino/tree/master/libraries/Statistic
#include <TimeLib.h>              // https://github.com/PaulStoffregen/Time
#include <TinyGPS++.h>            // https://github.com/mikalhart/TinyGPSPlus
#include <Wire.h>                 // https://www.arduino.cc/en/Reference/Wire
#include <wiring_private.h>       // https://github.com/arduino/ArduinoCore-samd/blob/master/cores/arduino/wiring_private.h (required for pinPeripheral() function)

// Pin definitons
#define GPS_EN_PIN            A5  // GPS enable pin
#define ROCKBLOCK_RX_PIN      10  // RockBLOCK 9603 RX pin
#define ROCKBLOCK_TX_PIN      11  // RockBLOCK 9603 TX pin
#define ROCKBLOCK_SLEEP_PIN   12  // RockBLOCK 9603 sleep pin
#define VBAT_PIN              A7  // Battery voltage measurement pin A7

// Debugging constants
#define DEBUG         true    // Output debug messages to Serial Monitor
#define DEBUG_GPS     false   // Echo NMEA sentences to Serial Monitor OLD = false
#define DIAGNOSTICS   true    // Output Iridium diagnostic messages to Serial Monitor
#define DEPLOY        false    // Disable debugging messages for deployment

// Create a new Serial/UART instance, assigning it to pins 10 and 11
// For more information see: https://www.arduino.cc/en/Tutorial/SamdSercom
Uart Serial2 (&sercom1, ROCKBLOCK_RX_PIN, ROCKBLOCK_TX_PIN, SERCOM_RX_PAD_2, UART_TX_PAD_0);
#define IridiumSerial Serial2
#define GpsSerial Serial1

// Attach the interrupt handler to the SERCOM
void SERCOM1_Handler() {
  Serial2.IrqHandler();
}

// Object instantiations
Adafruit_FXOS8700   accelmag  = Adafruit_FXOS8700(0x8700A, 0x8700B);
Adafruit_FXAS21002C gyro      = Adafruit_FXAS21002C(0x0021002C);
RTCZero             rtc;
IridiumSBD          modem(IridiumSerial, ROCKBLOCK_SLEEP_PIN);
TinyGPSPlus         gps;

// User defined global variable declarations
int           ErrorValue              = 5;     // dps for Gyroscope
int           WarningValue            = 2;     // dps for Gyroscope
unsigned long alarmInterval           = 300;   // Alarm sleep duration (Default: 1800 seconds)
unsigned int  sampleInterval          = 120;    // Sampling duration of current tilt measurements (Default: 120 seconds)
byte          sampleFrequency         = 1;      // Sampling frequency of current tilt measurements (Default: 1 second)
byte          transmitInterval        = 3;      // Number of messages sent in each Iridium transmission (340-byte limit)
byte          maxRetransmitCounter    = 1;      // Number of failed messages to reattempt in each Iridium transmission (340-byte limit)
byte          maxFixCounter           = 10;     // Minimum number of acquired GPS fixes

// Global variable and constant declarations
volatile bool alarmFlag               = false;  // Flag for alarm interrupt service routine
volatile byte watchdogCounter         = 0;      // Watchdog Timer trigger counter
bool          ledState                = LOW;    // Flag to toggle LED in blinkLed() function
bool          rtcFlag                 = false;  // Flag to indicate if RTC has been set
byte          gyroFlag                = 0;      // Gyroscope flag
byte          resetFlag               = 0;      // Flag to force Watchdog Timer system reset
byte          transmitBuffer[340]     = {};     // RockBLOCK 9603 transmission buffer
unsigned int  messageCounter          = 0;      // RockBLOCK 9603 transmitted message counter
unsigned int  retransmitCounter       = 0;      // RockBLOCK 9603 failed data transmission counter
unsigned int  transmitCounter         = 0;      // RockBLOCK 9603 transmission interval counter
unsigned long previousMillis          = 0;      // Global millis() timer variable
unsigned long unixtime                = 0;      // UNIX Epoch time variable
unsigned long alarmTime               = 0;      // Epoch alarm time variable
tmElements_t  tm;
sensors_event_t aEvent, mEvent, gEvent;         // Internal measurement unit variables

// Statistics objects
Statistic batteryStats;
Statistic axStats;
Statistic ayStats;
Statistic azStats;
Statistic mxStats;
Statistic myStats;
Statistic mzStats;
Statistic gxStats;
Statistic gyStats;
Statistic gzStats;

// Structure and union to store and send data byte-by-byte via RockBLOCK
typedef union {
  struct {
    uint32_t  unixtime;           // Unix epoch time                (4 bytes)
    //int16_t   temperature;        // Temperature                    (2 bytes)
    int16_t   axMean;             // Accelerometer x                (2 bytes)
    int16_t   ayMean;             // Accelerometer y                (2 bytes)
    int16_t   azMean;             // Accelerometer z                (2 bytes)
    int16_t   axStdev;            // Accelerometer x                (2 bytes)
    int16_t   ayStdev;            // Accelerometer y                (2 bytes)
    int16_t   azStdev;            // Accelerometer z                (2 bytes)
    int16_t   mxMean;             // Magnetometer x                 (2 bytes)
    int16_t   myMean;             // Magnetometer y                 (2 bytes)
    int16_t   mzMean;             // Magnetometer z                 (2 bytes)
    int8_t    gyroFlag;           // Gyroscope flag                 (1 byte)
    int32_t   latitude;           // Latitude                       (4 bytes)
    int32_t   longitude;          // Longitude                      (4 bytes)
    uint16_t  voltage;            // Battery voltage (mV)           (2 bytes)
    uint16_t  transmitDuration;   // Previous transmission duration (2 bytes)
    uint16_t  messageCounter;     // Message counter                (2 bytes)
  } __attribute__((packed));                                        // Total: 39 bytes (w/o temp 37 bytes)
  uint8_t bytes[37]; // default:39  To do: Look into flexible arrays in structures
} SBDMESSAGE;

SBDMESSAGE message;
size_t messageSize = sizeof(message); // Size (in bytes) of data message to be transmitted
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Setup
void setup() {

  // Pin assignments
  pinMode(GPS_EN_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(VBAT_PIN, INPUT);
  digitalWrite(GPS_EN_PIN, HIGH);
  digitalWrite(LED_BUILTIN, LOW);

  // Start Serial at 115200 baud
  Serial.begin(115200);
  //while (!Serial); // Wait for user to open Serial Monitor
  delay(5000); // Delay to allow user to open Serial Monitor

  // Watchdog Timer Configuration
  configureWatchdog();

  Serial.println(F("---------------------------------------"));
  Serial.println(F("Cryologger - Ice Tethered Current Meter"));
  Serial.println(F("---------------------------------------"));

  // I2C Configuration
  Wire.begin();           // Initialize I2C bus
  Wire.setClock(100000);  // Set I2C clock speed to 100kHz

  // Analog-to-digital converter (ADC) Configuration
  analogReadResolution(12); // Change the ADC resolution to 12 bits

  // Inertial Measurement Unit (IMU) Configuration
  if (accelmag.begin(ACCEL_RANGE_2G)) {
    Serial.println("FXOS8700 initialized.");
  }
  else {
    Serial.println("Warning: FXOS8700 not detected. Please check wiring.");
    while (1);
  }

  if (gyro.begin()) {
    Serial.println("FXAS21002C initialized.");
  }
  else {
    Serial.println("Warning: FXAS21002C not detected. Please check wiring.");
    while (1);
  }

  // RockBLOCK 9603 Configuration
  if (modem.isConnected()) {
    modem.setPowerProfile(IridiumSBD::DEFAULT_POWER_PROFILE); // Assume battery power
    modem.adjustSendReceiveTimeout(120); // Default = 300 seconds
    modem.adjustATTimeout(20);
    Serial.println(F("RockBLOCK 9603 intialized."));
  }
  else {
    Serial.println(F("Warning: RockBLOCK 9603N not detected. Please check wiring."));
  }

  // Real-time clock (RTC) Configuration
  /*
    Alarm matches:
    MATCH_OFF          = Never
    MATCH_SS           = Every Minute
    MATCH_MMSS         = Every Hour
    MATCH_HHMMSS       = Every Day
    MATCH_DHHMMSS      = Every Month
    MATCH_MMDDHHMMSS   = Every Year
    MATCH_YYMMDDHHMMSS = Once, on a specific date and time
  */
  rtc.begin();                      // Initialize the RTC
  Serial.println(F("RTC initialized."));
  Serial.println(F("Syncing RTC date and time with GPS..."));
  readGps();                        // Set the RTC's date and time from GPS
  rtc.setAlarmTime(0, 0, 0);        // Set alarm
#if DEBUG
  rtc.enableAlarm(rtc.MATCH_SS);  // Set alarm to seconds match
  //rtc.enableAlarm(rtc.MATCH_MMSS);  // Set alarm to seconds and minutes match
#else if DEPLOY
  rtc.enableAlarm(rtc.MATCH_MMSS);  // Set alarm to seconds and minutes match
#endif
  rtc.attachInterrupt(alarmMatch);  // Attach alarm interrupt

  // Print RTC's alarm date and time
  Serial.print(F("Next alarm: ")); printAlarm();

  // Print operating mode
  Serial.print(F("Mode: "));
#if DEBUG
  Serial.println(F("DEBUG"));
#else if DEPLOY
  Serial.println(F("DEPLOY"));
#endif

  // Print current date and time
  Serial.print(F("Datetime: ")); printDateTime();

  // Blink LED to indicate setup has completed
  blinkLed(10, 100);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop() {

  // Check if alarm interrupt service routine was triggered
  if (alarmFlag) {

    // Read the RTC
    readRtc();
    printUnion();
    printUnionBinary();
    // Pet the Watchdog Timer
    petDog();

    // Read the battery voltage
    readBattery();

    // Print date and time
    Serial.print("Alarm trigger: "); printDateTime();

    // Perform current measurements
    for (int i = 0; i < sampleInterval; i++) {
      petDog(); // Pet the Watchdog Timer
      readImu(); // Read IMU
#if DEBUG
      blinkLed(1, 100);
#else if DEPLOY
      // Go to sleep to reduce power consumption
      // To do: Investigate why the sleep duration appears to be double
      // (e.g. 1000 ms sleep = 2000 ms on the oscilloscope)
      LowPower.sleep(500);
#endif
    }

    // Read GPS
    readGps();

    // Perform statistics on measurements
    printStatistics();
    calculateStatistics();

    // Write data to buffer
    writeBuffer();

    // Check if data should be transmitted
    if (transmitCounter == transmitInterval) {
      transmitData();       // Transmit data
      transmitCounter = 0;  // Reset transmit counter
    }

    // Set the RTC alarm
    alarmTime = unixtime + alarmInterval; // Calculate next alarm

    // Check if alarm was set in the past
    if (alarmTime <= rtc.getEpoch()) {
      Serial.println(F("Warning! Alarm set in the past."));
      alarmTime = rtc.getEpoch() + alarmInterval; // Calculate new alarm
      rtc.setAlarmTime(0, 0, 0);  // Set alarm to next hour rollover
      rtc.enableAlarm(rtc.MATCH_MMSS); // Set alarm to seconds and minutes match
    }
    else {
      rtc.setAlarmTime(hour(alarmTime), minute(alarmTime), 0);
      rtc.setAlarmDate(day(alarmTime), month(alarmTime), year(alarmTime) - 2000);
      rtc.enableAlarm(rtc.MATCH_MMDDHHMMSS); // Set alarm to seconds, minutes, hours, day, and month match
    }
    rtc.attachInterrupt(alarmMatch);

    Serial.print(F("Next alarm: ")); printAlarm();
  }
  alarmFlag = false; // Clear alarm interrupt service routine flag
 // printUnionBinary();
  
#if DEBUG
  blinkLed(1, 500);
#endif

#if DEPLOY
  blinkLed(1, 10);
  LowPower.deepSleep(); // Enter deep sleep
#endif
}
