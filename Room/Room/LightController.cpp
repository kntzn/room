// 
// 
// 

#include "LightController.h"

LightController::LightController ()
    {
    }

void LightController::update (float dt)
    {
    LED.update (dt);
    Lamp.update ();
    Torchere.update ();
    }

void LightController::syncWithAnalyzer (Analyzer & analyzer, float dt)
    {
    LED.setFreqValues (analyzer.getFreqValues ());
    //LED.setFreq3values (analyzer.getFreq3Values ());
    }

void LightController::setProfile (byte mode)
    {
    switch (mode)
        {
        case off:
            {
            LED.setMode (StripController::off);
            LED.setTableMode (StripController::sync);
            Torchere.setState (BulbController::off);
            Lamp.setState (BulbController::off);
            break;
            }
        case full:
            {
            LED.setColor (CRGB::White);
            LED.setMode (StripController::mono);
            LED.setTableMode (StripController::sync);
            Torchere.setState (BulbController::on);
            Lamp.setState (BulbController::on);
            break;
            }
        case noLed:
            {
            LED.setMode (StripController::off);
            LED.setTableMode (StripController::sync);
            Torchere.setState (BulbController::on);
            Lamp.setState (BulbController::on);
            break;
            }
        case film:
            {
            LED.setMode (StripController::RVD);
            LED.setTableMode (StripController::sync);
            Torchere.setState (BulbController::off);
            Lamp.setState (BulbController::on);
            break;
            }
        case night:
            {
            LED.setMode (StripController::night);
            LED.setTableMode (StripController::sync);
            Torchere.setState (BulbController::off);
            Lamp.setState (BulbController::off);
            break;
            }

        default:
            break;
        }
    }
