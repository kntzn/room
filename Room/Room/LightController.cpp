// 
// 
// 

#include "LightController.h"

void LightController::restartAnimation ()
    {
    LED.setMode (lastAnimationMode);
    }

LightController::LightController ()
    {
    Lamp.setLogicLevel (true);
    Torchere.setLogicLevel (true);
    }

void LightController::update (float dt, DoorSensor* sens = nullptr)
    {
    bool isTrig = false;

    if (sens != nullptr)
        isTrig = sens->isTriggered ();
    else
        isTrig = true;

    LED.setTrigger (isTrig);

    LED.update (dt);
    if (isTrig)
        {
        Lamp.update ();
        Torchere.update ();
        }
    }

void LightController::syncWithAnalyzer (Analyzer & analyzer, float dt)
    {
    LED.setFreqValues (analyzer.getFreqValues ());
    LED.setVU_val (analyzer.getVUout ());
    LED.setFreq3values (analyzer.getFreq3Values ());
    }

void LightController::setProfile (byte mode, DoorSensor* sens = nullptr)
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
            LED.setMode (StripController::fullWhite);
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
            if (sens != nullptr)
                sens->setTriggerType (DoorSensor::ifOpening);

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
        case def:
            {
            restartAnimation ();
            LED.setTableMode (StripController::sync);
            Torchere.setState (BulbController::on);
            Lamp.setState (BulbController::on);
            break;
            }
        default:
            break;
        }
    }

void LightController::setLedMode (byte newMode)
    {
    if (newMode != StripController::mainStripMode::off &&
        newMode != StripController::mainStripMode::fullWhite &&
        newMode != StripController::mainStripMode::rise &&
        newMode != StripController::mainStripMode::night)
        lastAnimationMode = newMode;

    LED.setMode (newMode);
    }
void LightController::setLedTableMode (byte newMode)
    {
    LED.setTableMode (newMode);
    }
void LightController::setLedColor (CRGB newColor)
    {
    LED.setColor (newColor);
    }
void LightController::setLedAnimationFrequency (float newFreq)
    {
    LED.setRainbowFrequency (newFreq);
    }
void LightController::setLedAnimationSpeed (float newSpeed)
    {
    LED.setRainbowSpeed (newSpeed);
    }
void LightController::setLedAnimationSpeedVU (float newSpeed)
    {
    LED.setPaletteSpeed (newSpeed);
    }
void LightController::setLedAnalyzerAnimationFrequency (float newFreq)
    {
    LED.setFreqModeRainFreq (newFreq);
    }
void LightController::setLedAnalyzerAnimationOffset (float newOffset)
    {
    LED.setFreqModeRainOffset (newOffset);
    }

void LightController::setTorchereState (byte newState)
    {
    Torchere.setState (newState);
    }
void LightController::setLampState (byte newState)
    {
    Lamp.setState (newState);
    }