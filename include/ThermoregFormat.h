#pragma once
#include "Arduino.h"
#include "settings.h"
#include "metadata/ConfigFormat.h"
#include "metadata/TemperatureFormat.h"


/**
 * The class handles the thermoregulation process in the device
 */
class thermoreg_t {
private:
    const DeviceConfig& m_deviceConfig;
    const TemperatureFormat& m_tempFormat;

public:
    thermoreg_t(const DeviceConfig& conf,
                const TemperatureFormat& tempFormat)
            : m_deviceConfig(conf),
              m_tempFormat(tempFormat)
    {
        pinMode(PIN_RELAY_PUMP, OUTPUT);
    }

public:
    /**
     * Invoke thermoregulation algorithm and get return code.
     * @example return code:\n
     * 0 - there is no need in thermoregulation;\n
     * INFO_THERMOREGULATION_PROCESS - the thermoregulation process is running;\n
     * INFO_THERMOREGULATION_START - the thermoregulation process has started;\n
     * * INFO_THERMOREGULATION_END - the thermoregulation process has ended
     * @return return code
     */
    uint8_t request();

private:
    /**
     * Handles the thermoregulation process by reading the input temperature.
     * The thermoregulation process proceeds by enabling or disabling the pump with cooling fluid
     *
     * @param tempC input temperature
     * @retval INFO_THERMOREGULATION_START thermoregulation process has been started
     * @retval INFO_THERMOREGULATION_PROCESS thermoregulation process is running
     * @retval INFO_THERMOREGULATION_END thermoregulation process has ended
     */
    uint8_t thermoregulation(float tempC) const;

    /**
     * Handles the returning of the thermoregulation method (macros)
     *
     * @param [in] logCode the log code of the thermoregulation process
     * @param [out] outState sets state before returning
     * @param [in] getState read and sd_append state before returning
     * @return the log code
     */
    static uint8_t setState(uint8_t logCode, bool &outState, bool getState) ;

    /**
     * Enable or disable the pump based on inputTempC value and
     * measurement of temperature rise rate
     *
     * @param inputTempC input temperature
     * @retval true enable the pump
     * @retval false disable the pump
     */
    bool getRelay(float inputTempC) const;
};