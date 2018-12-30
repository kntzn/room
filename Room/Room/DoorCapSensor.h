// DoorCapSensor.h

#ifndef _DOORCAPSENSOR_h
#define _DOORCAPSENSOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Predefined.h"

class DoorCapSensor
    {
    private:
        bool toggle;
        bool flag;
        byte sensorPin;
        float smoothAverage;
        float rsVal = 0;
        float maxRsVal = 0;

        unsigned long lastSwitchMillis;

    public:
        DoorCapSensor (byte pin_input);

        void update (float dt);

        bool itIsTimeToSwitchIsntIt ();


        float riseVal ()
            {
            return rsVal;
            }
        float maxRiseVal ()
            {
            return maxRsVal;
            }
    };


#endif

