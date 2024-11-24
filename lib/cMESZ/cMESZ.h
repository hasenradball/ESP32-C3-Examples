#ifndef _CMESZ_H_
#define _CMESZ_H_

#include <Arduino.h>

constexpr const char *TIMEZONE {"CET-1CEST,M3.5.0,M10.5.0/3"};
constexpr size_t MAXLEN{30U};

class MESZ {
  
  public:
    MESZ();

    static void set_time_synchronized_true(struct timeval *_tv);
    static void set_time_synchronized_false(void);
    static bool get_time_synchronized(void);

    bool setTimeZone(const char *timezone = nullptr);
    bool setTimeServers(const char *timeServer1 = nullptr, const char *timeServer2 = nullptr, const char *timeServer3 = nullptr);
    bool init(void);
    int setUnixTime(timeval *tv);
    time_t getTimeUnix(void);
    long getMicroSeconds(void);
    long getMilliSeconds(void);
    int getSecond_ms(void);
    int getSecond(void);
    bool IsSecondEqualZero(void);
    int getMinute(void);
    int getMinuteUnix(void);
    int getHour(void);
    int getHourUnix(void);
    struct tm * getStruct(void);
    char * get_asctime(void);
    int getDST(void);
    void showTime(void);
    void get_timeformatted(char *, size_t, const char *);
    void get_Unix_timeformatted(char *, size_t, const char *);

  private:
    char _NTPServer[3U][MAXLEN + 1] {{"ptbtime1.ptb.de"}, {"ptbtime2.ptb.de"}, {"ptbtime3.ptb.de"}};
    char _timezone[MAXLEN + 1] {"GMT0"};
    struct timeval _tv {0, 0};
    struct tm _tm;    
    int _t1, _t2, _calc_year;
    static bool _time_synchronized;
    void clear_tm(void) {_tm = {0, 0, 0, 0, 0, 0, 0, 0, -1};};
    
    bool UpdateTime(bool unix = false);
    bool isSummerTime(time_t);
};

// include inline member functions
#include <cMESZ.inl>

#endif