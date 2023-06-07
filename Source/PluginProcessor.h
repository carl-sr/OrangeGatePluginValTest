#pragma once

//-------------------------------------------------------------------------------------------------------

#include "OrangeGate.h"

#include "RmsLevelCalculator.h"
#include "GateProcessor.h"
#include "VisualizerMonitor.h"
#include "Filter.h"
#include "SpectrumProcessor.h"

//-------------------------------------------------------------------------------------------------------

class OrangeGateAudioProcessor  : public juce::AudioProcessor
{
public:
    OrangeGateAudioProcessor();
    ~OrangeGateAudioProcessor() override;


    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    void innerProcessBlock(juce::AudioBuffer<float> &filterBuffer, juce::AudioBuffer<float> &outputBuffer);

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState Apvts;

    RmsLevelCalculator &getInRmsCalc();
    RmsLevelCalculator &getOutRmsCalc();

    VisualizerMonitor &getVisualizerMonitor();

    Filter &getFilter();

    SpectrumProcessor &getPreGateSpectrum();
    SpectrumProcessor &getPostGateSpectrum();
    SpectrumProcessor &getFilteredSpectrum();

private:
    RmsLevelCalculator m_inRmsLevel;
    RmsLevelCalculator m_outRmsLevel;
    RmsLevelCalculator m_filterRmsLevel;

    GateProcessor m_gate;

    Filter m_filter;
    SpectrumProcessor m_preGateSpectrum{ 13 };
    SpectrumProcessor m_postGateSpectrum{ 13 };
    SpectrumProcessor m_filteredSpectrum{ 13 };

    VisualizerMonitor m_visualizerMonitor;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OrangeGateAudioProcessor)
};

//-------------------------------------------------------------------------------------------------------
