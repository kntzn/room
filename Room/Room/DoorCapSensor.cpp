#include "DoorCapSensor.h"

DoorCapSensor::DoorCapSensor (byte pin_input) :
    wasSwitched (true),
    virtualCapCharge (0),
    sensorPin (pin_input)
    {
    pinMode (pin_input, INPUT);
    }

void DoorCapSensor::update (float dt)
    {
    if (digitalRead (sensorPin))
        virtualCapCharge = 1.0;
    else if (virtualCapCharge > 0)
        virtualCapCharge -= PULSES_PERIOD*dt;
    else
        {
        virtualCapCharge = 0;
        wasSwitched = false;
        }
    }

bool DoorCapSensor::itIsTimeToSwitchIsntIt ()
    {
    if (wasSwitched == true)
        {
        wasSwitched = false;
        return true;
        }

    return false;
    }
