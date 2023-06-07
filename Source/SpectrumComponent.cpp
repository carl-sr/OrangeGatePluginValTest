#include "SpectrumComponent.h"
#include "SpectrumProcessor.h"

//---------------------------------------------------------------------------------------------------
// SpectrumComponent
//---------------------------------------------------------------------------------------------------

SpectrumComponent::SpectrumComponent(SpectrumProcessor &p)
    : SpectrumComponentBase(p)
{
    startTimerHz(60);
}

//---------------------------------------------------------------------------------------------------

void SpectrumComponent::timerCallback()
{
    if (isEnabled())
        repaint();
}

//---------------------------------------------------------------------------------------------------

std::vector<SpectrumLevelValue> &SpectrumComponent::createSmoothedLevels()
{
    const std::vector<float> &curveLevels = createResponseLevels();
    if (curveLevels.size() != m_smoothedLevels.size())
    {
        m_smoothedLevels.assign(curveLevels.size(), {});
        return m_smoothedLevels;
    }

    // smooth level changes
    for (int i = 0; i < m_smoothedLevels.size(); i++) {
        float h = (float)getHeight();
        float updateLevel = (curveLevels[i] * h);
        float currentLevel = m_smoothedLevels[i].getCurrentValue();
        m_smoothedLevels[i].setValue(updateLevel);
    }

    return m_smoothedLevels;
}

//---------------------------------------------------------------------------------------------------

std::vector<float> &SpectrumComponent::createValues()
{
    std::vector<SpectrumLevelValue> &curveLevels = createSmoothedLevels();
    
    m_interpolatedValues.clear();
    m_interpolatedValues.reserve(curveLevels.size());
    for (auto &i : curveLevels)
        m_interpolatedValues.push_back(i.getNextValue());

    // smooth level transitions
    for (int j = 0; j < 4; j++)
    {
        // forward
        for (int i = 2; i < m_interpolatedValues.size(); i++)
        {
            float mid = midPoint(m_interpolatedValues[i], m_interpolatedValues[i - 2]);
            m_interpolatedValues[i - 1] = mid;
        }
        // backwards
        for (int i = m_interpolatedValues.size() - 1; i > 2; i--)
        {
            float mid = midPoint(m_interpolatedValues[i], m_interpolatedValues[i - 2]);
            m_interpolatedValues[i - 1] = mid;
        }
    }


    return m_interpolatedValues;
}

//---------------------------------------------------------------------------------------------------

float SpectrumComponent::midPoint(float a, float b)
{
    float distance = abs(a - b);
    distance /= 2.0f;
    if (a > b)
        return a - distance;
    else
        return a + distance;
}

//---------------------------------------------------------------------------------------------------

void SpectrumComponent::resized()
{
}

//---------------------------------------------------------------------------------------------------

void SpectrumComponent::paint(juce::Graphics &g)
{
    g.setColour(m_drawColour);

    std::vector<float> &interpolatedValues = createValues();

    juce::Path curvePath;
    curvePath.startNewSubPath({ 0.0f, (float)getHeight() });

    for (int i = 0; i < interpolatedValues.size(); i++)
    {
        const float val0to1{ (float)i / (float)interpolatedValues.size() };
        const float mul{ juce::jmap(val0to1, 0.0f, 5.0f) }; // 0.0 - 5.0 looks good
        const float x{ (float)i / 2.0f };
        const float y{ getHeight() - (interpolatedValues[i] * mul) };
        curvePath.lineTo({ x, y });
    }
    curvePath.lineTo({ (float)getWidth(), (float)getHeight()});

    g.fillPath(curvePath);
}

//---------------------------------------------------------------------------------------------------

void SpectrumComponent::setDrawColour(juce::Colour c)
{
    m_drawColour = c;
}

//---------------------------------------------------------------------------------------------------
// DoubleSpectrumComponent
//---------------------------------------------------------------------------------------------------

void DoubleSpectrumComponent::timerCallback()
{
    if (front.isEnabled() || back.isEnabled())
        repaint();
}

//---------------------------------------------------------------------------------------------------

DoubleSpectrumComponent::DoubleSpectrumComponent(SpectrumProcessor &frontSpectrum, SpectrumProcessor &backSpectrum)
    : front(frontSpectrum)
    , back(backSpectrum)
{
    addAndMakeVisible(front);
    addAndMakeVisible(back);

    front.stopTimer();
    back.stopTimer();

    startTimerHz(60);
}

//---------------------------------------------------------------------------------------------------

void DoubleSpectrumComponent::setFrontSpectrumVisible(bool visible)
{
    front.setVisible(visible);
    front.setEnabled(visible);
}

//---------------------------------------------------------------------------------------------------

void DoubleSpectrumComponent::setBackSpectrumVisible(bool visible)
{
    back.setVisible(visible);
    back.setEnabled(visible);
}

//---------------------------------------------------------------------------------------------------

void DoubleSpectrumComponent::resized()
{
    front.setBounds(getLocalBounds());
    back.setBounds(getLocalBounds());
}

//---------------------------------------------------------------------------------------------------
