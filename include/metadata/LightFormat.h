#pragma once
#include "FormatBase.h"
#include "utils.h"


class LightFormat: public FormatBase {
public:
    float lux = 0;

public:
    void deserialize(const uint8_t* data) override;

    uint8_t *serialize() const override;

    size_t size() const override;
};
