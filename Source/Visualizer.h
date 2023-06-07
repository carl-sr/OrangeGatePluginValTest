#pragma once

//-------------------------------------------------------------------------------------------------------

#include "OrangeGate.h"

#include "LookAndFeels.h"

class OrangeGateAudioProcessor;
class VisualizerMonitor;

//-------------------------------------------------------------------------------------------------------

struct tVisualizerSample
{
    float inDb{ -60.0f };
    float outDb{ -60.0f };
    float mul{ 0.0f };
};

//-------------------------------------------------------------------------------------------------------

class Visualizer
    : public juce::Component
    , private juce::HighResolutionTimer
    , private juce::Timer
    , private VisualizerLookAndFeel
{
private:
    std::vector<tVisualizerSample> m_samples;

    int m_sampleCapacity{ 280 };

    int m_time{ 10 };
    int m_timeSet{ 10 };

    VisualizerMonitor *m_monitor;

    juce::ParameterAttachment m_thresholdDbAttachment;
    float m_thresholdDb{ -20.0f };

    void timerCallback() override;
    void hiResTimerCallback() override;

    const juce::Colour m_backgroundColor = juce::Colour::fromRGB(0x20, 0x20, 0x20);
    const juce::Colour m_inWaveformColor = juce::Colour::fromRGB(0x8f, 0x8f, 0x8f);
    const juce::Colour m_outWaveformColor = juce::Colour::fromRGB(0xcf, 0x62, 0x1d);
    const juce::Colour m_mulColor = juce::Colours::white;
    const juce::Colour m_thresholdColor = juce::Colours::white;

    juce::ToggleButton m_inToggle;
    juce::ToggleButton m_outToggle;
    juce::ToggleButton m_mulToggle;

    juce::Slider m_speed;

    OrangeGateAudioProcessor *m_processor;

public:
    Visualizer(OrangeGateAudioProcessor *processor, VisualizerMonitor *monitor);
    ~Visualizer();

    void resized() override;
    void paint(juce::Graphics &g) override;
};

//-------------------------------------------------------------------------------------------------------
