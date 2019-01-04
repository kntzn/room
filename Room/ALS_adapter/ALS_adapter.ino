/*
 Name:		ALS_adapter.ino
 Created:	1/4/2019 1:37:44 PM
 Author:	CODEBOOK
*/

#include <Wire.h>

#include <FastLED.h>


#define N_LEDS_MAIN 164
#define N_LEDS_TABLE 42

#define STRIP_DATA_MAIN 5
#define STRIP_DATA_TABLE 7

CRGB leds_main [N_LEDS_MAIN] = {};
CRGB leds_table [N_LEDS_TABLE] = {};

int commandCharCounter = 0;
byte args [17] = {};

void setup ()
    {
    LEDS.addLeds <WS2811, STRIP_DATA_MAIN, GRB> (leds_main, N_LEDS_MAIN);
    LEDS.addLeds <WS2811, STRIP_DATA_TABLE, GRB> (leds_table, N_LEDS_TABLE);

    Wire.begin (8);                // join i2c bus with address #8
    Wire.onReceive (receiveEvent); // register event
    Serial.begin (9600);           // start serial for output
    }

void loop ()
    {
    
    for (int i = 0; i < N_LEDS_MAIN; i++)
        leds_main [i] = CHSV (i, 255, 255);

    for (int i = 0; i < N_LEDS_TABLE; i++)
        leds_table [i] = CHSV (i, 255, 255);

    //Serial.println (LEDS.getFPS ());

    LEDS.show ();
    }


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
                    for (int i = 0; i < 17; i++)
                        {
                        Serial.print (int (args [i]));
                        Serial.print (' ');
                        }

                    Serial.println ();

                    commandCharCounter = 0;
                    }
                }

            }
        
        }
    }
