#include "DBG_Print.h"

// ### === Print-Funktionen === ###
/**
 * @brief Start Serial.begin for DBG-Functions
 * 
 * @param baud Baudrate
 */
void DBG__SERIALBEGIN(unsigned long baud __attribute__((unused))) {
#ifdef DEBUG
#ifdef ESP8266
  Serial.begin(baud, SERIAL_8N1, (SerialMode) UART_TX_ONLY, 1, false);
#else
  Serial.begin(baud);
#endif
  while(!Serial) yield();
#endif
}

/**
 * @brief Stop of Serial for DBG-Functions
 * 
 */
void DBG__SERIALEND(void) {
#ifdef DEBUG
  delay(100);
  Serial.end();
#endif
}

/**
 * @brief Print Status of ESP8266 or ESP32
 * 
 */
// ### === ESP Status === ###
void printESPStatus(void) {
#ifdef DEBUG
#ifdef ESP8266
  DBG__PRINT("\n\n ### --- ESP-Status ---###");
  DBG__PRINT("reset reason:       ", ESP.getResetReason());
  DBG__PRINT("free heap:          ", ESP.getFreeHeap());
  DBG__PRINT("chip ID:            ", ESP.getChipId());
  DBG__PRINT("coreVersion:        ", ESP.getCoreVersion());
  DBG__PRINT("SDK-Version:        ", ESP.getSdkVersion(), "\n");
  DBG__PRINT("CPU Frequency:      ", ESP.getCpuFreqMHz());
  DBG__PRINT("sketch Size:        ", ESP.getSketchSize());
  DBG__PRINT("sketch Space:       ", ESP.getFreeSketchSpace());
  DBG__PRINT("sketch MD5:         ", ESP.getSketchMD5());
  DBG__PRINT("Flash ChipID:       ", ESP.getFlashChipId());
  DBG__PRINT("Flash ChipSize:     ", ESP.getFlashChipSize());
  DBG__PRINT("Flash ChipRealSize: ", ESP.getFlashChipRealSize());
  DBG__PRINT("Flash Chip Speed:   ", ESP.getFlashChipSpeed());
  DBG__PRINT("cycle count:        ", ESP.getCycleCount());
  DBG__PRINT("mac address:        ", WiFi.macAddress());
  DBG__PRINT("\n >>> Wifi-Diag <<< ");
  WiFi.printDiag(Serial);
  DBG__PRINT("### --- ESP-Status End---###\n");
#elif ESP32
  DBG__PRINT("\n\n ### --- ESP-Status --- ###\n");
  DBG__PRINT("Heap size:           ", ESP.getHeapSize());
  DBG__PRINT("Free heap:           ", ESP.getFreeHeap());
  DBG__PRINT("CPU frequency:       ", ESP.getCpuFreqMHz());
  DBG__PRINT("Sketch size:         ", ESP.getSketchSize());
  DBG__PRINT("Sketch space:        ", ESP.getFreeSketchSpace());
  DBG__PRINT("Sketch MD5:          ", ESP.getSketchMD5());
  DBG__PRINT("SDK version:         ", ESP.getSdkVersion(),"\n");
  DBG__PRINT("Flash chip revision: ", ESP.getChipRevision());
  DBG__PRINT("Flash chip size:     ", ESP.getFlashChipSize());
  DBG__PRINT("Flash chip mode:     ", ESP.getFlashChipMode());
  DBG__PRINT("Flash chip speed:    ", ESP.getFlashChipSpeed());
  DBG__PRINT("Cycle count:         ", ESP.getCycleCount());
  DBG__PRINT("MAC address:         ", WiFi.macAddress());
  DBG__PRINT("\n >>> Wifi-Diag <<< ");
  WiFi.printDiag(Serial);
  DBG__PRINT("### --- ESP-Status End --- ###");
#endif
#endif
}
