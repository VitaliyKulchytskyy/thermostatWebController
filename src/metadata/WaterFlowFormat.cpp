#include "metadata/WaterFlowFormat.h"


void WaterFlowFormat::deserialize(const uint8_t *data) {
    waterFlow = ptov<float>(data);
}

uint8_t *WaterFlowFormat::serialize() const {
    auto output = new uint8_t [this->size()] {};
    memcpy(output, &waterFlow, this->size());
    return output;
}

size_t WaterFlowFormat::size() const {
    return sizeof(waterFlow);
}

