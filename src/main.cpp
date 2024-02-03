#include <Arduino.h>
#include "handler.h"


#ifndef UNIT_TEST_MODE
void setup() {
    Serial.begin(9600);
    setupHandlers();
}

void loop() {
    loopHandlers();
    g_ftpSrv.handleFTP();
    onUpdateEvent();
}
#endif
