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
