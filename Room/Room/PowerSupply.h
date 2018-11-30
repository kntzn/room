// Wattmeter.h

#ifndef _POWERSUPPLY_h
#define _POWERSUPPLY_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Predefined.h"
#include <OneWire.h>
#include <DallasTemperature.h>

class PowerSupply
    {
    private:
        
        DallasTemperature DS18B20;

        byte pinVoltage;
        byte pinCurrent;

        float voltage, current, power, temperature;

        long aver_analog (uint8_t pin, size_t times = 10);
        
        long readVcc ();
    
        float voltage_prec (byte pin);
    
    public:
        PowerSupply (byte pinI, byte pinV, byte pinT);
    
        void update ();

        float getVoltage     ();
        float getCurrent     ();
        float getPower       ();
        float getTemperature ();

    };

#endif

