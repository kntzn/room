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
        enum triggerTp
            {
            ifOpen,
            ifClosed,
            ifOpening,
            ifClosing
            };
    protected:
        bool state = opened, triggered = false;
        byte input_pin = 0, trigger_type = ifOpen;

    public:
        DoorSensor (byte pin)
            {
            input_pin = pin;
            }

        void update ();
        bool getState ();

        void setTriggerType (byte trg_type);

    };

#endif

