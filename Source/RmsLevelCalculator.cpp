#include "RmsLevelCalculator.h"

//-------------------------------------------------------------------------------------------------------

RmsLevelCalculator::RmsLevelCalculator()
{
    m_level.reset(30);
    m_level.setCurrentAndTargetValue(0.0f);
}

//-------------------------------------------------------------------------------------------------------

void RmsLevelCalculator::prepare(const juce::dsp::ProcessSpec &spec)
{
}

//-------------------------------------------------------------------------------------------------------

void RmsLevelCalculator::process(juce::AudioBuffer<float> &buffer)
{
    float level = buffer.getRMSLevel(0, 0, buffer.getNumSamples()) + buffer.getRMSLevel(1, 0, buffer.getNumSamples());
    if (level > m_level.getCurrentValue())
        m_level.setCurrentAndTargetValue(level);
    else
        m_level.setTargetValue(level);
}

//-------------------------------------------------------------------------------------------------------

void RmsLevelCalculator::reset()
{
}

//-------------------------------------------------------------------------------------------------------

float RmsLevelCalculator::getGainDb()
{
    return juce::Decibels::gainToDecibels(m_level.getTargetValue() / 2.0f);
}

//-------------------------------------------------------------------------------------------------------

float RmsLevelCalculator::getGainLevel()
{
    return m_level.getNextValue();
}

//-------------------------------------------------------------------------------------------------------
