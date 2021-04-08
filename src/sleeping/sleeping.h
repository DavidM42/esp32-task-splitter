#ifndef SLEEPING_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define SLEEPING_H

#include <Arduino.h>

#include "../dates/dates.h"
#include "../touch/touch.h"

#define S_To_uS_FACTOR 1000000  //Conversion factor for seconds to micro seconds

class Sleeping
{
private:
    /** Helper methods for verbosity **/
    void print_wakeup_reason();
    void print_wakeup_touchpad();

    /* Methods to configure deepsleep triggers */
    void wakeup_after_midnight(Dates dates);
    void wakeup_after_touch(void (*tp_1_cb)(), void (*tp_2_cb)(), void (*tp_3_cb)());

public:
    /** Process what woke esp up and stuff **/
    void setup();

    boolean was_woken_up_by_timer();
    
    /** Configure all wanted wakeup sources (timer and 3 touchpads then go to deepsleep) **/
    void deepsleep_start(Dates dates, void (*tp_1_cb)(), void (*tp_2_cb)(), void (*tp_3_cb)());
};

#endif
