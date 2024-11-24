/**
 * @brief for the ESP32: set the variable _time_synchronized to true
 * 
 * @param _tv 
 */
inline void MESZ::set_timeHasBeenSynchronized_true(struct timeval *_tv) {
  _timeHasBeenSynchronized = true;
}

/**
* @brief set the variable _time_synchronized to false
* 
*/
inline void MESZ::set_timeHasBeenSynchronized_false(void) {
  _timeHasBeenSynchronized = false;
}

/**
 * @brief get the value of _time_synchronized
 * 
 * @return true if time was synched else false
 */
inline bool MESZ::get_timeHasBeenSynchronized(void) {
  return _timeHasBeenSynchronized;
}

/**
 * @brief get time_t in unix time
 * 
 * @return time_t in unix time
 */
inline time_t MESZ::getTimeUnix(void) {
  UpdateTime(true);
  return _tv.tv_sec;
}

/**
 * @brief return microseconds of the struct tm
 * 
 * @return tv.tv_usec
 */
inline long MESZ::getMicroSeconds(void) {
  UpdateTime();
  return _tv.tv_usec;
}

/**
 * @brief return microseconds of the struct tv
 * 
 * @return tv.tv_usec/1000
 */
inline long MESZ::getMilliSeconds(void) {
  UpdateTime();
  return _tv.tv_usec/1000;
}

/**
 * @brief return seconds as millisecons of struct tm.tm_sec
 * 
 * @return tm.tm_sec*1000
 */
inline int MESZ::getSecond_ms(void) {
  UpdateTime();
  return _tm.tm_sec * 1000;
}

/**
 * @brief return seconds of struct tm
 * 
 * @return tm.tm_sec
 */
inline int MESZ::getSecond(void) {
  UpdateTime();
  return _tm.tm_sec;
}

/**
 * @brief check if the second of tm.tm_sec == 0
 * 
 * @return true if second == 0 else false
 */
inline bool MESZ::IsSecondEqualZero(void) {
  return (getSecond() == 0);
}

/**
 * @brief return minute of struct tm
 * 
 * @return tm.tm_min
 */
inline int MESZ::getMinute(void) {
  UpdateTime();
  return _tm.tm_min;
}

/**
 * @brief get hour of struct tm in local time
 * 
 * @return tm.tm_hour
 */
inline int MESZ::getHour(void) {
  UpdateTime();
  return _tm.tm_hour;
}

/**
 * @brief get hour of struct tm in unix time
 * 
 * @return tm.tm_hour
 */
inline int MESZ::getHourUnix(void) {
  UpdateTime(true);
  return _tm.tm_hour;
}

/**
 * @brief get pointer to struct tm
 * 
 * @return struct tm *
 */
inline struct tm * MESZ::getStruct(void) {
  UpdateTime();
  return &_tm;
}

/**
 * @brief get char * of acstime
 * 
 * @return char * of asctime
 */
inline char * MESZ::get_asctime(void) {
  UpdateTime();
  return asctime(&_tm);
}

/**
 * @brief get flag isdst -> if summertime is active
 * 
 * @return tm.tm_isdst
 */
// get DST Flag
inline int MESZ::getDST(void) {
  UpdateTime();
  return _tm.tm_isdst;
}