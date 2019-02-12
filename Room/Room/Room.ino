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
#include "PowerSupply.h"

#include "HardwareMonitor.h"
#include "Analyzer.h"

#include <LiquidCrystal_I2C.h>
#include <RF24-master\RF24.h>
#include <CapacitiveSensor.h>
#include "UI.h"


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
    controller.setProfile (LightController::full);
    controller.setLedColor (CRGB::Green);
    controller.setLedMode (StripController::fade_smooth);
    controller.setLedAnimationSpeed (0.05f);
    //randomizeParameters (controller);
    controller.setLedLinear (false);

    // Serial initialization
    Serial.begin (BAUD_RATE_SERIAL);
    pinMode (AVAIL_TO_READ, OUTPUT);

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
    DoorCapSensor cs_door;
    LampCapSensor cs_lamp (CS_LAMP_TX_PIN, 
                           CS_LAMP_RX_PIN,
                           CS_LAMP_OFF_TH,
                           CS_LAMP_ON_TH);
    
    // Power supply
    PowerSupply pSupply (CURRENT_SENSOR_PIN, 
                         VOLTAGE_SENSOR_PIN);

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

        bool analyzer_conn = analyzer.connected ();
        bool analyzer_avail = analyzer.signalAvailable ();
        
        if (analyzer_conn && analyzer_avail)
            controller.setLedTableMode (StripController::VU_rain);
        else
            controller.setLedTableMode (StripController::sync);
        // !Analyzer
        
        // Capacitive sensors
        cs_door.update ();
        cs_lamp.update (controller.getLampState ());

        // !Capacitive sensors
        
        // NRF24L01
        /*
        while (nrf.available ())
            {
            nrf.read (nrfBuf, NRF_BUF_SIZE);

            Serial.println (int (nrfBuf [0]));
            }
        */

        // Light controller
        if (cs_lamp.getState () == LampCapSensor::Hold)
            {
            controller.setLampState (!controller.getLampState ());
            }
        if (cs_door.getState () == DoorCapSensor::Hold)
            {
            controller.setTorchereState (!controller.getTorchereState ());
            }
        

        digitalWrite (AVAIL_TO_READ, LOW);
        controller.update (dt);
        digitalWrite (AVAIL_TO_READ, HIGH);
        // !Light controller

        // Hardware monitor
        hwm.update ();
        
        // UI
        ui.update (hwm, controller, analyzer);

        // Window controller
        window.update (controller);
        }

    return 0;
    }
