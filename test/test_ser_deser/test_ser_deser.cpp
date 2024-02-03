#include <Arduino.h>
#include "unity.h"
#include "metadata.h"

#define UNIT_TIME        ("24.01.10 18:46:30")
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

void setUp(void) {
    /* --- THD Setup --- */
    g_date.convertFromStr(UNIT_TIME);
    g_tempIn.tempC = UNIT_TEMP_IN;
    g_tempOut.tempC = UNIT_TEMP_OUT;
    g_tempWater.tempC = UNIT_TEMP_WATER;
    g_light.lux = UNIT_LUX;
    g_log.log = UNIT_LOG;
    g_waterFlow.waterFlow = UNIT_WATER_FLOW;

//    /* --- Device Config Setup --- */
//    g_deviceConfig.intervalDataSec = UNIT_INT_DATA_SEC;
//    g_deviceConfig.coefInert = UNIT_COEF_INERT;
//    g_deviceConfig.hysteresis = UNIT_HYSTERESIS;
//    g_deviceConfig.maxTempC = UNIT_TEMP_MAX;

    /* --- Other --- */
    g_deserialized = (uint8_t*) malloc(formatSize(g_uartFormat, g_uartSize));
}

void tearDown(void) {
    delete[] g_deserialized;
}

void test_serialization_and_deserialization() {
    serialize(g_uartFormat, g_uartSize, g_deserialized);
    deserialize(g_uartFormat, g_uartSize, g_deserialized);

    TEST_ASSERT_MESSAGE(g_tempIn.tempC == UNIT_TEMP_IN,         "Inside Temperature");
    TEST_ASSERT_MESSAGE(g_tempOut.tempC == UNIT_TEMP_OUT,       "Outside Temperature");
    TEST_ASSERT_MESSAGE(g_tempWater.tempC == UNIT_TEMP_WATER,   "Water Temperature");
    TEST_ASSERT_MESSAGE(g_light.lux == UNIT_LUX,                "Light Module");
    TEST_ASSERT_MESSAGE(g_log.log == UNIT_LOG,                  "Logging");
    TEST_ASSERT_MESSAGE(g_waterFlow.waterFlow == UNIT_WATER_FLOW, "Water Flow");
}

void test_crash_serialization() {
    size_t rawSize = serialize(g_uartFormat, g_uartSize, g_deserialized);

    for(int i = 0; i < 10; i++) {
        const size_t pseudoRnd = millis() % rawSize;
        g_deserialized[pseudoRnd]++;
    }

    deserialize(g_uartFormat, g_uartSize, g_deserialized);

    const bool tempInCheck    = g_tempIn.tempC == UNIT_TEMP_IN;
    const bool tempOutCheck   = g_tempOut.tempC == UNIT_TEMP_OUT;
    const bool tempWaterCheck = g_tempWater.tempC == UNIT_TEMP_WATER;
    const bool lightCheck     = g_light.lux == UNIT_LUX;
    const bool logCheck       = g_log.log == UNIT_LOG;
    const bool waterFlowCheck = g_waterFlow.waterFlow == UNIT_WATER_FLOW;

    TEST_ASSERT((tempInCheck
    && tempOutCheck
    && tempWaterCheck
    && lightCheck
    && logCheck
    && waterFlowCheck) == false);
}

void setup(void) {
    UNITY_BEGIN();
    RUN_TEST(test_serialization_and_deserialization);
    RUN_TEST(test_crash_serialization);
    UNITY_END();
}

void loop(void) {}