// 
// 
// 

#include "WindowController.h"

WindowController::WindowController ()
    {
    unsigned long int init_start = millis ();

    while (millis () - init_start < INIT_TIME_TO_OPEN*1000)
        {
        bridge.setSpeed (255);
        bridge.update ();
        }

    }

void WindowController::setMode (byte newMode)    
    {
    mode_switch = millis ();
    mode = newMode;
    }

void WindowController::update ()
    {
    unsigned long int dt = millis () - mode_switch;

    bridge.update ();
    
    
    }
