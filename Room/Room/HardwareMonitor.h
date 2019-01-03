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
        char raw_input [270];
        int params [50];
        byte index;
        String converted_string;

        LiquidCrystal_I2C lcd;

    public:
        
        HardwareMonitor ();

        void listenSerial ();
        
        //int getParameter ();
    };

#endif

