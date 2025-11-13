#pragma once

#include <Arduino.h>

class hc_sr04 {
public:
    // trigPin/echoPin are Arduino pin numbers. timeout is in microseconds.
    hc_sr04(uint8_t trigPin, uint8_t echoPin, unsigned long timeout = 30000UL);

    // Performs a single measurement and returns distance in centimeters.
    // Returns -1 if no echo was received (timeout).
    long readCm();

    // Take `samples` readings and return the median value (helps reject outliers).
    // Uses `delayMs` between samples (default 50ms). Returns -1 if all samples timed out.
    long readFiltered(uint8_t samples = 5, unsigned int delayMs = 50);

private:
    uint8_t _trigPin;
    uint8_t _echoPin;
    unsigned long _timeout;

    // low-level pulse measurement (returns microseconds, 0 on timeout)
    unsigned long pingMicroseconds();
};