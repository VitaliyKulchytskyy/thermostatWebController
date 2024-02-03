#pragma once
#include "Arduino.h"

/**
 * Interface of all serializable & deserializable classes
 */
class FormatBase {
public:
    /**
     * Deserialize the array of bytes into the instance of this class
     * @param [in] data array of bytes
     */
    virtual void deserialize(const uint8_t* data) = 0;

    /**
     * Get size of this class after serialization
     */
    virtual size_t size() const = 0;

    /**
     * Serialize this class into the array of bytes
     */
    virtual uint8_t* serialize() const = 0;
};