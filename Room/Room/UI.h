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
#include "HardwareMonitor.h"
#include "LightController.h"

class UI
    {
    private: 
        bool on, scrUpdAvail;
        signed char screenId, prev_screenId;
        float brightnessLevel;
        unsigned long lastScreenUpdate;
        Button lButton, mButton, rButton;
        LiquidCrystal_I2C lcd;
        
    public:
        UI ();

        void update (HardwareMonitor &hwm, LightController &ctrlr);

        void showHwmInfo (HardwareMonitor & hwm);

        void printLightingProfile (byte profile);
    };

#endif

