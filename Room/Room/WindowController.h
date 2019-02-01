// WindowController.h

#ifndef _WINDOWCONTROLLER_h
#define _WINDOWCONTROLLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#include <Servo.h>

#include "LightController.h"
#include "Predefined.h"

class BrightnessListener
    {
    private:
        byte pin = 255;
        int output_value = 0;
        unsigned long int measurementTime = 0;
    public:
        BrightnessListener (byte newPin);
    
        void update ();

        int getBrightness ();
        int getValue ();
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
        
        void update (LightController &ctrlr);

        void listenBrightness (LightController &ctrlr);


    };

#endif