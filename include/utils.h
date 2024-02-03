#pragma once
#include "Arduino.h"


/**
 * Convert the array of bytes into the value of N-type.
 * @tparam N value type
 * @param [in] pArray the array of bytes
 * @return converted value
 */
template<typename N>
N ptov(const uint8_t * pArray) {
    const uint8_t sizeOfType = sizeof(N);
    auto temp = new uint8_t[sizeOfType]{};

    memcpy(temp, pArray, sizeOfType);
    N output = (*(N*)temp);
    delete[] temp;

    return output;
}

/**
 * atoi with replace value if the value more than min.
 * @param value str to convert
 * @param min min value
 * @param replace replace value
 * @return converted value to int
 */
extern int atoi_s(const char* value, int min = 0, int replace = 0);

/**
 * atof with replace value if the value more than min.
 * @param value str to convert
 * @param min min value
 * @param replace replace value
 * @return converted value to float
 */
extern float atof_s(const char* value, float min = 0, float replace = 0);