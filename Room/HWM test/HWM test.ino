/*
Что идёт в порт: 0-CPU temp, 1-GPU temp, 2-mother temp, 3-max HDD temp, 4-CPU load, 5-GPU load, 6-RAM use, 7-GPU memory use,
8-maxFAN, 9-minFAN, 10-maxTEMP, 11-minTEMP, 12-manualFAN, 13-manualCOLOR, 14-fanCtrl, 15-colorCtrl, 16-brightCtrl, 17-LOGinterval, 18-tempSource
*/
// ------------------------ НАСТРОЙКИ ----------------------------
// настройки пределов скорости и температуры по умолчанию (на случай отсутствия связи)

#include <string.h>             // библиотека расширенной работы со строками
#include <Wire.h>               // библиотека для соединения
#include <LiquidCrystal_I2C.h>  // библтотека дислея

#include <SoftwareSerial.h>

char inData [270];       // массив входных значений (СИМВОЛЫ)
int PCdata [50];        // массив численных значений показаний с компьютера
byte index = 0;
String string_convert;

SoftwareSerial Serial1 (2, 3);

void setup ()
    {
    Serial.begin (9600);
    Serial1.begin (9600);
    }
// 8-maxFAN, 9-minFAN, 10-maxTEMP, 11-minTEMP, 12-mnlFAN

long lastUpd = millis ();
long last_millis = millis ();

// ------------------------------ ОСНОВНОЙ ЦИКЛ -------------------------------

int parsingTime = 0;
int nIter = 0;
int bufSize = 0;


void loop ()
    {
    last_millis = millis ();

    parsing ();                          // парсим строки с компьютера
                                         //updatePlot ();                       // обновляем массив данных графика
    
    

    if (millis () - lastUpd > 400)
        {
        lastUpd = millis ();
        
        Serial.print ("Bus speed: ");
        Serial.println (PCdata [4]);
        Serial.print ("CPU cores: ");
        Serial.print (PCdata [0]);
        Serial.print (" ");
        Serial.print (PCdata [1]);
        Serial.print (" ");
        Serial.print (PCdata [2]);
        Serial.print (" ");
        Serial.println (PCdata [3]);
        Serial.print ("CPU cores' temp: ");
        Serial.print (PCdata [5]);
        Serial.print (" ");
        Serial.print (PCdata [6]);
        Serial.print (" ");
        Serial.print (PCdata [7]);
        Serial.print (" ");
        Serial.print (PCdata [8]);
        Serial.print (" /Package: ");
        Serial.println (PCdata [9]);
        Serial.print ("CPU cores' load: ");
        Serial.print (PCdata [10]);
        Serial.print (" ");
        Serial.print (PCdata [11]);
        Serial.print (" ");
        Serial.print (PCdata [12]);
        Serial.print (" ");
        Serial.print (PCdata [13]);
        Serial.print (" /Total: ");
        Serial.println (PCdata [14]);
        Serial.print ("TDP Pack.: ");
        Serial.print (PCdata [15]);
        Serial.print (" /Cores: ");
        Serial.print (PCdata [16]);
        Serial.print (" /Graphics: ");
        Serial.print (PCdata [17]);
        Serial.print (" /DRAM: ");
        Serial.println (PCdata [18]);
        Serial.println ();

        Serial.print ("RAM load: ");
        Serial.print (PCdata [20]);
        Serial.print (" GB (");
        Serial.print (PCdata [19]);
        Serial.println ("%)");
        Serial.print ("Hard disk: ");
        Serial.print (PCdata [32]);
        Serial.println (" %");
        //Serial.println (PCdata [21]); - avail ram mem
        
        Serial.print ("GPU Core: ");
        Serial.print (PCdata [22]);
        Serial.print (" /Mem: ");
        Serial.print (PCdata [23]);
        Serial.print (" /Shader: ");
        Serial.println (PCdata [24]);

        Serial.print ("GPU Core load: ");
        Serial.print (PCdata [25]);
        Serial.print (" /Mem ctrlr: ");
        Serial.print (PCdata [26]);
        Serial.print (" /Vid. eng.: ");
        Serial.print (PCdata [27]);
        Serial.print (" /Mem: ");
        Serial.println (PCdata [28]);

        //Serial.println (PCdata [29]); - gpu mem free
        //Serial.println (PCdata [30]); - gpu mem used
        //Serial.println (PCdata [31]); - gpu mem total

        Serial.println ();
        Serial.print (millis () - lastUpd);
        Serial.print (" ");
        Serial.print (parsingTime);
        Serial.print (" buf: ");
        Serial.println (bufSize);
        
        parsingTime = nIter = bufSize = 0;
        }


    while (millis () - last_millis < 40)
        {
        }
    
    }
// ------------------------------ ОСНОВНОЙ ЦИКЛ -------------------------------

void parsing ()
    {
    long start = millis ();

    while (Serial1.available () > 0)
        {
        bufSize++;

        lastUpd = millis ();

        char aChar = Serial1.read ();
        if (aChar != 'E')
            {
            inData [index] = aChar;
            index++;
            inData [index] = '\0';
            }
        else
            {
            char *p = inData;
            char *str;
            index = 0;
            String value = "";
            while ((str = strtok_r (p, ";", &p)) != NULL)
                {
                string_convert = str;
                PCdata [index] = string_convert.toInt ();
                index++;
                }
            index = 0;
            }
        }

    nIter++;
    parsingTime += millis () - start;
    }