#include "GateProcessor.h"

#include "PluginProcessor.h"

//-------------------------------------------------------------------------------------------------------

GateProcessor::GateProcessor(OrangeGateAudioProcessor *processor)
	: ApvtsCallback(processor->Apvts)
	, m_processor(processor)
{
	addParameterSetter(Apvts::ThresholdDb, m_thresholdDb);
	addParameterCallback(Apvts::AttackMs, [this](float f) { m_gateMultiplier.setAttackMs((uint64_t)f); });
	addParameterCallback(Apvts::ReleaseMs, [this](float f) { m_gateMultiplier.setReleaseMs((uint64_t)f); });
}

//-------------------------------------------------------------------------------------------------------

void GateProcessor::prepare(const juce::dsp::ProcessSpec &spec)
{
	m_gateMultiplier.setSampleRate((int)spec.sampleRate);
}

//-------------------------------------------------------------------------------------------------------

void GateProcessor::process(const juce::AudioBuffer<float> &filterBuffer, juce::AudioBuffer<float> &outputBuffer)
{
	float rms = filterBuffer.getRMSLevel(0, 0, filterBuffer.getNumSamples()) + filterBuffer.getRMSLevel(1, 0, filterBuffer.getNumSamples());
	 
	float inputDb = juce::Decibels::gainToDecibels(rms / 2.0f);
	
	if (inputDb > m_thresholdDb)
		m_gateMultiplier.setState(GateMultiplier::State::Attack);
	else
		m_gateMultiplier.setState(GateMultiplier::State::Release);
	
	for (int c = 0; c < outputBuffer.getNumChannels(); c++)
	{
		for (int i = 0; i < outputBuffer.getNumSamples(); i++)
		{
			outputBuffer.getWritePointer(c)[i] *= (float)m_gateMultiplier;
		}
		m_gateMultiplier++;
	}
}

//-------------------------------------------------------------------------------------------------------

void GateProcessor::reset()
{
	jassertfalse;
}

//-------------------------------------------------------------------------------------------------------

float GateProcessor::getMultiplierValue()
{
	return (float)m_gateMultiplier;
}

//-------------------------------------------------------------------------------------------------------
