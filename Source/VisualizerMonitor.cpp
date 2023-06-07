#include "VisualizerMonitor.h"

#include <algorithm>

#include "PluginProcessor.h"

//-------------------------------------------------------------------------------------------------------
// VisualizerQueue
//-------------------------------------------------------------------------------------------------------

void VisualizerQueue::reset()
{
    m_readIndex.store(0);
    m_writeIndex.store(0);
}

//-------------------------------------------------------------------------------------------------------

void VisualizerQueue::push(const tVisualizerSample &sample)
{
    if ((m_writeIndex - m_readIndex) >= dataLength) // overrun - samples are not being read as fast as they are being written
        m_readIndex = m_writeIndex - (dataLength / 2);

    m_data[m_writeIndex % dataLength] = sample;
    m_writeIndex++;
}

//-------------------------------------------------------------------------------------------------------

tVisualizerSample VisualizerQueue::pop()
{
    if (m_readIndex >= m_writeIndex) // underrun - samples are not being written as fast as they are being read
        m_readIndex = m_writeIndex - 1;

    tVisualizerSample ret = m_data[m_readIndex % dataLength];
    m_readIndex++;
    return ret;
}

//-------------------------------------------------------------------------------------------------------
// VisualizerMonitor
//-------------------------------------------------------------------------------------------------------

void VisualizerMonitor::attachVisualizer(Visualizer *visualizer)
{
    jassert(m_visualizer.load() == nullptr);
    m_queue.reset();

    m_processedSamples = 0;
    m_pushedBuffers = 0;
    isFirstRead = true;

    m_visualizer.store(visualizer);
}

//-------------------------------------------------------------------------------------------------------

void VisualizerMonitor::unattachVisualizer()
{
    jassert(m_visualizer.load() != nullptr);
    m_visualizer.store(nullptr);
}

//-------------------------------------------------------------------------------------------------------

void VisualizerMonitor::setSampleRate(int sampleRate)
{
    m_sampleRate = sampleRate; // how many samples in 1 ms of samples at this samplerate
    m_samplesPerMs = m_sampleRate / 1000;
}

//-------------------------------------------------------------------------------------------------------

tVisualizerSample VisualizerMonitor::getSectionWithLength(int ms)
{
    jassert(m_visualizer.load() != nullptr); // visualizer should exist here

    tVisualizerSample retSample{};

    while (ms-- > 0)
    {
        tVisualizerSample combineSample = m_queue.pop();
        retSample.inDb = std::max(combineSample.inDb, retSample.inDb);
        retSample.outDb = std::max(combineSample.outDb, retSample.outDb);
        retSample.mul = std::max(combineSample.mul, retSample.mul);
    }

    return retSample;
}

//-------------------------------------------------------------------------------------------------------

void VisualizerMonitor::pushBuffer(int bufferLength, float inDb, float outDb, float mul)
{
    if (!m_visualizer.load()) // no reason to do this if nothing is being visualized
        return;

    m_cacheSample.inDb = std::max(m_cacheSample.inDb, inDb);
    m_cacheSample.outDb = std::max(m_cacheSample.outDb, outDb);
    m_cacheSample.mul = std::max(m_cacheSample.mul, mul);

    m_processedSamples += bufferLength;

    // only actually push back a buffer if we have 1ms of recorded sample data
    if (m_processedSamples / m_samplesPerMs > m_pushedBuffers)
    {
        m_queue.push(m_cacheSample);
        m_pushedBuffers++;
        m_cacheSample = {};

        if (m_pushedBuffers == 1000) // 1 second - reset so that the time remains semi accurate
        {
            m_processedSamples = 0;
            m_pushedBuffers = 0;
        }
    }
}

//-------------------------------------------------------------------------------------------------------
