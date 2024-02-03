#pragma once
#include "Arduino.h"
#include "metadata.h"


/**
 * Substitute values from the modules into the defined placeholders on the frontend.
 * @example Placeholders:\n
 * FORMAT_THREAD_SAVING - interval of the saving data on the SD;\n
 * TEMPERATURE_MAX - maximum acceptable temperature on the thermostat (thermoregulation algorithm);\n
 * TEMPERATURE_HISTERESYS - the histeresys (thermoregulation algorithm);\n
 * COEF_INERTIA - the coefficient of inertia (thermoregulation algorithm).\n
 * @param placeholder one of the defined placeholders
 * @return substituted value or empty string (if the placeholder isn't defined)
 */
String processor(const String& placeholder);