#pragma once
#include <SoftwareSerial.h>
#include "settings.h"
#include "../metadata.h"
#include "../handlers/sdHandler.h"


using callback_t = void(*)();
using saveCallback_t = void(*)(uint8_t*, size_t);
using signalCallback_t = bool(*)();
extern SoftwareSerial g_SUART;

/**
 * Invoke this function every time when data is received by UART (1 byte per invoke)
 */
extern void onUartReceive();

#ifdef UNIT_TEST_MODE
/// Mock of onUartReceive() to unit tests
extern void onUartReceiveMock(uint8_t data);
#endif

/**
 * Invoke when the UART has ended up receiving a package
 * @param buffer received package
 * @param bufferSize size of the received package (in bytes)
 */
extern void onUartEndReceive(uint8_t* buffer, size_t bufferSize);

/**
 * Set a callback to handle the ending up of package receiving
 * @param callback callback function
 */
extern void setEndReceiveCallback(signalCallback_t callback);

/**
 * Set a callback to handle the saving of the received package
 * @param callback callback function
 */
extern void setSaveCallback(saveCallback_t callback);

/**
 * Setup UART
 * @param callback call the function whenever the UART has ended to receive a package
 * @flags SERIAL_BAUD
 */
extern bool setupUART();