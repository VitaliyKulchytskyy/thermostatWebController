#include "handlers/llmnrHandler.h"


bool setupLLMNR() {
    return LLMNR.begin(device_name.c_str());
}
