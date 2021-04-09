#include "dates.h"

void Dates::setup(int timezone_offset)
{
    local_timezone = timezone_offset;   
    DateTime.setTimeZone(local_timezone);
    DateTime.setServer("pool.ntp.org");
    DateTime.begin();
    if (!DateTime.isTimeValid()) {
        Serial.println("Failed to get time from server.");
        // TODO crash with new error module and message on display?
    }

    DateTimeParts p = DateTime.getParts();
    today_relative_to_day = p.getMonthDay();
    today_relative_to_month = p.getMonth();
    display_update_needed = false;
}

boolean Dates::needs_to_update_display()
{
    DateTimeParts p = DateTime.getParts();

    // if day or month does not match the day changed and display info is not relative to correct info anymore
    if (today_relative_to_day != p.getMonthDay() || today_relative_to_month != p.getMonth()) {
        today_relative_to_day = p.getMonthDay();
        today_relative_to_month = p.getMonth();
        display_update_needed = true;
    }
    return display_update_needed;
}


/******** Fancy formatting for display *********/

boolean Dates::isToday(time_t timestamp) 
{
    DateTimeParts p = DateTimeParts::from(timestamp);
    // same month and day+1 == today
    return p.getMonth() == today_relative_to_month && p.getMonthDay() == today_relative_to_day;
}

boolean Dates::isYesterday(time_t timestamp) 
{
    DateTimeParts p = DateTimeParts::from(timestamp);
    // same month and day+1 == today
    return p.getMonth() == today_relative_to_month && (p.getMonthDay() + 1) == today_relative_to_day;
}

boolean Dates::isDayBeforeYesterday(time_t timestamp) 
{
    DateTimeParts p = DateTimeParts::from(timestamp);
    // same month and day+2 == today
    return p.getMonth() == today_relative_to_month && (p.getMonthDay() + 2) == today_relative_to_day;
}


String Dates::format_timestamp_display_ready(time_t timestamp)
{
    if(timestamp <= 0) {
        // not 1 Jan 1970 but better empty string
        // more sane fallback for my use case
        return "";
    } else if (isToday(timestamp)) {
        // TODO localization
        String dateString = "Heute";
        String time = DateFormatter::format(time_only_string_fmt, timestamp, local_timezone);
        dateString.concat(time);
        return dateString;
    } else if (isYesterday(timestamp)) {
        // TODO localization
        String dateString = "Gestern";
        String time = DateFormatter::format(time_only_string_fmt, timestamp, local_timezone);
        dateString.concat(time);
        return dateString;
    } else if (isDayBeforeYesterday(timestamp)) {
        // TODO localization
        String dateString = "Vorgestern";
        String time = DateFormatter::format(time_only_string_fmt, timestamp, local_timezone);
        dateString.concat(time);
        return dateString;
    }
    // normal non yesterday or day before, soo longer time ago
    return DateFormatter::format(dt_string_fmt, timestamp, local_timezone);
}


/******** Needed functions *********/
String Dates::now_formatted()
{
    return format_timestamp_display_ready(now_timestamp());
}

time_t Dates::now_timestamp()
{
    return DateTime.getTime();
}