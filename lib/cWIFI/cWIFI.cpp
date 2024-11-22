/*
  Projekt: Wifi Klasse als Wrapper für die ESP8266 WiFi Klasse
  Microcontroller: ESP8266-01
  Date: 18.11.2018
  Issuer: Frank Häfele


*/

#include "cWIFI.h"
#include "DBG_Print.h"

/**
 * @brief Construct a new Wifi object given by all parameters
 * 
 * @param ssid 
 * @param password 
 * @param ip 
 * @param gate 
 * @param subnet 
 * @param dns1 
 * @param hostname 
 */
Wifi::Wifi(const char *ssid, const char *password, const char *ip, const char *gate, const char *subnet, const char *dns1, const char *hostname) :
   _ssid {ssid},
  _password {password},
  _ip {ip},
  _gate {gate},
  _subnet {subnet},
  _dns1 {dns1},
  _hostname {hostname}
{
#ifdef ESP8266
  WiFi.setAutoConnect(false);
#endif
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  //Wifi_Disconnect();
}

/**
 * @brief Construct a new Wifi object given only by ssid and password
 * 
 * @param ssid 
 * @param password 
 */
Wifi::Wifi(const char *ssid, const char *password) : Wifi {ssid, password, nullptr, nullptr, nullptr, nullptr, nullptr}
{}

/**
 * @brief Construct a new Wifi object given only by ssid, password and hostname
 * 
 * @param ssid 
 * @param password 
 * @param hostname 
 */
Wifi::Wifi(const char *ssid, const char *password, const char *hostname) : Wifi {ssid, password, nullptr, nullptr, nullptr, nullptr, hostname}
{}


/**
 * @brief WiFi Start with last credentials when WiFi.persistent(true)
 * 
 * @return true 
 * @return false 
 */
bool Wifi::Wifi_Start() {
  bool isconnected {false};
#ifdef ESP32
  if (_hostname != nullptr) WiFi.setHostname(_hostname);
  DBG__PRINT("Set WiFi mode to WIFI_STA - ", WiFi.mode(WIFI_STA));
#elif ESP8266
  DBG__PRINT("Set WiFi mode to WIFI_STA - ", WiFi.mode(WIFI_STA));
  if (_hostname != nullptr) WiFi.hostname(_hostname);
#endif
  WiFi.begin();
  // Warte auf Verbindung
  if(WiFi.waitForConnectResult() == WL_CONNECTED) {
    DBG__PRINT("\n Connection with Wifi_Start() - successful <<<");
    printWiFiInfo();
    isconnected = true;
  }
  else {
    DBG__PRINT("\n Connection with Wifi_Start() - failed <<<");
    Wifi_Status();
    
  }
  DBG__PRINT("");
  return isconnected;
}

/**
 * @brief WiFi Start using dynamic IP with credentials set by ssid, password
 * 
 * @return true 
 * @return false 
 */
bool Wifi::Wifi_STA_DynIP_Start() {
  bool isconnected {false};
#ifdef ESP32
  if (_hostname != nullptr) WiFi.setHostname(_hostname);
  DBG__PRINT("Set WiFi mode to WIFI_STA - ", WiFi.mode(WIFI_STA));
#elif ESP8266
  DBG__PRINT("Set WiFi mode to WIFI_STA - ", WiFi.mode(WIFI_STA));
  if (_hostname != nullptr) WiFi.hostname(_hostname);
#endif
  // Warte auf Verbindung
  WiFi.begin(_ssid, _password);
  if(WiFi.waitForConnectResult() == WL_CONNECTED) {
    DBG__PRINT("\n Connection with Wifi_STA_DynIP_Start() - successful <<<");
    printWiFiInfo();
    isconnected = true;
  }
  else {
    DBG__PRINT("\n Connection with Wifi_STA_DynIP_Start() - failed <<<");
    Wifi_Status();
  }
  DBG__PRINT("");
  return isconnected;
}

/**
 * @brief  WiFi Start using staitc IP with credentials set by ssid, password
 * 
 * @return true 
 * @return false 
 */
