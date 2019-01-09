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
    lastUpdate (-UPS_HWM * 1000),
    lastHWMupdate (-UPS_HWM * 1000),
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
                params [index] = converted_string.toInt ();
                
                index++;
                }
            index = 0;
            }
        }

    digitalWrite (LED_BUILTIN, LOW);
    }

int HardwareMonitor::getParameter (paramId id)
    {
    if (paramId::lowVal      < id &&
        paramId::nParameters > id)
        {
        return params [static_cast <int> (id)];
        }

    return -1;
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

bool HardwareMonitor::available ()
    {
    bool avail = (millis () - lastHWMupdate < HWM_TIMEOUT * 1000);

    return avail;
    }