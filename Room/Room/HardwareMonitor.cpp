#include "HardwareMonitor.h"
// 
// 
// 

#include "HardwareMonitor.h"




HardwareMonitor::HardwareMonitor () :
    lastState (false),
    raw_input ({}),
    index (0),
    params (),
    brightnessLevel (0.f),
    lastHWMupdate (0),
    converted_string ("")
    {
    SERIAL_HW_MONITOR.begin (9600);

    pinMode (LED_BUILTIN, OUTPUT);
    }

void HardwareMonitor::listenSerial ()
    {
    
    while (SERIAL_HW_MONITOR.available () > 0)
        {
        digitalWrite (LED_BUILTIN, HIGH);

        // Updates the timer
        lastHWMupdate = millis ();

        char aChar = SERIAL_HW_MONITOR.read ();

        // Debug
        /*Serial.print (aChar);
        if (aChar == ';')
            Serial.println ();*/

        if (aChar != 'E')
            {
            raw_input [index] = aChar;
            index++;
            raw_input [index] = '\0';
            }
        else
            {
            char *p = raw_input;
            char *str;
            index = 0;
            while ((str = strtok_r (p, ";", &p)) != NULL)
                {
                converted_string = str;
                params [index] = converted_string.toFloat ();
                
                

                index++;
                }
            index = 0;
            }
        }

    digitalWrite (LED_BUILTIN, LOW);
    }

void HardwareMonitor::update ()
    {
    listenSerial ();
    
    // Just to update flag
    if (!available ())
        index = 0;

    //if (millis () - lastUpdate > 1000.f / (UPS_HWM))
        //lastUpdate = millis ();
    }

float HardwareMonitor::getParameter (paramId id)
    {
    if (paramId::lowVal      < id &&
        paramId::nParameters > id)
        {

        //Serial.println (params [static_cast <int> (id)]);

        return params [static_cast <int> (id)];
        }

    return -1;
    }

bool HardwareMonitor::available ()
    {
    bool avail = (millis () - lastHWMupdate < HWM_TIMEOUT * 1000 &&
                  digitalRead (HWM_AVAILABLE));

    return avail;
    }

float HardwareMonitor::getCPUload ()
    {
    return (getParameter (HardwareMonitor::paramId::loadCore0) +
            getParameter (HardwareMonitor::paramId::loadCore1) +
            getParameter (HardwareMonitor::paramId::loadCore2) +
            getParameter (HardwareMonitor::paramId::loadCore3)) / 4;
    }

float HardwareMonitor::getCPUtemp ()
    {
    int core0 = getParameter (HardwareMonitor::paramId::tempCore0);
    int core1 = getParameter (HardwareMonitor::paramId::tempCore1);
    int core2 = getParameter (HardwareMonitor::paramId::tempCore2);
    int core3 = getParameter (HardwareMonitor::paramId::tempCore3);

    return max (max (core0, core1), max (core2, core3));
    }

float HardwareMonitor::getGPUload ()
    {
    return 100.f * getParameter (HardwareMonitor::paramId::GPUmemUsed)/
                   getParameter (HardwareMonitor::paramId::GPUmemTotal);
    }
