// StripController.h

#ifndef _STRIPCONTROLLER_h
#define _STRIPCONTROLLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <FastLED.h>



#include "Predefined.h"



class StripController
    {
    public:
        enum modes
            {
            off,
            mono,
            fade,
            fade_switch,
            rainbow_short,
            rainbow,
            rainbow_long,
            matrix,
            night
            };
        enum tableStripPower
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
        // parameters
        float palette_speed = 0.1f;

        byte table_mode = VU_rain;
        byte mode = mono;

        // internal variables
        float palette_offset = 0.f;

        int VU_val = 0;


        CRGB leds_main [N_LEDS_MAIN] = {};
        CRGB leds_table [N_LEDS_TABLE] = {};

    public:
        StripController ();
        
        void update (float dt);
        void setVU_val (int newVU_val);
        void display ();

    };

#endif