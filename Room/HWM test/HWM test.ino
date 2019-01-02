/*
Что идёт в порт: 0-CPU temp, 1-GPU temp, 2-mother temp, 3-max HDD temp, 4-CPU load, 5-GPU load, 6-RAM use, 7-GPU memory use,
8-maxFAN, 9-minFAN, 10-maxTEMP, 11-minTEMP, 12-manualFAN, 13-manualCOLOR, 14-fanCtrl, 15-colorCtrl, 16-brightCtrl, 17-LOGinterval, 18-tempSource
*/
// ------------------------ НАСТРОЙКИ ----------------------------
// настройки пределов скорости и температуры по умолчанию (на случай отсутствия связи)

#define BTN1 A3             // первая кнопка
#define BTN2 A2             // вторая кнопка

#include <string.h>             // библиотека расширенной работы со строками
#include <Wire.h>               // библиотека для соединения
#include <LiquidCrystal_I2C.h>  // библтотека дислея

#include <SoftwareSerial.h>

// -------- АВТОВЫБОР ОПРЕДЕЛЕНИЯ ДИСПЛЕЯ-------------
// Если кончается на 4Т - это 0х27. Если на 4АТ - 0х3f
#if (DRIVER_VERSION)
LiquidCrystal_I2C lcd (0x27, 16, 2);
#else
LiquidCrystal_I2C lcd (0x3f, 16, 2);
#endif
// -------- АВТОВЫБОР ОПРЕДЕЛЕНИЯ ДИСПЛЕЯ-------------

#define printByte(args)  write(args);

// значок градуса!!!! lcd.write(223);
byte degree [8] = { 0b11100,  0b10100,  0b11100,  0b00000,  0b00000,  0b00000,  0b00000,  0b00000 };
// правый край полосы загрузки
byte right_empty [8] = { 0b11111,  0b00001,  0b00001,  0b00001,  0b00001,  0b00001,  0b00001,  0b11111 };
// левый край полосы загрузки
byte left_empty [8] = { 0b11111,  0b10000,  0b10000,  0b10000,  0b10000,  0b10000,  0b10000,  0b11111 };
// центр полосы загрузки
byte center_empty [8] = { 0b11111, 0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b11111 };
// блоки для построения графиков
byte row8 [8] = { 0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111 };
byte row7 [8] = { 0b00000,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111 };
byte row6 [8] = { 0b00000,  0b00000,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111 };
byte row5 [8] = { 0b00000,  0b00000,  0b00000,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111 };
byte row4 [8] = { 0b00000,  0b00000,  0b00000,  0b00000,  0b11111,  0b11111,  0b11111,  0b11111 };
byte row3 [8] = { 0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b11111,  0b11111,  0b11111 };
byte row2 [8] = { 0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b11111,  0b11111 };
byte row1 [8] = { 0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b11111 };

char inData [82];       // массив входных значений (СИМВОЛЫ)
int PCdata [20];        // массив численных значений показаний с компьютера
byte PLOTmem [6] [16];   // массив для хранения данных для построения графика (16 значений для 6 параметров)
byte blocks, halfs;
byte index = 0;
int display_mode = 6;
String string_convert;
unsigned long timeout, uptime_timer, plot_timer;
boolean lightState, reDraw_flag = 1, updateDisplay_flag, updateTemp_flag, timeOut_flag = 1;
int duty, LEDcolor;
int k, b, R, G, B, Rf, Gf, Bf;
byte mainTemp;
byte lines[] = { 4, 5, 7, 6 };
byte plotLines[] = { 0, 1, 4, 5, 6, 7 };    // 0-CPU temp, 1-GPU temp, 2-CPU load, 3-GPU load, 4-RAM load, 5-GPU memory
String perc;
unsigned long sec, mins, hrs;
byte temp1, temp2;
boolean btn1_sig, btn2_sig, btn1_flag, btn2_flag;

// Названия для легенды графиков
const char plot_0[] = "CPU";
const char plot_1[] = "GPU";
const char plot_2[] = "RAM";

const char plot_3[] = "temp";
const char plot_4[] = "load";
const char plot_5[] = "mem";
// названия ниже должны совпадать с массивами сверху и идти по порядку!
static const char *plotNames0[] = {
    plot_0, plot_1, plot_0, plot_1, plot_2, plot_1
    };
