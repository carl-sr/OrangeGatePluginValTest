#include "PluginProcessor.h"
#include "PluginEditor.h"

//-------------------------------------------------------------------------------------------------------

OrangeGateAudioProcessorEditor::OrangeGateAudioProcessorEditor(OrangeGateAudioProcessor &p)
    : AudioProcessorEditor(&p)
    , audioProcessor(p)
    , m_inVuMeter(audioProcessor.getInRmsCalc())
    , m_outVuMeter(audioProcessor.getOutRmsCalc())
    , m_controlPanel(&audioProcessor, this)
    , m_visualizer(&audioProcessor, &audioProcessor.getVisualizerMonitor())
    , m_filterVisualizer(&audioProcessor)
{

    m_svg = juce::XmlDocument::parse(BinaryData::logo_svg);
    jassert(m_svg.get());
    m_logo = juce::Drawable::createFromSVG(*m_svg.get());
    jassert(m_logo.get());
    addAndMakeVisible(m_logo.get());

    setSize(800, 200);
    setResizable(true, false);
    setResizeLimits(600, 200, 1200, 200);

    addAndMakeVisible(m_backgroundImage, 0);

    addAndMakeVisible(m_header);

    addAndMakeVisible(m_inVuMeter);
    addAndMakeVisible(m_outVuMeter);
    addAndMakeVisible(m_controlPanel);

    addAndMakeVisible(m_visualizer);
    addChildComponent(m_filterVisualizer);

    addAndMakeVisible(m_openAboutBtn);
    m_openAboutBtn.setAlpha(0.0f);
    m_openAboutBtn.onClick = std::bind_front(&OrangeGateAudioProcessorEditor::openAboutPage, this);

    addAndMakeVisible(m_visualizerShadow);
    m_visualizerShadow.setInterceptsMouseClicks(false, false);

    setEditorMode(false);
}

//-------------------------------------------------------------------------------------------------------

OrangeGateAudioProcessorEditor::~OrangeGateAudioProcessorEditor()
{
}

//-------------------------------------------------------------------------------------------------------

void OrangeGateAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(Theme::BackgroundColor);
    g.setColour(juce::Colour(0xff, 0xff, 0xff));
}

//-------------------------------------------------------------------------------------------------------

void OrangeGateAudioProcessorEditor::paintStripe(juce::Graphics &g, juce::Rectangle<int> r, float offsetTop, float offsetBottom)
{
    juce::Path stripePath;

    auto topLeft = r.getTopLeft().toFloat();
    topLeft.addXY(offsetTop, 0);
    stripePath.startNewSubPath(topLeft);
    stripePath.lineTo(r.getTopRight().toFloat());
    stripePath.lineTo(r.getBottomRight().toFloat());

    auto bottomLeft = r.getBottomLeft().toFloat();
    bottomLeft.addXY(offsetBottom, 0);
    stripePath.lineTo(bottomLeft);
    stripePath.lineTo(topLeft);

    g.fillPath(stripePath);
}

//-------------------------------------------------------------------------------------------------------

void OrangeGateAudioProcessorEditor::openAboutPage()
{
    m_aboutPage = std::make_unique<AboutPage>();
    m_aboutPage->onCloseClicked = std::bind_front(&OrangeGateAudioProcessorEditor::closeAboutPage, this);
    addAndMakeVisible(m_aboutPage.get());
    resized();
    repaint();
}

//-------------------------------------------------------------------------------------------------------

void OrangeGateAudioProcessorEditor::closeAboutPage()
{
    // closeAboutPage() is called from the about page
    // stack needs to unwind before its destroyed
    startTimer(0);
}

//-------------------------------------------------------------------------------------------------------

void OrangeGateAudioProcessorEditor::resized()
{
    if (m_aboutPage)
    {
        m_aboutPage->setBounds(getLocalBounds());
        return;
    }

    auto localBounds = getLocalBounds();

    m_backgroundImage.setBounds(localBounds);

    const static int vuMeterWidth{ 10 };
    m_inVuMeter.setBounds(localBounds.removeFromLeft(vuMeterWidth));
    m_outVuMeter.setBounds(localBounds.removeFromRight(vuMeterWidth));
    
    localBounds.reduce(10, 10);
    
    m_header.setBounds(localBounds.removeFromLeft(150));
    
    localBounds.removeFromLeft(10);
    
    m_controlPanel.setBounds(localBounds.removeFromRight(110));
    
    m_visualizer.setBounds(localBounds);
    m_filterVisualizer.setBounds(localBounds);
    m_visualizerShadow.setBounds(localBounds);

    auto b = getLocalBounds().removeFromRight(50).removeFromBottom(40);
    b.removeFromTop(2);
    b.removeFromRight(vuMeterWidth + 5);
    b.removeFromBottom(17);
    m_logo->setBounds(b);
    m_openAboutBtn.setBounds(b);
}

//-------------------------------------------------------------------------------------------------------

void OrangeGateAudioProcessorEditor::setEditorMode(bool b)
{
    juce::MessageManagerLock mm;
    m_activeVisualizer->setVisible(false);
    if(b)
        m_activeVisualizer = &m_filterVisualizer;
    else
        m_activeVisualizer = &m_visualizer;
    m_activeVisualizer->setVisible(true);
}

//-------------------------------------------------------------------------------------------------------

void OrangeGateAudioProcessorEditor::timerCallback()
{
    m_aboutPage.reset();
    resized();
    stopTimer();
}

//-------------------------------------------------------------------------------------------------------
