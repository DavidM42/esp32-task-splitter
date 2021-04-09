#ifndef DATES_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define DATES_H

#include <Arduino.h>

// following docu on https://blog.mcxiaoke.com/ESPDateTime/
#include <DateTime.h>
#include <ESPDateTime.h>

class Dates
{
private:
    const char* local_timezone;

    // format time to string, using strftime
    // http://www.cplusplus.com/reference/ctime/strftime/
    const char* dt_string_fmt = "%e %h %R";  // e.g. " 7 Apr 17:57"
    const char* time_only_string_fmt = " %R";  // e.g. " 17:57"

    int today_relative_to_day;
    int today_relative_to_month;
    // const char* yesterday_relative_check_fmt = "%x"; // e.g. 08/23/01
    // String yesterday_relative_to;

public:
    // Time();
    // ~Time(); //destructor to close

    boolean display_update_needed;

    void setup(const char* timezone);

    /**
     * Check if day changed and today,yesterday,... not correct anymore.
     * As side task also checks wether today_relative_to_day and today_relative_to_month needs to be updated (happens once after midnight every day).
     * Only needed when device runs multiple days continiously because Screen would keep stating today then.
     * On restart always new displayed also why today_relative_to_day and so on are not saved to flash.
    **/
    boolean needs_to_update_display();

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
};

#endif
