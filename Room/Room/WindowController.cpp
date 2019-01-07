// 
// 
// 

#include "WindowController.h"

WindowController::WindowController ()
    {
    window.attach (WINDOW_OUTPUT);
    
    millis_init = millis ();
    }

void WindowController::setMode (byte newMode)    
    {
    if (!autoMode && mode != newMode)
        {
        window.attach (WINDOW_OUTPUT);
        mode_switch = millis ();
        prev_mode = mode;
        mode = newMode;
        }
    }

void WindowController::setAutoMode (bool isAuto)
    {
    autoMode = isAuto;
    }

void WindowController::update ()
    {
    if (millis () - millis_init < FULL_OPEN_TIME * 1000)
        window.writeMicroseconds (2300);
    else
        {
        // Time scice mode switch
        unsigned long int dt = millis () - mode_switch;

        // Updates sensors
        outside.update ();
        inside.update ();

        // Servo logic
        if (dt < (FULL_OPEN_TIME * 1000) / 2)
            {
            if (prev_mode == opened)
                {
                if (mode == closed_in)
                    {
                    window.writeMicroseconds (2300);
                    }
                }
            else if (prev_mode == closed_in &&
                     dt < (FULL_OPEN_TIME * 1000))
                {
                if (mode == opened)
                    {
                    window.writeMicroseconds (800);
                    }
                }
            }
        else
            {
            prev_mode = mode;
            window.detach ();
            }
        }

    }

void WindowController::listenBrightness ()
    {
    if (autoMode)
        {
        if (100 + outside.getBrightness () < inside.getBrightness ())
            mode = closed_in;
        else
            mode = opened;
        }
    }
