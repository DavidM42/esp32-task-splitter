#include "vibration.h"

void Vibration::setup()
{
    pinMode(vibration_pin, OUTPUT);
}

void Vibration::short_vibration()
{
    digitalWrite(vibration_pin, HIGH);
    delay(vibration_length_ms);
    digitalWrite(vibration_pin, LOW);
};
