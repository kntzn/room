// 
// 
// 

#include "UI.h"

UI::UI () :
    brightnessLevel (0.f),
    lButton (BUTT_LEFT),
    mButton (BUTT_MIDL),
    rButton (BUTT_RGHT),
    lcd (0x27, 16, 2)
    {
    lcd.init ();
    lcd.backlight ();
    pinMode (HWM_AUTO_BRIGHTNESS_PIN, OUTPUT);
    }

void UI::update (HardwareMonitor & hwm, LightController & ctrlr)
    {
    // Auto brightness (finally, i've done it)
    // Measures average value
    unsigned long int measuredBrightness = 0;
    for (int i = 0; i < N_MES; i++)
        measuredBrightness += analogRead (LIGHT_SENSOR_INSIDE);
    measuredBrightness /= N_MES;

    // Creates value limit
    measuredBrightness = constrain (measuredBrightness, 50, 400);
    // Filters the value
    brightnessLevel = 0.05f * measuredBrightness + 0.95f * float (measuredBrightness);
    analogWrite (HWM_AUTO_BRIGHTNESS_PIN,
                 map (brightnessLevel, 0, 400, 0, 255));

    //lcd.clear ();
    lcd.home ();

    
    lcd.display ();

    }

void UI::showHwmInfo (HardwareMonitor & hwm, hwmScreens hwmScreenId)
    {
    if (!hwm.available ())
        {
        lcd.print ("HWM is not");
        lcd.setCursor (4, 1);
        lcd.print ("available");
        }
    else
        {
        switch (hwmScreenId)
            {
            case hwmScreens::MAIN_SCR:
                lcd.setCursor (0, 0);
                lcd.print ("CPU:");
                //lcd.print (hwm.getParameter (HardwareMonitor::paramId::))
                //hwm.getParameter (HardwareMonitor::paramId::)

                break;
            default:
                break;
            }
        /*
        lcd.print ("TDP: ");
        lcd.print (hwm.getParameter (HardwareMonitor::paramId::TDPpackage));
        lcd.print ("W;");

        lcd.setCursor (9, 0);
        lcd.print ("TEMP:");

        lcd.setCursor (0, 1);

        lcd.print (hwm.getParameter (HardwareMonitor::paramId::tempCore0));
        lcd.print ("C|");
        lcd.print (hwm.getParameter (HardwareMonitor::paramId::tempCore1));
        lcd.print ("C|");
        lcd.print (hwm.getParameter (HardwareMonitor::paramId::tempCore2));
        lcd.print ("C|");
        lcd.print (hwm.getParameter (HardwareMonitor::paramId::tempCore3));
        lcd.print ("C|");
        */
        }


    }
