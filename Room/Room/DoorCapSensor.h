// DoorCapSensor.h

#ifndef _DOORCAPSENSOR_h
#define _DOORCAPSENSOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Predefined.h"

#include <CapacitiveSensor.h>

#ifndef HOLD_TIME_CAP
#define HOLD_TIME_CAP 500
#endif // !HOLD_TIME


class LampCapSensor
    {
    public:
        

    private:
        bool flag = false;
        bool hold_flag = false;
        byte state = 0;
        float averageValue = 0.f;
        long int th0 = 0, th1 = 0;
        unsigned long int last_press = 0;

        CapacitiveSensor cs;

        void reset ();

    public:
        LampCapSensor (byte b_pin0, byte b_pin1, 
                       long int threshold0, long int threshold1);

        void update (bool isActive);
        byte getState ();
    };

class DoorCapSensor
    {
    public:
        enum buttonState
            {
            Unpr,
            Rlsd,
            Prs,
            Prsd,
            Hold
            };

    private:
        bool flag = false;
        bool hold_flag = false;
        byte state = 0;
        float averageValue = 0.f;
        unsigned long int last_press = 0;

        void reset ();

    public:
        DoorCapSensor ();

        void update ();
        byte getState ();
    };


//
//
//class DoorCapSensor
//    {
//    private:
//        bool toggle;
//        bool flag;
//        byte sensorPin;
//        int nSwtch;
//        float smoothAverage;
//
//        float risemax = 0;
//        
//        long lasRiseMaxUpd = 0;
//
//        unsigned long lastSwitchMillis;
//    public:
//        DoorCapSensor (byte pin_input);
//
//        void update (float dt);
//
//        bool itIsTimeToSwitchIsntIt ();
//
//        int nSwitches ();
//
//        float getMaxRise ();
//    };


#endif

