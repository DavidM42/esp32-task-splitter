#ifndef DISPLAY_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define DISPLAY_H

// Library: https://github.com/ZinggJM/GxEPD2
#include <Arduino.h>
#include <SPI.h>
#include "Wire.h"

#include <StreamString.h>
#define PrintString StreamString

// see GxEPD2_wiring_examples.h for wiring suggestions and examples

// base class GxEPD2_GFX can be used to pass references or pointers to the display instance as parameter, uses ~1.2k more code
// enable or disable GxEPD2_GFX base class
#define ENABLE_GxEPD2_GFX 0

// uncomment next line to use class GFX of library GFX_Root instead of Adafruit_GFX
//#include <GFX.h>
// Note: if you use this with ENABLE_GxEPD2_GFX 1:
//       uncomment it in GxEPD2_GFX.h too, or add #include <GFX.h> before any #include <GxEPD2_GFX.h>

#include <GxEPD2_BW.h>
#include <Fonts/FreeMonoBold9pt7b.h>

// alternative fonts
#include "Free_Fonts.h"
#include "Free_Sans.h"

// or select the display class and display driver class in the following file (new style):
// #include "./GxEPD2_display_selection_new_style.h"

// some pre calculation for initialising in cpp file 
#define MAX_DISPLAY_BUFFER_SIZE 65536ul // e.g.
#define MAX_HEIGHT(EPD) (EPD::HEIGHT <= MAX_DISPLAY_BUFFER_SIZE / (EPD::WIDTH / 8) ? EPD::HEIGHT : MAX_DISPLAY_BUFFER_SIZE / (EPD::WIDTH / 8))
extern GxEPD2_BW<GxEPD2_290_T5, MAX_HEIGHT(GxEPD2_290_T5)> dp;

#include "../participants.h"

class Display
{
private:
    // alternative default would be e.g. FreeMonoBold9pt7b 
    const GFXfont used_font = FreeSans9pt8b;

    /**
     * Underline of participant with turn should be longer than name by this amount of pixels in both direction
    **/
    int16_t underline_overshoot = 5;

    /**
     * Underline of participant with turn should have padding of this to text of participant.
    **/
    int16_t padding_text_underline = 5;

    /**
     * How many pixels thick is the underline of the upcoming task supposed to be
    **/
    int16_t underline_height = 3;

public:
    // Display();
    // ~Display(); //destructor to close

    // some basic methods
    void setup();
    void poweroff();
    void hibernate();


    void drawCenterText(const char* text);
    void drawParticipantUpdate(Participants last_saved_task_p, int count_A, int count_B, String timestring_A, String timestring_B);
};

#endif
