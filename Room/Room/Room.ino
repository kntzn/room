/*
 Name:		Room.ino
 Created:	10/19/2018 8:10:31 AM
 Author:	CODEBOOK
*/


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

    // Initialization of controller and strip
    LightController controller;
    controller.setLedMode (StripController::fade_switch_random);
    controller.setLedTableMode (StripController::sync);
    controller.setLedAnimationSpeedVU (20.f);
    controller.setLedAnimationSpeed (0.2f);
    controller.setLedAnimationFrequency (5.f);
    controller.setLedAnalyzerAnimationFrequency (80);
    controller.setLedAnalyzerAnimationOffset (HUE_ORANGE);
    controller.setLedColor (CRGB::White);
    
    Analyzer analyzer;
    
    //HardwareMonitor hw_monitor;

    DoorSensor doorSens (DOOR_SENSOR_PIN);
    doorSens.setTriggerType (DoorSensor::ifOpen);

    
    float prev_t = float (millis ());

    while (true)
        {
        // Clock
        float dt = (float (millis ()) - prev_t)/1000.f;
        prev_t += dt*1000.f;
        // !Clock
    
        Serial.println (doorSens.isTriggered ());


        // Analyzer
        analyzer.update (dt);
        controller.syncWithAnalyzer (analyzer, dt);
        // !Analyzer

        // Strip controller
        if (!digitalRead (27))
            {
            controller.setProfile (LightController::film, &doorSens);
            Serial.println ("sht");
            }
        
        controller.update (dt, &doorSens);
        // !Strip coontroller

        doorSens.update ();

        //hw_monitor.listenSerial ();
        //hw_monitor.removeNoise ();
        //hw_monitor.print ();
        }

    return 0;
    }

void initPins ()
    {
    // Strip controller
    pinMode (STRIP_DATA_MAIN, OUTPUT);
    pinMode (STRIP_DATA_TABLE, OUTPUT);
    // !Strip controller

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
    