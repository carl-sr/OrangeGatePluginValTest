#include "ControlPanel.h"

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "LookAndFeels.h"

//-------------------------------------------------------------------------------------------------------

ControlPanel::ControlPanel(OrangeGateAudioProcessor *processor, OrangeGateAudioProcessorEditor *editor)
    : m_processor(processor)
    , m_thresholdSliderAttachment(*m_processor->Apvts.getParameter(Apvts::ThresholdDb), m_thresholdSlider)
    , m_attackSliderAttachment(*m_processor->Apvts.getParameter(Apvts::AttackMs), m_attackSlider)
    , m_releaseSliderAttachment(*m_processor->Apvts.getParameter(Apvts::ReleaseMs), m_releaseSlider)
    , m_cutoffSliderAttachment(*m_processor->Apvts.getParameter(Apvts::Cutoff), m_cutoffSlider)
    , m_resonanceSliderAttachment(*m_processor->Apvts.getParameter(Apvts::Resonance), m_resonanceSlider)
{
    addAndMakeVisible(m_thresholdSlider);
    m_thresholdSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    m_thresholdSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 0, 0);
    m_thresholdSlider.setLookAndFeel(this);

    addAndMakeVisible(m_attackSlider);
    m_attackSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    m_attackSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 0, 0);
    m_attackSlider.setName("Attack");
    m_attackSlider.setTextValueSuffix("ms");
    m_attackSlider.setLookAndFeel(this);
    m_attackSlider.setNumDecimalPlacesToDisplay(0);

    addAndMakeVisible(m_releaseSlider);
    m_releaseSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    m_releaseSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 0, 0);
    m_releaseSlider.setName("Release");
    m_releaseSlider.setTextValueSuffix("ms");
    m_releaseSlider.setLookAndFeel(this);
    m_releaseSlider.setNumDecimalPlacesToDisplay(0);

    addChildComponent(m_cutoffSlider);
    m_cutoffSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    m_cutoffSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 0, 0);
    m_cutoffSlider.setName("Cutoff");
    m_cutoffSlider.setTextValueSuffix("hz");
    m_cutoffSlider.setLookAndFeel(this);
    m_cutoffSlider.setNumDecimalPlacesToDisplay(0);

    addChildComponent(m_resonanceSlider);
    m_resonanceSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    m_resonanceSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, false, 0, 0);
    m_resonanceSlider.setName("Resonance");
    m_resonanceSlider.setLookAndFeel(this);
    m_resonanceSlider.setNumDecimalPlacesToDisplay(2);

    addAndMakeVisible(m_mode);
    m_mode.onToggled = [this, editor](bool b) {
        editor->setEditorMode(b);
        setControlPanelMode(b);
    };
}

//-------------------------------------------------------------------------------------------------------

ControlPanel::~ControlPanel()
{
}

//-------------------------------------------------------------------------------------------------------

void ControlPanel::resized()
{
    auto bounds = getLocalBounds();
    m_thresholdSlider.setBounds(bounds.removeFromLeft(20));
    m_mode.setBounds(bounds.removeFromLeft(10).withSizeKeepingCentre(10, 40).withWidth(18));

    bounds.removeFromLeft(10);
    bounds.removeFromBottom(25); // room for logo
    bounds.removeFromTop(25); // room for stripes
    auto topBounds = bounds.removeFromTop(bounds.getHeight() / 2);
    m_attackSlider.setBounds(topBounds);
    m_cutoffSlider.setBounds(topBounds);
    m_releaseSlider.setBounds(bounds);
    m_resonanceSlider.setBounds(bounds);
}

//-------------------------------------------------------------------------------------------------------

void ControlPanel::paint(juce::Graphics &g)
{
}

//-------------------------------------------------------------------------------------------------------

void ControlPanel::setControlPanelMode(bool b)
{
    juce::MessageManagerLock mm;
    m_activeTopSlider->setVisible(false);
    m_activeBottomSlider->setVisible(false);
    if (b)
    {
        m_activeTopSlider = &m_cutoffSlider;
        m_activeBottomSlider = &m_resonanceSlider;
    }
    else
    {
        m_activeTopSlider = &m_attackSlider;
        m_activeBottomSlider = &m_releaseSlider;
    }
    m_activeTopSlider->setVisible(true);
    m_activeBottomSlider->setVisible(true);
    m_thresholdSlider.setEnabled(!b);

    repaint();
}

//-------------------------------------------------------------------------------------------------------
