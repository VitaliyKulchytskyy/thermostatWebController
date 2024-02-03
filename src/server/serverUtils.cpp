#include "server/serverUtils.h"


String processor(const String& placeholder) {
    if(placeholder == "FORMAT_THREAD_SAVING") {
        char buffer[64];
        g_deviceConfig.getIntervalStr(buffer);
        return buffer;
    }
    if(placeholder == "TEMPERATURE_MAX") {
        return String(g_deviceConfig.maxTempC);
    }
    if(placeholder == "TEMPERATURE_HISTERESYS") {
        return String(g_deviceConfig.hysteresis);
    }
    if(placeholder == "COEF_INERTIA") {
        return String(g_deviceConfig.coefInert);
    }

    return {};
}