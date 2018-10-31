#include "LightBulbController.h"

BulbController::BulbController (byte pin)
    {
    output_pin = pin;
    }

void BulbController::update ()
    {
    digitalWrite (output_pin, state);        
    }

void BulbController::setState (bool newState)
    {
    state = newState;
    }
