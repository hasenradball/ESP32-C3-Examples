#ifndef _CWIFI_H_
#define _CWIFI_H_

#include <Arduino.h>

#ifdef ESP8266
#include <ESP8266WiFi.h>
#endif
#ifdef ESP32
#include <WiFi.h>
#endif


class Wifi {
  private:
    const char *_ssid;
    const char *_password;
    const char *_ip;
    const char *_gate;
    const char *_subnet;
    const char *_dns1;
    const char *_hostname {nullptr};

    void printWiFiInfo(void) const;
    bool Wifi_Start(void);
    bool Wifi_STA_DynIP_Start(void);
    bool Wifi_STA_StaticIP_Start(void);


  public:
    Wifi(const char *ssid, const char *password);
    Wifi(const char *ssid, const char *password, const char *hostname);
    Wifi(const char *ssid, const char *password, const char *ip, const char *gate, const char *subnet, const char *dns1);
    Wifi(const char *ssid, const char *password, const char *ip, const char *gate, const char *subnet, const char *dns1, const char *hostname);

    wl_status_t Wifi_Status(void) const;
    bool Wifi_Connect(bool static_ip = false);
    bool Wifi_AP_Start(void);
    bool Wifi_Stop(void);
    
    bool IsConnected(void) const {return WiFi.isConnected();};
    int32_t Wifi_rssi(void) const {return WiFi.RSSI();};
    
    bool Wifi_Disconnect(bool setWifiOff, bool eraseCredentials) {
      return WiFi.disconnect(setWifiOff, eraseCredentials);
    };
    
};

#endif
