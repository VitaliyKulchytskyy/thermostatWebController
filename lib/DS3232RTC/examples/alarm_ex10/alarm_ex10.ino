// Arduino DS3232RTC Library
// https://github.com/JChristensen/DS3232RTC
// Copyright (C) 2022 by Jack Christensen and licensed under
// GNU GPL v3.0, https://www.gnu.org/licenses/gpl.html
//
// DS3231/DS3232 Alarm Example Sketch #10
//
// Wakes the MCU at regular intervals with an interrupt generated by
// an RTC alarm. After waking, the built-in LED is turned on for several
// seconds, then the MCU sleeps until the next alarm.
// Assumes that the RTC time was previously set and that the RTC is running.
//
// The "wakeInterval" variable sets the alarm interval, in minutes.
// The "ledInterval" variable sets the LED on-time, in seconds.
//
// Power to the RTC is supplied from an MCU pin, which is turned off while
// sleeping to minimize power consumption.
// Supply current while sleeping should be less than 1μA.
//
// Hardware:
// Arduino Uno, DS3231/2 RTC.
// Connect RTC SDA to Arduino pin A4.
// Connect RTC SCL to Arduino pin A5.
// Connect RTC INT/SQW to Arduino pin 2 or 3.
// Connect RTC Vcc to Arduino pin 5.
//
// Jack Christensen 17Jan2022

#include <avr/sleep.h>
#include <DS3232RTC.h>      // https://github.com/JChristensen/DS3232RTC
#include <Streaming.h>      // https://github.com/janelia-arduino/Streaming
#include <TimeLib.h>        // https://github.com/PaulStoffregen/Time
#include <Wire.h>           // https://arduino.cc/en/Reference/Wire

// globals
constexpr uint32_t wakeInterval {5};    // wake interval in minutes
constexpr uint32_t ledInterval {10};    // seconds to leave the LED on after an alarm
static_assert(ledInterval < wakeInterval*60, "ledInterval must be less than wakeInterval!");
constexpr uint8_t rtcIntPin {3};        // RTC interrupt. Pin 2 for INT0, Pin 3 for INT1.
constexpr uint8_t  rtcPower {5};        // RTC power is supplied by the MCU
volatile bool rtcAlarm {false};         // ISR flag
DS3232RTC myRTC;

void setup()
{
    Serial.begin(115200);
    Serial << F( "\n" __FILE__ "\n" __DATE__ " " __TIME__ "\n" );
    myRTC.begin();
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(rtcPower, OUTPUT);
    digitalWrite(rtcPower, HIGH);

    // enable pullup on interrupt pin (RTC SQW pin is open drain)
    pinMode(rtcIntPin, INPUT_PULLUP);

    // set up the interrupt handler
    attachInterrupt(digitalPinToInterrupt(rtcIntPin), rtcInterrupt, FALLING);

    myRTC.begin();
    Serial << F("Sketch start "); printTime(myRTC.get());
    Serial << F("Wake interval: ") << wakeInterval << F(" minutes.\n");
    Serial << F("Using interrupt INT") << digitalPinToInterrupt(rtcIntPin) << endl;
    Serial << F("RTC interrupt pin: ") << rtcIntPin << endl;
    Serial << F("RTC power pin: ") << rtcPower << endl;

    // initialize the alarms to known values, clear the alarm flags, clear the alarm interrupt flags
    myRTC.setAlarm(DS3232RTC::ALM1_MATCH_DATE, 0, 0, 0, 1);
    myRTC.setAlarm(DS3232RTC::ALM2_MATCH_DATE, 0, 0, 0, 1);
    myRTC.alarm(DS3232RTC::ALARM_1);
    myRTC.alarm(DS3232RTC::ALARM_2);
    myRTC.alarmInterrupt(DS3232RTC::ALARM_1, false);
    myRTC.alarmInterrupt(DS3232RTC::ALARM_2, false);
    myRTC.squareWave(DS3232RTC::SQWAVE_NONE);   // assert INT/SQW pin on alarm
}

void loop()
{
    enum states_t { wait, alarm, ledOn, setAlarm };
    static states_t state { setAlarm };
    static uint32_t msLED;

    switch (state)
    {
        case wait:
            Serial << F("Going to sleep at "); printTime(myRTC.get()); Serial.flush();
            digitalWrite(rtcPower, LOW);
            pinMode(rtcPower, INPUT);       // high-z, turn off power to the rtc
            pinMode(SDA, INPUT);            // high-z for the i2c bus
            pinMode(SCL, INPUT);
            goToSleep();
            pinMode(rtcPower, OUTPUT);
            digitalWrite(rtcPower, HIGH);
            delay(20);                      // a little wake-up time for the rtc & i2c
            if (rtcAlarm) { state = alarm; }
            break;

        case alarm:
            state = ledOn;
            Serial << F("Alarm at "); printTime(myRTC.get());
            myRTC.alarm(DS3232RTC::ALARM_1);    // clear alarm flag
            myRTC.alarmInterrupt(DS3232RTC::ALARM_1, false);
            noInterrupts();
            rtcAlarm = false;
            interrupts();
            msLED = millis();
            digitalWrite(LED_BUILTIN, HIGH);
            break;

        case ledOn:
            if (millis() - msLED >= ledInterval * 1000) {
                state = setAlarm;
                digitalWrite(LED_BUILTIN, LOW);
            }
            break;

        case setAlarm:
            state = wait;
            time_t utc = myRTC.get();
            time_t wake = wakeInterval * 60;
            time_t alm = utc - utc % wake + wake;
            Serial << F("Setting alarm for "); printTime(alm);
            myRTC.setAlarm(DS3232RTC::ALM1_MATCH_DATE, second(alm), minute(alm), hour(alm), day(alm));
            myRTC.alarmInterrupt(DS3232RTC::ALARM_1, true);
            break;
    }
}

// RTC interrupt handler
void rtcInterrupt()
{
    rtcAlarm = true;
}

// format and print a time_t value
void printTime(time_t t)
{
    char buf[25];
    char m[4];    // temporary storage for month string (DateStrings.cpp uses shared buffer)
    strcpy(m, monthShortStr(month(t)));
    sprintf(buf, "%.2d:%.2d:%.2d %s %.2d %s %d",
        hour(t), minute(t), second(t), dayShortStr(weekday(t)), day(t), m, year(t));
    Serial.println(buf);
}

void goToSleep()
{
    uint8_t adcsra = ADCSRA;        // sd_append the ADC Control and Status Register A
    ADCSRA = 0;                     // disable the ADC
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    cli();                          // stop interrupts to ensure the BOD timed sequence executes as required
    sleep_enable();
    sleep_bod_disable();            // disable brown-out detection while sleeping (20-25µA)
    sei();                          // ensure interrupts enabled so we can wake up again
    sleep_cpu();                    // go to sleep
    sleep_disable();                // wake up here
    ADCSRA = adcsra;                // restore ADCSRA
}
