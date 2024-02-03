#pragma once
#include "settings.h"
#include "ESPAsyncWebServer.h"


/// Async Web Server
extern AsyncWebServer g_webServer;

/**
 * Setup WiFi.
 * @flags AP_MODE
 * @return IP address
 */
extern bool setupWifi();