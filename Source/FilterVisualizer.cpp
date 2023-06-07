#include "FilterVisualizer.h"

#include "PluginProcessor.h"
#include "LookAndFeels.h"

//-------------------------------------------------------------------------------------------------------

void FilterVisualizer::doToggleStates()
{
    bool isEnabled = m_enableToggle.getToggleState();
    bool isSolo = m_soloToggle.getToggleState();

    m_inToggle.setEnabled(isEnabled && !isSolo);
    m_outToggle.setEnabled(isEnabled && !isSolo);
    m_filterToggle.setEnabled(isEnabled);

    m_soloToggle.setEnabled(isEnabled);
    m_filterTypeCbx.setEnabled(isEnabled);
    m_enableToggle.setButtonText(isEnabled ? "On" : "Off");

    bool front = m_inToggle.getToggleState();
    m_gateSpectrum.setFrontSpectrumVisible(isEnabled && !isSolo && front);
    bool back = m_outToggle.getToggleState();
    m_gateSpectrum.setBackSpectrumVisible(isEnabled && !isSolo && back);

    m_filteredSpectrumComponent.setEnabled(isEnabled && isSolo);
    m_filteredSpectrumComponent.setVisible(isEnabled && isSolo);

    m_filterView.setVisible(isEnabled && m_filterToggle.getToggleState());
}

//-------------------------------------------------------------------------------------------------------

FilterVisualizer::FilterVisualizer(OrangeGateAudioProcessor *processor)
    : m_soloToggleAttachment(*processor->Apvts.getParameter(Apvts::FilterSolo), m_soloToggle)
    , m_enableToggleAttachment(*processor->Apvts.getParameter(Apvts::FilterEnable), m_enableToggle)
    , m_filterTypeCbxAttachment(*processor->Apvts.getParameter(Apvts::FilterType), m_filterTypeCbx)
    , m_filterView(processor->getFilter())
    , m_gateSpectrum(processor->getPreGateSpectrum(), processor->getPostGateSpectrum())
    , m_filteredSpectrumComponent(processor->getFilteredSpectrum())

{
    m_inToggle.setButtonText("In");
    m_inToggle.setLookAndFeel(this);
    m_inToggle.setToggleState(true, juce::NotificationType::dontSendNotification);
    addAndMakeVisible(m_inToggle);
    m_inToggle.onClick = [this]() {
        doToggleStates();
    };

    m_outToggle.setButtonText("Out");
    m_outToggle.setLookAndFeel(this);
    m_outToggle.setToggleState(true, juce::NotificationType::dontSendNotification);
    addAndMakeVisible(m_outToggle);
    m_outToggle.onClick = [this]() {
        doToggleStates();
    };

    m_filterToggle.setButtonText("Filter");
    m_filterToggle.setLookAndFeel(this);
    m_filterToggle.setToggleState(true, juce::NotificationType::dontSendNotification);
    addAndMakeVisible(m_filterToggle);
    m_filterToggle.onClick = [this]() {
        doToggleStates();
    };

    m_soloToggle.setButtonText("Solo");
    addAndMakeVisible(m_soloToggle);

    m_soloToggle.onClick = [this]() {
        doToggleStates();
    };

    addAndMakeVisible(m_enableToggle);
    m_enableToggle.setLookAndFeel(this);

    m_enableToggle.onClick = [this]() {
        doToggleStates();
    };

    addAndMakeVisible(m_filterTypeCbx);
    m_filterTypeCbx.setLookAndFeel(this);
    auto comboParam = dynamic_cast<juce::AudioParameterChoice *>(processor->Apvts.getParameter("FilterType"));
    m_filterTypeCbx.addItemList(comboParam->getAllValueStrings(), 1);
    m_filterTypeCbx.setSelectedItemIndex(comboParam->getIndex());

    addAndMakeVisible(m_gateSpectrum);
    m_gateSpectrum.front.setDrawColour(juce::Colour::fromRGB(0x8f, 0x8f, 0x8f));
    m_gateSpectrum.back.setDrawColour(juce::Colour::fromRGB(0xcf, 0x62, 0x1d));

    addAndMakeVisible(m_filteredSpectrumComponent);
    m_filteredSpectrumComponent.setDrawColour(juce::Colours::white);

    addAndMakeVisible(m_filterView);
    
    doToggleStates();
}

//-------------------------------------------------------------------------------------------------------

void FilterVisualizer::resized()
{
    auto bounds = getLocalBounds();

    const static int buttonDim = 30;
    auto controlBounds = bounds.removeFromTop(15);
    m_inToggle.setBounds(controlBounds.removeFromLeft(buttonDim));
    m_outToggle.setBounds(controlBounds.removeFromLeft(buttonDim));
    m_filterToggle.setBounds(controlBounds.removeFromLeft(buttonDim));

    m_filterTypeCbx.setBounds(controlBounds.removeFromRight(100));
    m_soloToggle.setBounds(controlBounds.removeFromRight(buttonDim));
    m_enableToggle.setBounds(controlBounds.removeFromRight(buttonDim));

    m_filterView.setBounds(bounds);
    m_gateSpectrum.setBounds(bounds);
    m_filteredSpectrumComponent.setBounds(bounds);
}

//-------------------------------------------------------------------------------------------------------

void FilterVisualizer::paint(juce::Graphics &g)
{
    g.setColour(m_backgroundColor);
    g.fillAll();

    // outline
    g.setColour(m_backgroundColor);
    g.drawRect(getLocalBounds().reduced(-1), 2.0f);

    // fade from top to hide clash between threshold line and controls
    auto controlBounds = getLocalBounds().removeFromTop(30);
    g.fillRect(controlBounds.removeFromTop(15));
    g.setGradientFill(juce::ColourGradient(m_backgroundColor, controlBounds.getTopRight().toFloat(), m_backgroundColor.withAlpha(0.0f), controlBounds.getBottomRight().toFloat(), false));
    g.fillRect(controlBounds);
}

//-------------------------------------------------------------------------------------------------------
