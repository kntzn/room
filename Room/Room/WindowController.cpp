// 
// 
// 

#include "WindowController.h"

WindowController::WindowController ()
    {
    window.attach (WINDOW_OUTPUT);
    window.write (WINDOW_ZERO_SPEED);

    unsigned long int init_start = millis ();

    while (millis () - init_start < INIT_TIME_TO_OPEN*1000)
        {
        window.write (WINDOW_ZERO_SPEED + 50);
        
        //bridge.setSpeed (255);
        //bridge.update ();
        }


    window.write (WINDOW_ZERO_SPEED);
    }

void WindowController::setMode (byte newMode)    
    {
    if (!autoMode)
        {
        mode_switch = millis ();
        mode = newMode;
        }
    }

void WindowController::setAutoMode (bool isAuto)
    {
    autoMode = isAuto;
    }

void WindowController::update ()
    {
    // Time scice mode switch
    unsigned long int dt = millis () - mode_switch;

    // Updates sensors
    outside.update ();
    inside.update ();

    // Bridge logic
    if (dt < FULL_OPEN_TIME)
        {
        if (prev_mode == opened)
            {
            if (mode == closed_in &&
                dt < FULL_OPEN_TIME / 2)
                {
                //bridge.setSpeed (255);

                window.write (WINDOW_ZERO_SPEED + 50);
                }
            if (mode == closed_out &&
                dt < FULL_OPEN_TIME / 2)
                {
                //bridge.setSpeed (-255);
                window.write (WINDOW_ZERO_SPEED - 50);
                }
            }
        else if (prev_mode == closed_in &&
                 dt < FULL_OPEN_TIME)
            {
            if (mode == opened &&
                dt < FULL_OPEN_TIME / 2)
                {
                //bridge.setSpeed (-255);
                window.write (WINDOW_ZERO_SPEED - 50);
                }
            if (mode == closed_out &&
                dt < FULL_OPEN_TIME)
                {
                //bridge.setSpeed (-255);
                window.write (WINDOW_ZERO_SPEED - 50);

                }
            }
        else if (prev_mode == closed_out &&
                 dt < FULL_OPEN_TIME)
            {
            if (mode == opened && dt < FULL_OPEN_TIME / 2)
                {
                //bridge.setSpeed (255);
                window.write (WINDOW_ZERO_SPEED + 50);
                }
            if (mode == closed_in && dt < FULL_OPEN_TIME)
                {
                //bridge.setSpeed (255);
                window.write (WINDOW_ZERO_SPEED + 50);
                }
            }
        else
            {
            window.write (WINDOW_ZERO_SPEED);
            }//bridge.setSpeed (0);

        }
    else
        prev_mode = mode;

    // Updates the bridge
    //bridge.update ();
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
