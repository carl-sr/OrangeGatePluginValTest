#pragma once

//-------------------------------------------------------------------------------------------------------

#include "OrangeGate.h"
#include "PluginProcessor.h"

#include "VuMeter.h"
#include "ControlPanel.h"
#include "Header.h"
#include "Visualizer.h"
#include "FilterVisualizer.h"
#include "BoxShadow.h"
#include "BackgroundImage.h"
#include "AboutPage.h"

//-------------------------------------------------------------------------------------------------------

class OrangeGateAudioProcessorEditor
    : public juce::AudioProcessorEditor
    , private juce::Timer
{
public:
    explicit OrangeGateAudioProcessorEditor(OrangeGateAudioProcessor &);
    ~OrangeGateAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void setEditorMode(bool b);

    void timerCallback() override;


private:
    void paintStripe(juce::Graphics &g, juce::Rectangle<int>, float offsetTop, float offsetBottom);

    std::unique_ptr<AboutPage> m_aboutPage;
    void openAboutPage();
    void closeAboutPage();

    OrangeGateAudioProcessor& audioProcessor;

    Header m_header;

    VuMeter m_inVuMeter;
    VuMeter m_outVuMeter;
    
    ControlPanel m_controlPanel;

    Visualizer m_visualizer;
    FilterVisualizer m_filterVisualizer;

    juce::Component *m_activeVisualizer{ &m_visualizer };

    std::unique_ptr<juce::XmlElement> m_svg;
    std::unique_ptr<juce::Drawable> m_logo;
    juce::TextButton m_openAboutBtn;

    BoxShadow m_visualizerShadow;

    BackgroundImage m_backgroundImage;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OrangeGateAudioProcessorEditor)
};

//-------------------------------------------------------------------------------------------------------
