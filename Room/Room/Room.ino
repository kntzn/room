/*
 Name:		Room.ino
 Created:	10/19/2018 8:10:31 AM
 Author:	CODEBOOK
*/

#include "LightController.h"
#include "Predefined.h"

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

    // Serial initialization
    Serial.begin (BAUD_RATE_SERIAL);

    pinMode (29, INPUT_PULLUP);
    pinMode (27, INPUT_PULLUP);
    pinMode (25, INPUT_PULLUP);

    // Initialization of strip
    
    LightController controller;
    controller.setLedMode (StripController::oldschoolRND);
    controller.setLedTableMode (StripController::sync);
    controller.setLedAnimationSpeedVU (20.f);
    controller.setLedAnimationSpeed (-0.25f);
    controller.setLedAnimationFrequency (1.f);
    controller.setLedAnalyzerAnimationFrequency (20.f);
    controller.setLedAnalyzerAnimationOffset (HUE_AQUA);
    controller.setLedColor (CRGB::DeepPink);

    Analyzer analyzer;
    
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
        if (!digitalRead (29))
            {
            controller.setLedMode (StripController::night);
            controller.setLedTableMode (StripController::sync);
            }
        if (!digitalRead (27))
            {
            controller.setLedMode (StripController::RVD);
            controller.setLedTableMode (StripController::sync);
            }
        if (!digitalRead (25))
            {
            controller.setLedMode (StripController::rise);
            controller.setLedTableMode (StripController::sync);
            }
        
        controller.update (dt);
        // !Strip coontroller

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
    }
