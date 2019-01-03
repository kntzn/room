/*
 Name:		Room.ino
 Created:	10/19/2018 8:10:31 AM
 Author:	CODEBOOK
*/

#include "DoorCapSensor.h"
#include "Button.h"
#include "WindowController.h"
#include "Hbridge.h"
#include "LightController.h"
#include "Predefined.h"
#include "Button.h"

#include "HardwareMonitor.h"
#include "Analyzer.h"

#include <LiquidCrystal_I2C.h>


void randomizeParameters (LightController &controller);

int main ()
    {
    // Microcontroller initialization
    init ();
    
    // Random initialization
    srand (analogRead (NC));

    // Serial1 initialization
    Serial.begin (BAUD_RATE_SERIAL);

    LiquidCrystal_I2C lcd (0x27, 16, 2);
    lcd.init ();
    lcd.backlight ();
    Serial1.begin (9600);

    // Initialization of controller and strip
    LightController controller;
    controller.setProfile (LightController::def);

    controller.setLedMode (StripController::fade_smooth);
    controller.setLedTableMode (StripController::VU);
    controller.setLedAnimationSpeedVU (20.f);
    controller.setLedAnimationSpeed (-0.25f);
    controller.setLedAnimationFrequency (0.5f);
    controller.setLedAnalyzerAnimationFrequency (40);
    controller.setLedAnalyzerAnimationOffset (HUE_RED);
    controller.setLedColor (CRGB::White);
    

    Analyzer analyzer;
    
    Button button_left (BUTT_LEFT);
    Button button_mid (BUTT_MIDL);
    Button button_right (BUTT_RGHT);

    HardwareMonitor hw_monitor;
    
    DoorCapSensor cs_door (CAP_SENSOR_DOOR);

    pinMode (46, OUTPUT);

    long prev_t = millis ();

    int max = 0;

    while (true)
        {
        // Clock
        float dt = (float (millis ()) - prev_t)/1000.f;
        prev_t = millis ();
        
        // !Clock
    
       

        // Analyzer
        analyzer.update (dt);
        controller.syncWithAnalyzer (analyzer, dt);
        if (analyzer.signalAvailable ())
            controller.setLedTableMode (StripController::VU);
        else
            controller.setLedTableMode (StripController::sync);
        // !Analyzer

        // Buttons
        button_left.update ();
        button_mid.update ();
        button_right.update ();
        // !Buttons

        // Capacitive sensors
        cs_door.update (dt);
        // !Capacitive sensors

        // Strip controller

        
        if (button_left.getState () == Button::buttonState::Rlsd)
            {
            Serial.println ("Led only");

            controller.setProfile (LightController::ledOnly);

            randomizeParameters (controller);
            }
        if (button_left.getState () == Button::buttonState::Hold)
            {
            Serial.println ("Rvd");

            controller.setProfile (LightController::film);
            }
        if (button_right.getState () == Button::buttonState::Rlsd)
            {
            Serial.println ("Night");

            controller.setProfile (LightController::night);
            }
        if (button_right.getState () == Button::buttonState::Hold)
            {
            Serial.println ("Rise");

            controller.setProfile (LightController::rise);
            }
        if (button_mid.getState () == Button::buttonState::Rlsd)
            {
            Serial.println ("Def");
            
            controller.setProfile (LightController::full);
            
            randomizeParameters (controller);
            }
        if (button_mid.getState () == Button::buttonState::Hold)
            {
            Serial.println ("Def");

            controller.setProfile (LightController::def);
            
            randomizeParameters (controller);
            }
        
        if (cs_door.itIsTimeToSwitchIsntIt ())
            {
            //Test
            digitalWrite (46, !digitalRead (46));

            //controller.setTorchereState (!controller.getTorchereState ());
            }
        controller.update (dt);

        hw_monitor.listenSerial ();
        
        lcd.clear ();
        lcd.print (cs_door.getMaxRise ());
        lcd.setCursor (0, 1);
        lcd.print (dt*1000.f);

        lcd.print (' ');
        lcd.print (hw_monitor.getParameter (HardwareMonitor::paramId::freqCore0));


        //Serial.println (dt*1000.f, 0);

        lcd.home ();
        lcd.display ();


        while (millis () - prev_t < 40)
            {}
        }

    return 0;
    }

// TODO:

// Cap sensor for lamp


// Cap buttons
// Hardware Monitor

void randomizeParameters (LightController & controller)
    {
    controller.setLedAnimationFrequency (float (rand () % 60001 -
                                            30000) / 60000.f);
    controller.setLedAnimationSpeed (float (rand () % 50001 -
                                            25000) / 100000.f);
    controller.setLedColor (CRGB (CHSV (rand(), 
                                        MAX_SATURATION,
                                        MAX_BRIGHTNESS)));
    byte mode = 0;
    while (mode == StripController::mainStripMode::off ||
            mode == StripController::mainStripMode::night ||
            mode == StripController::mainStripMode::rise)
        mode = rand () % StripController::n_modes;

    controller.setLedMode (mode);
    }
