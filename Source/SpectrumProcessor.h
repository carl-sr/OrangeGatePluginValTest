#pragma once

//-------------------------------------------------------------------------------------------------------

#include "OrangeGate.h"

#include <vector>

//-------------------------------------------------------------------------------------------------------

class SpectrumProcessor
{
private:
    std::vector<float> m_sampleQueue;
    std::atomic<uint64_t> m_atomicGuard;
    std::vector<float> m_dataBuffer;

    int m_fftOrder;

public:
    explicit SpectrumProcessor(int fftOrder);

    void prepare(const juce::dsp::ProcessSpec &spec);
    void process(juce::AudioBuffer<float> &buffer);

    int fftOrder();
    std::vector<float> &getData();
};

//-------------------------------------------------------------------------------------------------------
