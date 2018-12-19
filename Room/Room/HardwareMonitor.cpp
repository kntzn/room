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
    
    while (SERIAL_HW_MONITOR.available () > 0)
        {
        char inputChar = SERIAL_HW_MONITOR.read ();
        if (inputChar != 'E')
            {
            inData [index] = inputChar;
            index++;
            inData [index] = '\0';
            }
        else
            {
            char *p = inData;
            char *str;
            index = 0;
            while ((str = strtok_r (p, ";", &p)) != NULL) 
                {
                string_convert = str;
                if (index < 8)
                    {
                    Serial.print ("[");
                    Serial.print (index);
                    Serial.print ("] = ");
                    Serial.println (string_convert.toInt ());
                    }
                

                index++;
                }
            index = 0;
            }
        }
    /*
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
                    {
                    // ROL:

                    for (int i = 0; i < N_TICKS_SMOOTH - 1; i++)
                        hardwareState [index] [i] = hardwareState [index] [i + 1];


                    // Value conversion
                    hardwareState [index] [N_TICKS_SMOOTH - 1] = string_convert.toInt ();
                    
                    
                    }
                index++;
                }
            input = "";
            }
            
        }
        */
    }

void HardwareMonitor::removeNoise ()
    {
    // Filters last values
    for (int i = 0; i < N_HW_PARAMS; i++)
        {
        // if data > 100 % or > 100%
        if (hardwareState [i] [N_TICKS_SMOOTH - 1] > 1000)
            hardwareState [i] [N_TICKS_SMOOTH - 1] /= 100;
        else if (hardwareState [i] [N_TICKS_SMOOTH - 1] > 100)
            hardwareState [i] [N_TICKS_SMOOTH - 1] /= 10;
        }

    // Creates average values of last N_TICKS_SMOOTH updates
    for (int i = 0; i < N_HW_PARAMS; i++)
        {
        float summ = 0;
        for (int j = 0; j < N_TICKS_SMOOTH; j++)
            summ += (float) hardwareState [i] [j];
        
        hardwareStateFiltered [i] = summ / float (N_TICKS_SMOOTH);
        }
    }

void HardwareMonitor::print ()
    {
    if (updAvail)
        {
        lcd.clear ();

        lcd.setCursor (0, 0);
        lcd.print ("CPU: ");
        lcd.print (hardwareStateFiltered [0], 1);
        lcd.print ("C; ");
        lcd.print (hardwareState [4][N_TICKS_SMOOTH - 1]);
        lcd.print ("%");
        
        
        updAvail = false;
        }
    }

