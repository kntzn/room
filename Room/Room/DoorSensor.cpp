#include "DoorSensor.h"

void DoorSensor::update ()
    {
    bool new_state = !digitalRead (input_pin);
    
    // Trigger handling
    switch (trigger_type)
        {
        case ifOpen:
            triggered = new_state;
            break;
        case ifClosed:
            triggered = !new_state;
            break;
        case ifOpening:
            if (state == closed && new_state == opened)
                triggered = true;
            break;
        case ifClosing:
            if (state == opened && new_state == closed)
                triggered = true;
            break;
        default:
            break;
        }
    
    state = new_state;
    }

bool DoorSensor::getState ()
    {
    return state;
    }

void DoorSensor::setTriggerType (byte trg_type)
    {
    trigger_type = trg_type;
    triggered = false;
    }
