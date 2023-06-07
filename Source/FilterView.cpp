#include "FilterView.h"

//-------------------------------------------------------------------------------------------------------

void FilterView::timerCallback()
{
    if (m_coefficientRevision != m_sourceFilter.getCoefficientRevision())
    {
        m_coefficientRevision = m_sourceFilter.getCoefficientRevision();
        resized();
    }
}

//-------------------------------------------------------------------------------------------------------

FilterView::FilterView(Filter &filter)
    : m_sourceFilter(filter)
{
    startTimerHz(30);
}

//-------------------------------------------------------------------------------------------------------

void FilterView::generateCurve()
{
    std::vector<float> mags;
    auto coefficients = m_sourceFilter.coefficients().get();
    if (!coefficients)
        return;

    mags.resize(getWidth());

    for (int i = 0; i < getWidth(); ++i)
    {
        float mag = 1.0f;
        auto freq = juce::mapToLog10((float)i / (float)getWidth(), 20.0f, 20000.0f);

        mag *= coefficients->getMagnitudeForFrequency(freq, m_sourceFilter.getSampleRate());

        mags[i] = juce::Decibels::gainToDecibels(mag);
    }

    const static float curveHeight = 50.0f;
    for (auto &i : mags)
    {
        i *= curveHeight;
        i = getHeight() - i - curveHeight - 50.0f;
    }

    m_curvePath.clear();
    m_curvePath.startNewSubPath(juce::Point<float>(0.0f, getHeight()));

    for (int i = 0; i < mags.size(); i++)
    {
        m_curvePath.lineTo(juce::Point<float>(i, mags[i]));
    }
    m_curvePath.lineTo(juce::Point<float>(getWidth(), getHeight()));

    repaint();
}

//-------------------------------------------------------------------------------------------------------

void FilterView::resized()
{
    generateCurve();
}

//-------------------------------------------------------------------------------------------------------

void FilterView::paint(juce::Graphics &g)
{
    g.setColour(juce::Colours::white.withAlpha(0.2f));
    g.fillPath(m_curvePath);

    g.setColour(juce::Colours::white);
    g.strokePath(m_curvePath, juce::PathStrokeType(0.5f, juce::PathStrokeType::JointStyle::curved));
}

//-------------------------------------------------------------------------------------------------------
