#include "HardwareMonitor.h"
// 
// 
// 

#include "HardwareMonitor.h"




HardwareMonitor::HardwareMonitor () :
    lastState (false),
    lcd (0x27, 16, 2),
    raw_input ({}),
    params (),
    index (0),
    converted_string (""),
    lastUpdate (-UPS_HWM*1000),
    lastHWMupdate (-UPS_HWM * 1000)
    {
    SERIAL_HW_MONITOR.begin (BAUD_RATE_SERIAL);

    
    lcd.init ();
    lcd.backlight ();
    }

long HardwareMonitor::msSinceLastHWMupdate ()
    {
    return millis () - lastHWMupdate;
    }


void HardwareMonitor::listenSerial ()
    {
    while (SERIAL_HW_MONITOR.available () > 0)
        {
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
            String value = "";
            while ((str = strtok_r (p, ";", &p)) != NULL)
                {
                converted_string = str;
                params [index] = converted_string.toInt ();
                //Serial.println (params [index]);
                index++;
                }
            index = 0;
            }
        }
    }

long HardwareMonitor::HWMuptime ()
    {
    return millis () - availStart;
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
    

    // Just to update timer
    available ();

    if (millis () - lastUpdate > 1000.f / (UPS_HWM))
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
            lcd.print (getParameter (HardwareMonitor::paramId::RAMload));
            lcd.print (" ");
            lcd.print (getParameter (HardwareMonitor::paramId::HardDisk));
            }

        lcd.display ();
        }
    }

bool HardwareMonitor::available ()
    {
    bool avail = (millis () - lastHWMupdate < HWM_TIMEOUT * 1000);

    if (!avail)
        availStart = millis ();

    return avail;
    }

bool HardwareMonitor::becomeAvailable ()
    {
    if (lastState == false &&
        available () == true)
        {
        lastState = available ();
        return true;
        }
    
    lastState = available ();
    return false;
    }

