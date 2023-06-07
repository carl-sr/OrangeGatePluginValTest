#pragma once

//-----------------------------------------------------------------------------

#include "OrangeGate.h"

#include <functional>

//-----------------------------------------------------------------------------

class Apvts
{
public:
    static juce::AudioProcessorValueTreeState::ParameterLayout getParameterLayout();

    inline static juce::Identifier AttackMs{ "AttackMs" };
    inline static juce::Identifier ReleaseMs{ "ReleaseMs" };
    inline static juce::Identifier ThresholdDb{ "ThresholdDb" };
    inline static juce::Identifier Cutoff{ "Cutoff" };
    inline static juce::Identifier Resonance{ "Resonance" };
    inline static juce::Identifier FilterEnable{ "FilterEnable" };
    inline static juce::Identifier FilterSolo{ "FilterSolo" };
    inline static juce::Identifier FilterType{ "FilterType" };
};

//-----------------------------------------------------------------------------

using ParamCb = std::function<void(float)>;
using ParamCbList = std::vector<ParamCb>;
using ParamCbListMap = std::map<juce::Identifier, ParamCbList>;

//-----------------------------------------------------------------------------
// Call arbitrary functions whenever an Apvts value changes
//-----------------------------------------------------------------------------

class ApvtsCallback : private juce::AudioProcessorValueTreeState::Listener
{
private:
    void parameterChanged(const juce::String &parameterID, float newValue) override;
    juce::AudioProcessorValueTreeState &m_apvts;

    ParamCbListMap m_cbMp;

public:
    explicit ApvtsCallback(juce::AudioProcessorValueTreeState &apvts);
    ~ApvtsCallback() override;

    void addParameterCallback(const juce::Identifier &parameterId, const ParamCb &cb);

    void addParameterSetter(const juce::Identifier &parameterId, float &target);
    void addParameterSetter(const juce::Identifier &parameterId, int &target);
    void addParameterSetter(const juce::Identifier &parameterId, bool &target);
    void addParameterSetter(const juce::Identifier &parameterId, juce::SmoothedValue<float> &target);
};

//-----------------------------------------------------------------------------
