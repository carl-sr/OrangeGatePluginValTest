#include "Filter.h"

#include "PluginProcessor.h"
#include "FilterView.h"

//-------------------------------------------------------------------------------------------------------
// Filter
//-------------------------------------------------------------------------------------------------------

void Filter::doCoefficients()
{
    FilterCoefficients coefficients;

    if (m_spec.sampleRate == 0.0f)
        return;

    switch (m_filterType)
    {
        case FilterType::LowPass:
            coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(m_spec.sampleRate, m_frequency, m_resonance);
            break;
        case FilterType::HighPass:
            coefficients = juce::dsp::IIR::Coefficients<float>::makeHighPass(m_spec.sampleRate, m_frequency, m_resonance);
            break;
        case FilterType::BandPass:
            coefficients = juce::dsp::IIR::Coefficients<float>::makeBandPass(m_spec.sampleRate, m_frequency, m_resonance);
            break;
        case FilterType::BandStop:
            coefficients = juce::dsp::IIR::Coefficients<float>::makeNotch(m_spec.sampleRate, m_frequency, m_resonance);
            break;
        default:
            jassertfalse;
    }

    m_leftFilter.get<0>().coefficients = coefficients;
    m_leftFilter.get<1>().coefficients = coefficients;
    m_leftFilter.get<2>().coefficients = coefficients;
    m_leftFilter.get<3>().coefficients = coefficients;

    m_rightFilter.get<0>().coefficients = coefficients;
    m_rightFilter.get<1>().coefficients = coefficients;
    m_rightFilter.get<2>().coefficients = coefficients;
    m_rightFilter.get<3>().coefficients = coefficients;

    m_visualizerCoefficients = coefficients;
    m_coefficientRevision++;
}

//-------------------------------------------------------------------------------------------------------

Filter::Filter(OrangeGateAudioProcessor &og)
    : ApvtsCallback(og.Apvts)
{
    addParameterSetter(Apvts::Cutoff, m_frequency);
    addParameterCallback(Apvts::Cutoff, [this](float) { doCoefficients(); });
    addParameterSetter(Apvts::Resonance, m_resonance);
    addParameterCallback(Apvts::Resonance, [this](float) { doCoefficients(); });
    addParameterCallback(Apvts::FilterType, [this](float val) {
        m_filterType = (FilterType)(int)val;
        doCoefficients();
    });

    doCoefficients();
}

//-------------------------------------------------------------------------------------------------------

void Filter::prepare(const juce::dsp::ProcessSpec &spec)
{
    m_spec = spec;
    doCoefficients();
}

//-------------------------------------------------------------------------------------------------------

void Filter::process(juce::AudioBuffer<float> &buffer)
{
    juce::dsp::AudioBlock<float> block(buffer);

    auto rBlock = block.getSingleChannelBlock(0);
    juce::dsp::ProcessContextReplacing<float> leftContext(rBlock);
    auto lBlock = block.getSingleChannelBlock(1);
    juce::dsp::ProcessContextReplacing<float> rightContext(lBlock);

    m_leftFilter.process(leftContext);
    m_rightFilter.process(rightContext);
}

//-------------------------------------------------------------------------------------------------------

void Filter::reset()
{
}

//-------------------------------------------------------------------------------------------------------

juce::ReferenceCountedObjectPtr<juce::dsp::IIR::Coefficients<float>> Filter::coefficients()
{
    return m_visualizerCoefficients;
}

//-------------------------------------------------------------------------------------------------------

double Filter::getSampleRate()
{
    return m_spec.sampleRate;
}

//-------------------------------------------------------------------------------------------------------

uint64_t Filter::getCoefficientRevision()
{
    return m_coefficientRevision;
}

//-------------------------------------------------------------------------------------------------------
