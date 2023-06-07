#include "Apvts.h"

//-----------------------------------------------------------------------------

juce::AudioProcessorValueTreeState::ParameterLayout Apvts::getParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat> (AttackMs.toString(), "Attack", 1.0f, 500.0f, 200.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat> (ReleaseMs.toString(), "Release", 1.0f, 500.0f, 200.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat> (ThresholdDb.toString(), "Threshold", -60.0f, 6.0f, 6.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat> (Cutoff.toString(), "Cutoff", juce::NormalisableRange<float>(20.0f, 20000.0f, 1.0f, 0.25f), 20000.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat> (Resonance.toString(), "Resonance", 0.5f, 1.0f, 0.75f));
    layout.add(std::make_unique<juce::AudioParameterBool>  (FilterEnable.toString(), "FilterEnable", false));
    layout.add(std::make_unique<juce::AudioParameterBool>  (FilterSolo.toString(), "FilterSolo", false));
    layout.add(std::make_unique<juce::AudioParameterChoice>(FilterType.toString(), "FilterType", juce::StringArray({ "LowPass", "HighPass", "BandPass", "BandStop" }), 0));

    return layout;
}

//-----------------------------------------------------------------------------

void ApvtsCallback::parameterChanged(const juce::String &parameterID, float newValue)
{
    juce::Identifier paramId(parameterID);

    auto list = m_cbMp.find(paramId);

    if (list == m_cbMp.end())
        return;

    for (const auto &cb : list->second)
        cb(newValue);
}

//-----------------------------------------------------------------------------

ApvtsCallback::ApvtsCallback(juce::AudioProcessorValueTreeState &apvts)
    : m_apvts(apvts)
{
}

//-----------------------------------------------------------------------------

ApvtsCallback::~ApvtsCallback()
{
    for (const auto &[id, _] : m_cbMp)
        m_apvts.removeParameterListener(id, this);
}

//-----------------------------------------------------------------------------

void ApvtsCallback::addParameterCallback(const juce::Identifier &parameterId, const ParamCb &cb)
{
    if (!m_cbMp.contains(parameterId))
        m_apvts.addParameterListener(parameterId, this);

    auto param = m_apvts.getParameter(parameterId);
    auto val = param->getNormalisableRange().convertFrom0to1(param->getValue());
    cb(val);
    m_cbMp[parameterId].push_back(cb);
}

//-----------------------------------------------------------------------------

void ApvtsCallback::addParameterSetter(const juce::Identifier &parameterId, float &target)
{
    auto param = m_apvts.getParameter(parameterId);
    if (dynamic_cast<juce::AudioParameterFloat *>(param) == nullptr)
    {
        jassertfalse; // parameterId is not a float parameter
        return;
    }

    addParameterCallback(parameterId, [&target](float f) { target = f; });
}

//-----------------------------------------------------------------------------

void ApvtsCallback::addParameterSetter(const juce::Identifier &parameterId, int &target)
{
    auto param = m_apvts.getParameter(parameterId);
    if (dynamic_cast<juce::AudioParameterInt *>(param) == nullptr)
    {
        jassertfalse; // parameterId is not an int parameter
        return;
    }

    addParameterCallback(parameterId, [&target](float f) { target = (int)f; });
}

//-----------------------------------------------------------------------------

void ApvtsCallback::addParameterSetter(const juce::Identifier &parameterId, bool &target)
{
    auto param = m_apvts.getParameter(parameterId);
    if (dynamic_cast<juce::AudioParameterBool *>(param) == nullptr)
    {
        jassertfalse; // parameterId is not a bool parameter
        return;
    }

    addParameterCallback(parameterId, [&target](float f) { target = f != 0.0f; });
}

//-----------------------------------------------------------------------------

void ApvtsCallback::addParameterSetter(const juce::Identifier &parameterId, juce::SmoothedValue<float> &target)
{
    auto param = m_apvts.getParameter(parameterId);
    if (dynamic_cast<juce::AudioParameterFloat *>(param) == nullptr)
    {
        jassertfalse; // parameterId is not a float parameter
        return;
    }

    addParameterCallback(parameterId, [&target](float f) { target.setTargetValue(f); });
}

//-----------------------------------------------------------------------------
