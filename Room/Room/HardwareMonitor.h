// HardwareMonitor.h

#ifndef _HARDWAREMONITOR_h
#define _HARDWAREMONITOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

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

            GPUmemTotal = 22,
            GPUmemUsed = 23,
            GPUmemFree = 24,
            
            HardDisk = 25,

            nParameters = 26
            };

    private:
        bool lastState;
        char raw_input [400];
        byte index;
        float params [static_cast <int> (paramId::nParameters)];
        float brightnessLevel;
        long lastHWMupdate;
        
        String converted_string;
        
        void listenSerial ();

    public:
        HardwareMonitor ();


        void update ();

        float getParameter (paramId id);
        bool available ();

        float getCPUload ();
        float getCPUtemp ();
        float getGPUload ();

    };

#endif

