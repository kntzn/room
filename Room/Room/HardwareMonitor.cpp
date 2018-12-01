// 
// 
// 

#include "HardwareMonitor.h"


HardwareMonitor::HardwareMonitor ():
    lcd (0x27, 16, 2)
    {
    Serial1.begin (BAUD_RATE_SERIAL);
    }

void HardwareMonitor::listenSerial ()
    {
    //SERIAL_HW_MONITOR.

    // Parses full string
    while (Serial1.available ())
        {
        lastChar = Serial1.read ();
        if (lastChar == ';' || lastChar == 'E')
            {
            Serial.println (input.toInt ());
            if (lastChar == 'E')
                {
                Serial.println ();
                lastChar = 0;
                }
            input = "";
            }
        else
            input += lastChar;
        }

    
    }

