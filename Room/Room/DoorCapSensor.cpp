#include "DoorCapSensor.h"

DoorCapSensor::DoorCapSensor (byte pin_input) :
    flag (false),
    toggle (false),
    nSwtch (0),
    smoothAverage (0),
    sensorPin (pin_input),
    lastSwitchMillis (millis ())
    {
    pinMode (pin_input, INPUT);
    }

void DoorCapSensor::update (float dt)
    {
    // PART 0:
    // is sensor activated?
    
    smoothAverage = 0.1*float (analogRead (CAP_SENSOR_DOOR)) + 0.9 *smoothAverage;
    
    bool currentState = bool (analogRead (CAP_SENSOR_DOOR) > smoothAverage + ((1.1 - pow (7,  (-dt / 0.280))) * 100));
    
    float delta = analogRead (CAP_SENSOR_DOOR) - smoothAverage;
    
    // PART 1:
    // triggers

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
            
            nSwtch++;

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

int DoorCapSensor::nSwitches ()
    {
    return nSwtch;
    }

float DoorCapSensor::getMaxRise ()
    {
    return risemax;
    }
