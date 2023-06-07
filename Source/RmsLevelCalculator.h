#pragma once

//-------------------------------------------------------------------------------------------------------

#include "OrangeGate.h"

#include "ProcessorInterface.h"

//-------------------------------------------------------------------------------------------------------

class RmsLevelCalculator : public GainProcessorInterface
{
private:
	//float m_level{ 0.0f };
	juce::SmoothedValue<float> m_level;

public:
	RmsLevelCalculator();
	void prepare(const juce::dsp::ProcessSpec &spec) override;
	void process(juce::AudioBuffer<float> &buffer) override;
	void reset() override;

	float getGainDb() override;
	float getGainLevel() override;
};

//-------------------------------------------------------------------------------------------------------
