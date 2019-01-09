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
        float brightnessLevel;
        Button lButton, mButton, rButton;
        LiquidCrystal_I2C lcd;

        enum class hwmScreens
            {
            MAIN_SCR,
            CPU_SCR,
            GPU_SCR,
            MEM_SCR
            };

    public:
        UI ();

        void update (HardwareMonitor &hwm, LightController &ctrlr);

        void showHwmInfo (HardwareMonitor &hwm, hwmScreens hwmScreenId);
    };

#endif

