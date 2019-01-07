// WindowController.h

#ifndef _WINDOWCONTROLLER_h
#define _WINDOWCONTROLLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#include <Servo.h>

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

        return (value / N_MES);
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
        enum class state
            {
            closed_in = 0,
            opened = 1
            };
    private:
        bool autoMode;
        state mode, prev_mode;
        unsigned long int mode_switch, millis_init;
        BrightnessListener outside, inside;
       
        Servo window;

    public:
        WindowController ();

        void setMode (state newMode);
        void setAutoMode (bool isAuto);
        
        void update ();

        void listenBrightness ();


    };

#endif