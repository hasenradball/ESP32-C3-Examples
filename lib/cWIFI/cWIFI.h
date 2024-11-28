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
    
#ifdef ESP8266
  /**
   * @brief wifi disconnect function
   * 
   * @return true 
   * @return false 
   */
    bool Wifi_Disconnect() {return WiFi.disconnect()};
#endif

#ifdef ESP32
  /**
   * @brief wifi disconnect function
   * 
   * @param setWifiOff set mode to WIFI_OFF
   * @param eraseCredentials delete credentials
   * @return true 
   * @return false 
   */
    bool Wifi_Disconnect(bool setWifiOff = false, bool eraseCredentials = false) {
      return WiFi.disconnect(setWifiOff, eraseCredentials);
    };
#endif
};

#endif
