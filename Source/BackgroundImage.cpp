#include "BackgroundImage.h"

//-------------------------------------------------------------------------------------------------------

BackgroundImage::BackgroundImage()
{
    m_overlayRect.setFill(juce::Colours::black.withAlpha(0.2f));
    addAndMakeVisible(m_overlayRect);

    m_image = juce::ImageFileFormat::loadFrom(BinaryData::background_png, BinaryData::background_pngSize);
    setPaintStripes(true);
}

//-------------------------------------------------------------------------------------------------------

void BackgroundImage::setPaintStripes(bool shouldPaintStripes)
{
    if (shouldPaintStripes)
        addAndMakeVisible(m_stripes);
    else
        removeChildComponent(&m_stripes);
}

//-------------------------------------------------------------------------------------------------------

void BackgroundImage::paint(juce::Graphics &g)
{
    g.drawImage(m_image, getLocalBounds().toFloat(), juce::RectanglePlacement::xRight | juce::RectanglePlacement::doNotResize, false);

    g.setColour(juce::Colour::fromRGB(0x20, 0x20, 0x20));
    g.drawRect(getLocalBounds().toFloat());
}

//-------------------------------------------------------------------------------------------------------

void BackgroundImage::resized()
{
    m_overlayRect.setRectangle(getLocalBounds().expanded(1, 1).toFloat());
    m_stripes.setBounds(getLocalBounds());
}

//-------------------------------------------------------------------------------------------------------

void Stripes::paint(juce::Graphics &g)
{

    g.setColour(juce::Colours::white);

    auto stripeBounds = getLocalBounds().removeFromRight(100);
    stripeBounds.removeFromTop(10);
    stripeBounds.removeFromBottom(10);

    const static float offset = 10.0f;

    paintStripe(g, stripeBounds.removeFromTop(5), 0.0f, offset);
    paintStripe(g, stripeBounds.removeFromBottom(5), offset, 0.0f);
    stripeBounds.removeFromTop(10);
    stripeBounds.removeFromLeft(10);
    paintStripe(g, stripeBounds.removeFromTop(5), 0.0f, offset);
}

//-------------------------------------------------------------------------------------------------------

void Stripes::paintStripe(juce::Graphics &g, juce::Rectangle<int> r, float offsetTop, float offsetBottom)
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
