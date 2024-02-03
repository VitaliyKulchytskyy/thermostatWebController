#include "metadata/LogFormat.h"


bool LogFormat::isCodeSet(uint8_t bitNum) const {
    if(bitNum > 7)
        return false;

    return 1 & (log >> bitNum);
}

void LogFormat::deserialize(const uint8_t *data) {
    log = data[0];
}

uint8_t *LogFormat::serialize() const {
    return new uint8_t [this->size()] {log};
}

size_t LogFormat::size() const {
    return 1;
}

