/*
  Projekt: Berechnung der Mitteleuropäischen Sommerzeit (MESZ)
  Microcontroller: ESP32-C3
  Date: 24.11.2024
  Issuer: Frank Häfele
*/
#include "esp_sntp.h"
#include "cMESZ.h"
#include "DBG_Print.h"

bool MESZ::_time_synchronized {false};

/**
 * @brief Construct a new MESZ::MESZ object
 */
MESZ::MESZ() {
  sntp_set_time_sync_notification_cb(MESZ::set_time_synchronized_true);
  clear_tm();
}

/**
 * @brief set the Timezone according to your area
 * 
 * @param timeZone Timizone String
 * @return true or false
 */
bool MESZ::setTimeZone(const char *timeZone) {
  bool changed {false};
  if (timeZone != nullptr) {
    strlcpy(_timezone, timeZone, sizeof(_timezone));
    changed = true;
  }
  return changed;
}

/**
 * @brief set the time servers for use with configTime
 * 
 * @param timeServer1 c-string which contains the timeserver
 * @param timeServer2 c-string which contains the timeserver
 * @param timeServer3 c-string which contains the timeserver
 * @return true if one of the servers was changed/updated
 */
bool MESZ::setTimeServers(const char *timeServer1, const char *timeServer2, const char *timeServer3) {
  bool changed {false};
  if (timeServer1 != nullptr) {
    strlcpy(_NTPServer[0], timeServer1, sizeof(_NTPServer[0]));
    changed = true;
  }
  if (timeServer2 != nullptr) {
    strlcpy(_NTPServer[1], timeServer2, sizeof(_NTPServer[1]));
    changed = true;
  }
  if (timeServer3 != nullptr) {
    strlcpy(_NTPServer[2], timeServer3, sizeof(_NTPServer[2]));
    changed = true;
  }
  return changed;
}

/**
 * @brief init of MESZ by call configTzTime
 * 
 * @return true
 */
bool MESZ::init(void) {
  DBG__PRINT("Configured Timeserver-settings: ...");
  DBG__PRINT("\tTimesZone:   ", _timezone);
  DBG__PRINT("\tTimeserver1: ", _NTPServer[0]);
  DBG__PRINT("\tTimeserver2: ", _NTPServer[1]);
  DBG__PRINT("\tTimeserver3: ", _NTPServer[2]);
  _time_synchronized = false;
  configTzTime(_timezone, _NTPServer[0], _NTPServer[1], _NTPServer[2]);
  return true;
}

/**
 * @brief get the actual time from the microcontroller and updates the struct tv and tm
 * 
 * @param unix bool parameter if the unix Time are queried or localtime
 * @return true or false
 */
bool MESZ::UpdateTime(bool unix) {
  gettimeofday(&_tv, nullptr);
  if (unix) { 
    gmtime_r(&_tv.tv_sec, &_tm);
    return true;
  }
  else {
  // remove if it works on esp32
  // #if ESP32
  //     _tv.tv_sec += 3600;
  //     if (isSummerTime(_tv.tv_sec)) {
  //       _tv.tv_sec += 3600;
  //     }
  // #endif
    localtime_r(&_tv.tv_sec, &_tm);
  }
  return true;
}

/**
 * @brief method to calculate if the Sommerzeit or Winterzeit is aktiv
 * 
 * @param _ti time_t value
 * @return true if is summer time else false
 */
bool MESZ::isSummerTime(time_t _ti) {
  int AnzStd;
  struct tm tmp;
  gmtime_r(&_ti, &tmp);

  // keine Sommerzeit zwischen November und Februar
  if (tmp.tm_mon < 2 || tmp.tm_mon > 9) return false;

  // Sommerzeit zwischen April und September
  if (tmp.tm_mon > 2 && tmp.tm_mon < 9) return true;

  // Berechnung der Sommerzeit im März und Oktober
  if (tmp.tm_year + 1900 != _calc_year ) {
    _calc_year = tmp.tm_year + 1900;
    // Berechne die Stunden zum Zeitpunkt der Umstellung im März
    // Die Sommerzeit beginnt immer am letzten Sonntag im März
    _t1 = 24 * (31 - (5 * _calc_year / 4 + 4) % 7) + 1;
    // Berechne die Stunden zum Zeitpunkt der Umstellung im Oktober
    // Die Winterzeit beginnt immer am letzten Sonntag im Oktober
    _t2 = 24 * (31 - (5 * _calc_year / 4 + 1) % 7) + 1;
  }
  AnzStd = tmp.tm_hour + 24 * tmp.tm_mday;
  // Wenn März und Std >= Zeitpunkt der Umstellung --> Sommerzeit
  if (tmp.tm_mon == 2 && AnzStd >= _t1) return true;
  // Wenn Oktober und Std < Zeitpunkt der Umstellung --> Sommerzeit
  if (tmp.tm_mon == 9 && AnzStd < _t2) return true;
  // sonst Winterzeit
  return false;
}



/**
 * @brief set the unix time of the system by a given time_t value
 * 
 * @param unixTime time stamp in time_t format
 * @param microseconds microseconds of microcontroller
 * @return return 0 for success, or -1 for failure
 */
int MESZ::setUnixTime(timeval * tv) {
  _tv = {tv->tv_sec, tv->tv_usec};
  int status = settimeofday(&_tv, nullptr);
#if ESP8266
  setTZ(_timezone);
#elif ESP32
  setenv("TZ",_timezone,1);
  tzset();
#endif
  return status;
}

/**
 * @brief print time in formated manner ("2001-08-23 14:55:02")
 * 
 */
void MESZ::showTime(void) {
  char t[24];
  const char *timeformat = "%F %X";
  get_timeformatted(t, sizeof(t), timeformat);
  DBG__PRINT("Time: ", t);
}

/**
 * @brief function to call stftime for local time
 * 
 * @param ptrtimestr pointer wher to save the time string
 * @param buffersize buffer size of time string buffer 
 * @param timeformat format specifier
 */
void MESZ::get_timeformatted(char *ptrtimestr, size_t buffersize, const char *timeformat) {
  UpdateTime();
  strftime(ptrtimestr, buffersize, timeformat, &_tm);
}

/**
 * @brief function to call stftime for unix time
 * 
 * @param ptrtimestr pointer wher to save the time string
 * @param buffersize buffer size of time string buffer 
 * @param timeformat format specifier
 */
// get the time special formatted
void MESZ::get_Unix_timeformatted(char *ptrtimestr, size_t buffersize, const char *timeformat) {
  UpdateTime(true);
  strftime(ptrtimestr, buffersize, timeformat, &_tm);
}