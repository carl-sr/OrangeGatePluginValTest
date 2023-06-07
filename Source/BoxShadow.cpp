#include "BoxShadow.h"

//-------------------------------------------------------------------------------------------------------

void BoxShadow::resized()
{
}

//-------------------------------------------------------------------------------------------------------

void BoxShadow::paint(juce::Graphics &g)
{

    const static int stages = 5;
    
    float stageMul = 0.25f / (float)stages;
    
    for (int i = 0; i < stages; i++)
    {
        g.setColour(juce::Colour::fromRGB(0x20, 0x20, 0x20).withAlpha(stageMul * (stages - i)));
        g.drawRect(getLocalBounds(), i + 1);
    }

    g.setColour(juce::Colours::black);
    g.drawRect(getLocalBounds());
}

//-------------------------------------------------------------------------------------------------------
