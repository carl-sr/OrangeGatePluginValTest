#pragma once

//-------------------------------------------------------------------------------------------------------

#include "OrangeGate.h"

#include "GateMultiplier.h"

//-------------------------------------------------------------------------------------------------------

class AnimatedToggleButton
    : public juce::Component
    , private juce::Timer
{
private:
    GateMultiplier m_togglePos;
    juce::ToggleButton m_button;

public:
    AnimatedToggleButton();

    void resized();
    void paint(juce::Graphics &g) override;

    void timerCallback() override;

    std::function<void(bool)> onToggled;
};

//-------------------------------------------------------------------------------------------------------
