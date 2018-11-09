// Hbridge.h

#ifndef _HBRIDGE_h
#define _HBRIDGE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class Hbridge
    {
    private:
        bool invert = false;
        byte out0 = 255, out1 = 255;
        int speed = 0;


    public:
        Hbridge (byte pin0, byte pin1);

        // Sets new speed for H-bridge
        void setSpeed (int newSpeed);

        // Updates the bridge
        void update ();

        // Reverses the current direction of the bridge
        void reverse ();    
    };


#endif

