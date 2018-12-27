// DoorCapSensor.h

#ifndef _DOORCAPSENSOR_h
#define _DOORCAPSENSOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class DoorCapSensor
    {
    private:
        bool wasSwitched;
        byte sensorPin;
        float virtualCapCharge;

    public:
        DoorCapSensor (byte pin_input);

        void update (float dt);

        bool itIsTimeToSwitchIsntIt ();
        
    };


#endif

