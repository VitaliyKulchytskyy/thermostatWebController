#pragma once
#include "FormatBase.h"
#include "DS3232RTC.h"
#include "TimeLib.h"
#include "TimeZone.h"


struct DateFormat: FormatBase {
public:
    /// Value: 0 - 23
    uint8_t m_hours = 0;
    /// Value: 0 - 59
    uint8_t m_minutes = 0;
    /// Value: 0 - 59
    uint8_t m_seconds = 0;
    /// Value: 1 - 31
    uint8_t m_day = 1;
    /// Value: 1 - 12
    uint8_t m_month = 1;
    /// Value: 0 - 99
    uint8_t m_year = 0;

private:
    static DS3232RTC rtc;

public:
    DateFormat() = default;

public:
    /**
     * Initialize the RTC module
     */
    static void begin();

    void deserialize(const uint8_t* data) override;

    /**
     * Convert string to DateTime instance.
     * @example String format: 20XX(year).XX(month).XX(day) XX(hours):XX(minutes):XX(seconds)\n
     * This method ignores separators (like ".", " ", ":"), and extracts only XX values.
     * @param [in] raw_date the string in defined format
     */
    void convertFromStr(const char* raw_date);

    /**
     * Update the internal time from the RTC
     * @return ERROR_RTC_SET_UP if the time is reset
     */
    uint8_t request();

    const char* toString() const;

    size_t size() const override;

    uint8_t *serialize() const override;

private:
    /**
     * Check if the time was reset.
     * @attention Time reset would occur by discharged battery
     * or if a user changed the battery in the wrong way.
     * @warning The method is not 100% guaranteed that time was reset.
     * @retval True - time was reset
     * @retval False - time wasn't reset
     */
    bool isTimeReset() const;

    /**
     * Update the RTC module from the internal time
     */
    void updateRtc();
};