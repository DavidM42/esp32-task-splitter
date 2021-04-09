#include "display.h"

// manually started not selection file for custom pinout of az delivery mosi,sck force pins
GxEPD2_BW<GxEPD2_290_T5, MAX_HEIGHT(GxEPD2_290_T5)> dp = GxEPD2_BW<GxEPD2_290_T5, MAX_HEIGHT(GxEPD2_290_T5)>(GxEPD2_290_T5(/*CS=*/26, /*DC=*/25, /*RST=*/33, /*BUSY=*/27));


void Display::setup()
{
    dp.init(115200);
}

/********** Re implement some display driver methods *************/

/**
 * turns off generation of panel driving voltages, avoids screen fading over time
**/
void Display::poweroff()
{
    dp.powerOff();
}

/**
 * turns powerOff() and sets controller to deep sleep for minimum power use, ONLY if wakeable by RST (rst >= 0)
**/
void Display::hibernate()
{
    dp.hibernate();
}
/******************************************/


/**
 * Generic draw some text at center of screen with full window refresh.
 * Maybe used later for error messages
**/
void Display::drawCenterText(const char* text)
{
    //Serial.println("helloWorld");
    dp.setRotation(1);
    dp.setFont(&used_font);
    dp.setTextColor(GxEPD_BLACK);
    int16_t tbx, tby; uint16_t tbw, tbh;
    dp.getTextBounds(text, 0, 0, &tbx, &tby, &tbw, &tbh);
    // center bounding box by transposition of origin:
    uint16_t x = ((dp.width() - tbw) / 2) - tbx;
    uint16_t y = ((dp.height() - tbh) / 2) - tby;
    dp.setFullWindow();
    dp.firstPage();
    do
    {
        dp.fillScreen(GxEPD_WHITE);
        dp.setCursor(x, y);
        dp.print(text);
    }
    while (dp.nextPage());

    // TODO when used for error messages then also poweroff display before deepsleep
    // only rare updates of screen planned so okay after every change
    // poweroff();
}


