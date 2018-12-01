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
        bool updAvail = 0;
        char lastChar = 0;
        String input;

        LiquidCrystal_I2C lcd;
        int hardwareState [N_HW_PARAMS] = {};

    public:
        HardwareMonitor ();

        void listenSerial ();
        
        void removeNoise ();

        void log ();
        void print ();

    };

#endif

