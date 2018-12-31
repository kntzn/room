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
    controller.setLedMode (StripController::fade_smooth);
    controller.setLedTableMode (StripController::sync);
    controller.setLedAnimationSpeedVU (20.f);
    controller.setLedAnimationSpeed (-0.25f);
    controller.setLedAnimationFrequency (0.5f);
    controller.setLedAnalyzerAnimationFrequency (80);
    controller.setLedAnalyzerAnimationOffset (HUE_ORANGE);
    controller.setLedColor (CRGB::White);
    controller.setProfile (LightController::def);

    Analyzer analyzer;
    
    Button button_left (BUTT_LEFT);
    Button button_mid (BUTT_MIDL);
    Button button_right (BUTT_RGHT);

    //HardwareMonitor hw_monitor;
    
    DoorCapSensor cs_door (CAP_SENSOR_DOOR);

    long prev_t = millis ();

    int max = 0;

    while (true)
        {
        // Clock
        float dt = (float (millis ()) - prev_t)/1000.f;
        prev_t = millis ();
        
        // !Clock
    
        
        // Analyzer
        //analyzer.update (dt);
        //controller.syncWithAnalyzer (analyzer, dt);
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
            controller.setTorchereState (!controller.getTorchereState ());
            }
        controller.update (dt);

        lcd.print (cs_door.getMaxRise ());
        lcd.setCursor (0, 1);
        lcd.print (dt*1000.f);
        Serial.println (dt*1000.f, 0);

        lcd.home ();
        lcd.display ();


        while (millis () - prev_t < 50)
            {}
        }

    return 0;
    }

// TODO:
// dt for smoothAver ! ! ! ! 
// Cap sensor for lamp
// Autodetect microphone
// Recreate schematic
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
