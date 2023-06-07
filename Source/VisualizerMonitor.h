#pragma once

//-------------------------------------------------------------------------------------------------------

#include "OrangeGate.h"

#include <array>
#include <atomic>

#include "Visualizer.h"

//-------------------------------------------------------------------------------------------------------

class OrangeGateAudioProcessor;
class Visualizer;

//-------------------------------------------------------------------------------------------------------
// atomic stack for storing visualizer samples

class VisualizerQueue
{
private:
    const static int dataLength{ 100 };
    std::array<tVisualizerSample, dataLength> m_data;
    std::atomic<uint64_t> m_readIndex{ 0 };
    std::atomic<uint64_t> m_writeIndex{ 0 };

public:
    void reset();

    void push(const tVisualizerSample &sample);
    tVisualizerSample pop();
};

//-------------------------------------------------------------------------------------------------------

class VisualizerMonitor
{
private:
    std::atomic<Visualizer *> m_visualizer{ nullptr };
    bool isFirstRead{ true };
    VisualizerQueue m_queue;

    uint64_t m_processedSamples{ 0 };
    uint64_t m_pushedBuffers{ 0 };
    uint64_t m_samplesPerMs{ 1 };
    uint64_t m_sampleRate{ 0 }; // use this to correct samplesPerMs


    tVisualizerSample m_cacheSample{};

public:
    void attachVisualizer(Visualizer *visualizer);
    void unattachVisualizer();

    void setSampleRate(int sampleRate);

    // grab the section at ms since last grabbed
    tVisualizerSample getSectionWithLength(int ms);

    void pushBuffer(int bufferLength, float inDb, float outDb, float mul);
};

//-------------------------------------------------------------------------------------------------------
