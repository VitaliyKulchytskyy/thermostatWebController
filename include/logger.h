#pragma once
#include <ESP8266WiFi.h>
#include <SD.h>
#include "metadata/DateFormat.h"
#include "metadata/LogFormat.h"
#include "settings.h"


/// Name of a(n) error/warning of the log code
enum logStatus {
    LOG_BAD_REQUEST_THERMOMETER_INSIDE = 0,
    LOG_BAD_REQUEST_THERMOMETER_OUTSIDE = 1,
    LOG_BAD_REQUEST_THERMOMETER_WATER = 2,
    LOG_ERROR_RTC_SET_UP = 3,
    LOG_BAD_REQUEST_LIGHT_SENSOR = 4,
    LOG_INFO_THERMOREGULATION_PROCESS = 5,
    LOG_INFO_THERMOREGULATION_START = 6,
    LOG_INFO_THERMOREGULATION_END = 7
};

enum logPosition {
    LOG_POSITION_SENSORS_START = LOG_BAD_REQUEST_THERMOMETER_INSIDE,
    LOG_POSITION_SENSORS_END = LOG_BAD_REQUEST_LIGHT_SENSOR,
    LOG_POSITION_THERMOREGULATION_START = LOG_INFO_THERMOREGULATION_PROCESS,
    LOG_POSITION_THERMOREGULATION_END = LOG_INFO_THERMOREGULATION_END
};

/**
 * Write a log line into the file
 * @flags VERBOSE_MODE
 * @param filename name of the file
 * @param message the log line message
 */
extern void writeLog(const char* filename, const String& message);

/**
 * Write a log line into the "user_log.txt"
 * @param date current date
 * @param ip user IP
 * @param message the log line message
 */
extern void writeUserLog(const DateFormat& date, const IPAddress& ip, const String& message);

/**
 * Write a log line into the "system_log.txt"
 * @param date current date
 * @param message the log line message
 */
extern void writeSystemLog(const DateFormat& date, const String& message);

/**
 * Get a name of a log code
 * @param log log code
 * @return name from a log code
 */
const char* getLogName(logStatus log);

/**
 * Get a verbose description of the log code (wo thermoregulation)
 * @param log log code
 * @param isOneTime set an option if the previous log is equal to the new log code it will return an empty str
 * @return description
 */
String getSensorLog(const LogFormat& log, bool isOneTime);
