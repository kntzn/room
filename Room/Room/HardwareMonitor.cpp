// 
// 
// 

#include "HardwareMonitor.h"


HardwareMonitor::HardwareMonitor ():
    lcd (0x27, 16, 2)
    {
    SERIAL_HW_MONITOR.begin (BAUD_RATE_SERIAL);

    lcd.init ();
    lcd.backlight ();
    }

void HardwareMonitor::listenSerial ()
    {
    int parameter_ID = 0;
    
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
                if (index < N_HW_PARAMS)
                    hardwareState [index] = string_convert.toInt ();
                index++;
                }
            input = "";
            }
            
        }
    }

void HardwareMonitor::removeNoise ()
    {
    for (int i = 0; i < N_HW_PARAMS; i++)
        {
        // if data > 100 % or > 100%
        if (hardwareState [i] > 1000)
            hardwareState [i] /= 100;
        else if (hardwareState [i] > 100)
            hardwareState [i] /= 10;
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
    if (updAvail)
        {
        lcd.clear ();

        lcd.setCursor (0, 0);
        lcd.print ("CPU: ");
        lcd.print (hardwareState [0]);
        lcd.print ("C; ");
        lcd.print (hardwareState [4]);
        lcd.print ("%");
        
        lcd.setCursor (0, 1);
        lcd.print ("GPU L:");
        lcd.print (hardwareState [5]);
        lcd.print ("% M:");
        lcd.print (hardwareState [7]);
        lcd.print ("%");

        
        updAvail = false;
        }
    }

