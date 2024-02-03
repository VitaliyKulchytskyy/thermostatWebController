#include "server/serverEvent.h"


AsyncEventSource g_events("/events");
uint64_t m_timerEvent = 0;

void onClientConnect(AsyncEventSourceClient* client) {
    if(client->lastId()) {
        #ifdef VERBOSE_MODE
            Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
        #endif
    }
}

void onUpdateEvent() {
    const uint64_t thisMillis = millis();

    if(thisMillis - m_timerEvent > UPDATE) {
        g_events.send(String(g_tempIn.tempC).c_str(), "temp_inside", thisMillis);
        g_events.send(String(g_tempOut.tempC).c_str(), "temp_outside", thisMillis);
        g_events.send(String(g_tempWater.tempC).c_str(), "temp_liquid", thisMillis);
        g_events.send(String(g_light.lux).c_str(), "illumination", thisMillis);

        g_events.send(String(g_waterFlow.waterFlow).c_str(), "water_flow", thisMillis);
        g_events.send(String(g_log.log).c_str(), "log_code", thisMillis);

        g_events.send(g_date.toString(), "last_query_date", thisMillis);
        g_events.send(g_sdLastSaveDate.toString(), "last_record_down_date", thisMillis);

        m_timerEvent = thisMillis;
    }
}