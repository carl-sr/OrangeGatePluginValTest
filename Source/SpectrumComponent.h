#pragma once

//-------------------------------------------------------------------------------------------------------

#include "OrangeGate.h"

#include "SpectrumProcessor.h"
#include "SpectrumComponentBase.h"
#include "SpectrumLevelValue.h"

//---------------------------------------------------------------------------------------------------

class SpectrumComponent
    : public SpectrumComponentBase
    , public juce::Timer
{
private:
    float m_rampLengthSeconds{ 0.50f };

    std::vector<SpectrumLevelValue> m_smoothedLevels;
    std::vector<SpectrumLevelValue> &createSmoothedLevels();
    std::vector<float> m_interpolatedValues;
    std::vector<float> &createValues();

    juce::Colour m_drawColour{ juce::Colours::white };

    float midPoint(float a, float b);

public:
    explicit SpectrumComponent(SpectrumProcessor &p);

    void timerCallback() override;
    void resized() override;
    void paint(juce::Graphics &g) override;

    void setDrawColour(juce::Colour c);
};

//---------------------------------------------------------------------------------------------------

class DoubleSpectrumComponent
    : public juce::Component
    , private juce::Timer
{
private:
    void timerCallback() override;

public:

    SpectrumComponent back;
    SpectrumComponent front;

    DoubleSpectrumComponent(SpectrumProcessor &frontSpectrum, SpectrumProcessor &backSpectrum);

    void setFrontSpectrumVisible(bool visible);
    void setBackSpectrumVisible(bool visible);

    void resized() override;

};

//-------------------------------------------------------------------------------------------------------

