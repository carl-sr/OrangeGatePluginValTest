#pragma once

//-------------------------------------------------------------------------------------------------------

#include "OrangeGate.h"

#include "AnimatedToggleButton.h"
#include "LookAndFeels.h"

//-------------------------------------------------------------------------------------------------------

class OrangeGateAudioProcessor;
class OrangeGateAudioProcessorEditor;

//-------------------------------------------------------------------------------------------------------

class ControlPanel
    : public juce::Component
    , public OrangeGateLookAndFeel
{
private:
    OrangeGateAudioProcessor *m_processor;

    juce::Slider m_thresholdSlider;
    juce::SliderParameterAttachment m_thresholdSliderAttachment;

    juce::Slider m_attackSlider;
    juce::SliderParameterAttachment m_attackSliderAttachment;

    juce::Slider m_releaseSlider;
    juce::SliderParameterAttachment m_releaseSliderAttachment;

    juce::Slider m_cutoffSlider;
    juce::SliderParameterAttachment m_cutoffSliderAttachment;

    juce::Slider m_resonanceSlider;
    juce::SliderParameterAttachment m_resonanceSliderAttachment;

    juce::Slider *m_activeTopSlider{ &m_attackSlider };
    juce::Slider *m_activeBottomSlider{ &m_releaseSlider };

    AnimatedToggleButton m_mode;

    void setControlPanelMode(bool b);

public:
    ControlPanel(OrangeGateAudioProcessor *processor, OrangeGateAudioProcessorEditor *editor);
    ~ControlPanel();

    void resized() override;
    void paint(juce::Graphics &g) override;

};

//-------------------------------------------------------------------------------------------------------
