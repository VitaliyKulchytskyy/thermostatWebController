#include "handlers/wifiHandler.h"


AsyncWebServer g_webServer(SERVER_PORT);

bool setupWifi() {
    WiFi.hostname(device_name);

#ifdef AP_MODE
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid_ap, password_ap);
    WiFi.hostname(device_name);
#else
    WiFi.begin(ssid, password);
    int i = 0;
    while (WiFi.status() != WL_CONNECTED) {
        #ifdef VERBOSE_MODE
            delay(1000);
            Serial.print('.');
        #endif
    }
#endif

    g_webServer.begin();

    return true;
}
