/*
 Name:		ALS_adapter.ino
 Created:	1/4/2019 1:37:44 PM
 Author:	CODEBOOK
*/

#include <Wire.h>

#include <FastLED.h>
#include "StripController.h"


// Global vars for the receiveEvent
bool readyToInterpret = false;
byte args [17] = {};
byte args_safe_copy [17] = {};
int commandCharCounter = 0;

int main ()
    {
    init ();
    
    StripController controller;
    controller.setColor (CRGB::Red);
    controller.setMode (StripController::fade_smooth);
    controller.setTableMode (StripController::sync);
    controller.setRainbowSpeed (0.1f);

    Wire.begin (8);                // join i2c bus with address #8
    Wire.onReceive (receiveEvent); // register event
    

    pinMode (LED_BUILTIN, OUTPUT);

    // start serial for output
    //Serial.begin (9600);        
    
    long prev_t = millis ();

    while (true)
        {
        // Clock
        float dt = (float (millis ()) - prev_t) / 1000.f;
        prev_t = millis ();
        // !Clock

        controller.update (0.05f);
        controller.display ();
        
        digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN));

        //while (millis () - prev_t < 15)
            //{}

        //Serial.println (dt*1000.f);
        }
    }

// Proto

//  0   1   2  3  4   5  6   7
// char + char + char + char \0

// Commands list
// m - mode + 1 val
// t - table mode + 1 val
// c - set color + 3 val
// f - anim freq + 1 val
// s - anim speed + 1 val
// r - freq mode rain freq + 1 val
// o - freq mode rain offset + 1 val
// v - set vu val + 1 val
// p - set peaks val + 3 val
// a - set freq values + 16 values





// function that executes whenever data is received from master
// this function is registered as an event, see setup()


void receiveEvent (int howMany)
    {
    if (Wire.available ())
        {
        


        while (Wire.available ())
            {
            byte c = Wire.read ();

            if (commandCharCounter == 0)
                {
                if (c == 'm')
                    args [commandCharCounter] = 0;
                if (c == 't')
                    args [commandCharCounter] = 1;
                if (c == 'c')
                    args [commandCharCounter] = 2;
                if (c == 'f')
                    args [commandCharCounter] = 3;
                if (c == 's')
                    args [commandCharCounter] = 4;
                if (c == 'r')
                    args [commandCharCounter] = 5;
                if (c == 'o')
                    args [commandCharCounter] = 6;
                if (c == 'v')
                    args [commandCharCounter] = 7;
                if (c == 'p')
                    args [commandCharCounter] = 9;
                if (c == 'a')
                    args [commandCharCounter] = 10;

                
                commandCharCounter++;
                }
            else
                {
                args [commandCharCounter] = int (c);
                commandCharCounter++;

                if (commandCharCounter >= 17)
                    {
                    readyToInterpret = true;

                    for (int i = 0; i < 17; i++)
                        args_safe_copy [i] = args [i];

                    commandCharCounter = 0;
                    }
                }

            }
        
        }
    }

    