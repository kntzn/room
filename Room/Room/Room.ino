/*
 Name:		Room.ino
 Created:	10/19/2018 8:10:31 AM
 Author:	CODEBOOK
*/



#include "Button.h"
#include "DoorSensor.h"
#include "WindowController.h"
#include "Hbridge.h"
#include "LightController.h"
#include "Predefined.h"

#include "HardwareMonitor.h"
#include "Analyzer.h"

void initPins ();

int main ()
    {
    // Microcontroller initialization
    init ();
    // Pins initialization
    initPins ();

    // Random initialization
    srand (analogRead (NC));

    // Serial1 initialization
    Serial.begin (BAUD_RATE_SERIAL);

    pinMode (29, INPUT_PULLUP);
    pinMode (27, INPUT_PULLUP);
    pinMode (25, INPUT_PULLUP);


    DoorSensor doorSens (DOOR_SENSOR_PIN);
    doorSens.setTriggerType (DoorSensor::ifOpen);


    // Initialization of controller and strip
    LightController controller;
    controller.setLedMode (StripController::oldschoolRND);
    controller.setLedTableMode (StripController::sync);
    controller.setLedAnimationSpeedVU (20.f);
    controller.setLedAnimationSpeed (-0.05f);
    controller.setLedAnimationFrequency (0.5f);
    controller.setLedAnalyzerAnimationFrequency (80);
    controller.setLedAnalyzerAnimationOffset (HUE_ORANGE);
    controller.setLedColor (CRGB::White);
    
    Analyzer analyzer;
    
    //HardwareMonitor hw_monitor;

    controller.setProfile (LightController::def, &doorSens);
    
    float prev_t = float (millis ());

    while (true)
        {
        // Clock
        float dt = (float (millis ()) - prev_t)/1000.f;
        prev_t += dt*1000.f;
        // !Clock
    
        

        // Analyzer
        analyzer.update (dt);
        controller.syncWithAnalyzer (analyzer, dt);
        // !Analyzer

        // Strip controller
        if (!digitalRead (27))
            {
            Serial.println ("Film");

            controller.setProfile (LightController::ledOnly, &doorSens);
            }
        if (!digitalRead (29))
            {
            Serial.println ("Night");

            if (controller.getProfile () != LightController::night)
                controller.setProfile (LightController::night, &doorSens);
            else
                controller.setProfile (LightController::rise, &doorSens);
            }
        if (!digitalRead (25))
            {
            Serial.println ("def");
            
            controller.setLedMode (rand () % (StripController::n_modes - 2) + 2);

            controller.setProfile (LightController::def, &doorSens);
            }
        
        controller.update (dt, &doorSens);

        // !Strip coontroller

        doorSens.update ();

        }

    return 0;
    }

void initPins ()
    {
    // Strip controller
    pinMode (STRIP_DATA_MAIN, OUTPUT);
    pinMode (STRIP_DATA_TABLE, OUTPUT);
    // !Strip controller

    // Light controller
    pinMode (RELAY_LAMP, OUTPUT);
    pinMode (RELAY_TORCHERE, OUTPUT);
    // !Light controller

    // Analyzer
    pinMode (MIC_INPUT, INPUT);
    pinMode (MIC_INPUT_FREQ, INPUT);
    pinMode (JACK_INPUT, INPUT);
    pinMode (JACK_INPUT_FREQ, INPUT);
    // !Analyzer

    // Door
    pinMode (DOOR_SENSOR_PIN, INPUT_PULLUP);
    // !Door

    // Power supply
    pinMode (CURRENT_SENSOR_PIN, INPUT);
    pinMode (VOLTAGE_SENSOR_PIN, INPUT);
    // !Power supply
    }
    
