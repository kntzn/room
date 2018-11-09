// 
// 
// 

#include "Hbridge.h"


Hbridge::Hbridge (byte pin0, byte pin1)
    {
    if (!digitalPinHasPWM (pin0) || !digitalPinHasPWM (pin1))
        return;
    
    pinMode (pin0, OUTPUT);
    pinMode (pin1, OUTPUT);
    }

void Hbridge::setSpeed (int newSpeed)
    {
    if (out0 == 255 || out1 == 255)
        return;

    if (invert)
        newSpeed = -newSpeed;

    if (newSpeed > 0)
        {
        newSpeed %= 256;
        speed = newSpeed;
        }
    if (newSpeed < 0)
        {
        newSpeed = -(-newSpeed % 256);
        speed = newSpeed;
        }
    else
        speed = 0;
    }

void Hbridge::update ()
    {
    if (out0 == 255 || out1 == 255)
        return;

    if (speed > 0)
        {
        analogWrite (out0, 0);
        analogWrite (out1, speed);
        }
    else if (speed < 0)
        {
        analogWrite (out1, 0);
        analogWrite (out0, -speed);
        }
    else
        speed = 0;
    }

void Hbridge::reverse ()
    {
    invert = !invert;
    }
