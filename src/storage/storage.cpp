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


void Storage::backup()
{   
    // TODO find way of not doing this that often
    Participants part_a = (Participants)0;
    Participants part_b = (Participants)1;

    // get current data into RAM
    Participants last_task_done = get_last_task_done_participant();
    int count_A = get_participant_count(part_a);
    int count_B = get_participant_count(part_b);
    uint32_t dt_A = get_participant_timestamp(part_a);
    uint32_t dt_B = get_participant_timestamp(part_b);

    // end preferences of main
    preferences.end();

    // save it all to backup namespace
    preferences.begin(backup_namespace_name, false); 
    save_participant_data(part_a, count_A, dt_A);
    save_participant_data(part_b, count_B, dt_B);
    save_last_task_done_participant(last_task_done);
    preferences.end();

    // and begin normal namespace again
    preferences.begin(namespace_name, false);
}

void Storage::restore()
{
    Participants part_a = (Participants)0;
    Participants part_b = (Participants)1;

    // end preferences of main
    preferences.end();

    // begin backup ns
    preferences.begin(backup_namespace_name, false); 

    // get backup data into RAM
    Participants last_task_done = get_last_task_done_participant();
    int count_A = get_participant_count(part_a);
    int count_B = get_participant_count(part_b);
    uint32_t dt_A = get_participant_timestamp(part_a);
    uint32_t dt_B = get_participant_timestamp(part_b);

    // close backup ns
    preferences.end();

    // save it all to default namespace
    preferences.begin(namespace_name, false); 
    save_participant_data(part_a, count_A, dt_A);
    save_participant_data(part_b, count_B, dt_B);
    save_last_task_done_participant(last_task_done);
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

void Storage::save_last_task_done_participant(Participants participant)
{
    preferences.putInt(last_save_participant_key, participant);
}

Participants Storage::get_last_task_done_participant()
{
    // TODO default value breaking here? At least no false values when no saved value exists but crash
    return (Participants)preferences.getInt(last_save_participant_key, 100);
}

void Storage::save_participant_data(Participants participant, int count, uint32_t timestamp)
{
    save_participant_count(count, participant);
    save_participant_timestamp(timestamp, participant);
    // remember who last did a task implicitly by who got last saved to
    save_last_task_done_participant(participant);
}