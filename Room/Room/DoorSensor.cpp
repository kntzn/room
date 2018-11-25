#include "DoorSensor.h"

void DoorSensor::update ()
    {
    state = !digitalRead (input_pin);
    }

bool DoorSensor::getState ()
    {
    return state;
    }
