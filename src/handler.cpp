#include "handler.h"


const handler_t handlers[] = {
        /* --- HARDWARE --- */
        handler_t{.pSetup     = setupMetadata,
                  .successMsg = "The metadata is initialized.",
                  .errorMsg   = ""},

        handler_t{.pSetup     = setupSD,
                  .successMsg = "The SD is initialized",
                  .errorMsg   = "The SD isn't initialized!"},

        /* --- SOFTWARE --- */
        handler_t{.pSetup     = setupWifi,
                  .successMsg = "The WiFi is initialized",
                  .errorMsg   = "The WiFi isn't initialized!"},

        handler_t{.pSetup     = setupFTP,
                  .successMsg = "FTP initialized. Name: " + device_name,
                  .errorMsg   = "FTP isn't initialized!"},

#ifdef USE_LLMNR
        handler_t{.pSetup     = setupLLMNR,
                  .successMsg = "LLMNR: http://" + device_name + "/",
                  .errorMsg   = "Error setting up LLMNR responder!"},
#endif

        handler_t{.pSetup     = setupWebServer,
                  .successMsg = "The Web Server is initialized.",
                  .errorMsg   = "The Web Server isn't initialized!"},

        handler_t{.pSetup     = setupTimer,
                  .successMsg = "The timer handler is initialized.",
                  .errorMsg   = ""},

        handler_t{.pSetup     = setupUART,
                .successMsg   = "The UART is initialized",
                .errorMsg     = "The UART isn't initialized!"}
};

void handler_t::setup() const {
    bool status = this->pSetup();
    writeSystemLog(g_date, status ? successMsg : errorMsg);

    #ifdef VERBOSE_MODE
        Serial.println(status ? successMsg : errorMsg);
    #endif
}

void onSaveMetadata(uint8_t* buffer, size_t bufferSize) {
    char filename[24];
    sprintf(filename, "20%02i/20%02i-%02i.thd",
            g_date.m_year,
            g_date.m_year,
            g_date.m_month);

    g_sdLastSaveDate.request();
    sd_append(filename, buffer, bufferSize);
}

void setupHandlers() {
    for (const auto& handler : handlers)
        handler.setup();

    if(sdTimeSetting(g_date))
        writeSystemLog(g_date, "The RTC was set up from \"time.bin\".");

    setEndReceiveCallback(timerSaveMetadata);
    setSaveCallback(onSaveMetadata);
}

void loopHandlers() {
    if(timerDateRequest())
        g_date.request();
}