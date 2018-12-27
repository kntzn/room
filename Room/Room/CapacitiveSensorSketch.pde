#include <CapacitiveSensor.h>
#include <SoftwareSerial.h>

CapacitiveSensor cs_7_8 = CapacitiveSensor (7, 4); //10M Resistor between pins 7 and 8, you may also connect an antenna on pin 8
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

void CSread ()
    {
    long cs = cs_7_8.capacitiveSensor (100);
    if (cs > 2000)
        {
        csSum += cs;
        Serial.println (csSum);
        if (csSum >= 8000)
            {
            //ss.print ("Trigger: ");
            digitalWrite (9, HIGH);
            digitalWrite (11, HIGH);
            Serial.print ("tr: ");
            Serial.println (csSum);
            if (csSum > 0)
                csSum = 0;

            cs_7_8.reset_CS_AutoCal (); //Stops readings
            }
        }
    else 
        {
        csSum = 0;
        digitalWrite (9, LOW);
        digitalWrite (11, LOW);
        }
    }
