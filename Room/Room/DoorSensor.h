// DoorSensor.h

#ifndef _DOORSENSOR_h
#define _DOORSENSOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class DoorSensor
    {
    public:
        enum doorState
            {
            opened = 0,
            closed = 1
            };
    protected:
        bool state = opened;
        byte input_pin = 0;
    public:
        DoorSensor (byte pin)
            {
            input_pin = pin;
            }

        void update ();
        bool getState ();
    };

#endif

