#include "SpectrumLevelValue.h"

//---------------------------------------------------------------------------------------------------

bool SpectrumLevelValue::m_isTransitionInitialized{ false };
std::array<float, 20> SpectrumLevelValue::m_transition;

//---------------------------------------------------------------------------------------------------

SpectrumLevelValue::SpectrumLevelValue()
{
    if (m_isTransitionInitialized)
        return;

    float inc = 1.0f / (float)m_transition.size();
    for (int i = 0; i < m_transition.size(); i++)
    {
        float x = inc * i;
        m_transition[i] = sqrt(1.0f - (x * x));
    }
    m_isTransitionInitialized = true;
}

//---------------------------------------------------------------------------------------------------

void SpectrumLevelValue::setValue(float val) {
    m_targetValue = val;
    if (m_targetValue > m_value)
    {
        m_value = m_targetValue;
        m_lastHoldLevel = m_value;
        m_transitionPos = 0;
    }
}

//---------------------------------------------------------------------------------------------------

float SpectrumLevelValue::getCurrentValue() const
{
    return m_value;
}

//---------------------------------------------------------------------------------------------------

float SpectrumLevelValue::getNextValue() {
    if (m_transitionPos >= m_transition.size())
        return 0.0f;

    m_value = m_lastHoldLevel * m_transition[m_transitionPos];
    ++m_transitionPos;

    if (m_targetValue > m_value)
    {
        m_value = m_targetValue;
        m_lastHoldLevel = m_value;
        m_transitionPos = 0;
    }
    return m_value;
}

//---------------------------------------------------------------------------------------------------
