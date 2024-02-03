#include "metadata/LightFormat.h"


void LightFormat::deserialize(const uint8_t *data) {
    lux = ptov<float>(data);
}

uint8_t *LightFormat::serialize() const {
    auto output = new uint8_t [this->size()] {};
    memcpy(output, &lux, this->size());
    return output;
}

size_t LightFormat::size() const {
    return sizeof(lux);
}
