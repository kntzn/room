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
        enum class hwmScreens
            {
            MIN_VAL,

            MAIN_SCR = 1,
            CPU_SCR  = 2,
            GPU_SCR  = 3,
            MEM_SCR  = 4,

            MAX_VAL
            };
        bool on;
        byte screenId;
        float brightnessLevel;
        unsigned long lastScreenUpdate;
        Button lButton, mButton, rButton;
        LiquidCrystal_I2C lcd;
        

    public:
        UI ();

        void update (HardwareMonitor &hwm, LightController &ctrlr);

        void showHwmInfo (HardwareMonitor &hwm, byte hwmScreenId);
    };

#endif

