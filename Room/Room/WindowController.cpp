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
        window.writeMicroseconds (2000);

        //bridge.setSpeed (255);
        //bridge.update ();
        }

    //window.detach ();
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
    //int val = analogRead (A7);

    //aver = 0.05*float (val) + 0.95*aver;

    //Serial.println (aver);

    //window.writeMicroseconds (map (aver, 0, 1023, 1000, 2000));

    // Time scice mode switch
    unsigned long int dt = millis () - mode_switch;

    // Updates sensors
    outside.update ();
    inside.update ();

    

    #define AMPL WINDOW_ZERO_SPEED
    
    // Bridge logic
    if (dt < (FULL_OPEN_TIME * 1000))
        {
        if (prev_mode == opened)
            {
            if (mode == closed_in &&
                dt < (FULL_OPEN_TIME * 1000) / 2)
                {
                //bridge.setSpeed (255);

                window.writeMicroseconds (2300);
                }
            if (mode == closed_out &&
                dt < (FULL_OPEN_TIME * 1000) / 2)
                {
                //bridge.setSpeed (-255);
                window.writeMicroseconds (800);
                }
            }
        else if (prev_mode == closed_in &&
                 dt < (FULL_OPEN_TIME * 1000))
            {
            if (mode == opened &&
                dt < (FULL_OPEN_TIME * 1000) / 2)
                {
                //bridge.setSpeed (-255);
                window.writeMicroseconds (800);
                }
            if (mode == closed_out &&
                dt < (FULL_OPEN_TIME * 1000))
                {
                //bridge.setSpeed (-255);
                window.writeMicroseconds (800);

                }
            }
        else if (prev_mode == closed_out &&
                 dt < (FULL_OPEN_TIME * 1000))
            {
            if (mode == opened && dt < (FULL_OPEN_TIME * 1000) / 2)
                {
                //bridge.setSpeed (255);
                window.writeMicroseconds (2300);
                }
            if (mode == closed_in && dt < (FULL_OPEN_TIME * 1000))
                {
                //bridge.setSpeed (255);
                window.writeMicroseconds (2300);
                }
            }
        else
            {
            window.write (WINDOW_ZERO_SPEED);
            }//bridge.setSpeed (0);

        }
    else
        {
        prev_mode = mode;
        window.detach ();
        }
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
