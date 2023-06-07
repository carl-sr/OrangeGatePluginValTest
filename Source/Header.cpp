#include "Header.h"

#include "LookAndFeels.h"

//-------------------------------------------------------------------------------------------------------

void Header::timerCallback()
{
    m_activeBackgroundColour = m_transition[m_transitionIndex];
    ++m_transitionIndex;
    repaint();
    if (m_transitionIndex == m_transition.size())
        stopTimer();
}

//-------------------------------------------------------------------------------------------------------

Header::Header()
{
    m_image = juce::ImageFileFormat::loadFrom(BinaryData::header_png, BinaryData::header_pngSize);
    jassert(!m_image.isNull());
    m_imageComponent.setImage(m_image);

    addAndMakeVisible(m_imageComponent);

    addAndMakeVisible(m_boxShadow);

    addAndMakeVisible(m_trigger);
    m_trigger.setAlpha(0.0f);
    m_trigger.onClick = [this]() {
        m_transitionIndex = 0;
        startTimer(15);
    };

    // generate transition
    juce::Colour a = juce::Colour::fromRGB(0xff, 0xff, 0xff);
    juce::Colour b = juce::Colour::fromRGB(0x10, 0x10, 0x10);

    int size = m_transition.size();
    for (int i = 0; i < size; i++)
    {
        float aMul = 1.0f - i * (1.0f / (float)size);
        float bMul = i * (1.0f / (float)size);

        uint8_t rd = aMul * a.getRed() + bMul * b.getRed();
        uint8_t gr = aMul * a.getGreen() + bMul * b.getGreen();
        uint8_t bl = aMul * a.getBlue() + bMul * b.getBlue();

        m_transition[i] = { rd, gr, bl };
    }

    m_activeBackgroundColour = m_transition.back();
}

//-------------------------------------------------------------------------------------------------------

Header::~Header()
{
}

//-------------------------------------------------------------------------------------------------------

void Header::resized()
{
    m_imageComponent.setBounds(getLocalBounds());
    m_boxShadow.setBounds(getLocalBounds());
    m_trigger.setBounds(getLocalBounds());
}

//-------------------------------------------------------------------------------------------------------

void Header::paint(juce::Graphics &g)
{
    g.fillAll(m_activeBackgroundColour);
}

//-------------------------------------------------------------------------------------------------------
