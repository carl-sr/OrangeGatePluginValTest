#include "OrangeGate.h"
#include "VuMeter.h"

//-------------------------------------------------------------------------------------------------------

VuMeter::VuMeter(RmsLevelCalculator &sourceRms)
    : m_sourceRms(sourceRms)
{
    startTimerHz(60);
}

//-------------------------------------------------------------------------------------------------------

VuMeter::~VuMeter()
{
}

//-------------------------------------------------------------------------------------------------------

void VuMeter::paint(juce::Graphics &g)
{
    g.fillAll(juce::Colour::fromRGB(0x20, 0x20, 0x20));
    g.setColour(juce::Colours::white);

    float gainLevel = m_sourceRms.getGainLevel();

    auto drawRect = getLocalBounds().removeFromBottom((float)getHeight() * gainLevel);
    drawRect.reduce(2, 2);
    g.fillRect(drawRect);

    g.setColour(juce::Colour::fromRGB(0x20, 0x20, 0x20));
    g.drawRect(getLocalBounds().reduced(-1), 2);
}

//-------------------------------------------------------------------------------------------------------

void VuMeter::resized()
{
}

//-------------------------------------------------------------------------------------------------------

void VuMeter::timerCallback()
{
    repaint();
}

//-------------------------------------------------------------------------------------------------------
