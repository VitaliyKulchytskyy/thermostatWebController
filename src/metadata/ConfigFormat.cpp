#include "metadata/ConfigFormat.h"

DeviceConfig::DeviceConfig(int intervalDataSec, float maxTempC, float hysteresis, float coefInert) : intervalDataSec(
        intervalDataSec), maxTempC(maxTempC), hysteresis(hysteresis), coefInert(coefInert) {}

void DeviceConfig::getIntervalStr(char *buf) {
    const int hour = intervalDataSec / 3600;
    const int min  = (intervalDataSec % 3600) / 60;
    const int sec  = intervalDataSec % 60;

    if(hour > 0)
        sprintf(buf, "%i год. %i хв. %i сек.", hour, min, sec);
    else if(min > 0)
        sprintf(buf, "%i хв. %i сек.", min, sec);
    else
        sprintf(buf, "%i сек.", sec);
}

void DeviceConfig::deserialize(const uint8_t *data) {
    intervalDataSec = ptov<int>(data);
    maxTempC = ptov<float>(&data[4]);
    hysteresis = ptov<float>(&data[8]);
    coefInert = ptov<float>(&data[12]);
}

size_t DeviceConfig::size() const {
    return sizeof(intervalDataSec)
         + sizeof(maxTempC)
         + sizeof(hysteresis)
         + sizeof(coefInert);
}

uint8_t *DeviceConfig::serialize() const {
    auto output = new uint8_t[this->size()] {};

    memcpy(output, &this->intervalDataSec, sizeof(this->intervalDataSec));
    size_t offset = sizeof(this->intervalDataSec);

    memcpy(&output[offset], &this->maxTempC, sizeof(this->maxTempC));
    offset += sizeof(this->maxTempC);

    memcpy(&output[offset], &this->hysteresis, sizeof(this->hysteresis));
    offset += sizeof(this->hysteresis);

    memcpy(&output[offset], &this->coefInert, sizeof(this->coefInert));

    return output;
}
