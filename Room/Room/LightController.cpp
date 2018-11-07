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
