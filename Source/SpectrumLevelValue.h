#pragma once

//-------------------------------------------------------------------------------------------------------

#include "OrangeGate.h"

#include "SpectrumProcessor.h"

//---------------------------------------------------------------------------------------------------

class SpectrumLevelValue
{
private:
    float m_value{ 0.0f };
    float m_targetValue{ 0.0f };

    float m_lastHoldLevel{ 0.0f };

    static bool m_isTransitionInitialized;
    static std::array<float, 20> m_transition;
    size_t m_transitionPos{ 0 };


public:
    SpectrumLevelValue();

    void setValue(float val);
    float getCurrentValue() const;
    float getNextValue();
};

//---------------------------------------------------------------------------------------------------
