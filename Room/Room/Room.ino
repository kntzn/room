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

    HardwareMonitor hwm;
    
    DoorCapSensor cs_door (CAP_SENSOR_DOOR);

    pinMode (46, OUTPUT);

    long prev_t = millis ();

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


        hwm.update ();

        Serial.println (hwm.HWMuptime ());
        
        if (hwm.becomeAvailable ())
            {
            controller.setLedMode (StripController::mono);
            controller.setLedTableMode (StripController::sync);
            }

        controller.setLedFreeze (hwm.HWMuptime () > 1.5f * (MODE_SWITCH_FADE_TIME * 1000.f));
        controller.update (dt);

        // Creates constant dt (limits the UPS)
        while (millis () - prev_t < 40)
            {
            // Why not update instead of waiting?
            hwm.update ();
            }
        }

    return 0;
    }

// TODO:

// Cap sensor for lamp


// Cap buttons
// Hardware Monitor

// XOR (HWM, Mode != mono)

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
