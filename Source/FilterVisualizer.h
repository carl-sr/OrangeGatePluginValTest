#pragma once

//-------------------------------------------------------------------------------------------------------

#include "OrangeGate.h"

#include "FilterView.h"
#include "SpectrumComponent.h"
#include "LookAndFeels.h"

//-------------------------------------------------------------------------------------------------------

class OrangeGateAudioProcessor;

//-------------------------------------------------------------------------------------------------------

class FilterVisualizer
    : public juce::Component
    , private VisualizerLookAndFeel
{
private:
    const juce::Colour m_backgroundColor = juce::Colour::fromRGB(0x20, 0x20, 0x20);
    const juce::Colour m_inWaveformColor = juce::Colour::fromRGB(0x8f, 0x8f, 0x8f);
    const juce::Colour m_outWaveformColor = juce::Colour::fromRGB(0xcf, 0x62, 0x1d);
    const juce::Colour m_mulColor = juce::Colours::white;
    const juce::Colour m_thresholdColor = juce::Colours::white;

    juce::ToggleButton m_inToggle;
    juce::ToggleButton m_outToggle;
    juce::ToggleButton m_filterToggle;
    BlinkingToggleButton m_soloToggle;
    juce::ButtonParameterAttachment m_soloToggleAttachment;
    juce::ToggleButton m_enableToggle;
    juce::ButtonParameterAttachment m_enableToggleAttachment;

    juce::ComboBox m_filterTypeCbx;
    juce::ComboBoxParameterAttachment m_filterTypeCbxAttachment;

    FilterView m_filterView;

    DoubleSpectrumComponent m_gateSpectrum;
    SpectrumComponent m_filteredSpectrumComponent;

    void doToggleStates();

public:
    explicit FilterVisualizer(OrangeGateAudioProcessor *processor);

    void resized() override;
    void paint(juce::Graphics &g) override;
};

//-------------------------------------------------------------------------------------------------------
