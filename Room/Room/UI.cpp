// 
// 
// 

#include "UI.h"

UI::UI () :
    currScreen (hwmScreens::MAIN_SCR),
    brightnessLevel (0.f),
    lastScreenUpdate (0),
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

    // Updates the screen
    if (millis () - lastScreenUpdate > 1000 / (UPS_SCREEN))
        {
        lastScreenUpdate = millis ();
        
        showHwmInfo (hwm, currScreen);
        
        lcd.display ();
        }

    }

void UI::showHwmInfo (HardwareMonitor & hwm, hwmScreens hwmScreenId)
    {
    if (!hwm.available ())
        {
        lcd.setCursor (0, 0);
        lcd.print ("   HWM is not   ");
        lcd.setCursor (0, 1);
        lcd.print ("   available    ");
        }
    else
        {
        switch (hwmScreenId)
            {
            case hwmScreens::MAIN_SCR:
                // CPU
                lcd.setCursor (0, 0);
                lcd.print ("CPU:");
                lcd.print (hwm.getCPUtemp ());
                lcd.print ("C   ");
                lcd.setCursor (8, 0);
                lcd.print ("LOAD:");
                lcd.print (hwm.getCPUload ());
                lcd.print ("%   ");

                // GPU
                lcd.setCursor (0, 1);
                lcd.print ("GPU:");
                lcd.print (hwm.getParameter (HardwareMonitor::paramId::GPUmemLoad));
                lcd.print ("% ");

                // RAM
                lcd.setCursor (8, 1);
                lcd.print ("RAM:");
                lcd.print (hwm.getParameter (HardwareMonitor::paramId::RAMloadPerc));
                lcd.print ("%");
                
                break;
            default:
                break;
            }
        }


    }
