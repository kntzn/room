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
    
    if (dt < FULL_OPEN_TIME)
        {
        if (prev_mode == opened)
            {
            if (mode == closed_in &&
                dt < FULL_OPEN_TIME / 2)
                {
                bridge.setSpeed (255);
                }
            if (mode == closed_out &&
                dt < FULL_OPEN_TIME / 2)
                {
                bridge.setSpeed (-255);
                }
            }
        else if (prev_mode == closed_in &&
                 dt < FULL_OPEN_TIME)
            {
            if (mode == opened &&
                dt < FULL_OPEN_TIME / 2)
                {
                bridge.setSpeed (-255);
                }
            if (mode == closed_out &&
                dt < FULL_OPEN_TIME)
                {
                bridge.setSpeed (-255);
                }
            }
        else if (prev_mode == closed_out &&
                 dt < FULL_OPEN_TIME)
            {
            if (mode == opened && dt < FULL_OPEN_TIME / 2)
                {
                bridge.setSpeed (255);
                }
            if (mode == closed_in && dt < FULL_OPEN_TIME)
                {
                bridge.setSpeed (255);
                }
            }
        else
            bridge.setSpeed (0);

        }
    else
        prev_mode = mode;
    }

void WindowController::listenBrightness ()
    {
    if (autoMode)
        {
        }
    }
