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
        // Updates timer
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

void HardwareMonitor::update ()
    {
    listenSerial ();

    if (millis () - lastUpdate > 1000.f / static_cast <float> (UPS_HWM))
        {
        lastUpdate = millis ();

        Serial.println ();

        lcd.clear ();
        lcd.home ();

        if (millis () - lastHWMupdate > HWM_TIMEOUT*1000)
            {
            lcd.print ("HWM is not");
            lcd.setCursor (4, 1);
            lcd.print ("available");
            }
        else
            {
            /*
            Serial.println ();
            Serial.print (getParameter (HardwareMonitor::paramId::RAMloadPerc));
            Serial.print (" ");
            Serial.println (getParameter (HardwareMonitor::paramId::HardDisk));
            */
            }

        lcd.display ();
        }
    }

bool HardwareMonitor::available ()
    {
    return (millis () - lastHWMupdate < HWM_TIMEOUT * 1000);
    }
