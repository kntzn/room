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
            fullWhite,
            mono,
            fade,
            fade_switch,
            fade_switch_random,
            rainbow_Sine,
            rainbow_HSV,
            night,
            RVD,
            rise,
            oldschool6,
            oldschoolRND,
            RVD_RND,
            //matrix,
            n_modes
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

    private:
        // ----------------------------------------
        // Enums
        enum frequences
            {
            lows,
            mids,
            highs
            };

        // ----------------------------------------
        // Parameters

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

        // Color of sections
        CRGB currColor3sections [N_SEC] =
            { 
            CRGB::Black,
            CRGB::Black,
            CRGB::Black
            };
        CRGB currColor3sectionsRVD [N_SEC] =
            {
            CRGB::Black,
            CRGB::Black,
            CRGB::Black
            };
        const CRGB Color3sectionsRVD [N_COLORS_RVD] =
            {
            CRGB::Red,
            CRGB::Blue,
            CRGB (32, 0, 255),
            CRGB::Yellow,
            CRGB (191, 0, 255),
            CRGB (0, 178, 255)
            };

        // ----------------------------------------
        // Internal Variables

        // Night mode activation time
        unsigned long int mode_activation_time = 0;
        
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

        CRGB leds_main_copy [N_LEDS_MAIN] = {};
        CRGB leds_table_copy [N_LEDS_TABLE] = {};

        // ----------------------------------------
        // Communication

        // Volume fou VU, VU_bright and VU_rain
        int VU_val = 0;

        // frequences arrays
        float frequency_3 [3] = {};
        float frequency_full [SPECTRUM_SIZE] = {};

        // -----------------UTIL-------------------
        // ----------------------------------------
        // Sync function
        void sync_strips ();

        // Fills sections with their colors
        void fillSections (CRGB sections [N_SEC]);
        
        // HSV HUE to rgb
        CRGB Wheel (byte WheelPos);

        /*
        off,
            fullWhite,
            mono,
            fade,
            fade_switch,
            fade_switch_random,
            rainbow_Sine,
            rainbow_HSV,
            night,
            RVD,
            rise,
            oldschool6,
            oldschoolRND,
            RVD_RND
        */

        void mainStrip_off_mode ();
        void mainStrip_fullWhite_mode ();
        void mainStrip_mono_mode ();
        void mainStrip_fade_mode ();
        void mainStrip_fade_switch_mode ();
        void mainStrip_fade_random_mode ();
        void mainStrip_rainbow_Sine_mode ();
        void mainStrip_rainbow_HSV_mode ();
        void mainStrip_night_mode ();
        void mainStrip_RVD_mode ();
        void mainStrip_rise_mode ();
        void mainStrip_oldschool_mode ();
        void mainStrip_RVD_RND_mode ();

        // ----------------------------------------
        // Display function
        // Shows the array
        void display ();

    public:
        // ----------------------------------------
        // Constructor
        // Initializes led strips
        StripController ();
        
        // ----------------------------------------
        // Update function
        // Fills and updates the arrays
        void update (float dt);

        // ----------SETTERS and GETTERS-----------
        void setMode (byte newMode);
        void setTableMode (byte newMode); 
        
        void setColor (CRGB newColor);
        void setSectionColor (CRGB newColor, byte sec_id);

        void setRainbowFrequency (float newFreq);
        void setRainbowSpeed (float newSpeed);
        void setPaletteSpeed (float newSpeed);
        void setFreqModeRainFreq (float newFreq);
        void setFreqModeRainOffset (float newOffset);

        // --------------COMMUNICATION-------------
        void setTrigger (bool value);
        void setVU_val (int newVU_val);
        void setFreq3values (float newFreqVal [3]);
        void setFreqValues (float newFreqVal [SPECTRUM_SIZE]);
    };

#endif