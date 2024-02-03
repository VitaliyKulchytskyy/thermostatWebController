#pragma once
#include "FormatBase.h"
#include "utils.h"


struct DeviceConfig: FormatBase {
public:
    int intervalDataSec = 1;
    float maxTempC = 0;
    float hysteresis = 0;
    float coefInert = 0;

public:
    DeviceConfig() {}
    DeviceConfig(int intervalDataSec, float maxTempC, float hysteresis, float coefInert);

public:
    /**
     * Get time time interval in readable format
     * @example
     * 3661 sec = "1 год. 1 хв. 1 сек.";\n
     * 122 sec = "2 хв. 2 сек.";\n
     * 54 sec = "54 сек."
     * @param [in,out] buf buffer
     */
    void getIntervalStr(char* buf);

    void deserialize(const uint8_t *data) override;

    size_t size() const override;

    uint8_t *serialize() const override;
};