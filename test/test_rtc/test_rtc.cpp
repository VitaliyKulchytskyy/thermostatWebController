#include <Arduino.h>
#include <unity.h>
#include "metadata/DateFormat.h"

DateFormat g_dateFormat{};

void setUp(void) {
    g_dateFormat.begin();
}

void tearDown(void) {

}

void test_convertFromStr() {
    g_dateFormat.convertFromStr("2024.01.15 14:28:00");
    const char* time = g_dateFormat.toString();
    TEST_ASSERT(strcmp("15-01-2024 14:28:00", time) == 0);
}

void test_request() {
    delay(5000);
    const DateFormat prevTime = g_dateFormat;
    g_dateFormat.request();
    const bool hour =  prevTime.m_hours == g_dateFormat.m_hours;
    const bool min =   prevTime.m_minutes == g_dateFormat.m_minutes;
    const bool sec =   prevTime.m_seconds == g_dateFormat.m_seconds;
    const bool day =   prevTime.m_day == g_dateFormat.m_day;
    const bool month = prevTime.m_month == g_dateFormat.m_month;
    const bool year =  prevTime.m_year == g_dateFormat.m_year;

    TEST_ASSERT(
           hour
        && min
        && sec
        && day
        && month
        && year);
}

void setup() {
    UNITY_BEGIN();
    RUN_TEST(test_convertFromStr);
    RUN_TEST(test_request);
    UNITY_END();
}

void loop() {}