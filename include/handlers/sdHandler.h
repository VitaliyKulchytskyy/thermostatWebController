#pragma once
#include <SD.h>
#include "settings.h"
#include "metadata/DateFormat.h"
#include "handler.h"


/**
 * Setup the SD module.
 * @flags CS_PIN
 * @retval True - the SD is initialized
 * @retval False - the SD isn't initialized
 */
extern bool setupSD();

/**
 * Setup RTC time from the "time.bin" file on the SD and delete this file.
 * @example Bin format: SSMMHHDDMMYY [SecMinHourDayMonthYear] (1 byte per value)
 * @param [out] date date format
 * @retval True - the time is initialized
 * @retval False - the time isn't initialized (there is no the "time.bin" file)
 */
extern bool sdTimeSetting(DateFormat& date);

/**
 * Append a line to a file on the SD
 * @param filename the name of the file
 * @param buffer the buffer to be attached
 * @param bufferSize the size of the buffer
 */
extern void sd_append(const char* filename, uint8_t* buffer, size_t bufferSize);

/**
 * Override the file with the buffer
 * @param filename the name of the file
 * @param buffer the buffer to be attached
 * @param bufferSize the size of the buffer
 */
extern void sd_write(const char* filename, uint8_t* buffer, size_t bufferSize);