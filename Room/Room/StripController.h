// StripController.h

#ifndef _STRIPCONTROLLER_h
#define _STRIPCONTROLLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Predefined.h"

#include <FastLED.h>

class StripController
    {
    public:
        enum mainStripMode
            {
            off,
            mono,
            fade,
            fade_switch,
            rainbow_SineWheel,
            rainbow_HSV,
            rainbow_Wheel,
            matrix,
            night
            };
        enum tableStripMode
            {
            sync,
            VU,
            VU_rain,
            VU_bright,
            FREQ_5,
            FREQ_3,
            FREQ_FULL
            };
        enum freqences
            {
            lows,
            mids,
            highs
            };

    private:
        // ----------------------------------------
        // Parameters
        float palette_speed = 0.1f;
        // Number of full rainbows shift
        // Values: [0; 1]
        float rainbow_speed = 0;
        // Number of full rainbows in strip
        float rainbow_freq = 0;

        // Mode of main strip
        byte mode = off;
        // Mode of table strip
        byte table_mode = sync;
        
        // ----------------------------------------
        // Internal Variables

        // Offset of hue in VU_rain mode
        float palette_offset = 0.f;

        // Volume fou VU and VU_rain
        int VU_val = 0;

        // Freqences arrays
        float freqency_3 [3] = {};
        float freqency_full [3] = {};

        // Hue offset for rainbow/gradient modes
        float rainbow_offset = 0.f;

        // ----------------------------------------
        // Leds Arrays
        CRGB leds_main [N_LEDS_MAIN] = {};
        CRGB leds_table [N_LEDS_TABLE] = {};

    public:
        // ----------------------------------------
        // Constructor
        // Initializes led strips
        StripController ();
        
        // ----------------------------------------
        // Update function
        // Fills and updates the arrays
        void update (float dt);
        
        // ----------------------------------------
        // Display function
        // Shows the array
        void display ();

        // -----------------UTIL-------------------
        // ----------------------------------------
        // Sync function
        void sync_strips ();

        
        // setters
        void setMode (byte newMode);
        void setTableMode (byte newMode); 

        void setVU_val (int newVU_val);
        void setFreq3values (float newFreqVal [3]);
    };

#endif