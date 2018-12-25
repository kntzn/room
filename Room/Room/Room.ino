/*
 Name:		Room.ino
 Created:	10/19/2018 8:10:31 AM
 Author:	CODEBOOK
*/

#include "Button.h"
#include "WindowController.h"
#include "Hbridge.h"
#include "LightController.h"
#include "Predefined.h"
#include "Button.h"

#include "HardwareMonitor.h"
#include "Analyzer.h"

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
    controller.setLedMode (StripController::oldschoolRND);
    controller.setLedTableMode (StripController::sync);
    controller.setLedAnimationSpeedVU (20.f);
    controller.setLedAnimationSpeed (-0.05f);
    controller.setLedAnimationFrequency (0.5f);
    controller.setLedAnalyzerAnimationFrequency (80);
    controller.setLedAnalyzerAnimationOffset (HUE_ORANGE);
    controller.setLedColor (CRGB::White);


    Analyzer analyzer;
    
    Button button_left (BUTT_LEFT);
    Button button_mid (BUTT_MIDL);
    Button button_right (BUTT_RGHT);

    //HardwareMonitor hw_monitor;

    float prev_t = float (millis ());

    while (true)
        {
        // Clock
        float dt = (float (millis ()) - prev_t)/1000.f;
        prev_t += dt*1000.f;
        // !Clock
    
        // Analyzer
        analyzer.update (dt);
        controller.syncWithAnalyzer (analyzer, dt);
        // !Analyzer

        // Buttons
        button_left.update ();
        button_mid.update ();
        button_right.update ();
        // !Buttons

        // Strip controller
        if (button_left.getState () == Button::buttonState::Rlsd)
            {
            Serial.println ("Led only");

            controller.setProfile (LightController::ledOnly);

            controller.setLedMode (rand () % (StripController::n_modes - 2) + 2);
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
            
            controller.setLedMode (rand () % (StripController::n_modes - 2) + 2);

            }
        if (button_mid.getState () == Button::buttonState::Hold)
            {
            Serial.println ("Def");

            controller.setProfile (LightController::def);
            
            controller.setLedMode (rand () % (StripController::n_modes - 2) + 2);
            }
        
        controller.update (dt);
        }

    return 0;
    }