// TODO maybe split this up better into functions for readability
// functions to calculate x and y values e.g.
/**
 * Draw complete UI and info onto E-Paper. Full screen refresh for now with all infos.
 * Ensures longer E-Paper lifetime since only rare refresh.
**/
void Display::drawParticipantUpdate(Participants last_saved_task_p,
    int count_A,
    int count_B,
    String timestring_A,
    String timestring_B
){
    // some setup
    dp.setRotation(1);
    dp.setFont(&used_font);
    dp.setTextColor(GxEPD_BLACK);

    /***************************** PARCTIPICANTS ***************************/

    // get both participants names as const char*
    const char* participant_A = ParticipantAsString((Participants)0);
    const char* participant_B = ParticipantAsString((Participants)1);

    /*** 
     * Figure out position of first participants name dynamically by screen size 
    **/
    int16_t tbx_A, tby_A; uint16_t tbw_A, tbh_A;
    dp.getTextBounds(participant_A, 0, 0, &tbx_A, &tby_A, &tbw_A, &tbh_A);
    // left top quadrant center bounding box by transposition of origin:
    uint16_t x_A = ((dp.width() - tbw_A) / 4);
    uint16_t y_A = ((dp.height() - tbh_A) / 2 / 2) - tby_A;

    /*** 
     * Figure out position of second participants name dynamically by screen size 
    **/
    int16_t tbx_B, tby_B; uint16_t tbw_B, tbh_B;
    dp.getTextBounds(participant_B, 0, 0, &tbx_B, &tby_B, &tbw_B, &tbh_B);
    // right top quadrant center bounding box by transposition of origin:
    uint16_t x_B = ((dp.width() - tbw_B) / 2 * 1.5);
    uint16_t y_B = ((dp.height() - tbh_B) / 2 / 2) - tby_B;

    /**************************** COUNTS ******************************/


    // char count_str_A[3];
    // String temp_str_a_count = String(count_A);
    // temp_str_a_count.toCharArray(count_str_A,3); 
    String count_str_A = String(count_A);
    // TODO internalization module
    count_str_A.concat(" mal");

    // char count_str_B[3];
    // String temp_str_b_count = String(count_B);
    // temp_str_b_count.toCharArray(count_str_B,3); 
    String count_str_B = String(count_B);
    count_str_B.concat(" mal");

    /*** 
     * Figure out position of first participants count center left
    **/
    int16_t tbx_count_A, tby_count_A; uint16_t tbw_count_A, tbh_count_A;
    dp.getTextBounds(count_str_A, 0, 0, &tbx_count_A, &tby_count_A, &tbw_count_A, &tbh_count_A);
    // left top quadrant center bounding box by transposition of origin:
    uint16_t x_count_A = ((dp.width() - tbw_count_A) / 4) - tbx_count_A;
    uint16_t y_count_A = ((dp.height() - tbh_count_A) / 2) - tby_count_A;

    /*** 
     * Figure out position of second participants count center right
    **/
    int16_t tbx_count_B, tby_count_B; uint16_t tbw_count_B, tbh_count_B;
    dp.getTextBounds(participant_B, 0, 0, &tbx_count_B, &tby_count_B, &tbw_count_B, &tbh_count_B);
    // right top quadrant center bounding box by transposition of origin:
    // uint16_t x_count_B = ((dp.width() - tbw_count_B) / 4 * 3) - tbx_count_B;
    uint16_t x_count_B = ((dp.width()- tbw_count_B) / 2 * 1.5);
    uint16_t y_count_B = ((dp.height() - tbh_count_B) / 2) - tby_count_B;


    /**************************** DATETIMES ******************************/

    /*** 
     * Figure out position of first participants datetime center left bottom quadrant
    **/
    int16_t tbx_dt_A, tby_dt_A; uint16_t tbw_dt_A, tbh_dt_A;
    dp.getTextBounds(timestring_A, 0, 0, &tbx_dt_A, &tby_dt_A, &tbw_dt_A, &tbh_dt_A);
    // left top quadrant center bounding box by transposition of origin:
    // uint16_t x_dt_A = ((dp.width() - tbw_dt_A) / 2 / 2) - tbx_dt_A;
    // uint16_t x_dt_A = ((dp.width() -tbw_dt_A) / 4) - tbx_dt_A;
    uint16_t x_dt_A = ((dp.width() - tbw_dt_A) / 4 * 0.75) - tbx_dt_A;
    uint16_t y_dt_A = ((dp.height() - tbh_dt_A) / 2 / 2 * 3) - tby_dt_A;

    /*** 
     * Figure out position of second participants datetime center right bottom quadrant
    **/
    int16_t tbx_dt_B, tby_dt_B; uint16_t tbw_dt_B, tbh_dt_B;
    dp.getTextBounds(timestring_B, 0, 0, &tbx_dt_B, &tby_dt_B, &tbw_dt_B, &tbh_dt_B);
    // left top quadrant center bounding box by transposition of origin:
    // uint16_t x_dt_B = ((dp.width() - tbw_dt_B) / 2 / 2 * 3) - tbx_dt_B;
    uint16_t x_dt_B = ((dp.width()- tbw_dt_B) / 2 * 1.75);
    uint16_t y_dt_B = ((dp.height() - tbh_dt_B) / 2 / 2 * 3) - tby_dt_B;


    /**************************** UI SPACERS ******************************/

    /**
     * Calculation for middle line seperating
    **/
    int line_width = 4;
    int line_x = (dp.width()/2 - (line_width/2));
    int line_y = dp.height();
    // int line_y = (dp.height() * 0.70); // only go 70% down so can show additional info in center

    /**************************** Next task assignment ******************************/
    
    Participants part_A = (Participants)0;
    Participants part_B = (Participants)1;

    // for now default fallback to me to catch blame but preferred no fallback
    Participants task_assigned_p = part_B;
    if (count_A == 0 && count_B == 0) {
        task_assigned_p = part_B;
    } else if (count_A < count_B) {
        task_assigned_p = part_A;
    } else if (count_B < count_A) {
        task_assigned_p = part_B;
    } else {
        // have same amount of count so flip flow between depending on last task
        if (last_saved_task_p == part_A) {
            task_assigned_p = part_B;
        } else if (last_saved_task_p == part_B) {
            task_assigned_p = part_A;
        }
    }


    /**************************** FINALLY PAINT ******************************/

    dp.setFullWindow();
    dp.firstPage();
    do
    {
        dp.fillScreen(GxEPD_WHITE);

        // draw seperation middle line
        dp.fillRect(line_x, 0, line_width, line_y, GxEPD_BLACK);

        // finally draw both participants
        dp.setCursor(x_A, y_A);
        dp.print(participant_A);
        if (task_assigned_p == part_A) {
            // it's turn of participant A so border around him
            dp.fillRect(x_A - underline_overshoot,
                y_A + padding_text_underline,
                tbw_A + (2*underline_overshoot),
                underline_height,
                GxEPD_BLACK
            );
        } // and count of A
        dp.setCursor(x_count_A, y_count_A);
        dp.print(count_str_A);
        dp.setCursor(x_dt_A, y_dt_A); // and time
        dp.print(timestring_A);


        dp.setCursor(x_B, y_B);
        dp.print(participant_B);
        if (task_assigned_p == part_B) {
            // it's turn of participant B so border around him
            dp.fillRect(x_B - underline_overshoot,
                y_B + padding_text_underline,
                tbw_B + (2*underline_overshoot),
                underline_height,
                GxEPD_BLACK
            );
        } // and count of B
        dp.setCursor(x_count_B, y_count_B);
        dp.print(count_str_B);
        dp.setCursor(x_dt_B, y_dt_B); // and time
        dp.print(timestring_B);
    }
    while (dp.nextPage());

    // finally also shutdown driver circuit for longer lifetime of display
    // only rare updates of screen planned so okay after every change
    poweroff();
}