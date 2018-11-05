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

    // Serial initialization
    Serial.begin (BAUD_RATE_SERIAL);

    pinMode (29, INPUT_PULLUP);
    pinMode (27, INPUT_PULLUP);
    pinMode (25, INPUT_PULLUP);

    // Initialization of strip controller
    StripController controller;
    controller.setMode (StripController::rainbow_HSV);
    controller.setTableMode (StripController::sync);
    controller.setPaletteSpeed (20.f);
    controller.setRainbowSpeed (-0.05f);
    controller.setRainbowFrequency (1.f);
    controller.setFreqModeRainFreq (20.f);
    controller.setFreqModeRainOffset (HUE_AQUA);
    controller.setColor (CRGB::DeepPink);

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
        controller.setFreqValues (analyzer.getFreqValues ());
        // !Analyzer

        // Strip controller
        if (!digitalRead (29))
            {
            controller.setMode (StripController::night);
            controller.setTableMode (StripController::sync);
            }
        if (!digitalRead (27))
            {
            controller.setMode (StripController::RVD);
            controller.setTableMode (StripController::sync);
            }
        if (!digitalRead (25))
            {
            controller.setMode (StripController::rise);
            controller.setTableMode (StripController::sync);
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
