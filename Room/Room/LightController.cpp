// 
// 
// 

#include "LightController.h"

LedController::LedController ()
    {
    }

void LedController::update (float dt)
    {
    LED.update (dt);
    Lamp.update ();
    Torchere.update ();
    }
