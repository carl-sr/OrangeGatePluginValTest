#include "GateMultiplier.h"


//-------------------------------------------------------------------------------------------------------
// update attack / release intervals

void GateMultiplier::doMath()
{
    if (m_sampleRate == -1)
        return; // no sample rate, cannot calculate attack/release

    float samplesPerMs = static_cast<float>(m_sampleRate) / 1000.0f;
    float totalMultiplyRange = fabs(m_baseValue - m_targetValue);

    // attack interval
    uint64_t samplesInAttack = samplesPerMs * m_attackMs;

    if (samplesInAttack == 0)
        m_attackInterval = totalMultiplyRange;
    else
        m_attackInterval = totalMultiplyRange / static_cast<float>(samplesInAttack);

    if (m_targetValue < m_baseValue)
        m_attackInterval *= -1.0f;

    // release interval
    uint64_t samplesInRelease = samplesPerMs * m_releaseMs;

    if (samplesInRelease == 0)
        m_releaseInterval = totalMultiplyRange;
    else
        m_releaseInterval = totalMultiplyRange / static_cast<float>(samplesInRelease);

    if(m_baseValue < m_targetValue)
        m_releaseInterval *= -1.0f;
    
    internalSetState(m_internalState);
}

//-------------------------------------------------------------------------------------------------------
// make sure the current value is between baseValue and targetValue

void GateMultiplier::clipValue()
{
    if (m_currentValue < std::min(m_baseValue, m_targetValue))
        m_currentValue = std::min(m_baseValue, m_targetValue);

    else if (m_currentValue > std::max(m_baseValue, m_targetValue))
        m_currentValue = std::max(m_baseValue, m_targetValue);
}

//-------------------------------------------------------------------------------------------------------

void GateMultiplier::internalSetState(State s)
{
    m_internalState = s;

    if (m_internalState == State::Attack)
        m_activeInterval = m_attackInterval;
    else
        m_activeInterval = m_releaseInterval;
}

//-------------------------------------------------------------------------------------------------------

void GateMultiplier::setSampleRate(const int sampleRate)
{
    if (sampleRate <= 0)
    {
        jassertfalse; // sampleRate cannot be set to less than 0
        return;
    }
    m_sampleRate = sampleRate;
    doMath();
}

//-------------------------------------------------------------------------------------------------------

void GateMultiplier::setAttackMs(const uint64_t attackMs)
{
    if (m_attackMs != attackMs)
    {
        m_attackMs = attackMs;
        doMath();
    }
}

//-------------------------------------------------------------------------------------------------------

void GateMultiplier::setReleaseMs(const uint64_t releaseMs)
{
    if (m_releaseMs != releaseMs)
    {
        m_releaseMs = releaseMs;
        doMath();
    }
}

//-------------------------------------------------------------------------------------------------------

uint64_t GateMultiplier::getAttackMs()
{
    return m_attackMs;
}

//-------------------------------------------------------------------------------------------------------

uint64_t GateMultiplier::getReleaseMs()
{
    return m_releaseMs;
}

//-------------------------------------------------------------------------------------------------------

void GateMultiplier::setTargetValue(const float targetValue)
{
    m_targetValue = targetValue;
    clipValue();
    doMath();
}

//-------------------------------------------------------------------------------------------------------

void GateMultiplier::setBaseValue(const float baseValue)
{
    m_baseValue = baseValue;
    clipValue();
    doMath();
}

//-------------------------------------------------------------------------------------------------------

float GateMultiplier::getTargetValue() const
{
    return m_targetValue;
}

//-------------------------------------------------------------------------------------------------------

float GateMultiplier::getBaseValue() const
{
    return m_baseValue;
}

//-------------------------------------------------------------------------------------------------------

bool GateMultiplier::isAttack()
{
    return m_activeInterval == m_attackInterval;
}

//-------------------------------------------------------------------------------------------------------

void GateMultiplier::setState(State s)
{
    if (s != m_internalState)
        internalSetState(s);
}

//-------------------------------------------------------------------------------------------------------

bool GateMultiplier::isInAttack()
{
    return m_internalState == State::Attack;
}

//-------------------------------------------------------------------------------------------------------

bool GateMultiplier::isInRelease()
{
    return m_internalState == State::Release;
}

//-------------------------------------------------------------------------------------------------------
// is the multiplier at the end of its attack phase, and currently at the target level

bool GateMultiplier::isFullyAttacked()
{
    return isInAttack() && m_currentValue == m_targetValue;
}

//-------------------------------------------------------------------------------------------------------
// is the multiplier at the end of its release phase, and currently at the base level

bool GateMultiplier::isFullyReleased()
{
    return isInRelease() && m_currentValue == m_baseValue;
}

//-------------------------------------------------------------------------------------------------------

GateMultiplier::operator float() const
{
    return m_currentValue;
}

//-------------------------------------------------------------------------------------------------------
// Update the current value

void GateMultiplier::operator++(int)
{
    m_currentValue += m_activeInterval;
    clipValue();
}

//-------------------------------------------------------------------------------------------------------
