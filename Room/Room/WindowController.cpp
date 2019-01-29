// 
// 
// 

#include "WindowController.h"


BrightnessListener::BrightnessListener (byte newPin)
    {
    pin = newPin;
    measurementTime = millis ();
    }

int BrightnessListener::getBrightness ()
    {
    unsigned long int value = 0;
    for (int i = 0; i < N_MES; i++)
        value += analogRead (pin);

    return (value / N_MES);
    }

void BrightnessListener::update ()
    {
    if (millis () - measurementTime > BRIGHTNESS_UPDATE_PERIOD * 1000)
        {
        output_value = getBrightness ();
        measurementTime = millis ();
        }
    }

int BrightnessListener::getValue ()
    {
    return output_value;
    }




WindowController::WindowController () :
    autoMode (false),
    mode (state::closed_in),
    prev_mode (state::closed_in),
    mode_switch (0),
    millis_init (millis ()),
    outside (LIGHT_SENSOR_OUTSIDE),
    inside (LIGHT_SENSOR_INSIDE)
    {
    window.attach (WINDOW_OUTPUT);
    window.write (SERVO_POSITIVE_SPEED);
    }

void WindowController::setMode (state newMode)
    {
    if (mode != newMode &&
        millis () - mode_switch > (FULL_OPEN_TIME * 1000))
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
    // Updates the mode if autoMode is enabled
    listenBrightness ();

    // Moves the blind to initial position (closed)
    if (millis () - millis_init < (FULL_OPEN_TIME * 1000))
        window.writeMicroseconds (SERVO_POSITIVE_SPEED);
    // Mode switch logic
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
            if (prev_mode == state::opened)
                {
                if (mode == state::closed_in)
                    {
                    window.writeMicroseconds (2300);
                    }
                }
            else if (prev_mode == state::closed_in)
                {
                if (mode == state::opened)
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
        inside.update ();
        outside.update ();

        Serial.print ("Out: ");
        Serial.print (outside.getValue ());
        Serial.print (" In: ");
        Serial.println (inside.getValue ());
        if (outside.getValue () < inside.getBrightness () - BRIGHTNESS_TH)
            setMode (state::closed_in);
        else
            setMode (state::opened);
        }
    }