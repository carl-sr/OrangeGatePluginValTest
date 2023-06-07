#pragma once

//-------------------------------------------------------------------------------------------------------

#include "OrangeGate.h"

#include <mutex>

#include "ProcessorInterface.h"
#include "Apvts.h"

//-------------------------------------------------------------------------------------------------------

class OrangeGateAudioProcessor;
class FilterView;

//-------------------------------------------------------------------------------------------------------

enum class FilterType {
    LowPass = 0,
    HighPass = 1,
    BandPass = 2,
    BandStop = 3
};

//-------------------------------------------------------------------------------------------------------

class Filter
    : public AudioProcessorInterface
    , private ApvtsCallback
{
private:
    using FilterCoefficients = juce::ReferenceCountedObjectPtr<juce::dsp::IIR::Coefficients<float>>;

    float m_frequency{ 1.0f };
    float m_resonance{ 1.0f };
    FilterType m_filterType{ FilterType::LowPass };

    juce::dsp::ProcessSpec m_spec{};

    using IIRFilter = juce::dsp::IIR::Filter<float>;

    using MonoChain = juce::dsp::ProcessorChain <IIRFilter, IIRFilter, IIRFilter, IIRFilter>;
    MonoChain m_leftFilter;
    MonoChain m_rightFilter;
    FilterCoefficients m_visualizerCoefficients;

    uint64_t m_coefficientRevision{ 0 };

    void doCoefficients();

public:
    Filter(OrangeGateAudioProcessor &og);

    void prepare(const juce::dsp::ProcessSpec &spec) override;
    void process(juce::AudioBuffer<float> &buffer) override;
    void reset() override;

    juce::ReferenceCountedObjectPtr<juce::dsp::IIR::Coefficients<float>> coefficients();
    double getSampleRate();

    uint64_t getCoefficientRevision();
};

//-------------------------------------------------------------------------------------------------------
