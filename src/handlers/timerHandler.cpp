#include "handlers/timerHandler.h"


namespace {
    uint64_t m_saveMetadataTimer;
    uint64_t m_dateRequestTimer;
}

bool setupTimer() {
    m_saveMetadataTimer = millis();
    return true;
}

bool timerSaveMetadata() {
    const uint64_t thisMillis = millis();

    if (thisMillis - m_saveMetadataTimer > (uint64_t)g_deviceConfig.intervalDataSec * 1000
        || 1 & (g_log.log >> INFO_THERMOREGULATION_START)
        || 1 & (g_log.log >> INFO_THERMOREGULATION_END))
    {
        m_saveMetadataTimer = thisMillis;
        return true;
    }

    return false;
}

bool timerDateRequest() {
    const uint64_t thisMillis = millis();

    if (thisMillis - m_saveMetadataTimer > 1000) {
        m_dateRequestTimer = thisMillis;
        return true;
    }

    return false;
}