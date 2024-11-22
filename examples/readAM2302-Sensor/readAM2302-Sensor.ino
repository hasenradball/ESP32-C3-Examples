#include <Arduino.h>
#include <WiFi.h>
#include <DBG_Print.h>
#include <AM2302-Sensor.h>

constexpr unsigned int SENSOR_PIN {0U};

AM2302::AM2302_Sensor am2302{SENSOR_PIN};

void setup() {
   WiFi.mode(WIFI_OFF);
   DBG__SERIALBEGIN();
   delay(1000);
   DBG__PRINT(F("\n\nESP32-C3 AM2302-Sensor\n\n"));

   // set pin and check for sensor
   if (am2302.begin()) {
      // this delay is needed to receive valid data,
      // when the loop directly read again
      delay(3000);
   }
   else {
      while (true) {
      DBG__PRINT("Error: sensor check. => Please check sensor connection!");
      delay(10000);
      }
   }
}

void loop() {
   auto status = am2302.read();
   dprintf("\n\nstatus of sensor read(): %d\n", status);

   dprintf("\tTemperature : %5.2f °C\n", am2302.get_Temperature());

   dprintf("\tHumidity    : %5.1f %%\n", am2302.get_Humidity());
   delay(10000);
}