/*
 Name:		Room.ino
 Created:	10/19/2018 8:10:31 AM
 Author:	CODEBOOK
*/

#include "Predefined.h"

#include "StripController.h"
#include "Analyzer.h"

int main ()
    {
    // Microcontroller initiaalization
    init ();

    // Serial initialization
    Serial.begin (BAUD_RATE_SERIAL);

    pinMode (29, INPUT_PULLUP);
    pinMode (27, INPUT_PULLUP);

    // Initialization of strip controller
    StripController controller;
    controller.setMode (StripController::rainbow_HSV);
    controller.setTableMode (StripController::FREQ_FULL);
    controller.setPaletteSpeed (20.f);
    controller.setRainbowSpeed (0.1f);
    controller.setRainbowFrequency (0.5f);
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


        analyzer.update ();
        float output [SPECTRUM_SIZE] = {};


        uint8_t max = 0;
        for (int i = 2; i < SPECTRUM_SIZE; i++)
            if (analyzer.getFreqValues() [i] > max)
                max = analyzer.getFreqValues () [i];

        for (int i = 2; i < SPECTRUM_SIZE; i++)
            output [i] = float (FREQ_MAX)*float (analyzer.getFreqValues () [i]) / float (max);

        
        controller.setFreqValues (output);

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
        
        controller.update (dt);
        controller.display ();
        // !Strip coontroller

        }

    return 0;
    }