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
    
    sbi (ADCSRA, ADPS2);
    cbi (ADCSRA, ADPS1);
    sbi (ADCSRA, ADPS0);


    // Serial initialization
    Serial.begin (BAUD_RATE_SERIAL);

    pinMode (29, INPUT_PULLUP);

    // Initialization of strip controller
    StripController controller;
    controller.setMode (StripController::rainbow_HSV);
    controller.setTableMode (StripController::VU);
    controller.setRainbowSpeed (0.1f);
    controller.setRainbowFrequency (0.5f);
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
        controller.setVU_val (analyzer.getVUout ());

        // Strip controller
        if (!digitalRead (29))
            controller.setMode (StripController::night);
        
        controller.update (dt);
        controller.display ();
        // !Strip coontroller

        }

    return 0;
    }