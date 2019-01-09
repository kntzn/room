// UI.h

#ifndef _UI_h
#define _UI_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <LiquidCrystal_I2C.h>
#include "Button.h"

class UI
    {
    private: 
        Button lButton, mButton, rButton;
        LiquidCrystal_I2C lcd;

    public:
        UI ();


    };

#endif

