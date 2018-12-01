// HardwareMonitor.h

#ifndef _HARDWAREMONITOR_h
#define _HARDWAREMONITOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <LiquidCrystal_I2C.h>
#include "Predefined.h"

class HardwareMonitor
    {
    private:
        char lastChar = 0;
        String input;

        LiquidCrystal_I2C lcd;

    public:
        HardwareMonitor ();

        void listenSerial ();
    };

#endif

