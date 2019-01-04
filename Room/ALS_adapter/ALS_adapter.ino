/*
 Name:		ALS_adapter.ino
 Created:	1/4/2019 1:37:44 PM
 Author:	CODEBOOK
*/

#include <Wire.h>

#include <FastLED.h>
#include "StripController.h"


// Global vars for the receiveEvent
bool paramUpdAvail = false;
byte args [17] = {};
byte args_safe_copy [17] = {};
int commandCharCounter = 0;

void interpret (StripController &ctrlr)
    {
    if (paramUpdAvail)
        {
        paramUpdAvail = false;

        if (args_safe_copy [0] == 0)
            ctrlr.setMode (args_safe_copy [1]);
        if (args_safe_copy [0] == 1)
            ctrlr.setTableMode (args_safe_copy [1]);
        if (args_safe_copy [0] == 2)
            ctrlr.setColor (CRGB (args_safe_copy [1], args_safe_copy [2], args_safe_copy [3]));
        
        }
    }

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

        interpret (controller);
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
// [0] m - mode + 1 val
// [1] t - table mode + 1 val
// [2] c - set color + 3 val
// [3] f - anim freq + 1 val
// [4] s - anim speed + 1 val
// [5] r - freq mode rain freq + 1 val
// [6] o - freq mode rain offset + 1 val
// [7] v - set vu val + 1 val
// [8] p - set peaks val + 3 val
// [9] a - set freq values + 16 values





// function that executes whenever data is received from master
// this function is registered as an event, see setup()


void receiveEvent (int howMany)
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
                    args [commandCharCounter] = 8;
                if (c == 'a')
                    args [commandCharCounter] = 9;

                
                commandCharCounter++;
                }
            else
                {
                args [commandCharCounter] = int (c);
                commandCharCounter++;

                if (commandCharCounter >= 17)
                    {
                    paramUpdAvail = true;

                    for (int i = 0; i < 17; i++)
                        args_safe_copy [i] = args [i];

                    commandCharCounter = 0;
                    }
                }

            }
    }

    