#ifndef VIBRATION_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define VIBRATION_H

#include <Arduino.h>

class Vibration
{
private:
    int vibration_pin = 22;
    uint32_t vibration_length_ms = 400;

public:
    void setup();
    void short_vibration();
};

#endif
