#include "server/serverRequest.h"


void frontRequest(AsyncWebServerRequest* request) {
    request->send_P(200, "text/html", index_html, processor);
    writeUserLog(g_date, request->client()->remoteIP(), "connected/reconnected");
}

void monitoringRequest(AsyncWebServerRequest* request) {
    if(!setupSD()) {
        request->send(409, "text/plain",
                      "409: The SD card is not inserted.");
        writeUserLog(g_date, request->client()->remoteIP(), "error 409");
        return;
    }


    static char year[5];
    static char full_filename[32];

    if (request->hasParam("filename")) {
        String inputMessage = request->getParam("filename")->value();

        strncpy(year, inputMessage.c_str(), 4);

        sprintf(full_filename, "%s/%s",
                year,
                inputMessage.c_str());

#ifdef VERBOSE_MODE
        Serial.print("GET filename: " );
        Serial.println(full_filename);
#endif

        if (SD.exists(full_filename)) {
            File file = SD.open(full_filename, "r");
            AsyncWebServerResponse *response = request->beginResponse(SDFS, full_filename, "application/octet-stream");
            request->send(response);

#ifdef VERBOSE_MODE
            Serial.print("Download file: " );
            Serial.println(full_filename);
#endif
            file.close();


            writeUserLog(g_date, request->client()->remoteIP(), "download " + (String) full_filename);
        } else {
            request->send(404, "text/plain",
                          "404: the file \"" + inputMessage + "\" doesn't exist on the SD card.");

#ifdef VERBOSE_MODE
            Serial.println("The file doesn't exist");
#endif
        }
    }
}

void configRequest(AsyncWebServerRequest* request) {
    if (request->hasParam("interval_data_sec") && request->getParam("interval_data_sec")->value() != "") {
        const int prevValue = g_deviceConfig.intervalDataSec;
        g_deviceConfig.intervalDataSec = atoi_s(request->getParam("interval_data_sec")->value().c_str(), 1, 1);
        writeUserLog(g_date, request->client()->remoteIP(),
                     "intervalDataSec changed from " + (String) prevValue + " to " +
                     (String) g_deviceConfig.intervalDataSec);
    }

    if (request->hasParam("max_temp_c") && request->getParam("max_temp_c")->value() != "") {
        const float prevValue = g_deviceConfig.maxTempC;
        g_deviceConfig.maxTempC = atof_s(request->getParam("max_temp_c")->value().c_str());
        writeUserLog(g_date, request->client()->remoteIP(),
                     "maxTempC changed from " + (String) prevValue + " to " + (String) g_deviceConfig.maxTempC);
    }

    if (request->hasParam("hysteresis_c") && request->getParam("hysteresis_c")->value() != "") {
        const float prevValue = g_deviceConfig.hysteresis;
        g_deviceConfig.hysteresis = atof_s(request->getParam("hysteresis_c")->value().c_str());
        writeUserLog(g_date, request->client()->remoteIP(),
                     "hysteresis changed from " + (String) prevValue + " to " + (String) g_deviceConfig.hysteresis);
    }

    if (request->hasParam("coef_inert") && request->getParam("coef_inert")->value() != "") {
        const float prevValue = g_deviceConfig.coefInert;
        g_deviceConfig.coefInert = atof_s(request->getParam("coef_inert")->value().c_str());
        writeUserLog(g_date, request->client()->remoteIP(),
                     "coefInert changed from " + (String) prevValue + " to " + (String) g_deviceConfig.coefInert);
    }

    request->send(200, "text/html", "The values were changed. <br><a href=\"/\">home page</a>");

#ifdef VERBOSE_MODE
    Serial.printf("inteval: %i\nmax temp: %f\nhysteresis: %f\ncoef inert:%f\n",
                  g_deviceConfig.intervalDataSec,
                  g_deviceConfig.maxTempC,
                  g_deviceConfig.hysteresis,
                  g_deviceConfig.coefInert);
#endif

    static const size_t configSize = formatSize(g_configFormat, g_configSize);
    auto temp = new uint8_t[configSize]{};
    serialize(g_configFormat, g_configSize, temp);
    sd_write("config.bin", temp, configSize);
    delete[] temp;
}

void configDateRequest(AsyncWebServerRequest* request) {
    const DateFormat prevDate = g_date;
    if (request->hasParam("date") && request->getParam("date")->value() != "") {
        g_date.convertFromStr(request->getParam("date")->value().c_str());
    }

    request->send(200, "text/html", "The date was changed. <br><a href=\"/\">home page</a>");
    Serial.println("the date was changed");

    writeUserLog(g_date, request->client()->remoteIP(),
                 "RTC changed from \"" + (String) prevDate.toString() + "\" to \"" + (String) g_date.toString() +
                 "\"");
}