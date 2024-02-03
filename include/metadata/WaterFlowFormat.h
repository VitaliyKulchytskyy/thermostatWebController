#pragma once
#include "FormatBase.h"
#include "utils.h"


class WaterFlowFormat: public FormatBase {
public:
    float waterFlow = 0;

public:
    void deserialize(const uint8_t* data) override;

    uint8_t *serialize() const override;

    size_t size() const override;
};