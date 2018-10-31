// LightBulbController.h

#ifndef _LIGHTBULBCONTROLLER_h
#define _LIGHTBULBCONTROLLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class BulbController
    {
    public:
        enum state
            {
            off,
            on
            };

    private:
        // ----------------------------------------
        // Internal variables
        bool state = off;
        byte output_pin = 0;

    public:
        // ----------------------------------------
        // Constructor
        BulbController (byte pin);

        // ----------------------------------------
        // Update function
        // Updates the state of the pin
        void update ();

        // ----------------------------------------
        // Setstate function
        // Sets the state of the relay pin
        void setState (bool newState);      
    };

#endif

