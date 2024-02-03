#include "metadata.h"


DateFormat g_date{};
DateFormat g_sdLastSaveDate{};
TemperatureFormat g_tempIn{};
TemperatureFormat g_tempOut{};
TemperatureFormat g_tempWater{};
LightFormat g_light{};
LogFormat g_log{};
WaterFlowFormat g_waterFlow{};
DeviceConfig g_deviceConfig{};
thermoreg_t g_thermoreg(g_deviceConfig, g_tempIn);

FormatBase* g_uartFormat[] = {
        &g_tempIn,
        &g_tempOut,
        &g_tempWater,
        &g_light,
        &g_waterFlow,
        &g_log
};
size_t g_uartSize = sizeof(g_uartFormat) / sizeof(g_uartFormat[0]);

FormatBase* g_thdFormat[] = {
        &g_date,
        &g_tempIn,
        &g_tempOut,
        &g_tempWater,
        &g_light,
        &g_waterFlow,
        &g_log
};
size_t g_thdSize = sizeof(g_thdFormat) / sizeof(g_thdFormat[0]);

FormatBase* g_configFormat[] = {
        &g_deviceConfig
};
size_t g_configSize = sizeof(g_configFormat) / sizeof(g_configFormat[0]);


void deserialize(FormatBase **out, size_t outSize, uint8_t *in) {
    size_t shift = 0;

    for(size_t i = 0; i < outSize; i++) {
        out[i]->deserialize(&(in[shift]));
        shift += out[i]->size();
    }
}

size_t serialize(FormatBase **in, size_t inSize, uint8_t *out) {
    size_t shift = 0;

    for(size_t i = 0; i < inSize; i++) {
        const uint8_t formatSize = in[i]->size();
        auto pFormatRaw = in[i]->serialize();

        memcpy(&out[shift], pFormatRaw, formatSize);
        shift += formatSize;
        delete[] pFormatRaw;
    }

    return shift;
}

size_t formatSize(FormatBase** in, size_t inSize) {
    size_t size = 0;

    for(size_t i = 0; i < inSize; i++)
        size += in[i]->size();

    return size;
}

bool setupMetadata() {
    DateFormat::begin();
    g_date.request();
    g_sdLastSaveDate.request();
    return true;
}