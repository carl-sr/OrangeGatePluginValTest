#pragma once

//-------------------------------------------------------------------------------------------------------

#include "OrangeGate.h"

#include <array>

#include "BoxShadow.h"

//-------------------------------------------------------------------------------------------------------

class Header
    : public juce::Component
    , private juce::Timer
{
private:

    juce::Image m_image;
    juce::ImageComponent m_imageComponent;

    BoxShadow m_boxShadow;
    juce::TextButton m_trigger;

    std::array<juce::Colour, 30> m_transition;
    int m_transitionIndex{ 30 };

    juce::Colour m_activeBackgroundColour;

    void timerCallback();

public:
    Header();
    ~Header();

    void resized() override;
    void paint(juce::Graphics &g) override;
};

//-------------------------------------------------------------------------------------------------------
