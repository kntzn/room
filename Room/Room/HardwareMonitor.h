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
    public:
        enum class paramId
            {
            lowVal = -1,

            freqCore0 = 0,
            freqCore1 = 1,
            freqCore2 = 2,
            freqCore3 = 3,
            busSpeed = 4,
            tempCore0 = 5,
            tempCore1 = 6,
            tempCore2 = 7,
            tempCore3 = 8,
            tempPackage = 9,
            loadCore0 = 10,
            loadCore1 = 11,
            loadCore2 = 12,
            loadCore3 = 13,
            loadCPU = 14,
            TDPpackage = 15,
            TDPcores = 16,
            TDPgraphics = 17,
            TDP_DRAM = 18,

            RAMloadPerc = 19,
            RAMload = 20,
            RAMavail = 21,

            GPUcoreFreq = 22,
            GPUmemFreq = 23,
            GPUshaderFreq = 24,
            GPUcoreLoad = 25,
            GPUmemCtrlrLoad = 26,
            GPUvidEngLoad = 27,
            GPUmemLoad = 28,

            GPUmemFree = 29,
            GPUmemUsed = 30,
            GPUmemTotal = 31,

            HardDisk = 32,

            nParameters = 33
            };

    private:
        char raw_input [270];
        int params [static_cast <int> (paramId::nParameters)];
        byte index;
        String converted_string;

        long lastUpdate, lastHWMupdate;

        LiquidCrystal_I2C lcd;

    public:


        HardwareMonitor ();

        void listenSerial ();
        
        int getParameter (paramId id);

        void update ();
    };

#endif

