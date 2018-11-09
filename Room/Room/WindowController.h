// WindowController.h

#ifndef _WINDOWCONTROLLER_h
#define _WINDOWCONTROLLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Hbridge.h"
#include "Predefined.h"

struct BrightnessListener
    {
    byte pin = 255;
    int output_value = 0;
    unsigned long int measurementTime = 0;


    BrightnessListener (byte newPin)
        {
        pin = newPin;
        measurementTime = millis ();
        }
    
    int getBrightness ()
        {
        unsigned long int value = 0;
        for (int i = 0; i < N_MES; i++)
            value += analogRead (pin);

        return (value/ N_MES);
        }

    void update ()
        {
        if (millis () - measurementTime > BRIGHTNESS_UPDATE_PERIOD * 1000)
            {
            output_value = getBrightness ();
            measurementTime = millis ();
            }
        }

    int getValue ()
        {
        return output_value;
        }

    };

class WindowController
    {
    public:
        enum state
            {
            closed_in,
            opened,
            closed_out
            };
    private:
        bool autoMode = false;
        byte mode = closed_in, prev_mode = closed_in;
        unsigned long int mode_switch = 0;
        BrightnessListener outside = BrightnessListener (LIGHT_SENSOR_OUTSIDE);
        BrightnessListener inside = BrightnessListener (LIGHT_SENSOR_INSIDE);
        Hbridge bridge = Hbridge (BRIDGE_0, BRIDGE_1);
        
    public:
        WindowController ();

        void setMode (byte newMode);
        void setAutoMode (bool isAuto);

        void update ();

        void listenBrightness ();

    };

#endif