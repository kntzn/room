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

    // Random initialization
    srand (analogRead (NC));

    // Initialization of the controller
    digitalWrite (RELAY_TORCHERE, HIGH);
    digitalWrite (RELAY_LAMP, HIGH);
    LightController controller;
    controller.setProfile (LightController::fullWhite);
    randomizeParameters (controller);

    // Serial initialization
    Serial.begin (BAUD_RATE_SERIAL);

    // Window initialization
    WindowController window;
    window.setAutoMode (true);

    // NRF initialization
    byte nrfBuf [NRF_BUF_SIZE] = {};
    RF24 nrf (NRF_CE, NRF_CSN);
    nrf.begin ();
    nrf.powerUp ();
    nrf.startListening ();
    nrf.openReadingPipe (1, 0xF0F0F0F0F0);
    nrf.setChannel (0x57);
    nrf.setPayloadSize (16);
    
    // Analyzer initialization
    Analyzer analyzer;
    
    // Screen initialization
    HardwareMonitor hwm;
    UI ui;

    // Capacitive sensors initialization
    DoorCapSensor cs_door (CAP_SENSOR_DOOR);
    LampCapSensor cs_lamp (CS_LAMP_TX_PIN, 
                           CS_LAMP_RX_PIN,
                           CS_LAMP_OFF_TH,
                           CS_LAMP_ON_TH);
    
    long prev_t = millis ();

    while (true)
        {
        // Clock
        // Creates constant dt (limits the UPS)
        while (millis () - prev_t < (1000 / UPS_SYSTEM));
        // Counts real delay
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
