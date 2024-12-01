/*
 * pin 1 - not used          |  Micro SD card     |
 * pin 2 - CS (SS)           |                   /
 * pin 3 - DI (MOSI)         |                  |__
 * pin 4 - VDD (3.3V)        |                    |
 * pin 5 - SCK (SCLK)        | 8 7 6 5 4 3 2 1   /
 * pin 6 - VSS (GND)         | ▄ ▄ ▄ ▄ ▄ ▄ ▄ ▄  /
 * pin 7 - DO (MISO)         | ▀ ▀ █ ▀ █ ▀ ▀ ▀ |
 * pin 8 - not used          |_________________|
 *                             ║ ║ ║ ║ ║ ║ ║ ║
 *                     ╔═══════╝ ║ ║ ║ ║ ║ ║ ╚═════════╗
 *                     ║         ║ ║ ║ ║ ║ ╚══════╗    ║
 *                     ║   ╔═════╝ ║ ║ ║ ╚═════╗  ║    ║
 * Connections for     ║   ║   ╔═══╩═║═║═══╗   ║  ║    ║
 * full-sized          ║   ║   ║   ╔═╝ ║   ║   ║  ║    ║
 * SD card             ║   ║   ║   ║   ║   ║   ║  ║    ║
 * Pin name         |  -  DO  VSS SCK VDD VSS DI CS    -  |
 * SD pin number    |  8   7   6   5   4   3   2   1   9 /
 *                  |                                  █/
 *                  |__▍___▊___█___█___█___█___█___█___/
 *
 * Note:  The SPI pins can be manually configured by using `SPI.begin(sck, miso, mosi, cs).`
 *        Alternatively, you can change the CS pin and use the other default settings by using `SD.begin(cs)`.
 *
 * +--------------+---------+-------+----------+----------+----------+----------+----------+
 * | SPI Pin Name | ESP8266 | ESP32 | ESP32‑S2 | ESP32‑S3 | ESP32‑C3 | ESP32‑C6 | ESP32‑H2 |
 * +==============+=========+=======+==========+==========+==========+==========+==========+
 * | CS (SS)      | GPIO15  | GPIO5 | GPIO34   | GPIO10   | GPIO7    | GPIO18   | GPIO0    |
 * +--------------+---------+-------+----------+----------+----------+----------+----------+
 * | DI (MOSI)    | GPIO13  | GPIO23| GPIO35   | GPIO11   | GPIO6    | GPIO19   | GPIO25   |
 * +--------------+---------+-------+----------+----------+----------+----------+----------+
 * | DO (MISO)    | GPIO12  | GPIO19| GPIO37   | GPIO13   | GPIO5    | GPIO20   | GPIO11   |
 * +--------------+---------+-------+----------+----------+----------+----------+----------+
 * | SCK (SCLK)   | GPIO14  | GPIO18| GPIO36   | GPIO12   | GPIO4    | GPIO21   | GPIO10   |
 * +--------------+---------+-------+----------+----------+----------+----------+----------+
 *
 * For more info see file README.md in this library or on URL:
 * https://github.com/espressif/arduino-esp32/tree/master/libraries/SD
 */

#include <Arduino.h>
#include <cWIFI.h>
#include "wifi_secrets.h"
#include <cMESZ.h>
#include <Wire.h>
#include <DS3231-RTC.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"

#define ESP32_C3_SDA    6
#define ESP32_C3_SCL    7

#define ESP32_C3_SCK    8
#define ESP32_C3_MISO   9
#define ESP32_C3_MOSI  10
#define ESP32_C3_CS     5

Wifi wifi{SSID, PASSWORD, HOSTNAME};
MESZ uhr{};
RTClib myRTC;
DS3231 Clock;

const char *filename {"/time.log"};

