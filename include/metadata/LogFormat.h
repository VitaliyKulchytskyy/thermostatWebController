#pragma once
#include "FormatBase.h"


class LogFormat: public FormatBase {
public:
    uint8_t log = 0;

public:
    LogFormat(uint8_t _log = 0) : log(_log) {}

public:
    /**
     * Check if the bit in the byte is set.
     * @param bitNum bit number of the byte (from 0 to 7).
     * @retval True - the bit is set
     * @retval False - the bit isn't set (or bitNum > 7)
     */
    bool isCodeSet(uint8_t bitNum) const;

    void deserialize(const uint8_t* data) override;

    uint8_t *serialize() const override;

    size_t size() const override;
};