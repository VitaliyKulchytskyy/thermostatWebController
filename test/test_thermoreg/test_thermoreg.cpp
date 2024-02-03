#include <Arduino.h>
#include <unity.h>
#include "ThermoregFormat.h"

#define MAX_TEMP_c      32
#define HYSTERESIS      0.5
#define COEF_INERT      1

DeviceConfig g_conf(0, MAX_TEMP_c, HYSTERESIS, COEF_INERT);
TemperatureFormat g_temp;
thermoreg_t g_thermo(g_conf, g_temp);

float g_tempRecord[]{25, 25, 27, 29, 30.5, 33, 33, 32, 31, 30, 25};
size_t g_tempRecordSize = sizeof(g_tempRecord) / sizeof(g_tempRecord[0]);

void setUp(void) {
}

void tearDown(void) {
}

/// In real-time test
void testRelaySwitching() {
    for(size_t i = 0; i < g_tempRecordSize; i++) {
        g_temp.tempC = g_tempRecord[i];
        g_thermo.request();
        delay(500);
    }
    TEST_ASSERT(true);
}

void setup() {
    UNITY_BEGIN();
    RUN_TEST(testRelaySwitching);
    UNITY_END();
}

void loop() {}