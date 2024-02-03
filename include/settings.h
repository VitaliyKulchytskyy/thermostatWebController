#pragma once
#include "Arduino.h"
#include "password.h"


/* --- General Settings --- */
/// The name of the device
extern String device_name;

/* --- WiFi & FTP Settings --- */
/// Enable using of LLMNR (it only works on the Windows (Phone & Desktop), Linux-based OS?)
#define USE_LLMNR
/// Web g_webServer port
#define SERVER_PORT     80
// TODO: придумати норм назву
#define UPDATE          1000

/* --- Pin Setup --- */
/// Chip Select pin in the SD module
#define CS_PIN          D8
#define PIN_RELAY_PUMP  D0

/* --- Serial Setup --- */
#define SERIAL_BAUD     9600

/* --- Log flags --- */
/// Flag of coursing of the thermoregulation process
#define INFO_THERMOREGULATION_PROCESS       5
/// Flag of starting of the thermoregulation process
#define INFO_THERMOREGULATION_START         6
/// Flag of ending of the thermoregulation process
#define INFO_THERMOREGULATION_END           7

/* --- Debug --- */
/// For unit tests. Disable VERBOSE_MODE.
//#define UNIT_TEST_MODE
/// If AP_MODE is defined the device will be working
/// as access point otherwise it will connect to the wifi
//#define AP_MODE
#ifndef UNIT_TEST_MODE
    /// Print verbose debug information into the console
    #define VERBOSE_MODE
#endif
/// Print thermoregulation plot debug info
//#define PLOT_MODE