void writeToSd(fs::FS &fs, const char *path, const char *message, const char *mode = FILE_WRITE) {
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, mode);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if (file.println(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

void printSdCardType() {
   uint8_t cardType = SD.cardType();
   if (cardType == CARD_NONE) {
     Serial.println("No SD card attached");
     return;
  }

  Serial.print("SD Card Type: ");
  if (cardType == CARD_MMC) {
    Serial.println("MMC");
  } else if (cardType == CARD_SD) {
    Serial.println("SDSC");
  } else if (cardType == CARD_SDHC) {
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }
}

void printSdCardSize() {
   uint64_t cardSize = SD.cardSize() / (1024 * 1024);
   Serial.printf("SD Card Size: %llu MB\n\n", cardSize);
}

/**
 * @brief setup TimeZone, Servers and Init clock
 * 
 */
void setupTimeConfigurationAndInit(void) {
  Serial.println(F(">>> Init MESZ"));
  uhr.setTimeZone("CET-1CEST,M3.5.0,M10.5.0/3");
  uhr.setTimeServers("192.168.178.1");
  uhr.init();
}

void compareTimes() {
   File file = SD.open(filename, FILE_APPEND);
   if (!file) {
      Serial.println("Failed to open file for writing");
   }
   char buffer[80];
   DateTime datetime = myRTC.now();
   Serial.print("\n\nTime of ESP:    ");
   file.print("\n\nTime of ESP:    ");
   uhr.get_timeformatted(buffer, sizeof(buffer), "%a %F %X - weekday %w; CW %W");
   Serial.println(buffer);
   file.println(buffer);
   Serial.print("Time of DS3231: ");
   file.print("Time of DS3231: ");
   datetime.strf_DateTime(buffer, sizeof(buffer), "%a %F %X - weekday %w; CW %W");
   Serial.print(buffer);
   file.println(buffer);
   file.close();
}

// ==========================
// ===       setup()      ===
// ==========================

void setup() {
   Serial.begin(115200);
   while (!Serial) {
      yield();
   }
   delay(2000);
   Serial.print(F("\n\n=== ESP32-C3 Example copare RTCs with SDcard logging ===\n\n"));

   SPI.begin(ESP32_C3_SCK, ESP32_C3_MISO, ESP32_C3_MOSI, ESP32_C3_CS);
   if (!SD.begin(ESP32_C3_CS)) {
      Serial.println(F("SDcard Mount Failed"));
      return;
   }
   else {
      printSdCardType();
      printSdCardSize();
   }

   writeToSd(SD, filename, "Clear SDcard!");
   
   File myFile = SD.open(filename, FILE_APPEND);
   if (!myFile) {
      Serial.println(F("File object is invalid after FileOpen"));
   }

   myFile.print("\n\n=== ESP32-C3 Example copare RTCs with SDcard logging ===\n\n");

   // setup wire
   Wire.setPins(ESP32_C3_SDA, ESP32_C3_SCL);
   Wire.begin();

   if (wifi.Wifi_Connect()) {
      // init clock
      setupTimeConfigurationAndInit();
   }

   // wait if SNTP has synchronized internal clock
   while (!MESZ::get_timeHasBeenSynchronized()) {
      yield();
   }
   Serial.println(F("Time successfully synchronized!"));
   myFile.println(F("Time successfully synchronized!"));
   
   MESZ::set_timeHasBeenSynchronized_false();
   auto syncTime_ms = sntp_get_sync_interval();
   
   Serial.printf("\tthe sntp sync interval is set to : %ld ms / %ld s / %ld min / %ld h\n", \
   syncTime_ms, syncTime_ms/1000U, syncTime_ms/60000U, syncTime_ms/3600000U);
   
   myFile.printf("\tthe sntp sync interval is set to : %ld ms / %ld s / %ld min / %ld h\n", \
   syncTime_ms, syncTime_ms/1000U, syncTime_ms/60000U, syncTime_ms/3600000U);

   Serial.printf("\nZeit: %s", uhr.get_asctime());

   // Now feed the Ds3231-RTC clock
   // feed UnixTimeStamp and don' t use localtime
   Clock.setEpoch(uhr.getTimeUnix(), true);
   // set to 24h
   Clock.setClockMode(false);

   // clock has actual time, then dicconnect Wifi
   // and switch wifi off to prevent from synchronizing
   if (wifi.Wifi_Disconnect(true)) {
      Serial.println("\nWifi disconnected");
      myFile.println("\nWifi disconnected");
   }
   myFile.close();

   while(uhr.getSecond()) {
      yield();
   }
   compareTimes();
}

// ==========================
// ===       loop()      ===
// ==========================
void loop() {
   unsigned int ms{millis()};
   static unsigned int last_ms {ms};

   if (ms - last_ms >= 3600000U) {
      // print time from ESP and Time from DS3231-RTC
      last_ms = ms;
      compareTimes();
   }
}
