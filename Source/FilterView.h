#pragma once

//-------------------------------------------------------------------------------------------------------

#include "OrangeGate.h"

#include "Filter.h"

//-------------------------------------------------------------------------------------------------------

class FilterView
    : public juce::Component
    , private juce::Timer
{
private:
    void timerCallback() override;

    Filter &m_sourceFilter;
    uint64_t m_coefficientRevision{ 0 };

    juce::Path m_curvePath;

public:
    FilterView(Filter &filter);
    ~FilterView() = default;

    void generateCurve();

    void resized() override;
    void paint(juce::Graphics &g) override;
};

//-------------------------------------------------------------------------------------------------------
