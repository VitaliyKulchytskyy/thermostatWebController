#include <Arduino.h>
#include "unity.h"
#include "handler.h"
#include "metadata.h"

const char* UNIT_TIME = "24.01.10 18:46:30";
#define UNIT_TEMP_IN     10
#define UNIT_TEMP_OUT    20
#define UNIT_TEMP_WATER  30
#define UNIT_LUX         40
#define UNIT_LOG         4
#define UNIT_WATER_FLOW  50
#define UNIT_INT_DATA_SEC   1
#define UNIT_COEF_INERT     2
#define UNIT_HYSTERESIS     3
#define UNIT_TEMP_MAX       4

uint8_t* g_deserialized = nullptr;
size_t g_packageSize = 0;
const char* g_filename = "test_unit.thd";


void setUp(void) {
    /* --- THD Setup --- */
    g_date.convertFromStr(UNIT_TIME);
    g_tempIn.tempC = UNIT_TEMP_IN;
    g_tempOut.tempC = UNIT_TEMP_OUT;
    g_tempWater.tempC = UNIT_TEMP_WATER;
    g_light.lux = UNIT_LUX;
    g_log.log = UNIT_LOG;
    g_waterFlow.waterFlow = UNIT_WATER_FLOW;

    /* --- Device Config Setup --- */
    g_deviceConfig.intervalDataSec = UNIT_INT_DATA_SEC;
    g_deviceConfig.coefInert = UNIT_COEF_INERT;
    g_deviceConfig.hysteresis = UNIT_HYSTERESIS;
    g_deviceConfig.maxTempC = UNIT_TEMP_MAX;

    /* --- Other --- */
    g_packageSize = formatSize(g_uartFormat, g_thdSize);
    g_deserialized = new uint8_t [g_packageSize]{};

    /* --- Initialize hard & callbacks --- */
    setEndReceiveCallback([]() -> bool {
        return true;
    });
    setSaveCallback([](uint8_t** buffer, size_t bufferSize) {
        sd_write(g_filename, buffer, bufferSize);
    });
}

void tearDown(void) {
    if(SD.exists(g_filename))
        SD.remove(g_filename);

    delete[] g_deserialized;
}

void test_setupSD() {
    TEST_ASSERT(setupSD() == true);
}

void test_transmitReceivePackage() {
    if(SD.exists(g_filename))
        SD.remove(g_filename);

    serialize(g_uartFormat, g_thdSize, g_deserialized);
    for(size_t i = 0; i < g_packageSize; i++)
        onUartReceiveMock(g_deserialized[i]);

    File file = SD.open(g_filename, "r");

    for(size_t i = 0; i < g_packageSize; i++) {
        file.seek(i);
        if(file.read() != g_deserialized[i])
            TEST_ASSERT(false);
    }
    file.close();

    TEST_ASSERT(true);
}

void setup() {
    UNITY_BEGIN();
    RUN_TEST(test_setupSD);
    RUN_TEST(test_transmitReceivePackage);
    UNITY_END();
}

void loop() {}