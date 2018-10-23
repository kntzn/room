/*
 Name:		Room.ino
 Created:	10/19/2018 8:10:31 AM
 Author:	CODEBOOK
*/

#include "StripController.h"

int main ()
    {
    // Microcontroller initiaalization
    init ();
    
    // Serial initialization
    Serial.begin (BAUD_RATE_SERIAL);

    // Initialization of strip controller
    StripController controller;
    controller.setMode (StripController::rainbow_HSV);
    controller.setTableMode (StripController::FREQ_FULL);
    controller.setRainbowSpeed (0.1f);
    controller.setRainbowFrequency (0.5f);
    controller.setColor (CRGB::DeepPink);

    float prev_t = float (millis ());

    while (true)
        {
        // Clock
        float dt = (float (millis ()) - prev_t)/1000.f;
        prev_t += dt*1000.f;
        // !Clock


        // Strip controller
        controller.update (dt);
        controller.display ();
        // !Strip coontroller

        }

    return 0;
    }