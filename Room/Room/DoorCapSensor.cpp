#include "DoorCapSensor.h"


void LampCapSensor::reset ()
    {
    flag = false;
    state = 0;
    }

LampCapSensor::LampCapSensor (byte b_pin0, byte b_pin1, 
                              long int threshold0, long int threshold1):
    cs (b_pin0, b_pin1),
    th0 (threshold0),
    th1 (threshold1)
    {
    cs.set_CS_AutocaL_Millis (1000);
    }

void LampCapSensor::update (bool isActive)
    {
    long start = micros ();
    long int value = cs.capacitiveSensor (60);
    long end = micros ();

    long int threshold = th1;
    if (!isActive)
        threshold = th0;

    bool active = (value > threshold);
    
    // Debug
    /*Serial.print (value);
    Serial.print (' ');
    Serial.print (threshold);
    Serial.print (' ');
    Serial.print ((end - start) / 1000.f);
    Serial.println ("ms");*/

    if (active)
        {
        last_press = millis ();
        }
    
    if (millis () - last_press > 300)
        state = 0;

    if (state == 0)
        if (active)
            state = 1;
    
    if (state == 1)
        if (!active)
            state = 2;

    if (state == 2)
        if (active)
            state = 3;

    if (state == 3)
        if (!active)
            state = 4;

    if (state == 4)
        if (!active)
            state = 0;

    Serial.println (state);
    }

byte LampCapSensor::getState ()
    {
    if (state == 3)
        {
        state = 4;
        return true;
        }
    else
        return false;
    }



void DoorCapSensor::reset ()
    {
    flag = false;
    state = buttonState::Prsd;
    }

DoorCapSensor::DoorCapSensor ()
    {
    }

void DoorCapSensor::update ()
    {
    long int value = analogRead (CAP_SENSOR_DOOR);
    
    bool active = (value > DOOR_CAP_TH);

    // Debug
    /*Serial.print (value);
    Serial.print (' ');
    Serial.println (DOOR_CAP_TH);*/

    if (active)
        {
        if (flag == false)
            {
            flag = true;
            last_press = millis ();
            state = buttonState::Prs;
            }
        else
            {
            if (millis () - last_press > HOLD_TIME_CAP / 2)
                {
                if (hold_flag == false)
                    {
                    hold_flag = true;
                    state = buttonState::Hold;
                    }
                else
                    state = buttonState::Unpr;
                }

            else
                state = buttonState::Prsd;
            }
        }
    else
        {
        if (flag == true && hold_flag == false)
            {
            flag = false;
            state = buttonState::Rlsd;
            }
        else
            {
            state = buttonState::Unpr;
            hold_flag = false;
            flag = false;
            }

        }

    }

byte DoorCapSensor::getState ()
    {
    return state;
    }



//
//
//DoorCapSensor::DoorCapSensor (byte pin_input) :
//    flag (false),
//    toggle (false),
//    nSwtch (0),
//    smoothAverage (0),
//    sensorPin (pin_input),
//    lastSwitchMillis (millis ())
//    {
//    pinMode (pin_input, INPUT);
//    }
//
//void DoorCapSensor::update (float dt)
//    {
//    // PART 0:
//    // is sensor activated?
//    
//    int analogResult = analogRead (CAP_SENSOR_DOOR);
//
//    smoothAverage = 0.1*float (analogRead (CAP_SENSOR_DOOR)) + 0.9 *smoothAverage;
//    
//    bool currentState = bool (analogResult > smoothAverage + DOOR_CAP_TH);
//    
//    // Debug
//    Serial.print (analogResult);
//    Serial.print (' ');
//    Serial.print (smoothAverage + DOOR_CAP_TH);
//
//    float delta = analogResult - smoothAverage;
//
//    if (risemax < delta)
//        {
//        risemax = delta;
//        lasRiseMaxUpd = millis ();
//        }
//    if (millis () - lasRiseMaxUpd > 5000)
//        risemax = 0;
//
//    Serial.print (" riseMax: ");
//    Serial.println (risemax);
//
//    // PART 1:
//    // triggers
//
//    if (currentState == true)
//        {
//        if (!flag)
//            {
//            flag = true;
//            toggle = true;
//            }
//        }
//    else
//        {
//        flag = false;
//        }
//    
//    }
//
//bool DoorCapSensor::itIsTimeToSwitchIsntIt ()
//    {
//    if (toggle == true)
//        {
//        toggle = false;
//    
//        if (millis () - lastSwitchMillis > 500)
//            {
//            lastSwitchMillis = millis ();
//            
//            nSwtch++;
//
//            return true;
//            }
//        else
//            {
//            lastSwitchMillis = millis ();
//            return false;
//            }
//        }
//
//    return false;
//    }
//
//int DoorCapSensor::nSwitches ()
//    {
//    return nSwtch;
//    }
//
//float DoorCapSensor::getMaxRise ()
//    {
//    return risemax;
//    }
