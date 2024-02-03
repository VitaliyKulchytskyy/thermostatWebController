#include "metadata/DateFormat.h"

/// Flag of incorrect RTC set up
#define ERROR_RTC_SET_UP        3

/// Checks whether the RTC module is reset by comparing the pivot year
#define CHECKS_BY_THE_YEAR      23

TimeChangeRule myDST = {"EEST", Last, Sun, Mar, 2, 60};
TimeChangeRule mySTD = {"EET", Last, Sun, Oct, 2, 0};
Timezone myTZ(myDST, mySTD);

DS3232RTC DateFormat::rtc{};

void DateFormat::begin() {
    rtc.begin();
    setSyncProvider(rtc.get);
}

void DateFormat::deserialize(const uint8_t *data) {
    m_hours = data[0];
    m_minutes = data[1];
    m_seconds = data[2];
    m_day = data[3];
    m_month = data[4];
    m_year = data[5];
    updateRtc();
}

void DateFormat::convertFromStr(const char *raw_date) {
    char buff[3];

    strncpy(buff, &raw_date[2], 2);
    m_year = atoi(buff);

    strncpy(buff, &raw_date[5], 2);
    m_month = atoi(buff);

    strncpy(buff, &raw_date[8], 2);
    m_day = atoi(buff);

    strncpy(buff, &raw_date[11], 2);
    m_hours = atoi(buff);

    strncpy(buff, &raw_date[14], 2);
    m_minutes = atoi(buff);

    strncpy(buff, &raw_date[17], 2);
    m_seconds = atoi(buff);

    updateRtc();
}

uint8_t DateFormat::request() {
    time_t nowTime = now();
    time_t local = myTZ.toLocal(nowTime);
    m_year = year(local);
    m_month = month(local);
    m_day = day(local);
    m_hours = hour(local);
    m_minutes = minute(local);
    m_seconds = second(local);
    uint8_t errorCode = (this->isTimeReset() << ERROR_RTC_SET_UP);
    return errorCode;
}

const char *DateFormat::toString() const {
    static char buf[26];
    sprintf(buf, "%02i-%02i-20%02i %02i:%02i:%02i",
            m_day,
            m_month,
            m_year,
            m_hours,
            m_minutes,
            m_seconds);
    return buf;
}

size_t DateFormat::size() const {
    return 6;
}

uint8_t *DateFormat::serialize() const {
    return new uint8_t[this->size()] {
            m_hours,
            m_minutes,
            m_seconds,
            m_day,
            m_month,
            m_year
    };
}

bool DateFormat::isTimeReset() const {
    return m_year < CHECKS_BY_THE_YEAR;
}

void DateFormat::updateRtc() {
    tmElements_t tm;
    tm.Year = m_year - 18;
    tm.Month = m_month;
    tm.Day = m_day;
    tm.Hour = m_hours;
    tm.Minute = m_minutes;
    tm.Second = m_seconds;

    time_t t = makeTime(tm);
    if(myTZ.locIsDST(t))
        tm.Hour -= myDST.offset / 60;

    t = makeTime(tm);
    rtc.set(t);
    setSyncProvider(rtc.get);
}







