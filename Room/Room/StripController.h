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
        // ----------------------------------------
        // Enums
        enum mainStripMode
            {
            off,
            mono,
            fade,
            fade_switch,
            fade_switch_random,
            rainbow_Sine,
            rainbow_HSV,
            //matrix,
            //night
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
        // Speed of rainbow palette (palette_offset)
        float palette_speed = 0.1f;
        // Speed of rainbow (rainbow_offset)
        float rainbow_speed = 0;
        // Number of full rainbows in strip
        float rainbow_freq = 0;
        // Offset of freq_full rainbow [0; 360]
        float freq_full_rainbow_offset = 0;
        // Number of full rainbow in freq_full mode
        float freq_full_rainbow_freq = 0;

        // Mode of main strip
        byte mode = off;
        // Mode of table strip
        byte table_mode = sync;
        
        // Color of fade and mono modes
        CRGB currColor = CRGB::Black;

        // ----------------------------------------
        // Internal Variables

        // Current color for fade_switch mode
        CRGB fadeSwitchColor = CRGB::Black;
        bool switchedColorFlag = false;

        // Offset of hue in VU_rain mode
        float palette_offset = 0.f;

        
        // Hue offset for rainbow/gradient modes
        float rainbow_offset = 0.f;

        // ----------------------------------------
        // Leds Arrays
        CRGB leds_main [N_LEDS_MAIN] = {};
        CRGB leds_table [N_LEDS_TABLE] = {};

        // ----------------------------------------
        // Communication

        // Volume fou VU and VU_rain
        int VU_val = 0;

        // Freqences arrays
        float freqency_3 [3] = {};
        float freqency_full [3] = {};

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

        // ----------SETTERS and GETTERS-----------
        void setMode (byte newMode);
        void setTableMode (byte newMode); 
        
        void setColor (CRGB newColor);

        void setRainbowFrequency (float newFreq);
        void setRainbowSpeed (float newSpeed);
        void setPaletteSpeed (float newSpeed);
        void setFreqModeRainFreq (float newFreq);
        void setFreqModeRainOffset (float newOffset);

        // --------------COMMUNICATION-------------
        void setVU_val (int newVU_val);
        void setFreq3values (float newFreqVal [3]);
        void setFreqValues (float newFreqVal [SPECTRUM_SIZE]);
    };

#endif