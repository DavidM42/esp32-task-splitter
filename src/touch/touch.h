#ifndef TOUCH_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define TOUCH_H

#include <Arduino.h>

#include "../vibration/vibration.h"

/**
 * All the pins used for touching
**/
#define TOUCH_PIN_1 2
#define TOUCH_PIN_2 4
#define TOUCH_PIN_3 15

/**
 * Empirical value via testing
*/
#define TOUCH_THRESHOLD 15

class Touch
{
private:
    /**
     * Delay to wait after first positive.
     * To make sure it's not a false positive repeat reading after time
    **/
    uint32_t make_sure_delay = 75;

public:
    // Touch();
    // void setup();

    /** Raw touch sensor values **/
    uint16_t touch1_raw_val();
    uint16_t touch2_raw_val();
    uint16_t touch3_raw_val();

    /** Check if touch sensor is touched and also vibrate if it was **/
    boolean is_1_touched_w_feedback(Vibration vib);
    /** Check if touch sensor is touched and also vibrate if it was **/
    boolean is_2_touched_w_feedback(Vibration vib);
    /** Check if touch sensor is touched and also vibrate if it was **/
    boolean is_3_touched_w_feedback(Vibration vib);
};

#endif
