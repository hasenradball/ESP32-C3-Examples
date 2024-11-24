#include <Arduino.h>
#include <cWIFI.h>
#include "wifi_secrets.h"
#include <cMESZ.h>

Wifi wifi{SSID, PASSWORD, HOSTNAME};
MESZ uhr{};

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

void setup() {
   Serial.begin(115200);
   while (!Serial) {
      yield();
   }
   delay(1000);

   Serial.print("\n\n=== ESP32-c3 Example to use MESZ (Mean European Summertime) ===\n\n");

   if (wifi.Wifi_Connect()) {
      // init clock
      setupTimeConfigurationAndInit();
   }

   // wait if SNTP has synchronized internal clock
   while (!MESZ::get_time_synchronized()) {
      yield();
   }
   Serial.println("Time successfully syncronized!\n");
   MESZ::set_time_synchronized_false();
   auto syncTime_ms = sntp_get_sync_interval();
   Serial.printf("the sntp sync intervall is set to : %ld ms / %ld s / %ld min / %ld h", \
   syncTime_ms, syncTime_ms/1000U, syncTime_ms/60000U, syncTime_ms/3600000U);

   Serial.printf("\nZeit: %s", uhr.get_asctime());
}


void loop() {
   if (MESZ::get_time_synchronized()) {
      Serial.println("Time successfully syncronized!\n");
      MESZ::set_time_synchronized_false();
      Serial.printf("\n Zeit: %s", uhr.get_asctime());
   }
}
