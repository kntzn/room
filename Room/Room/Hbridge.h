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
        int speed = 0;
        byte out0 = 255, out1 = 255;


    public:
        Hbridge (byte pin0, byte pin1);

        // Sets new speed for H-bridge
        void setSpeed (int newSpeed);

        // Updates the bridge
        void update ()
            {
            if (out0 == 255 || out1 == 255)
                return;

            if (speed > 0)
                {
                analogWrite (out0, 0);
                analogWrite (out1, speed);
                }
            else if (speed < 0)
                {
                analogWrite (out1, 0);
                analogWrite (out0, -speed);
                }
            else
                speed = 0;
            }
    };


#endif

