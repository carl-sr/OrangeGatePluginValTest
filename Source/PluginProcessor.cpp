#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Apvts.h"

//-------------------------------------------------------------------------------------------------------

OrangeGateAudioProcessor::OrangeGateAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
    , Apvts(*this, nullptr, "Parameters", Apvts::getParameterLayout())
    , m_gate(this)
    , m_filter(*this)
{
}

//-------------------------------------------------------------------------------------------------------

OrangeGateAudioProcessor::~OrangeGateAudioProcessor()
{
}

//-------------------------------------------------------------------------------------------------------

const juce::String OrangeGateAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

//-------------------------------------------------------------------------------------------------------

bool OrangeGateAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

//-------------------------------------------------------------------------------------------------------

bool OrangeGateAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

//-------------------------------------------------------------------------------------------------------

bool OrangeGateAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

//-------------------------------------------------------------------------------------------------------

double OrangeGateAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

//-------------------------------------------------------------------------------------------------------

int OrangeGateAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

//-------------------------------------------------------------------------------------------------------

int OrangeGateAudioProcessor::getCurrentProgram()
{
    return 0;
}

//-------------------------------------------------------------------------------------------------------

void OrangeGateAudioProcessor::setCurrentProgram (int index)
{
}

//-------------------------------------------------------------------------------------------------------

const juce::String OrangeGateAudioProcessor::getProgramName (int index)
{
    return {};
}

//-------------------------------------------------------------------------------------------------------

void OrangeGateAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//-------------------------------------------------------------------------------------------------------

void OrangeGateAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getNumInputChannels();
    spec.sampleRate = sampleRate;

    m_inRmsLevel.prepare(spec);
    m_outRmsLevel.prepare(spec);
    m_filterRmsLevel.prepare(spec);

    m_gate.prepare(spec);

    m_visualizerMonitor.setSampleRate(spec.sampleRate);

    m_filter.prepare(spec);
}

//-------------------------------------------------------------------------------------------------------

void OrangeGateAudioProcessor::releaseResources()
{
}

//-------------------------------------------------------------------------------------------------------

#ifndef JucePlugin_PreferredChannelConfigurations
bool OrangeGateAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

//-------------------------------------------------------------------------------------------------------

void OrangeGateAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;

    static const int bufferLength{ 32 }; // process this many samples at once

    bool filterEnabled = Apvts.getRawParameterValue("FilterEnable")->load() == 1.0f;
    bool filterSoloed = Apvts.getRawParameterValue("FilterSolo")->load() == 1.0f;

    if (filterEnabled && filterSoloed)
    {
        m_inRmsLevel.process(buffer);
        m_filter.process(buffer);
        m_filteredSpectrum.process(buffer);
        m_outRmsLevel.process(buffer);
        return;
    }

    // filterBuffer is what drives the gate, if the filter is enabled it is a copy of buffer data with m_filter applied.
    // if filter is disabled it is a reference to unaltered buffer data
    juce::AudioBuffer<float> filterBuffer;
    
    if (filterEnabled)
    {
        filterBuffer = buffer;
        m_filter.process(filterBuffer);
    }
    else
    {
        filterBuffer.setDataToReferTo(buffer.getArrayOfWritePointers(), buffer.getNumChannels(), buffer.getNumSamples());
    }

    auto bufferData = (float *const *)buffer.getArrayOfReadPointers();
    auto FilterBufferdata = (float *const *)filterBuffer.getArrayOfReadPointers();

    int i = 0;
    for (; i < (buffer.getNumSamples() / bufferLength) - 1; i++)
    {
        juce::AudioBuffer<float> subBufferData(bufferData, buffer.getNumChannels(), i * bufferLength, bufferLength);
        juce::AudioBuffer<float> subfilterBufferData(FilterBufferdata, buffer.getNumChannels(), i * bufferLength, bufferLength);

        innerProcessBlock(subfilterBufferData, subBufferData);
    }

    // last buffer 
    juce::AudioBuffer<float> subBufferData(bufferData, buffer.getNumChannels(), i * bufferLength, buffer.getNumSamples() - (i * bufferLength));
    juce::AudioBuffer<float> subfilterBufferData(FilterBufferdata, buffer.getNumChannels(), i * bufferLength, buffer.getNumSamples() - (i * bufferLength));
    innerProcessBlock(subfilterBufferData, subBufferData);
}

//-------------------------------------------------------------------------------------------------------

void OrangeGateAudioProcessor::innerProcessBlock(juce::AudioBuffer<float> &filterBuffer, juce::AudioBuffer<float> &outputBuffer)
{
    m_filterRmsLevel.process(filterBuffer);

    m_inRmsLevel.process(outputBuffer);
    m_preGateSpectrum.process(outputBuffer);
    m_gate.process(filterBuffer, outputBuffer);
    m_outRmsLevel.process(outputBuffer);

    m_filteredSpectrum.process(filterBuffer);
    m_postGateSpectrum.process(outputBuffer);

    m_visualizerMonitor.pushBuffer(outputBuffer.getNumSamples(), m_filterRmsLevel.getGainDb(), m_outRmsLevel.getGainDb(), m_gate.getMultiplierValue());
}

//-------------------------------------------------------------------------------------------------------

bool OrangeGateAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* OrangeGateAudioProcessor::createEditor()
{
    return new OrangeGateAudioProcessorEditor(*this);
}

//-------------------------------------------------------------------------------------------------------
void OrangeGateAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream mos(destData, true);
    Apvts.state.writeToStream(mos);
}

//-------------------------------------------------------------------------------------------------------

void OrangeGateAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    auto tree = juce::ValueTree::readFromData(data, sizeInBytes);
    if (tree.isValid())
    {
        Apvts.replaceState(tree);
    }
}

//-------------------------------------------------------------------------------------------------------

RmsLevelCalculator &OrangeGateAudioProcessor::getInRmsCalc()
{
    return m_inRmsLevel;
}

//-------------------------------------------------------------------------------------------------------

RmsLevelCalculator &OrangeGateAudioProcessor::getOutRmsCalc()
{
    return m_outRmsLevel;
}

//-------------------------------------------------------------------------------------------------------

VisualizerMonitor &OrangeGateAudioProcessor::getVisualizerMonitor()
{
    return m_visualizerMonitor;
}

//-------------------------------------------------------------------------------------------------------

Filter &OrangeGateAudioProcessor::getFilter()
{
    return m_filter;
}

//-------------------------------------------------------------------------------------------------------

SpectrumProcessor &OrangeGateAudioProcessor::getPreGateSpectrum()
{
    return m_preGateSpectrum;
}

//-------------------------------------------------------------------------------------------------------

SpectrumProcessor &OrangeGateAudioProcessor::getPostGateSpectrum()
{
    return m_postGateSpectrum;
}

//-------------------------------------------------------------------------------------------------------

SpectrumProcessor &OrangeGateAudioProcessor::getFilteredSpectrum()
{
    return m_filteredSpectrum;
}

//-------------------------------------------------------------------------------------------------------

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new OrangeGateAudioProcessor();
}

//-------------------------------------------------------------------------------------------------------
