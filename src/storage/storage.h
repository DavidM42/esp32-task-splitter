#ifndef STORAGE_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define STORAGE_H

#include <Arduino.h>

// learned from
// https://randomnerdtutorials.com/esp32-save-data-permanently-preferences/
#include <Preferences.h>

#include "../participants.h"

extern Preferences preferences;

class Storage
{
private:

    const char* namespace_name = "spuelen";

    // these keys can not be longer than certain length!!
    // else it will silently fail!!
    const char* count_base_key = "count_";
    const char* last_datetime_base_key = "last_dt_";

    const char* last_save_participant_key = "last_task_p";

    /**
     * Save count of participant from permanent storage
    **/
    void save_participant_count(int count, Participants participant);

    /**
     * Save last datetime of participant doing it to permanent storage
    **/
    void save_participant_timestamp(uint32_t timestamp, Participants participant);

public:
    // Storage();
    ~Storage(); //destructor to close

    void setup();

    /**
     * Clears all values from namespace (related to project)
    **/
    void reset();

    /**
     * Get count of participant from permanent storage
    **/
    int get_participant_count(Participants participant);

    /**
     * Get last datetime of participant doing it from permanent storage
    **/
    uint32_t get_participant_timestamp(Participants participant);

    /**
     * Shorthand method to easily save all data for participant, count and timestsamp
    **/
    void save_participant_data(Participants participant, int count, uint32_t timestamp);

    /**
     * Get who last did task from permanent storage. Calculated by who last had count saved to.
    **/
    Participants get_last_task_done_participant();
};

#endif
