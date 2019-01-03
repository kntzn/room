// 
// 
// 

#include "HardwareMonitor.h"


HardwareMonitor::HardwareMonitor () :
    lcd (0x27, 16, 2),
    raw_input ({}),
    params (),
    index (0),
    converted_string ("")
    {
    SERIAL_HW_MONITOR.begin (BAUD_RATE_SERIAL);

    lcd.init ();
    lcd.backlight ();
    }


void HardwareMonitor::listenSerial ()
    {
    while (SERIAL_HW_MONITOR.available () > 0)
        {
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
            String value = "";
            while ((str = strtok_r (p, ";", &p)) != NULL)
                {
                converted_string = str;
                params [index] = converted_string.toInt ();
                index++;
                }
            index = 0;
            }
        }

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
