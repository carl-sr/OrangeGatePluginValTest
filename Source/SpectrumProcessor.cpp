#include "SpectrumProcessor.h"

//-------------------------------------------------------------------------------------------------

SpectrumProcessor::SpectrumProcessor(int fftOrder)
    : m_sampleQueue(0b1 << fftOrder, 0.0f)
    , m_dataBuffer(0b1 << (fftOrder + 1), 0.0f)
    , m_fftOrder(fftOrder)
{
}

//-------------------------------------------------------------------------------------------------

void SpectrumProcessor::prepare(const juce::dsp::ProcessSpec &spec)
{
}

//-------------------------------------------------------------------------------------------------

void SpectrumProcessor::process(juce::AudioBuffer<float> &buffer)
{
    const float *data = buffer.getReadPointer(0);

    for (int i = 0; i < buffer.getNumSamples(); i++)
    {
        std::shift_left(m_sampleQueue.begin(), m_sampleQueue.end(), 1);
        m_sampleQueue.back() = data[i];
    }
}

//-------------------------------------------------------------------------------------------------

int SpectrumProcessor::fftOrder()
{
    return m_fftOrder;
}

//-------------------------------------------------------------------------------------------------

std::vector<float> &SpectrumProcessor::getData()
{
    std::copy(m_sampleQueue.begin(), m_sampleQueue.end(), m_dataBuffer.begin());
    return m_dataBuffer;
}

//-------------------------------------------------------------------------------------------------
