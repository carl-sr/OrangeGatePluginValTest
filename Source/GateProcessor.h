#pragma once

//-------------------------------------------------------------------------------------------------------

#include "OrangeGate.h"

#include "ProcessorInterface.h"
#include "GateMultiplier.h"
#include "Apvts.h"

//-------------------------------------------------------------------------------------------------------

class OrangeGateAudioProcessor;

//-------------------------------------------------------------------------------------------------------

class GateProcessor
	: public AudioProcessorInterface
	, private ApvtsCallback
{
private:
	OrangeGateAudioProcessor *m_processor;

	GateMultiplier m_gateMultiplier;

	float m_thresholdDb{ 0.0f };

public:
	explicit GateProcessor(OrangeGateAudioProcessor *processor);

	void prepare(const juce::dsp::ProcessSpec &spec) override;
	void process(juce::AudioBuffer<float> &buffer) override { jassertfalse; };
	void process(const juce::AudioBuffer<float> &filterBuffer, juce::AudioBuffer<float> &outputBuffer);
	void reset() override;

	float getMultiplierValue();
};

//-------------------------------------------------------------------------------------------------------
