#include "handlers/webServerHandler.h"


bool setupWebServer() {
    g_events.onConnect(onClientConnect);
    g_webServer.on("/", HTTP_GET, frontRequest);
    g_webServer.on("/monitoring", HTTP_GET, monitoringRequest);
    g_webServer.on("/conf", HTTP_GET, configRequest);
    g_webServer.on("/confDate", HTTP_GET, configDateRequest);
    g_webServer.addHandler(&g_events);
    g_webServer.begin();
    return true;
}