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
        bool state;
        byte sensorPin;
        float virtualCapCharge;

        unsigned long lastSwitchMillis;

    public:
        DoorCapSensor (byte pin_input);

        void update (float dt);

        bool itIsTimeToSwitchIsntIt ();
        
    };


#endif

