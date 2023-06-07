#include "Visualizer.h"
#include "VisualizerMonitor.h"

#include "PluginProcessor.h"
#include "LookAndFeels.h"

//-------------------------------------------------------------------------------------------------------

void Visualizer::timerCallback()
{
    repaint();
}

//-------------------------------------------------------------------------------------------------------

void Visualizer::hiResTimerCallback()
{
    bool filterSoloed = m_processor->Apvts.getRawParameterValue("FilterSolo")->load() == 1.0f;
    bool filterEnabled = m_processor->Apvts.getRawParameterValue("FilterEnable")->load() == 1.0f;

    if (filterSoloed && filterEnabled)
        return;

    tVisualizerSample smp = m_monitor->getSectionWithLength(juce::HighResolutionTimer::getTimerInterval());
    m_samples.push_back(smp);

    if (m_samples.size() > m_sampleCapacity)
        m_samples.erase(m_samples.begin());


    if (m_timeSet != m_time)
    {
        juce::HighResolutionTimer::stopTimer();
        m_time = m_timeSet;
        juce::HighResolutionTimer::startTimer(m_time);
    }
}

//-------------------------------------------------------------------------------------------------------

Visualizer::Visualizer(OrangeGateAudioProcessor *processor, VisualizerMonitor *monitor)
    : m_monitor(monitor)
    , m_thresholdDbAttachment(*processor->Apvts.getParameter(Apvts::ThresholdDb), [this](float f) { m_thresholdDb = f; })
    , m_thresholdDb(processor->Apvts.getParameter(Apvts::ThresholdDb)->getValue())
    , m_processor(processor)
{
    m_monitor->attachVisualizer(this);
    
    m_thresholdDb = juce::jmap(m_thresholdDb, -60.0f, 6.0f);
    m_thresholdDb = std::max(-60.0f, std::min(m_thresholdDb, 6.0f));

    m_samples = std::vector<tVisualizerSample>(m_sampleCapacity);
    
    m_inToggle.setButtonText("In");
    m_inToggle.setLookAndFeel(this);
    m_inToggle.setToggleState(true, juce::NotificationType::dontSendNotification);
    addAndMakeVisible(m_inToggle);
    
    m_outToggle.setButtonText("Out");
    m_outToggle.setLookAndFeel(this);
    m_outToggle.setToggleState(true, juce::NotificationType::dontSendNotification);
    addAndMakeVisible(m_outToggle);
    
    m_mulToggle.setButtonText("Gate");
    m_mulToggle.setLookAndFeel(this);
    m_mulToggle.setToggleState(true, juce::NotificationType::dontSendNotification);
    addAndMakeVisible(m_mulToggle);
    
    m_speed.setRange(5.0f, 15.0f);
    m_speed.setLookAndFeel(this);
    m_speed.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox,false, 0, 0);
    addAndMakeVisible(m_speed);
    m_speed.onValueChange = [this]() {
        double invertVal = m_speed.getRange().getEnd() - m_speed.getValue() + 5.0f;
        m_timeSet = (int)invertVal;
    };
    m_speed.setValue(7.0f);

    juce::HighResolutionTimer::startTimer(m_time);
    juce::Timer::startTimerHz(60);
}

//-------------------------------------------------------------------------------------------------------

Visualizer::~Visualizer()
{
    juce::HighResolutionTimer::stopTimer();
    juce::Timer::stopTimer();

    m_monitor->unattachVisualizer();
}

//-------------------------------------------------------------------------------------------------------

void Visualizer::resized()
{
    m_sampleCapacity = getWidth();
    if (m_sampleCapacity > m_samples.size())
    {
        int samplesToInsert = m_sampleCapacity - m_samples.size();
        m_samples.insert(m_samples.begin(), samplesToInsert, {});
    }
    else if (m_sampleCapacity < m_samples.size())
    {
        int samplesToErase = m_samples.size() - m_sampleCapacity;
        m_samples.erase(m_samples.begin(), m_samples.begin() + samplesToErase);
    }

    const static int buttonDim = 30;
    auto controlBounds = getLocalBounds().removeFromTop(15);
    m_inToggle.setBounds(controlBounds.removeFromLeft(buttonDim));
    m_outToggle.setBounds(controlBounds.removeFromLeft(buttonDim));
    m_mulToggle.setBounds(controlBounds.removeFromLeft(buttonDim));
    m_speed.setBounds(controlBounds.reduced(4, 0));
}

//-------------------------------------------------------------------------------------------------------

void Visualizer::paint(juce::Graphics &g)
{
    g.setColour(m_backgroundColor);
    g.fillAll();
    bool filterSoloed = m_processor->Apvts.getRawParameterValue("FilterSolo")->load() == 1.0f;
    bool filterEnabled = m_processor->Apvts.getRawParameterValue("FilterEnable")->load() == 1.0f;

    if (filterSoloed && filterEnabled)
    {
        g.setColour(juce::Colours::white);
        g.setFont(10);
        g.drawText("Gate unavailable while input filter solo is active.", getLocalBounds(), juce::Justification::centred);
        return;
    }

    juce::Path inPath, outPath, mulPath;
    juce::Point<float> bottomLeft = getLocalBounds().getBottomLeft().toFloat();
    juce::Point<float> bottomRight = getLocalBounds().getBottomRight().toFloat();

    inPath.startNewSubPath(bottomLeft);
    outPath.startNewSubPath(bottomLeft);
    mulPath.startNewSubPath({ (float)0, getHeight() - (m_samples[0].mul) * 50.0f });

    int i = 0;
    for (const auto &a : m_samples)
    {
        float inDb = juce::jmap(a.inDb, -60.0f, 6.0f, 0.0f, 1.0f);
        float outDb = juce::jmap(a.outDb, -60.0f, 6.0f, 0.0f, 1.0f);

        inPath.lineTo({ (float)i, (1.0f - inDb) * getHeight() });
        outPath.lineTo({ (float)i, (1.0f - outDb) * getHeight() });
        mulPath.lineTo({ (float)i, getHeight() - (a.mul) * 50.0f });
        i++;
    }

    inPath.lineTo(bottomRight);
    inPath.lineTo(bottomLeft);

    outPath.lineTo(bottomRight);
    outPath.lineTo(bottomLeft);


    if (m_inToggle.getToggleState())
    {
        g.setColour(m_inWaveformColor);
        g.fillPath(inPath);
    }

    if (m_outToggle.getToggleState())
    {
        g.setColour(m_outWaveformColor);
        g.fillPath(outPath);
    }

    if (m_mulToggle.getToggleState())
    {
        g.setColour(m_mulColor);
        g.strokePath(mulPath, juce::PathStrokeType(1.0f, juce::PathStrokeType::JointStyle::curved));
    }

    // threshold line
    float thresholdLevel = juce::jmap(m_thresholdDb, -60.0f, 6.0f, 0.0f, 1.0f);
    g.setColour(m_thresholdColor);
    float lineHeight = (1.0f - thresholdLevel) * getHeight();
    float floatInterval[]{ 2.0f, 10.0f };
    g.drawDashedLine(juce::Line<float>(0, lineHeight, getWidth(), lineHeight), &floatInterval[0], 2);

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
