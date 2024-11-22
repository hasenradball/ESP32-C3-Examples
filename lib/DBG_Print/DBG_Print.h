#ifndef _DBG_PRINT_H_
#define _DBG_PRINT_H_

#include <Arduino.h>

#ifdef ESP8266
#include <ESP8266WiFi.h>
#elif ESP32
#include <WiFi.h>
#endif

// Switch between production mode (silent mode) vs. debug mode (verbose mode)
#ifdef DEBUG_ESP_PORT
//#warning("DEBUG symbol created")
#define DEBUG
#endif

// ### === Print-Funktionen === ###
void DBG__SERIALBEGIN(unsigned long baud = 115200);

void DBG__SERIALEND(void);

/**
 * @brief Special functionality for more comfortable print output with one Type as Parameter
 */
template <typename T1>
void DBG__PRINT(__attribute__((unused))T1 wert1, __attribute__((unused))const char *sep = "\n") {
#ifdef DEBUG
  //Serial.print(" ");
  Serial.print(wert1);
  Serial.print(sep);
#endif
}

/**
 * @brief Special functionality for more comfortable print output with two Types as Parameter
 */
template <typename T1, typename T2>
void DBG__PRINT(__attribute__((unused))T1 wert1, __attribute__((unused))T2 wert2, __attribute__((unused))const char *sep = "\n") {
#ifdef DEBUG
  //Serial.print(" ");
  Serial.print(wert1);
  Serial.print(wert2);
  Serial.print(sep);
#endif
}

/**
 * @brief Special functionality for more comfortable print output similar as printf
 */
template<typename... Types>
void dprintf(__attribute__((unused)) const char *txt, __attribute__((unused))Types... args) {
#if defined(ESP8266) || defined(ESP32)
// special realization of printf() functionality for debugging on the ESP's
#ifdef DEBUG
  //Serial.print(" ");
  Serial.printf(txt, args...);
#endif
#else
//special realization of printf() functionality for all HW's
#ifdef DEBUG
  constexpr int LENGTH {100};
  char buffer[LENGTH];
  int count = snprintf(buffer, LENGTH, txt, args...);
  Serial.print(" ");
  Serial.print(buffer);
  if (count > LENGTH) {
      Serial.print (" =>> ERROR: sprintf buffer too low for dprintf() functionality!\n Resulting output exeeds the available buffer\n");
  }
#endif
#endif
}

void printESPStatus(void);
#endif