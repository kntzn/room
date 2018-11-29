// Wattmeter.h

#ifndef _WATTMETER_h
#define _WATTMETER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Predefined.h"

class WattMeter
    {
    byte pinVoltage;
    byte pinCurrent;


    long aver_analog (uint8_t pin, size_t times = 10);
        
    long readVcc ();
    
    float voltage_prec (byte pin);
    
    public:
        WattMeter (byte pinI, byte pinV);
        
    private:

    };

#endif

