/*
 Name:		Room.ino
 Created:	10/19/2018 8:10:31 AM
 Author:	CODEBOOK
*/

#include "StripController.h"

#define NUM_LEDS 164
#define DATA_PIN 13
#define BUT_PIN 26

int main ()
    {
    init ();

    pinMode (BUT_PIN, INPUT_PULLUP);
    Serial.begin (9600);

    StripController controller;
    controller.setMode (StripController::rainbow_HSV);
    controller.setTableMode (StripController::VU_bright);
    controller.setRainbowSpeed (0.1f);
    controller.setRainbowFrequency (0.5f);
    controller.setColor (CRGB::Aquamarine);
    byte mode = 0;



    float prev_t = float (millis ());

    while (true)
        {
        float dt = (float (millis ()) - prev_t)/1000.f;
        prev_t += dt*1000.f;

        if (!digitalRead (BUT_PIN))
            {
            while (digitalRead (BUT_PIN))
                {
                }

            controller.setMode (mode);
            mode = (mode + 1) % 7;
            Serial.println (mode);
            }

        controller.setVU_val (analogRead (A0));
        controller.update (dt);
        
        controller.display ();
        }

    return 0;
    }