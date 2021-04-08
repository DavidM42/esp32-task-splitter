#include "sleeping.h"

/**
 * Function that prints the reason why the ESP32 woke up
**/
void Sleeping::print_wakeup_reason()
{
    esp_sleep_wakeup_cause_t wakeup_reason;
    wakeup_reason = esp_sleep_get_wakeup_cause();
    switch (wakeup_reason)
    {
        case 1:
            Serial.println("Wakeup caused by external signal using RTC_IO");
            break;
        case 2:
            Serial.println("Wakeup caused by external signal using RTC_CNTL");
            break;
        case 3:
            Serial.println("Wakeup caused by timer");
            break;
        case 4:
            Serial.println("Wakeup caused by touchpad");
            break;
        case 5:
            Serial.println("Wakeup caused by ULP program");
            break;
        default:
            Serial.println("Wakeup was not caused by deep sleep");
            break;
    }
}

/**
 * Returns true if ESP32 was woken up from deepsleep by timer set previously
**/
boolean Sleeping::was_woken_up_by_timer()
{
    esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();
    return wakeup_reason == (esp_sleep_wakeup_cause_t)3; 
}


/**
 * Function that prints the touchpad by which ESP32 has been awaken from sleep
**/
void Sleeping::print_wakeup_touchpad()
{
    touch_pad_t touchPin = esp_sleep_get_touchpad_wakeup_status();
    switch (touchPin)
    {
    case 0:
        Serial.println("Touch detected on GPIO 4");
        break;
    case 1:
        Serial.println("Touch detected on GPIO 0");
        break;
    case 2:
        Serial.println("Touch detected on GPIO 2");
        break;
    case 3:
        Serial.println("Touch detected on GPIO 15");
        break;
    case 4:
        Serial.println("Touch detected on GPIO 13");
        break;
    case 5:
        Serial.println("Touch detected on GPIO 12");
        break;
    case 6:
        Serial.println("Touch detected on GPIO 14");
        break;
    case 7:
        Serial.println("Touch detected on GPIO 27");
        break;
    case 8:
        Serial.println("Touch detected on GPIO 33");
        break;
    case 9:
        Serial.println("Touch detected on GPIO 32");
        break;
    default:
        Serial.println("Wakeup not by touchpad");
        break;
    }
}

void Sleeping::setup()
{
    //Print the wakeup reason for ESP32 and which touchpad too if possible
    print_wakeup_reason();
    print_wakeup_touchpad();
}

/**
 * Setup deepsleep mode to wake device up once 5 min after midnight to refresh relative days (today,tomorrow)
**/
void Sleeping::wakeup_after_midnight(Dates dates)
{
    // add 5 min just for good measure so it's actually tomorrow even with rtc offset
    const time_t seconds_left_today = dates.seconds_until_tomorrow() + (5 * 60);

    esp_sleep_enable_timer_wakeup(seconds_left_today * S_To_uS_FACTOR);
    Serial.println("Setup ESP32 to sleep for " + String(seconds_left_today / 60) +
                " Minutes");
}

/**
 * Add handlers to deepsleep to wakeup on touchpad presses
**/
void Sleeping::wakeup_after_touch(void (*tp_1_cb)(), void (*tp_2_cb)(), void (*tp_3_cb)())
{
    // setup wake up interrupts on my 3 touch pins
    touchAttachInterrupt(TOUCH_PIN_1, tp_1_cb, TOUCH_THRESHOLD);
    touchAttachInterrupt(TOUCH_PIN_2, tp_2_cb, TOUCH_THRESHOLD);
    touchAttachInterrupt(TOUCH_PIN_3, tp_3_cb, TOUCH_THRESHOLD);

    //Configure Touchpad as wakeup source
    // esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_TOUCHPAD);
    // touch_pad_intr_disable();
    esp_sleep_enable_touchpad_wakeup();
}

void Sleeping::deepsleep_start(Dates dates, void (*tp_1_cb)(), void (*tp_2_cb)(), void (*tp_3_cb)())
{
    // set wakeup timer for after midnight
    wakeup_after_midnight(dates);

    // set toucpad sensor triggers as wakeup source
    wakeup_after_touch(tp_1_cb, tp_2_cb, tp_3_cb);

    // and then go to sleep to
    esp_deep_sleep_start();
}
