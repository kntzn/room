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

    float prev_t = float (millis ());

    while (true)
        {
        float dt = float (millis ()) - prev_t;
        prev_t += dt;

        controller.setVU_val (analogRead (A0));
        controller.update (dt);
        
        
        controller.display ();

        Serial.println (analogRead (A0));

        }

    return 0;
    }