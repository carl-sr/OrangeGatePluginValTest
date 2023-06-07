#pragma once

//-------------------------------------------------------------------------------------------------------
#include "OrangeGate.h"

#include "RmsLevelCalculator.h"

//-------------------------------------------------------------------------------------------------------

class VuMeter
    : public juce::Component
    , public juce::Timer
{
private:
    RmsLevelCalculator &m_sourceRms;

public:
    explicit VuMeter(RmsLevelCalculator &sourceRms);
    ~VuMeter() override;

    void paint(juce::Graphics &) override;
    void resized() override;

    void timerCallback() override;
};

//-------------------------------------------------------------------------------------------------------
