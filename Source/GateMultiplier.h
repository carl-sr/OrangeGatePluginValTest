#pragma once

//-------------------------------------------------------------------------------------------------------

#include "OrangeGate.h"

#include <cstdint>

//-------------------------------------------------------------------------------------------------------

class GateMultiplier
{
public:
	enum class State
	{
		Attack,
		Release
	};

private:
	float m_baseValue{ 0.0f }; // level starts here
	float m_targetValue{ 1.0f }; // is here after attack, returns to m_baseValue on call to setRelease()
	float m_currentValue{ m_baseValue };

	int m_sampleRate{ -1 };

	uint64_t m_attackMs{ 1 };
	uint64_t m_releaseMs{ 1 };

	float m_activeInterval{ 0.0f };
	float m_attackInterval{ 0.0f };
	float m_releaseInterval{ 0.0f };

	State m_internalState{ State::Release };
	
	void doMath();
	void clipValue();
	void internalSetState(State s);

public:
	GateMultiplier(const float baseValue, const float toValue) : m_baseValue(baseValue), m_targetValue(toValue) {}
	GateMultiplier() { GateMultiplier(0.0f, 1.0f); }

	void setSampleRate(const int sampleRate);

	void setAttackMs(const uint64_t attackMs);
	void setReleaseMs(const uint64_t releaseMs);

	uint64_t getAttackMs();
	uint64_t getReleaseMs();

	void setTargetValue(const float targetValue);
	void setBaseValue(const float baseValue);

	float getTargetValue() const;
	float getBaseValue() const;

	bool isAttack();

	void setState(State s);

	bool isInAttack();
	bool isInRelease();
	bool isFullyAttacked();
	bool isFullyReleased();

	explicit operator float() const;
	void operator++(int);
};

//-------------------------------------------------------------------------------------------------------
