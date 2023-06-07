#include "AnimatedToggleButton.h"

#include "LookAndFeels.h"


//-------------------------------------------------------------------------------------------------------

AnimatedToggleButton::AnimatedToggleButton()
{
    m_button.onClick = [this]() {
        m_togglePos.setState(m_button.getToggleState() ? GateMultiplier::State::Attack : GateMultiplier::State::Release);
        startTimerHz(60);
        if (onToggled)
            onToggled(m_button.getToggleState());
    };

    m_togglePos.setAttackMs(60);
    m_togglePos.setReleaseMs(60);
    m_togglePos.setSampleRate(40);

    addAndMakeVisible(m_button);
    m_button.setAlpha(0.0f);
}

//-------------------------------------------------------------------------------------------------------

void AnimatedToggleButton::resized()
{
    m_button.setBounds(getLocalBounds());
}

//-------------------------------------------------------------------------------------------------------

void AnimatedToggleButton::paint(juce::Graphics &g)
{
    auto bounds = getLocalBounds();

    // rail
    auto railBounds = bounds.withTrimmedLeft(bounds.getWidth() / 2).reduced(0, bounds.getWidth() / 2);
    juce::Point<float> a = railBounds.getTopLeft().toFloat();
    juce::Point<float> b = railBounds.getBottomLeft().toFloat();
    
    juce::Path railPath;
    railPath.startNewSubPath(a);
    railPath.lineTo(b);
    
    g.setColour(Theme::BackPlateColor);
    g.strokePath(railPath, juce::PathStrokeType(bounds.getWidth(), juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    g.setColour(Theme::RailColor);
    g.strokePath(railPath, juce::PathStrokeType(bounds.getWidth() - 4, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    
    bounds.reduce(2, 0);
    
    
    // thumb
    float thumbPos = bounds.getHeight() - bounds.getWidth();
    thumbPos *= (float)m_togglePos;

    bounds.removeFromTop((int)thumbPos);
    bounds = bounds.withHeight(bounds.getWidth());


    
    g.setColour(Theme::RailColor);
    g.fillEllipse(bounds.toFloat());
    
    g.setColour(juce::Colours::white);
    bounds.reduce(1, 1);
    g.fillEllipse(bounds.toFloat());
}

//-------------------------------------------------------------------------------------------------------

void AnimatedToggleButton::timerCallback()
{
    repaint();
    m_togglePos++;
    if (m_togglePos.isFullyAttacked() || m_togglePos.isFullyReleased())
        stopTimer();
}

//-------------------------------------------------------------------------------------------------------
