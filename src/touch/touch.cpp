#include "touch.h"

// void Touch::Touch()
// {
// }

// TODO later deep sleep code with touch https://randomnerdtutorials.com/esp32-touch-wake-up-deep-sleep/
uint16_t Touch::touch1_raw_val()
{
    return touchRead(TOUCH_PIN_1);
}

uint16_t Touch::touch2_raw_val()
{
    return touchRead(TOUCH_PIN_2);
}

uint16_t Touch::touch3_raw_val()
{
    return touchRead(TOUCH_PIN_3);
}

boolean Touch::is_1_touched_w_feedback(Vibration vib)
{
    // repeat reading after delay to make sure no unwanted spikes get registred
    delay(make_sure_delay);
    boolean is_surely_touched = (touchRead(TOUCH_PIN_1) <= TOUCH_THRESHOLD);
    if (is_surely_touched) {
        // Serial.println(touch_threshold);
        // Serial.println(is_touched);
        vib.short_vibration();
        return true;
    }
    return false;
}

boolean Touch::is_2_touched_w_feedback(Vibration vib)
{
    delay(make_sure_delay);
    boolean is_surely_touched = (touchRead(TOUCH_PIN_2) <= TOUCH_THRESHOLD);
    if (is_surely_touched) {
        // Serial.println(touch_threshold);
        // Serial.println(is_touched);
        vib.short_vibration();
        return true;
    }
    return false;
}

boolean Touch::is_3_touched_w_feedback(Vibration vib)
{
    delay(make_sure_delay);
    boolean is_surely_touched = (touchRead(TOUCH_PIN_3) <= TOUCH_THRESHOLD);
    if (is_surely_touched) {
        // Serial.println(touch_threshold);
        // Serial.println(is_touched);
        vib.short_vibration();
        return true;
    }
    return false;
}