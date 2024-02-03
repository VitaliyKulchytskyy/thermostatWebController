#pragma once
#include "Arduino.h"
#include "../metadata.h"


/// Setup the timers
extern bool setupTimer();

/// Timer to save metadata
extern bool timerSaveMetadata();

/// Timer to update the RTC
extern bool timerDateRequest();