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
    SERIAL_HW_MONITOR.begin (9600);

    pinMode (LED_BUILTIN, OUTPUT);
    
    lcd.init ();
    lcd.backlight ();
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

    if (millis () - lastUpdate > 1000.f / (UPS_HWM))
        {
        lastUpdate = millis ();

      
        lcd.clear ();
        lcd.home ();

        if (!available())
            {
            lcd.print ("HWM is not");
            lcd.setCursor (4, 1);
            lcd.print ("available");
            }
        else
            {
            lcd.setBacklight (127);

            lcd.print ("TDP: ");
            lcd.print (getParameter (HardwareMonitor::paramId::TDPpackage));
            lcd.print ("W;");

            lcd.setCursor (9, 0);
            lcd.print ("TEMP:");

            lcd.setCursor (0, 1);
            
            lcd.print (getParameter (HardwareMonitor::paramId::tempCore0));
            lcd.print ("C|");
            lcd.print (getParameter (HardwareMonitor::paramId::tempCore1));
            lcd.print ("C|");
            lcd.print (getParameter (HardwareMonitor::paramId::tempCore2));
            lcd.print ("C|");
            lcd.print (getParameter (HardwareMonitor::paramId::tempCore3));
            lcd.print ("C|");
            }
        
        lcd.display ();
        }
    }

bool HardwareMonitor::available ()
    {
    bool avail = (millis () - lastHWMupdate < HWM_TIMEOUT * 1000);

    

    return avail;
    }