#include "handlers/uartHandler.h"


SoftwareSerial g_SUART(D4, D3);
namespace {
    signalCallback_t m_onEndCallback;
    saveCallback_t m_onSaveCallback;
    uint8_t m_buffer[32];
}

void onUartReceive() {
    static size_t count = 0;
    static const size_t packageSize = formatSize(g_thdFormat, g_thdSize);
    static const size_t dateSize = g_date.size();

    if(count == 0) {
        g_date.request();
        auto dateRaw = g_date.serialize();
        memmove(m_buffer, dateRaw, dateSize);
        delete[] dateRaw;
        count += dateSize;
    }

    byte n = g_SUART.available();

    if (n != 0) {
        uint8_t c = g_SUART.read();
        m_buffer[count++] = c;
    }

    if(count == packageSize) {
        count = 0;

#ifdef VERBOSE_MODE
        for (size_t i = 0; i < packageSize; i++)
            Serial.print(m_buffer[i], HEX);
        Serial.println();
#endif

        deserialize(g_uartFormat, g_uartSize, &m_buffer[dateSize]);
        onUartEndReceive(m_buffer, packageSize);
    }
}

#ifdef UNIT_TEST_MODE
void onUartReceiveMock(uint8_t data) {
    static size_t count = 0;
    static size_t packageSize = formatSize(g_uartFormat, g_uartSize);
    static auto buffer = (uint8_t*) malloc(packageSize);

    uint8_t c = data;
    buffer[count++] = c;

    if(count == packageSize) {
        count = 0;
        deserialize(g_uartFormat, g_uartSize, buffer);
        onUartEndReceive(buffer, packageSize);
    }
}
#endif

void onUartEndReceive(uint8_t* buffer, size_t bufferSize) {
    uint8_t thermoregCode = g_thermoreg.request();
    g_log.log |= thermoregCode;

    if(m_onEndCallback())
        m_onSaveCallback(buffer, bufferSize);

    writeSystemLog(g_date, getSensorLog(g_log, true));

    #ifdef VERBOSE_MODE
        Serial.println(g_date.toString());
        Serial.printf("Temp in: %f\n", g_tempIn.tempC);
        Serial.printf("Temp out: %f\n", g_tempOut.tempC);
        Serial.printf("Temp water: %f\n", g_tempWater.tempC);
        Serial.printf("Light: %f\n", g_light.lux);
        Serial.printf("Water flow: %f\n", g_waterFlow.waterFlow);
        Serial.printf("Log: %i\n", g_log.log);
    #endif
}

void setEndReceiveCallback(signalCallback_t callback) {
    m_onEndCallback = callback;
}

void setSaveCallback(saveCallback_t callback) {
    m_onSaveCallback = callback;
}

bool setupUART() {
    g_SUART.begin(SERIAL_BAUD);
    g_SUART.onReceive(onUartReceive);
    return true;
}