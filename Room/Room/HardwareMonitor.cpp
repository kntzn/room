// 
// 
// 

#include "HardwareMonitor.h"


HardwareMonitor::HardwareMonitor ():
    lcd (0x27, 16, 2)
    {
    SERIAL_HW_MONITOR.begin (BAUD_RATE_SERIAL);

    lcd.init ();
    
    }

void HardwareMonitor::listenSerial ()
    {
    int parameter_ID = 0;
    /*
    // Parses full string
    while (SERIAL_HW_MONITOR.available ())
        {
        lastChar = SERIAL_HW_MONITOR.read ();
        if (lastChar == ';' || lastChar == 'E')
            {
            if (parameter_ID < N_HW_PARAMS)
                hardwareState [parameter_ID] = input.toInt ();

            parameter_ID++;

            if (lastChar == 'E')
                {
                lastChar = 0;
                updAvail = true;

                }
            input = "";
            }
        else
            input += lastChar;
        }


        */

    
    while (SERIAL_HW_MONITOR.available ())
        {
        char aChar = SERIAL_HW_MONITOR.read ();
        if (aChar != 'E') 
            {
            input += aChar;
            }
        else 
            {
            updAvail = true;
            input += '\0';
            char *p = input.begin ();
            char *str;
            int index = 0;
            while ((str = strtok_r (p, ";", &p)) != NULL) 
                {
                String string_convert = str;
                hardwareState [index] = string_convert.toInt ();
                index++;
                }
            input = "";
            }
            
        }
    }

void HardwareMonitor::log ()
    {
    if (updAvail)
        {
        Serial.println ("HW MON LOG:");
        Serial.println ("-----------------");
        Serial.print ("CPU TEMP: ");
        Serial.print (hardwareState [0]);
        Serial.print ("degC;\nCPU LOAD: ");
        Serial.print (hardwareState [4]);
        Serial.print ("%;\nGPU TEMP: ");
        Serial.print (hardwareState [1]);
        Serial.print ("degC;\nGPU LOAD: ");
        Serial.print (hardwareState [5]);
        Serial.print ("%;\nGPU MEM LOAD: ");
        Serial.print (hardwareState [7]);
        Serial.print ("%;\nHDD TEMP: ");
        Serial.print (hardwareState [3]);
        Serial.print ("degC;\nRAM LOAD: ");
        Serial.print (hardwareState [6]);
        Serial.print ("%;\n");
        
        updAvail = false;
        }
    }

void HardwareMonitor::print ()
    {
    lcd.clear ();
    lcd.home ();
    lcd.print ("CPU TEMP: ");
    lcd.print (hardwareState [0]);
    lcd.setCursor (0, 1);
    lcd.print ("CPU LOAD: ");
    lcd.print (hardwareState [4]);
    }

