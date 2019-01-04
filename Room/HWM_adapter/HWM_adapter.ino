/*
 Name:		HWM_adapter.ino
 Created:	1/4/2019 6:55:34 PM
 Author:	CODEBOOK
*/

#include <AltSoftSerial.h>
#include <SoftwareSerial.h>

SoftwareSerial Serial1 (4, 2);

// the setup function runs once when you press reset or power the board
void setup() 
    {
    Serial.begin (9600);
    Serial1.begin (9600);

    pinMode (3, INPUT);

    pinMode (LED_BUILTIN, OUTPUT);
    }

int lastCharIter = 0;
char buffer [270] = {};


// the loop function runs over and over again until power down or reset
void loop() 
    {
    digitalWrite (LED_BUILTIN, !digitalRead (3));

    if (Serial.available ())
        {
        if (digitalRead (3) == LOW)
            Serial1.print (char (Serial.read ()));
        }
        
    
    }
