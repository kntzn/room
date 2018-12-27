#include "LightBulbController.h"

BulbController::BulbController (byte pin)
    {
    output_pin = pin;

    pinMode (output_pin, OUTPUT);
    }

void BulbController::update ()
    {
    if (inverted)
        digitalWrite (output_pin, !state);
    else
        digitalWrite (output_pin, state);
    }

void BulbController::setState (bool newState)
    {
    state = newState;
    }

void BulbController::setLogicLevel (bool lvl)
    {
    inverted = lvl;
    }

bool BulbController::getState ()
    {
    return state;
    }
