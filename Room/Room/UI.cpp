// 
// 
// 

#include "UI.h"

UI::UI () :
    on (false), 
    scrUpdAvail (true),
    screenId (0),
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

    // Buttons 
    lButton.update ();
    mButton.update ();
    rButton.update ();

    // Buttons' actions
    if (lButton.getState () == Button::Rlsd)
        {
        screenId--;
        scrUpdAvail = true;
        Serial.println (__LINE__);
        }
    if (rButton.getState () == Button::Rlsd)
        {
        screenId++;
        scrUpdAvail = true;
        Serial.println (__LINE__);
        }
    if (mButton.getState () == Button::Hold)
        {
        on = !on;
        scrUpdAvail = true;
        Serial.println (__LINE__);
        }
    if (!on && mButton.getState () == Button::Rlsd)
        ctrlr.setProfile (screenId);
        

    // Constrains screenId
    if (on)
        {
        if (screenId <= byte (hwmScreens::MIN_VAL))
            screenId = byte (hwmScreens::MIN_VAL) + 1;
        if (screenId >= byte (hwmScreens::MAX_VAL))
            screenId = byte (hwmScreens::MAX_VAL) - 1;
        }
    else
        {
        if (screenId < 0)
            screenId = LightController::n_profiles - 1;
        if (screenId > LightController::n_profiles - 1)
            screenId = 0;
        }

    // Updates the screen every 1/UPS_SCREEN seconds
    if (((millis () - lastScreenUpdate) > (1000 / (UPS_SCREEN))) &&
        on)
        scrUpdAvail = true;

    // Updates the screen
    if (scrUpdAvail)
        {
        scrUpdAvail = false;
        Serial.println (__LINE__);

        lastScreenUpdate = millis ();

        if (on)
            showHwmInfo (hwm, screenId);
        else
            printLightingProfile (screenId);

        lcd.display ();
        }
        

    }

void UI::showHwmInfo (HardwareMonitor & hwm, byte hwmScreenId)
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
        switch (static_cast <hwmScreens> (hwmScreenId))
            {
            case hwmScreens::MAIN_SCR:
                // CPU load
                lcd.setCursor (6, 0);
                lcd.print ("  LOAD:");
                lcd.print (hwm.getCPUload (), 0);
                lcd.print ("%   ");

                // RAM
                lcd.setCursor (6, 1);
                lcd.print ("  RAM:");
                lcd.print (hwm.getParameter (HardwareMonitor::paramId::RAMload), 1);
                lcd.setCursor (15, 1);
                lcd.print ("G");

                // CPU temp
                lcd.setCursor (0, 0);
                lcd.print ("CPU:");
                lcd.print (hwm.getCPUtemp (), 0);
                lcd.print ("C");

                // GPU
                lcd.setCursor (0, 1);
                lcd.print ("GPU:");
                lcd.print (hwm.getGPUload (), 0);
                lcd.print ("%");

                break;
            default:
                break;
            }
        }
    }

void UI::printLightingProfile (byte profile)
    {
    lcd.setCursor (0, 0);
    lcd.print ("  Light profile ");
    lcd.setCursor (0, 1);

    switch (profile)
        {
        case LightController::off:
            lcd.print ("       OFF      ");
            break;
        case LightController::ledOnly:
            lcd.print ("    LED only    ");
            break;
        case LightController::film:
            lcd.print ("     Film       ");
            break;
        case LightController::def:
            lcd.print ("    Default     ");
            break;
        case LightController::full:
            lcd.print ("  All light s.  ");
            break;
        case LightController::fullWhite:
            lcd.print (" Max brightness ");
            break;
        case LightController::noLed:
            lcd.print (" Lamp; torchere ");
            break;
        case LightController::night:
            lcd.print ("     Night      ");
            break;
        case LightController::rise:
            lcd.print ("      Rise      ");
            break;
        default:
            break;
        }
    }
