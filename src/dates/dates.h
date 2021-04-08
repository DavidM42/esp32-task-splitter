#ifndef DATES_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define DATES_H

#include <Arduino.h>

// following docu on https://blog.mcxiaoke.com/ESPDateTime/
#include <DateTime.h>
#include <ESPDateTime.h>

class Dates
{
private:
    int local_timezone;

    // format time to string, using strftime
    // http://www.cplusplus.com/reference/ctime/strftime/
    const char* dt_string_fmt = "%e %h %R";  // e.g. " 7 Apr 17:57"
    const char* time_only_string_fmt = " %R";  // e.g. " 17:57"

    const time_t seconds_in_day = 86400;

    int today_relative_to_day;
    int today_relative_to_month;

public:
    // Time();
    // ~Time(); //destructor to close

    // boolean display_update_needed;

    void setup(int timezone_offset);

    /**
     * Get current date and time in display ready format
    **/
    String now_formatted();

    /**
     * Get current date and time as localized timestamp (not utc)
    **/
    time_t now_timestamp();

    /**
     * Check if given date is today
    **/
    boolean isToday(time_t timestamp); 

    /**
     * Check if given date was yesterday
    **/
    boolean isYesterday(time_t timestamp); 
    
    /**
     * Check if given date was day before yesterday
    **/
    boolean isDayBeforeYesterday(time_t timestamp);

    /**
     * Get localized formatted date from local timestamp
    **/
    String format_timestamp_display_ready(time_t timestamp);

    /**
     * Give back seconds until tomorrow (so until midnight). Used by deepsleep timers
    **/
    uint64_t seconds_until_tomorrow();
};

#endif
