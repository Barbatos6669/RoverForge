#include "hc_sr04.hpp"

hc_sr04::hc_sr04(uint8_t trigPin, uint8_t echoPin, unsigned long timeout)
    : _trigPin(trigPin), _echoPin(echoPin), _timeout(timeout) {
    pinMode(_trigPin, OUTPUT);
    pinMode(_echoPin, INPUT);
}

unsigned long hc_sr04::pingMicroseconds() {
    // trigger a 10us pulse and measure echo in microseconds (pulseIn returns 0 on timeout)
    digitalWrite(_trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(_trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(_trigPin, LOW);

    return pulseIn(_echoPin, HIGH, _timeout);
}

long hc_sr04::readCm() {
    unsigned long duration = pingMicroseconds();
    if (duration == 0) {
        // timed out
        return -1;
    }

    // distance cm = (duration_us * 0.0343) / 2
    float dist = (duration * 0.0343f) / 2.0f;
    return (long)(dist + 0.5f);
}

long hc_sr04::readFiltered(uint8_t samples, unsigned int delayMs) {
    if (samples == 0) return -1;

    // collect samples
    const uint8_t maxSamples = 15;
    if (samples > maxSamples) samples = maxSamples;
    long readings[maxSamples];
    uint8_t validCount = 0;

    for (uint8_t i = 0; i < samples; ++i) {
        long r = readCm();
        readings[i] = r;
        if (r >= 0) validCount++;
        if (i + 1 < samples) delay(delayMs);
    }

    if (validCount == 0) return -1;

    // simple median of valid readings: copy valid ones into temp, sort, pick middle
    long tmp[maxSamples];
    uint8_t idx = 0;
    for (uint8_t i = 0; i < samples; ++i) {
        if (readings[i] >= 0) tmp[idx++] = readings[i];
    }

    // sort tmp[0..idx-1] (simple insertion sort, idx small)
    for (uint8_t i = 1; i < idx; ++i) {
        long key = tmp[i];
        int j = i - 1;
        while (j >= 0 && tmp[j] > key) {
            tmp[j + 1] = tmp[j];
            --j;
        }
        tmp[j + 1] = key;
    }

    uint8_t mid = idx / 2;
    return tmp[mid];
}

