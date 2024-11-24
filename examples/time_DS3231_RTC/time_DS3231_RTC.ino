#include <Arduino.h>
#include <cWIFI.h>
#include "wifi_secrets.h"
#include <cMESZ.h>
#include <Wire.h>
#include <DS3231-RTC.h>

#define ESP32_C3_SDA 6
#define ESP32_C3_SCL 7

Wifi wifi{SSID, PASSWORD, HOSTNAME};
MESZ uhr{};
RTClib myRTC;
DS3231 Clock;

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
   char buffer[80];
   DateTime datetime = myRTC.now();
   Serial.print("\n\nTime of ESP:    ");
   uhr.get_timeformatted(buffer, sizeof(buffer), "%a %F %X - weekday %w; CW %W");
   Serial.println(buffer);
   Serial.print("Time of DS3231: ");
   datetime.strf_DateTime(buffer, sizeof(buffer), "%a %F %X - weekday %w; CW %W");
   Serial.print(buffer);
}

void setup() {
   Serial.begin(115200);
   while (!Serial) {
      yield();
   }
   delay(1000);

   Serial.print("\n\n=== ESP32-c3 Example to use MESZ (Mean European Summertime) ===\n\n");

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
   Serial.println("Time successfully syncronized!");
   MESZ::set_timeHasBeenSynchronized_false();
   auto syncTime_ms = sntp_get_sync_interval();
   Serial.printf("\tthe sntp sync intervall is set to : %ld ms / %ld s / %ld min / %ld h\n", \
   syncTime_ms, syncTime_ms/1000U, syncTime_ms/60000U, syncTime_ms/3600000U);

   Serial.printf("\nZeit: %s", uhr.get_asctime());

   // Now feed the Ds3231-RTC clock
   // feed UnixTimeStamp and don' t use localtime
   Clock.setEpoch(uhr.getTimeUnix(), true);
   // set to 24h
   Clock.setClockMode(false);

   // clock has actual time then dicconnect Wifi
   // to prevent from synchronizing
   wifi.Wifi_Disconnect();
   compareTimes();
}

void loop() {
   unsigned int ms{millis()};
   static unsigned int last_ms {ms};

   if (ms - last_ms >= 36000000U) {
      // print time from ESP and Time from DS3231-RTC
      compareTimes();
   }
}
