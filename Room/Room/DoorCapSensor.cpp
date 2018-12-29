#include "DoorCapSensor.h"

DoorCapSensor::DoorCapSensor (byte pin_input) :
    flag (false),
    toggle (false),
    virtualCapCharge (0),
    sensorPin (pin_input),
    lastSwitchMillis (millis ())
    {
    pinMode (pin_input, INPUT);
    }

void DoorCapSensor::update (float dt)
    {
    bool currentState = bool (analogRead (CAP_SENSOR_DOOR) > DOOR_CAP_TH);

    if (currentState == true)
        {
        if (!flag)
            {
            flag = true;
            toggle = true;
            }
        }
    else
        {
        flag = false;
        }
    
    }

bool DoorCapSensor::itIsTimeToSwitchIsntIt ()
    {
    /*
    delay (100);
    Serial.println (toggle);
    if (toggle)
        Serial.println ("------------\n------------\n------------\n------------\n------------\n");
        */

    if (toggle == true)
        {
        toggle = false;
    
        if (millis () - lastSwitchMillis > 500)
            {
            lastSwitchMillis = millis ();
            return true;
            }
        else
            {
            lastSwitchMillis = millis ();
            return false;
            }
        }

    return false;
    }
