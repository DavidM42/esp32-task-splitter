#include <Arduino.h>
#include <SPI.h>
#include "Wire.h"

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <SPIFFS.h>

/**** Own modules ******/
#include "./display/display.h"
#include "./storage/storage.h"
#include "./dates/dates.h"
#include "./touch/touch.h"
#include "./vibration/vibration.h"
// TODO new error handling module
// has method to receive error centrally displays on fresh epaper, serial print then shutdown esp

// keep ssid,pass and participants out of git
#include "config.h"
#include "participants.h"

// unsigned because timestamp for my use case never before 1970 so never less than 0
// uint32_t example_timestamp = 1617802892;
uint32_t example_timestamp = 13478478;

Storage storage;
Vibration vibration;
Display display;
Dates dates;
Touch touch;

AsyncWebServer server(80);

// by index just get both for use later
Participants participant_A = (Participants)0;
Participants participant_B = (Participants)1;

/**
 * Update all the values displayed on the E-Paper with values from permanent storage
**/
void update_screen_saved_values() {
    String a_formatted_time = dates.format_timestamp_display_ready(storage.get_participant_timestamp(participant_A));
    int a_count =  storage.get_participant_count(participant_A);
    String b_formatted_time = dates.format_timestamp_display_ready(storage.get_participant_timestamp(participant_B));
    int b_count =  storage.get_participant_count(participant_B);
    Participants last_saved_task_p = storage.get_last_task_done_participant();
    display.drawParticipantUpdate(last_saved_task_p, a_count, b_count, a_formatted_time, b_formatted_time);
}

/**
 * What happens on unknown web server routes
**/
void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

/**
 * Starts filestystem
**/
void fs_setup()
{
    // Initialize SPIFFS
    if(!SPIFFS.begin(true)){
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }
    delay(200);
}

/**
 * Loop until connected to wifi also does MDNS
**/
void wifi_setup()
{
    Serial.print("Connecting wifi to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    delay(2000);
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    delay(50);

    if (!MDNS.begin(hostname)) { //http://esp32.local
        Serial.println("Error setting up MDNS responder!");
        while (1) {
            delay(1000);
        }
    }
    delay(200);
}

void web_server_setup()
{
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/html", "<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><meta http-equiv='X-UA-Compatible' content='IE=edge'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>Index</title></head><body><a href='/backup'>Backup data</a></br><a href='/restore'>Restore data from Backup</a></br><a href='/reset'>Reset data</a></body></html>");
    });
    server.on("/backup", HTTP_GET, [](AsyncWebServerRequest *request){
        storage.backup();
        request->send(200, "text/plain", "Backup complete");
    });
    server.on("/restore", HTTP_GET, [](AsyncWebServerRequest *request){
        storage.restore();
        update_screen_saved_values();
        request->send(200, "text/plain", "Restore complete");
    });
    server.on("/reset", HTTP_GET, [](AsyncWebServerRequest *request){
        storage.reset();
        update_screen_saved_values();
        request->send(200, "text/plain", "Reset complete");
    });

    server.onNotFound(notFound);

    // does not work sadly
    server.serveStatic("/", SPIFFS, "/www/").setDefaultFile("index.html");

    server.begin();
}

void ota_setup()
{
    // from https://github.com/espressif/arduino-esp32/blob/master/libraries/ArduinoOTA/examples/BasicOTA/BasicOTA.ino
    ArduinoOTA
    .onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
        else // U_SPIFFS
        type = "filesystem";

        // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
        Serial.println("Start updating " + type);
    })
    .onEnd([]() {
        Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
        else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });
    ArduinoOTA.begin();
}


/**
 * Save epaper lifetime and shut esp off
**/
void shutdown() {
    display.poweroff();
    esp_deep_sleep_start();
}

void setup() {
    Serial.begin(115200);
    delay(2000); // give me time to bring up serial monitor

    wifi_setup();

    fs_setup();

    // start modules 
    display.setup();
    storage.setup();
    dates.setup(timezone);
    vibration.setup();
    
    web_server_setup();

    ota_setup();

    display.drawCenterText("Booted successfully...");
    delay(300);
    update_screen_saved_values();

    // TODO do after errors or action to sleep    
    // shutdown();
}

void loop() {
    // touch.is_1_touched_w_feedback(vibration);
    // touch.is_2_touched_w_feedback(vibration);
    // touch.is_3_touched_w_feedback(vibration);
    // delay(90);

    // TODO not a main loop continously running but touch wakeups and deepsleep
    // every time wakeup also calculate ms until after midnight is reached then set that as deepsleep wakeup timer too

    if (touch.is_1_touched_w_feedback(vibration)) {
        // new values and save them
        int newCount = storage.get_participant_count(participant_A) + 1;
        uint32_t newTimestamp = dates.now_timestamp();
        String newFormattedTime = dates.now_formatted();
        storage.save_participant_data(participant_A, newCount, newTimestamp);

        // get rest of values to draw
        String b_formatted_time = dates.format_timestamp_display_ready(storage.get_participant_timestamp(participant_B));
        int b_count =  storage.get_participant_count(participant_B);
        Participants last_saved_task_p = participant_A;
        // could also get from storage but directly assigning works too
        // Participants last_saved_task_p = storage.get_last_task_done_participant();
        
        display.drawParticipantUpdate(last_saved_task_p, newCount, b_count, newFormattedTime, b_formatted_time);

        Serial.println("----------------------");
        Serial.print("David count now ");
        Serial.println(newCount);
        Serial.print("David new time: ");
        Serial.println(newFormattedTime);
    } else if(touch.is_2_touched_w_feedback(vibration)) {
        // new values and save them instantly
        int newCount = storage.get_participant_count(participant_B) + 1;
        uint32_t newTimestamp = dates.now_timestamp();
        String newFormattedTime = dates.now_formatted();
        storage.save_participant_data(participant_B, newCount, newTimestamp);

        // get rest of values to draw
        String a_formatted_time = dates.format_timestamp_display_ready(storage.get_participant_timestamp(participant_A));
        int a_count =  storage.get_participant_count(participant_A);
        Participants last_saved_task_p = participant_B;
        // could also get from storage but directly assigning works too
        // Participants last_saved_task_p = storage.get_last_task_done_participant();

        display.drawParticipantUpdate(last_saved_task_p, a_count, newCount, a_formatted_time, newFormattedTime);

        Serial.println("----------------------");
        Serial.print("Niklas count now ");
        Serial.println(newCount);
        Serial.print("Niklas new time: ");
        Serial.println(newFormattedTime);
    }
    /*
    else if(touch.is_3_touched_w_feedback(vibration)) {
        // TODO more reset logic with reset display on epaper
        Serial.println("Reset counts and timestamps");
        storage.reset();
        update_screen_saved_values();
    }*/
    delay(80);

    // would not need to be called so often but only every few minutes but whatever
    if (dates.needs_to_update_display())
    {
        dates.display_update_needed = false;
        display.drawCenterText("Will update values to new day!");
        delay(2000);
        update_screen_saved_values();
    }

    ArduinoOTA.handle();
}
