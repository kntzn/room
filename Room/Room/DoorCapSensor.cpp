#include "DoorCapSensor.h"

DoorCapSensor::DoorCapSensor (byte pin_input) :
    state (false),
    toggle (false),
    virtualCapCharge (0),
    sensorPin (pin_input),
    lastSwitchMillis (millis ())
    {
    pinMode (pin_input, INPUT);
    }

void DoorCapSensor::update (float dt)
    {
    if (state)
        {
        if (!digitalRead (CAP_SENSOR_DOOR))
            toggle = false;
        }
    else
        {
        if (digitalRead (CAP_SENSOR_DOOR))
            toggle = true;
        }
    }

bool DoorCapSensor::itIsTimeToSwitchIsntIt ()
    {
    Serial.println (toggle);

    if (toggle == true && millis () - lastSwitchMillis > 2000)
        {
        toggle = false;

        lastSwitchMillis = millis ();

        if (!digitalRead (CAP_SENSOR_DOOR))
            return true;
        }

    return false;
    }
