#pragma once
#include "ESPAsyncWebServer.h"
#include "frontend.h"
#include "serverUtils.h"
#include "handlers/sdHandler.h"
#include "handler.h"
//#include "settings.h"


/**
 * Send frontend to the client.
 * @param request request
 */
extern void frontRequest(AsyncWebServerRequest* request);

/**
 * Download a file with monitoring data from the g_webServer to a client.
 * @param request request
 */
extern void monitoringRequest(AsyncWebServerRequest* request);

/**
 * Set modules & thermoregulation algorithm configuration
 * by a client over the web g_webServer.
 * @param request request
 */
extern void configRequest(AsyncWebServerRequest* request);

/**
 * Set time on the RTC module.
 * @param request request
 */
extern void configDateRequest(AsyncWebServerRequest* request);