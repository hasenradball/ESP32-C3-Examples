#include <Arduino.h>
#include <Wire.h>
#include "cWIFI.h"
#include "DBG_Print.h"
#include "wifi_secrets.h"


//constexpr int SDA1_PIN {1};
//constexpr int SCL1_PIN {2};
//constexpr int SDA2_PIN {8};
//constexpr int SCL2_PIN {9};


Wifi wifi{SSID, PASSWORD, HOSTNAME};

bool is_connected{false};

void setup() {
   DBG__SERIALBEGIN();
   delay(1000);
   DBG__PRINT(F("\n\nESP32-C3 Wifi Test\n\n"));

   //Wire.setPins(SDA1_PIN, SCL1_PIN);
   //Wire.begin();
   //Wire1.setPins(SDA2_PIN, SCL2_PIN);
   //Wire1.begin();

   // ==> set WiFi events
   // define wifi event when StA connected
   WiFi.onEvent([] (WiFiEvent_t event, WiFiEventInfo_t info) -> void {
      DBG__PRINT("\n\nSTA connected!");
      DBG__PRINT("STA connected to: ", "");   
      DBG__PRINT(reinterpret_cast<const char*>(info.wifi_sta_connected.ssid));
      is_connected = true;
  }, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_CONNECTED);
  
  // define wifi event when disconnected
  WiFi.onEvent([] (WiFiEvent_t event, WiFiEventInfo_t info) -> void {
      DBG__PRINT("\n\nSTA disconnected!");
      DBG__PRINT("STA disconnected reason: ", "");
      DBG__PRINT(info.wifi_sta_disconnected.reason);
      is_connected = false;
      //digitalWrite(LED_R, HIGH);
      //digitalWrite(LED_Y, LOW);
      //digitalWrite(LED_G, LOW);
  }, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
  
  // define wifi event when got ip
  WiFi.onEvent([] (WiFiEvent_t event, WiFiEventInfo_t info) -> void {
      DBG__PRINT("\n\nGot IP address!");
      DBG__PRINT("IP address: ", "");
      DBG__PRINT(IPAddress(info.got_ip.ip_info.ip.addr));
      String str {WiFi.localIP().toString()};
  }, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);

   // ==> Wifi start
   wifi.Wifi_Connect();
}

void loop() {
   dprintf("Wifi status : %d\n", is_connected);
   delay(10000);
}