static const char *plotNames1[] = {
    plot_3, plot_3, plot_4, plot_4, plot_4, plot_5
    };
// 0-CPU temp, 1-GPU temp, 2-CPU load, 3-GPU load, 4-RAM load, 5-GPU memory

SoftwareSerial Serial1 (2, 3);

void setup ()
    {
    Serial.begin (9600);
    Serial1.begin (9600);

    pinMode (BTN1, INPUT_PULLUP);
    pinMode (BTN2, INPUT_PULLUP);

    lcd.init ();
    lcd.backlight ();
    lcd.clear ();            // очистить дисплей

    }
// 8-maxFAN, 9-minFAN, 10-maxTEMP, 11-minTEMP, 12-mnlFAN

// ------------------------------ ОСНОВНОЙ ЦИКЛ -------------------------------
void loop ()
    {
    //parsing ();                          // парсим строки с компьютера
                                         //updatePlot ();                       // обновляем массив данных графика
    
                                         
    while (Serial1.available ())
        Serial.print (Serial1.read ());
                                         /*
    Serial.print (PCdata [0]);
    Serial.print (" ");
    Serial.print (PCdata [1]);
    Serial.print (" ");
    Serial.print (PCdata [2]);
    Serial.print (" ");
    Serial.print (PCdata [3]);
    Serial.print (" ");
    Serial.print (PCdata [4]);
    Serial.print (" ");
    Serial.print (PCdata [5]);
    Serial.print (" ");
    Serial.print (PCdata [6]);
    Serial.print (" ");
    Serial.println (PCdata [7]);
    
    */


                                         //buttonsTick ();                      // опрос кнопок и смена режимов
                                         //updateDisplay ();                    // обновить показания на дисплее
    //timeoutTick ();                      // проверка таймаута
    }
// ------------------------------ ОСНОВНОЙ ЦИКЛ -------------------------------

void buttonsTick ()
    {
    btn1_sig = !digitalRead (BTN1);
    btn2_sig = !digitalRead (BTN2);
    if (btn1_sig && !btn1_flag) {
        display_mode++;
        reDraw_flag = 1;
        if (display_mode > 9) display_mode = 0;
        btn1_flag = 1;
        }
    if (!btn1_sig && btn1_flag) {
        btn1_flag = 0;
        }
    if (btn2_sig && !btn2_flag) {
        display_mode--;
        reDraw_flag = 1;
        if (display_mode < 0) display_mode = 9;
        btn2_flag = 1;
        }
    if (!btn2_sig && btn2_flag) {
        btn2_flag = 0;
        }
    }

void parsing ()
    {
    while (Serial1.available () > 0)
        {
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
            updateDisplay_flag = 1;
            updateTemp_flag = 1;
            }
        }
    }
void updatePlot ()
    {
    if ((millis () - plot_timer) > (PCdata [17] * 1000)) {
        for (int i = 0; i < 6; i++) {           // для каждой строки параметров
            for (int j = 0; j < 15; j++) {        // каждый столбец параметров (кроме последнего)
                PLOTmem [i] [j] = PLOTmem [i] [j + 1];  // сдвинуть весь массив на шаг ВЛЕВО
                }
            }
        for (int i = 0; i < 6; i++) {
            // запомнить общее число полосок графика в ПОСЛЕДНИЙ элемент массива
            PLOTmem [i] [15] = PCdata [plotLines [i]];
            }
        plot_timer = millis ();
        }
    }
void updateDisplay ()
    {
    if (updateDisplay_flag) {
        if (reDraw_flag) {
            lcd.clear ();
            switch (display_mode) {
                case 0:
                case 1:
                case 2:
                case 3:
                case 4:
                case 5: draw_plot_symb ();
                    break;
                case 6: draw_labels_11 ();
                    break;
                case 7: draw_labels_12 ();
                    break;
                case 8: draw_labels_21 ();
                    break;
                case 9: draw_labels_22 ();
                    break;
                }
            reDraw_flag = 0;
            }
        switch (display_mode) {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5: draw_plot ();
                break;
            case 6: draw_stats_11 ();
                break;
            case 7: draw_stats_12 ();
                break;
            case 8: draw_stats_21 ();
                break;
            case 9: draw_stats_22 ();
                break;
            case 50: debug ();
                break;
            }
        updateDisplay_flag = 0;
        }
    }

