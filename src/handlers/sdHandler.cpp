#include "handlers/sdHandler.h"

bool createConfigFile() {
    if(SD.exists("config.bin"))
        return false;

    size_t configSize = formatSize(g_configFormat, g_configSize);
    auto buf = new uint8_t[configSize]{};
    serialize(g_configFormat, g_configSize, buf);
    sd_write("config.bin", buf, configSize);
    delete[] buf;

    return true;
}

void recoverSDData() {
    File file = SD.open("config.bin");
    size_t configSize = formatSize(g_configFormat, g_configSize);
    auto buf = new uint8_t[configSize]{};
    file.read(buf, configSize);
    file.close();

    deserialize(g_configFormat, g_configSize, buf);
    delete[] buf;
}

bool setupSD() {
    const bool out = SD.begin(CS_PIN);

    if(!out)
        return false;

    if(!createConfigFile())
        recoverSDData();

    return true;
}

bool sdTimeSetting(DateFormat& date) {
    if(!SD.exists("time.bin"))
        return false;

    File file = SD.open("time.bin", "r");
    static uint8_t buffer[6];
    file.read(buffer, 6);
    date.deserialize(buffer);
    SD.remove("time.bin");

    return true;
}

void sd_do(const char* filename, uint8_t* buffer, size_t bufferSize, const char* mode) {
    File file = SD.open(filename, mode);
    file.write(buffer, bufferSize);
    file.close();

#ifdef VERBOSE_MODE
    Serial.printf("-> \'%s\' on SD file \"%s\":\n", mode, filename);
//    for(size_t i = 0; i < bufferSize; i++) {
//        Serial.print(*buffer[i], HEX);
//        Serial.print(" ");
//    }
//    Serial.println();
#endif
}

void sd_append(const char* filename, uint8_t* buffer, size_t bufferSize) {
    sd_do(filename, buffer, bufferSize, "a");
}

void sd_write(const char* filename, uint8_t* buffer, size_t bufferSize) {
    sd_do(filename, buffer, bufferSize, "w");
}
