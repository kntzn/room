/*
 Name:		Room.ino
 Created:	10/19/2018 8:10:31 AM
 Author:	CODEBOOK
*/



#include "UI.h"
#include "DoorCapSensor.h"
#include "Button.h"
#include "WindowController.h"
#include "LightController.h"
#include "Predefined.h"
#include "Button.h"

#include "HardwareMonitor.h"
#include "Analyzer.h"

#include <LiquidCrystal_I2C.h>
#include <RF24-master\RF24.h>
#include <CapacitiveSensor.h>
#include "UI.h"

void randomizeParameters (LightController &controller);

int main ()
    {
    // Microcontroller initialization
    init ();
    // This reduces the flashes while initializing
    digitalWrite (RELAY_TORCHERE, HIGH);
    digitalWrite (RELAY_LAMP, HIGH);
    
    // Random initialization
    srand (analogRead (NC));

    // Serial initialization
    Serial.begin (BAUD_RATE_SERIAL);

    WindowController window;
    window.setAutoMode (true);

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
    controller.setProfile (LightController::full);

    controller.setLedMode (StripController::fade_switch_random);
    controller.setLedTableMode (StripController::sync);
    controller.setLedAnimationSpeedVU (-20.f);
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
    UI ui;

    DoorCapSensor cs_door (CAP_SENSOR_DOOR);
    LampCapSensor cs_lamp (40, 42, 
                           600,
                           1100);
    
    long prev_t = millis ();

    while (true)
        {
        // Clock
        // Creates constant dt (limits the UPS)
        while (millis () - prev_t < 40)
            {}
        float dt = (float (millis ()) - prev_t) / 1000.f;
        prev_t = millis ();
        // !Clock
        
        // Analyzer
        analyzer.update (dt);
        controller.syncWithAnalyzer (analyzer, dt);
        if (analyzer.connected () && analyzer.signalAvailable ())
            controller.setLedTableMode (StripController::FREQ_FULL);
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
        cs_lamp.update (controller.getLampState ());
        // !Capacitive sensors
        
        // NRF24L01
        while (nrf.available ())
            {
            nrf.read (nrfBuf, 16);

            Serial.println (int (nrfBuf [0]));
            }
        
        // Light controller
        if (cs_lamp.getState () == LampCapSensor::Hold)
            {
            controller.setLampState (!controller.getLampState ());
            }
        if (cs_door.itIsTimeToSwitchIsntIt ())
            {
            controller.setTorchereState (!controller.getTorchereState ());
            }

        controller.update (dt);
        // !Light controller

        // Hardware monitor
        hwm.update ();
        // UI
        ui.update (hwm, controller);
        
        // Window controller
        window.update ();
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
