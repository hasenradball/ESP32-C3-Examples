#include <Arduino.h>
#include <WiFi.h>
#include <esp_sntp.h>

const char* WIFI_SSID = "YOUR_WIFI_SSID_HERE";
const char* WIFI_PASS = "YOUR_WIFI_PASS_HERE";

unsigned long lastUpdate = 0;

void sntp_sync_cb(struct timeval* tv) {
    Serial.print("Got new timestamp: ");
    Serial.print(tv->tv_sec);

    if (lastUpdate != 0) {
        unsigned long diff = (millis() - lastUpdate) / 1000;
        Serial.print(" Last update was ");
        Serial.print(diff);
        Serial.println(" seconds ago.");
    } else {
        Serial.println(" This was the first received timestamp.");
    }
    lastUpdate = millis();
}

void setupSNTP() {
    const uint32_t EVERY_15_SECONDS = 15000;
    sntp_set_sync_interval(EVERY_15_SECONDS);

    sntp_set_time_sync_notification_cb(sntp_sync_cb);

    const char* BERLIN_TIMEZONE = "CET-1CEST,M3.5.0,M10.5.0/3";
    const char* EUROPE_NTP_SERVER = "europe.pool.ntp.org";
    configTzTime(BERLIN_TIMEZONE, EUROPE_NTP_SERVER);
}

void setupWiFi() {
    Serial.print("Connecting Wifi");
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(100);
    }
    Serial.println("connected");
}

void setup() {
    Serial.begin(115200);
    setupWiFi();
    setupSNTP();
}

void loop() {
}