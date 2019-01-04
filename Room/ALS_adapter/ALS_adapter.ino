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



int i = 0;
byte ledData [N_LEDS_MAIN * 3 + N_LEDS_TABLE * 3] = {};

CRGB leds_main [N_LEDS_MAIN] = {};
CRGB leds_table [N_LEDS_TABLE] = {};

void setup ()
    {
    LEDS.addLeds <WS2811, STRIP_DATA_MAIN, GRB> (leds_main, N_LEDS_MAIN);
    LEDS.addLeds <WS2811, STRIP_DATA_TABLE, GRB> (leds_table, N_LEDS_TABLE);

    Wire.begin (8);                // join i2c bus with address #8
    //Wire.onReceive (receiveEvent); // register event
    Serial.begin (9600);           // start serial for output
    }

void loop ()
    {
    Serial.println ();

    for (int i = 0; i < N_LEDS_MAIN; i++)
        leds_main [i] = CHSV (i, 255, 255);

    for (int i = 0; i < N_LEDS_TABLE; i++)
        leds_table [i] = CHSV (i, 255, 255);

    LEDS.show ();
    }

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent (int howMany)
    {
    if (Wire.available ())
        {
        i++;

        if (i >= 164 * 3 + 42 * 3)
            i -= 164 * 3 + 42 * 3;
        while (Wire.available ())
            {
            char c = Wire.read (); // receive byte as a character
            ledData [i] = c;
            //Serial.print (c);         // print the character
            }
        
        }
    }
