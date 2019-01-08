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
    converted_string (""),
    lcd (0x27, 16, 2)
    {
    SERIAL_HW_MONITOR.begin (9600);

    pinMode (LED_BUILTIN, OUTPUT);
    pinMode (HWM_AUTO_BRIGHTNESS_PIN, OUTPUT);

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
    
    // Auto brightness (finally, i've done it)
    // Measures average value
    unsigned long int measuredBrightness = 0;
    for (int i = 0; i < N_MES; i++)
        measuredBrightness += analogRead (LIGHT_SENSOR_INSIDE);
    measuredBrightness /= N_MES;

    // Creates value limit
    measuredBrightness = constrain (measuredBrightness, 50, 400);
    // Filters the value
    brightnessLevel =  0.05f * measuredBrightness + 0.95f * float (measuredBrightness);
    analogWrite (HWM_AUTO_BRIGHTNESS_PIN, 
                 map (brightnessLevel, 0, 400, 0, 255));

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