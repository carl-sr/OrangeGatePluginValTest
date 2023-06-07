#pragma once

//-------------------------------------------------------------------------------------------------------

#include "OrangeGate.h"

//-------------------------------------------------------------------------------------------------------

class AudioProcessorInterface
{
public:
	virtual ~AudioProcessorInterface() = default;
	virtual void prepare(const juce::dsp::ProcessSpec &spec) = 0;
	virtual void process(juce::AudioBuffer<float> &buffer) = 0;
	virtual void reset() = 0;
};

//-------------------------------------------------------------------------------------------------------

class GainProcessorInterface : public AudioProcessorInterface
{
public:
	virtual float getGainDb() = 0;
	virtual float getGainLevel() = 0;
};

//-------------------------------------------------------------------------------------------------------
