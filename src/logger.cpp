#include "logger.h"


void writeLog(const char* filename, const String& message) {
    File file = SD.open(filename, "a");
    file.println(message);
    file.close();

#ifdef VERBOSE_MODE
    Serial.println(message);
#endif
}

void writeUserLog(const DateFormat& date, const IPAddress& ip, const String& message) {
    if(message.length() == 0)
        return;

    const String time = date.toString();
    writeLog("user_log.txt", "[" + time + " | " + ip.toString() + "]: " + message);
}

void writeSystemLog(const DateFormat& date, const String& message) {
    if(message.length() == 0)
        return;

    const String time = date.toString();
    writeLog("system_log.txt", "[" + time + "]: " + message);
}

const char* getLogName(logStatus log) {
    switch (log) {
        case LOG_BAD_REQUEST_THERMOMETER_INSIDE:
            return "thermometerInside";
        case LOG_BAD_REQUEST_THERMOMETER_OUTSIDE:
            return "thermometerOutside";
        case LOG_BAD_REQUEST_THERMOMETER_WATER:
            return "thermometerWater";
        case LOG_ERROR_RTC_SET_UP:
            return "realTimeClock";
        case LOG_BAD_REQUEST_LIGHT_SENSOR:
            return "lightSensor";
        case LOG_INFO_THERMOREGULATION_PROCESS:
            return "thermoregulationProcess";
        case LOG_INFO_THERMOREGULATION_START:
            return "thermoregulationStart";
        case LOG_INFO_THERMOREGULATION_END:
            return "thermoregulationEnd";
    }
    return "";
}

String getSensorLog(const LogFormat& log, bool isOneTime) {
    static LogFormat prevLog = 0xFF;
    static bool isFirstTime = true;

    if(isOneTime && (log.log == prevLog.log && !(isFirstTime && log.log == 0xFF))) {
        isFirstTime = false;
        return "";
    } else {
        isFirstTime = false;
    }

    String output = "";
    for(uint8_t i = LOG_POSITION_SENSORS_START; i <= LOG_POSITION_SENSORS_END; i++) {
//        if(!prevLog.isCodeSet(i) == log.isCodeSet(i)) {
        if(output.length() > 0)
            output += "; ";

        output += (String)getLogName((logStatus)i);
        output += ": ";
        output += log.isCodeSet(i) ? "DISABLE" : "ENABLE";
//        }
    }

    prevLog = log;
    return output;
}