void draw_stats_11 ()
    {
    lcd.setCursor (4, 0); lcd.print (PCdata [0]); lcd.write (223);
    lcd.setCursor (13, 0); lcd.print (PCdata [4]);
    if (PCdata [4] < 10) perc = "% ";
    else if (PCdata [4] < 100) perc = "%";
    else perc = "";  lcd.print (perc);
    lcd.setCursor (4, 1); lcd.print (PCdata [1]); lcd.write (223);
    lcd.setCursor (13, 1); lcd.print (PCdata [5]);
    if (PCdata [5] < 10) perc = "% ";
    else if (PCdata [5] < 100) perc = "%";
    else perc = "";  lcd.print (perc);

    for (int i = 0; i < 2; i++) {
        byte line = ceil (PCdata [lines [i]] / 16);
        lcd.setCursor (7, i);
        if (line == 0) lcd.printByte (1)
        else lcd.printByte (4);
        for (int n = 1; n < 5; n++) {
            if (n < line) lcd.printByte (4);
            if (n >= line) lcd.printByte (2);
            }
        if (line == 6) lcd.printByte (4)
        else lcd.printByte (3);
        }
    }

void draw_stats_12 ()
    {
    lcd.setCursor (13, 0); lcd.print (PCdata [7]);
    if (PCdata [7] < 10) perc = "% ";
    else if (PCdata [7] < 100) perc = "%";
    else perc = "";  lcd.print (perc);
    lcd.setCursor (13, 1); lcd.print (PCdata [6]);
    if (PCdata [6] < 10) perc = "% ";
    else if (PCdata [6] < 100) perc = "%";
    else perc = "";  lcd.print (perc);

    for (int i = 0; i < 2; i++) {
        byte line = ceil (PCdata [lines [i + 2]] / 16);
        lcd.setCursor (7, i);
        if (line == 0) lcd.printByte (1)
        else lcd.printByte (4);
        for (int n = 1; n < 5; n++) {
            if (n < line) lcd.printByte (4);
            if (n >= line) lcd.printByte (2);
            }
        if (line == 6) lcd.printByte (4)
        else lcd.printByte (3);
        }
    }

void draw_stats_21 ()
    {
    lcd.setCursor (13, 0); lcd.print (duty);
    if ((duty) < 10) perc = "% ";
    else if ((duty) < 100) perc = "%";
    else perc = "";  lcd.print (perc);

    lcd.setCursor (3, 1); lcd.print (temp1); lcd.write (223);
    lcd.setCursor (11, 1); lcd.print (temp2); lcd.write (223);

    byte line = ceil (duty / 16);
    lcd.setCursor (6, 0);
    if (line == 0) lcd.printByte (1)
    else lcd.printByte (4);
    for (int n = 1; n < 5; n++) {
        if (n < line) lcd.printByte (4);
        if (n >= line) lcd.printByte (2);
        }
    if (line == 6) lcd.printByte (4)
    else lcd.printByte (3);
    }
void draw_stats_22 ()
    {
    lcd.setCursor (2, 0); lcd.print (PCdata [2]); lcd.write (223);
    lcd.setCursor (10, 0); lcd.print (PCdata [3]); lcd.write (223);

    lcd.setCursor (7, 1);
    sec = (long)(millis () - uptime_timer) / 1000;
    hrs = floor ((sec / 3600));
    mins = floor (sec - (hrs * 3600)) / 60;
    sec = sec - (hrs * 3600 + mins * 60);
    if (hrs < 10) lcd.print (0);
    lcd.print (hrs);
    lcd.print (":");
    if (mins < 10) lcd.print (0);
    lcd.print (mins);
    lcd.print (":");
    if (sec < 10) lcd.print (0);
    lcd.print (sec);
    }
void draw_labels_11 ()
    {
    lcd.createChar (0, degree);
    lcd.createChar (1, left_empty);
    lcd.createChar (2, center_empty);
    lcd.createChar (3, right_empty);
    lcd.createChar (4, row8);
    lcd.setCursor (0, 0);
    lcd.print ("CPU:");
    lcd.setCursor (0, 1);
    lcd.print ("GPU:");
    }
