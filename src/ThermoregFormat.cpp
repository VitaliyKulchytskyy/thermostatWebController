#include "ThermoregFormat.h"


uint8_t thermoreg_t::request() {
    return thermoregulation(m_tempFormat.tempC);
}

uint8_t thermoreg_t::thermoregulation(float tempC) const {
    const bool state = getRelay(tempC);
    digitalWrite(PIN_RELAY_PUMP, state);

    static bool prevState = false;

    if (prevState && state)
        return setState(1 << INFO_THERMOREGULATION_PROCESS, prevState, state);
    else if (!prevState && !state)
        return setState(0, prevState, state);
    else if (!prevState && state)
        return setState(1 << INFO_THERMOREGULATION_START, prevState, state);
    else if (prevState  && !state)
        return setState(1 << INFO_THERMOREGULATION_END, prevState, state);

    return 0;
}

uint8_t thermoreg_t::setState(uint8_t logCode, bool &outState, bool getState) {
    outState = getState;
    return logCode;
}

bool thermoreg_t::getRelay(float inputTempC) const {
    static float prevInput = inputTempC;
    static bool relayState = false;

    static const float halfHysteresis = m_deviceConfig.hysteresis / 2;
    float signal;

    if (m_deviceConfig.coefInert > 0) {
        signal = inputTempC + ((inputTempC - prevInput) / ((float)m_deviceConfig.intervalDataSec / 1000)) * m_deviceConfig.coefInert;
        prevInput = inputTempC;
    } else {
        signal = inputTempC;
    }

    if      (signal < (m_deviceConfig.maxTempC - halfHysteresis)) relayState = false;
    else if (signal > (m_deviceConfig.maxTempC + halfHysteresis)) relayState = true;

    #ifdef PLOT_MODE
        Serial.print("signal:");
        Serial.print(signal);
        Serial.print(",");

        Serial.print("inputTempC:");
        Serial.print(inputTempC);
        Serial.print(",");

        Serial.print("setPointC:");
        Serial.print(m_deviceConfig.maxTempC);
        Serial.print(",");

        Serial.print("lowHysteresis:");
        Serial.print(m_deviceConfig.maxTempC - halfHysteresis);
        Serial.print(",");

        Serial.print("highHysteresis:");
        Serial.print(m_deviceConfig.maxTempC + halfHysteresis);
        Serial.print(",");

        Serial.print("relayState_x10:");
        Serial.println(relayState * 10);
    #endif

    return relayState;
}