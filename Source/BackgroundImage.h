#pragma once

//-------------------------------------------------------------------------------------------------------

#include "OrangeGate.h"

//-------------------------------------------------------------------------------------------------------

class Stripes : public juce::Component
{
private:
    void paintStripe(juce::Graphics &g, juce::Rectangle<int> r, float offsetTop, float offsetBottom);

public:
    void paint(juce::Graphics &g) override;
};

//-------------------------------------------------------------------------------------------------------

class BackgroundImage : public juce::Component
{
private:
    juce::Image m_image;
    juce::DrawableRectangle m_overlayRect;
    Stripes m_stripes;

public:
    BackgroundImage();

    void setPaintStripes(bool shouldPaintStripes);

    void paint(juce::Graphics &g) override;
    void resized() override;
};

//-------------------------------------------------------------------------------------------------------
