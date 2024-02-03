#pragma once
#include "metadata/DateFormat.h"
#include "metadata/LightFormat.h"
#include "metadata/LogFormat.h"
#include "metadata/TemperatureFormat.h"
#include "metadata/WaterFlowFormat.h"
#include "metadata/ConfigFormat.h"
#include "ThermoregFormat.h"


extern DateFormat g_date;
extern DateFormat g_sdLastSaveDate;
extern TemperatureFormat g_tempIn;
extern TemperatureFormat g_tempOut;
extern TemperatureFormat g_tempWater;
extern LightFormat g_light;
extern LogFormat g_log;
extern WaterFlowFormat g_waterFlow;
extern DeviceConfig g_deviceConfig;
extern thermoreg_t g_thermoreg;

/**
 * The order of the array's elements depends on the UART byte order
 * @attention ORDER SENSITIVE!!!
 */
extern FormatBase* g_uartFormat[];
extern size_t g_uartSize;

/**
 * The order of the array's elements depends on the ".thd" file format
 * @attention ORDER SENSITIVE!!!
 */
extern FormatBase* g_thdFormat[];
extern size_t g_thdSize;

/// The "config.bin" bytes order
extern FormatBase* g_configFormat[];
extern size_t g_configSize;

/**
 * Deserialize the input array of bytes into
 * the FormatBase array based on the out array.
 * @param [in,out] out out array
 * @param outSize size of the out array
 * @param [in] in array of bytes
 * @attention Inequality of the out & in arrays will occur undefined behavior
 * @return FormatBase array with the order of elements as in the out array (size=outSize)
 */
extern void deserialize(FormatBase** out,
                        size_t outSize,
                        uint8_t* in);

/**
 * Serialize the array of FormatBase into the array of bytes.
 * @param [in] in the input array of FormatBase elements
 * @param inSize size of the input array
 * @param [out] out the serialized array of bytes
 * @return size (in bytes) of the output array
 */
extern size_t serialize(FormatBase** in,
                      size_t inSize,
                      uint8_t* out);

/**
 * Calc size of the FormatBase** elements in bytes
 * @param in input array of FormatBase** elements
 * @param inSize number of the array elements
 * @return size of the input array
 */
extern size_t formatSize(FormatBase** in, size_t inSize);

/// Setup part of modules
extern bool setupMetadata();