bool Wifi::Wifi_STA_StaticIP_Start() {
  bool isconnected {false};
  IPAddress staticIP;
  IPAddress gateway;
  IPAddress subnetmask;
  IPAddress dns_server1;
  staticIP.fromString(_ip);
  gateway.fromString(_gate);
  subnetmask.fromString(_subnet);
  dns_server1.fromString(_dns1);
  
#ifdef ESP32
  if (_hostname != nullptr) WiFi.setHostname(_hostname);
  DBG__PRINT("Set WiFi mode to WIFI_STA - ", WiFi.mode(WIFI_STA));
  DBG__PRINT("Set Wifi Configuration - ", WiFi.config(staticIP, gateway, subnetmask, dns_server1));
#elif ESP8266
  DBG__PRINT("Set WiFi mode to WIFI_STA - ", WiFi.mode(WIFI_STA));
  DBG__PRINT("Set Wifi Configuration - ", WiFi.config(staticIP, gateway, subnetmask, dns_server1));
  if (_hostname != nullptr) WiFi.hostname(_hostname);
#endif
  WiFi.begin(_ssid, _password);
  // Warte auf Verbindung
  if(WiFi.waitForConnectResult() == WL_CONNECTED) {
    DBG__PRINT(F("\n Connection with Wifi_STA_StaticIP_Start() - successful <<<"));
    printWiFiInfo();
    isconnected = true;
  }
  else {
    DBG__PRINT(F("\n Connection with Wifi_STA_StaticIP_Start() - failed <<<"));
    Wifi_Status();
  }
  DBG__PRINT("");
  return isconnected;
}

/**
 * @brief method which connects to wifi trying by several connect methods
 * 
 * @param static_ip 
 * @return true 
 * @return false 
 */
bool Wifi::Wifi_Connect(bool static_ip) {
  if (IsConnected()) return true;
  else if (!static_ip) {
    if (Wifi_Start()) return true;
    else if(Wifi_STA_DynIP_Start()) return true;
  }
  else {
    if(Wifi_STA_StaticIP_Start()) return true;
  }
  return false;
}


/**
 * @brief gets the actual wifi status
 * 
 * @return wl_status_t 
 */
wl_status_t Wifi::Wifi_Status(void) const {
  // keep in mind
  /*
    WL_NO_SHIELD        = 255,   // for compatibility with WiFi Shield library
    WL_IDLE_STATUS      = 0,
    WL_NO_SSID_AVAIL    = 1,
    WL_SCAN_COMPLETED   = 2,
    WL_CONNECTED        = 3,
    WL_CONNECT_FAILED   = 4,
    WL_CONNECTION_LOST  = 5,
    WL_WRONG_PASSWORD   = 6,
    WL_DISCONNECTED     = 7
  */
  wl_status_t status = WiFi.status();
  if (status == WL_NO_SHIELD) {
    DBG__PRINT(F("\n WiFI.status =  NO_SHIELD"));
  }
  else if (status == WL_IDLE_STATUS) {
    DBG__PRINT(F("\n WiFI.status =  IDLE_STATUS"));
  }
  else if (status == WL_NO_SSID_AVAIL) {
    DBG__PRINT(F("\n WiFI.status =  NO_SSID_AVAIL"));
  }
  else if (status == WL_SCAN_COMPLETED) {
    DBG__PRINT(F("\n WiFI.status =  SCAN_COMPLETED"));
  }
  else if (status == WL_CONNECTED) {
    DBG__PRINT(F("\n WiFI.status =  CONNECTED"));
  }
  else if (status == WL_CONNECT_FAILED) {
    DBG__PRINT(F("\n WiFI.status =  CONNECT_FAILED"));
  }
  else if (status == WL_CONNECTION_LOST) {
    DBG__PRINT(F("\n WiFI.status =  CONNECTION_LOST"));
  }
#ifndef ESP32
  else if (status == WL_WRONG_PASSWORD) {
    DBG__PRINT(F("\n WiFI.status =  WRONG_PASSWORD"));
  }
#endif
  else if (status == WL_DISCONNECTED) {
    DBG__PRINT(F("\n WiFI.status =  DISCONNECTED"));
  }
  else {
     DBG__PRINT(F("\n No appropriate Status available!"));
  }
  return status;
}


/**
 * @brief start of wifi access point
 * 
 * @return true 
 * @return false 
 */
bool Wifi::Wifi_AP_Start(void) {
  bool status {WiFi.mode(WIFI_AP)};
  DBG__PRINT("WiFi mode set to WIFI_AP: ", status);
  WiFi.begin();
  delay(100);
  return status;
}

/**
 * @brief set wifi to mode WIFI_OFF
 * 
 * @return true 
 * @return false 
 */
bool Wifi::Wifi_Stop(void) {
  bool status {WiFi.mode(WIFI_OFF)};
  DBG__PRINT("WiFi mode set to WIFI_OFF: ", status);
  while (WiFi.status() != WL_DISCONNECTED) {
    yield();
  }
  return status;
}

/**
 * @brief print wifi information like ssid, ip, gateway, etc..
 * 
 */
void Wifi::printWiFiInfo(void) const {
  DBG__PRINT("\n \tVerbunden mit:\t", WiFi.SSID());
  DBG__PRINT("\tIP address:\t", WiFi.localIP());
  DBG__PRINT("\tgateway:\t", WiFi.gatewayIP());
  DBG__PRINT("\tsubnet mask:\t", WiFi.subnetMask());
  DBG__PRINT("\tDNS server:\t", WiFi.dnsIP()); 
  return;
}