void draw_labels_12 ()
    {
    lcd.createChar (0, degree);
    lcd.createChar (1, left_empty);
    lcd.createChar (2, center_empty);
    lcd.createChar (3, right_empty);
    lcd.createChar (4, row8);
    lcd.setCursor (0, 0);
    lcd.print ("GPUmem:");
    lcd.setCursor (0, 1);
    lcd.print ("RAMuse:");
    }
void draw_labels_21 ()
    {
    lcd.createChar (0, degree);
    lcd.createChar (1, left_empty);
    lcd.createChar (2, center_empty);
    lcd.createChar (3, right_empty);
    lcd.createChar (4, row8);

    lcd.setCursor (0, 0);
    lcd.print ("FANsp:");
    lcd.setCursor (0, 1);
    lcd.print ("T1: ");
    lcd.setCursor (8, 1);
    lcd.print ("T2:");
    }
void draw_labels_22 ()
    {
    lcd.createChar (0, degree);
    lcd.createChar (1, left_empty);
    lcd.createChar (2, center_empty);
    lcd.createChar (3, right_empty);
    lcd.createChar (4, row8);

    lcd.setCursor (0, 0);
    lcd.print ("M:");
    lcd.setCursor (7, 0);
    lcd.print ("HM:");
    lcd.setCursor (0, 1);
    lcd.print ("UPTIME:");
    }
void draw_legend ()
    {
    byte data = PCdata [plotLines [display_mode]];
    lcd.setCursor (10, 0); lcd.print (data);
    if (display_mode > 1) {
        if (data < 10) perc = "% ";
        else if (data < 100) perc = "%";
        else {
            perc = "";
            }
        lcd.print (perc);
        }
    else {
        if (data < 10) {
            lcd.write (223);
            lcd.print ("  ");
            }
        else if (data < 100) {
            lcd.write (223); lcd.print (" ");
            }
        else {
            lcd.write (223);
            }
        }
    }

void draw_plot ()
    {
    draw_legend ();

    lcd.setCursor (0, 1);
    for (int i = 0; i < 16; i++) {
        lcd.printByte (constrain (map (PLOTmem [display_mode] [i], 0, 100, 0, 7), 0, 7));
        }
    }

void draw_plot_symb ()
    {
    lcd.createChar (0, row1);
    lcd.createChar (1, row2);
    lcd.createChar (2, row3);
    lcd.createChar (3, row4);
    lcd.createChar (4, row5);
    lcd.createChar (5, row6);
    lcd.createChar (6, row7);
    lcd.createChar (7, row8);
    lcd.setCursor (0, 0);
    lcd.print (plotNames0 [display_mode]);
    lcd.setCursor (3, 0);
    lcd.print (plotNames1 [display_mode]);
    lcd.print (": ");
    }
void timeoutTick ()
    {
    if ((millis () - timeout > 5000) && timeOut_flag) {
        lcd.clear ();
        lcd.setCursor (3, 0);
        lcd.print ("CONNECTION");
        lcd.setCursor (5, 1);
        lcd.print ("FAILED");
        timeOut_flag = 0;
        reDraw_flag = 1;
        }
    }
void show_logo ()
    {
    
    lcd.setCursor (0, 0);
    lcd.printByte (0);
    lcd.printByte (1);
    lcd.printByte (2);
    lcd.setCursor (0, 1);
    lcd.printByte (3);
    lcd.printByte (4);
    lcd.printByte (5);
    lcd.setCursor (4, 0);
    lcd.print ("AlexGyver");
    lcd.setCursor (4, 1);
    lcd.print ("Technologies");
    }
void show_logo2 ()
    {
    lcd.setCursor (1, 0);
    lcd.print ("modified by");
    lcd.setCursor (5, 1);
    lcd.print ("klykov.net");
    }
void debug ()
    {
    lcd.clear ();
    lcd.setCursor (0, 0);
    for (int j = 0; j < 5; j++) {
        lcd.print (PCdata [j]); lcd.print ("  ");
        }
    lcd.setCursor (0, 1);
    for (int j = 6; j < 10; j++) {
        lcd.print (PCdata [j]); lcd.print ("  ");
        }
    lcd.setCursor (0, 2);
    for (int j = 10; j < 15; j++) {
        lcd.print (PCdata [j]); lcd.print ("  ");
        }
    lcd.setCursor (0, 3);
    for (int j = 15; j < 18; j++) {
        lcd.print (PCdata [j]); lcd.print ("  ");
        }
    }