// Button.h

#ifndef _BUTTON_h
#define _BUTTON_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#pragma once
#ifndef HOLD_TIME
#define HOLD_TIME 1500
#endif // !HOLD_TIME
class Button
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
        byte pin = 0;
        byte state = 0;
        unsigned long int last_press = 0;

        void reset ();
        
    public:
        Button (byte b_pin);
        
        void update ();
        byte getState ();
    };


#endif