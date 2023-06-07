#include "SpectrumComponentBase.h"


//---------------------------------------------------------------------------------------------------

const std::vector<float> &SpectrumComponentBase::createResponseLevels()
{
    std::vector<float> &data = processor.getData();

    m_windowingFunction.multiplyWithWindowingTable(data.data(), data.size());
    m_fft.performFrequencyOnlyForwardTransform(data.data());
    m_levels.clear();

    int bands = getWidth() * 2;
    for (int i = 0; i < bands; ++i)
    {
        float freqAtIndex = juce::mapToLog10((float)i / (float)bands, 20.0f, 20000.0f);
        float xSkew = juce::jmap(freqAtIndex, 20.0f, 20000.0f, 0.0f, 1.0f);

        size_t fftDataIndex = (size_t)juce::jlimit(0, (int)m_fftSize / 2, (int)(xSkew * (float)m_fftSize / 2));

        float levelScale = 1.0f - (float)i / (float)bands;
        if (levelScale < 0.01f)
            levelScale = 0.1f;

        float level = juce::jmap(data[fftDataIndex], 0.0f, 5.0f + (levelScale * 20000.0f), 0.0f, 1.0f);
        level *= 3.0f;
        m_levels.push_back(level);
    }

    return m_levels;
}

//---------------------------------------------------------------------------------------------------

SpectrumComponentBase::SpectrumComponentBase(SpectrumProcessor &p)
    : processor(p)
    , m_fftSize((size_t)0b1 << p.fftOrder())
    , m_windowingFunction(m_fftSize, juce::dsp::WindowingFunction<float>::WindowingMethod::blackmanHarris)
    , m_fft(p.fftOrder())
{
}

//-------------------------------------------------------------------------------------------------------
