/*
 Name:		HWM_adapter.ino
 Created:	1/4/2019 6:55:34 PM
 Author:	CODEBOOK
*/

#include <AltSoftSerial.h>
#include <SoftwareSerial.h>

SoftwareSerial Serial1 (3, 2);

// the setup function runs once when you press reset or power the board
void setup() 
    {
    Serial.begin (9600);
    Serial1.begin (9600);


    pinMode (LED_BUILTIN, OUTPUT);
    }

int lastCharIter = 0;
char buffer [270] = {};


// the loop function runs over and over again until power down or reset
void loop() 
    {
    while (Serial.available ())
        {
        buffer [lastCharIter + 1] = Serial.read ();

        lastCharIter++;

        if (lastCharIter >= 270)
            lastCharIter = 270 - 1;
        }
        
    if (Serial1.available ())
        {
        if (Serial1.read () == 'R')
            {
            buffer [0] = lastCharIter;
            Serial1.write (buffer, lastCharIter + 1);
            lastCharIter = 0;
            }
        }
    }
