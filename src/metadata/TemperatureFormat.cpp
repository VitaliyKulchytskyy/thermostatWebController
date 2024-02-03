#include "metadata/TemperatureFormat.h"


void TemperatureFormat::deserialize(const uint8_t *data) {
    tempC = ptov<float>(data);
}

uint8_t *TemperatureFormat::serialize() const {
    auto output = new uint8_t [this->size()] {};
    memcpy(output, &tempC, this->size());
    return output;
}

size_t TemperatureFormat::size() const {
    return sizeof(tempC);
}
