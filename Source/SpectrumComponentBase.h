#pragma once

//-------------------------------------------------------------------------------------------------------

#include "OrangeGate.h"

#include "SpectrumProcessor.h"

//---------------------------------------------------------------------------------------------------

class SpectrumComponentBase
    : public juce::Component
{
private:
    SpectrumProcessor &processor;
    size_t m_fftSize;

    juce::dsp::WindowingFunction<float> m_windowingFunction;
    juce::dsp::FFT m_fft;

    std::vector<float> m_levels;

protected:
    const std::vector<float> &createResponseLevels();

public:
    explicit SpectrumComponentBase(SpectrumProcessor &p);

};

//---------------------------------------------------------------------------------------------------
