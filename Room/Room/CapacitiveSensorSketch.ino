#include <CapacitiveSensor.h>
#include <SoftwareSerial.h>

CapacitiveSensor cs_7_8 = CapacitiveSensor (7, 4);
unsigned long csSum;

void setup ()
    {
    Serial.begin (9600);

    pinMode (9, OUTPUT);
    pinMode (11, OUTPUT);
    }

void loop ()
    {
    CSread ();
    }

float aver = 0;

void (*resetFunc) (void) = 0;

void CSread ()
    {
    long cs = cs_7_8.capacitiveSensor (100);

    cs = constrain (cs, 0, 512);

    aver = float (cs)*0.05 + 0.95*aver;

    int output = constrain (aver, 0, 1024);

    output = map (output, 0, 1024, 0, 255);

    Serial.println (output);

    analogWrite (9, output);
    analogWrite (11, output);

    if (millis () > 60000 ||
        output > 200)
        resetFunc ();
    }
