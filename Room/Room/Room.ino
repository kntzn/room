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

#include <RF24-master\RF24.h>

#include <Servo.h>


void randomizeParameters (LightController &controller);

int main ()
    {
    // Microcontroller initialization
    init ();
    
    // Random initialization
    srand (analogRead (NC));

    // Serial1 initialization
    Serial.begin (BAUD_RATE_SERIAL);

    Servo window;
    window.attach (WINDOW_OUTPUT);
    window.write (WINDOW_ZERO_SPEED);


    byte nrfBuf [16] = {};
    RF24 nrf (48, 49);
    nrf.begin ();
    nrf.powerUp ();
    nrf.startListening ();
    nrf.openReadingPipe (1, 0xF0F0F0F0F0);
    nrf.setChannel (0x57);
    nrf.setPayloadSize (16);
    
    // Initialization of controller and strip
    LightController controller;
    controller.setProfile (LightController::def);

    controller.setLedMode (StripController::fade_smooth);
    controller.setLedTableMode (StripController::sync);
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

    pinMode (44, OUTPUT);

    long prev_t = millis ();

    while (true)
        {
        // Clock
        float dt = (float (millis ()) - prev_t) / 1000.f;
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


        // NRF24L01
        while (nrf.available ())
            {
            nrf.read (nrfBuf, 16);

            Serial.println (int (nrfBuf [0]));
            }
        // !NRF24L01

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
            digitalWrite (44, !digitalRead (44));
            }

        hwm.update ();



        // detaching servo to avoid signal distortion
        window.detach ();
        controller.update (dt);
        window.attach (WINDOW_OUTPUT);

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
