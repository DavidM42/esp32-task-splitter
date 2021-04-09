#include "storage.h"

Preferences preferences = Preferences();

// TODO don't know if that works actually
Storage::~Storage()
{
    // Serial.println("Closing preferences...");
    preferences.end(); // close namespace again
}

void Storage::setup()
{
    // Serial.println("Opening preferences namespace");
    preferences.begin(namespace_name, false); // false for not read only 
}

void Storage::reset()
{
    preferences.clear();
}



/****************** Specific saving and getting of values related to programm *******/

void Storage::save_participant_count(int count, Participants participant)
{
    // only stringify the index on enum so no problems with too long keys
    String combined_key( String(count_base_key) + String(participant) );
    preferences.putInt(combined_key.c_str(), count);
};

int Storage::get_participant_count(Participants participant)
{
    String combined_key( String(count_base_key) + String(participant) );
    return preferences.getInt(combined_key.c_str());
};


void Storage::save_participant_timestamp(uint32_t timestamp, Participants participant)
{
    String combined_key( String(last_datetime_base_key) + String(participant) );
    preferences.putInt(combined_key.c_str(), timestamp);
};

uint32_t Storage::get_participant_timestamp(Participants participant)
{
    String combined_key( String(last_datetime_base_key) + String(participant) );
    return preferences.getInt(combined_key.c_str());
};

void Storage::save_participant_data(Participants participant, int count, uint32_t timestamp)
{
    save_participant_count(count, participant);
    save_participant_timestamp(timestamp, participant);
    // remember who last did a task implicitly by who got last saved to
    preferences.putInt(last_save_participant_key, participant);
}

Participants Storage::get_last_task_done_participant()
{
    // TODO default value breaking here? At least no false values when no saved value exists but crash
    return (Participants)preferences.getInt(last_save_participant_key, 100);
}