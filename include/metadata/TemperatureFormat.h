#pragma once
#include "FormatBase.h"
#include "utils.h"


struct TemperatureFormat: FormatBase {
public:
    float tempC = 0;

public:
    void deserialize(const uint8_t* data) override;

    uint8_t *serialize() const override;

    size_t size() const override;
};